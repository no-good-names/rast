#include "primitives.h"
#include "backend.h"
#include "math/math.h"

#include <stdint.h>
#include <stdlib.h>

// local render state
static struct rast_render_state r_state = { 0 };

// NOTE: only use if necessary or to test anything
struct rast_render_state *get_renderer_state() {
    return &r_state;
}

void render_init() {
    r_state.mode = RAST_LINE_MODE;
    glm_mat4_identity(r_state.s_matrix);
    r_state.ibuffer = NULL;
    r_state.vbuffer = NULL;
}

void set_render_mode(const uint32_t mode) {
	if(mode > 0x01) {
		return;
	}
	r_state.mode = mode;
}

void draw_2d_pixel(const int x, const int y, const uint32_t color) {
    if (x < 0 || x >= get_screen_width() || y < 0 || y >= get_screen_height()) return;
    get_pixels()[y * get_screen_width() + x] = color;
}

void draw_3d_pixel(const int x, const int y, const float z, const uint32_t color) {
    if (x < 0 || x >= get_screen_width() || y < 0 || y >= get_screen_height()) return;
    const size_t pos = y * get_screen_width() + x;
    if (z > get_backend_state()->zbuffer[pos]) return;

    get_pixels()[pos] = color;
    get_backend_state()->zbuffer[pos] = z;
}

void clear_color(const uint32_t color) {
	for (int y = 0; y <= get_screen_height(); y++) {
		for (int x = 0; x <= get_screen_width(); x++) {
			draw_2d_pixel(x, y, color);
		}
	}
}

void draw_2d_line(const vec2 p0, const vec2 p1, const uint32_t color) {
    int x0 = (int)p0[0];
    int y0 = (int)p0[1];
    int x1 = (int)p1[0];
    int y1 = (int)p1[1];
    bool steep = false;
    if (abs(x0 - x1) < abs(y0 - y1)) {
        m_iswap(&x0, &y0);
        m_iswap(&x1, &y1);
        steep = true;
    }
    if (x0 > x1) {
        m_iswap(&x0, &x1);
        m_iswap(&y0, &y1);
    }

    const int dx = x1 - x0;
    const int dy = y1 - y0;
    const int derror2 = abs(dy) * 2;
    int error2 = 0;
    int y = y0;
    for (int x = x0; x <= x1; x++) {
        if (steep)
            draw_2d_pixel(y, x, color);
        else
            draw_2d_pixel(x, y, color);

        error2 += derror2;
        if (error2 > dx) {
            y += (y1 > y0) ? 1 : -1;
            error2 -= dx * 2;
        }
    }
}

void draw_3d_line(const vec3 p0, const vec3 p1, const uint32_t color) {
    int x0 = (int)p0[0];
    int y0 = (int)p0[1];
    float z0 = p0[2];
    int x1 = (int)p1[0];
    int y1 = (int)p1[1];
    float z1 = p1[2];
    bool steep = false;
    if (abs(x0 - x1) < abs(y0 - y1)) {
        m_iswap(&x0, &y0);
        m_iswap(&x1, &y1);
        steep = true;
    }
    if (x0 > x1) {
        m_iswap(&x0, &x1);
        m_iswap(&y0, &y1);
        glm_swapf(&z0, &z1);
    }

    const int dx = x1 - x0;
    const int dy = y1 - y0;
    const int derror2 = abs(dy) * 2;
    int error2 = 0;
    int y = y0;
    for (int x = x0; x <= x1; x++) {
        const float t = (dx == 0) ? 0.0f : (float)(x-x0) / (float)dx;
		const float z = z0 + (z1 - z0) * t;
        if (steep)
            draw_3d_pixel(y, x, z, color);
        else
            draw_3d_pixel(x, y, z, color);

        error2 += derror2;
        if (error2 > dx) {
            y += (y1 > y0) ? 1 : -1;
            error2 -= dx * 2;
        }
    }
}

double signed_triangle_area(const int ax, const int ay, const int bx,
                            const int by, const int cx, const int cy) {
    return .5*((by-ay)*(bx+ax) + (cy-by)*(cx+bx) + (ay-cy)*(ax+cx));
}

void rasterized_triangle(const vec3 *pts, const uint32_t color) {
	int ax = pts[0][0]; int ay = pts[0][1]; float az = pts[0][2];
    int bx = pts[1][0]; int by = pts[1][1]; float bz = pts[1][2];
    int cx = pts[2][0]; int cy = pts[2][1]; float cz = pts[2][2];
	int bbminx = m_min(m_min(ax, bx), cx);
    int bbminy = m_min(m_min(ay, by), cy);
    int bbmaxx = m_max(m_max(ax, bx), cx);
    int bbmaxy = m_max(m_max(ay, by), cy);
    double ta = signed_triangle_area(ax, ay, bx, by, cx, cy);
#pragma omp parallel for
    if (fabs(ta) < 1) return;
    for (int x=bbminx; x<=bbmaxx; x++) {
        for (int y=bbminy; y<=bbmaxy; y++) {
            const double alpha = signed_triangle_area(x, y, bx, by, cx, cy) / ta;
            const double beta = signed_triangle_area(x, y, cx, cy, ax, ay) / ta;
            const double gamma = signed_triangle_area(x, y, ax, ay, bx, by) / ta;
            if (alpha<0 || beta<0 || gamma<0) continue;
            const float z = alpha * az + beta * bz + gamma * cz;
            draw_3d_pixel(x, y, z, color);
        }
    }
}

void world_to_screen(const float v[3], float out[3]) {
    out[0] = (v[0] + 1.0f) * get_screen_width() / 2;
    out[1] = (v[1] + 1.0f) * get_screen_height() / 2;
    out[2] = (v[2] + 1.0f) * 255.0f/2;
}

void wireframe_triangle(const vec3 *pts, const uint32_t color) {
    draw_3d_line(pts[0],
              pts[1],  0xFFFFFFFF);
    draw_3d_line(pts[1],
              pts[2], 0xFFFFFFFF);

    draw_3d_line(pts[2],
              pts[0], 0xFFFFFFFF);
}

void render_triangle(const vec3 *v, ivec3 *f, const int nFaces, const uint32_t *colors) {
    for (int i = 0; i<nFaces; i++) {
        ivec3 face;
        m_ivec3_copy(f[i], face);
        vec3 pts[3];
        for (int j = 0; j<3; j++) {
            world_to_screen(v[face[j]], pts[j]);
        }
        switch(r_state.mode) {
            case RAST_LINE_MODE:
                wireframe_triangle(pts, colors[i]);
                break;
            case RAST_FILL_MODE:
                rasterized_triangle(pts, colors[i]);
                break;
            default:
                break;
        }
    }
}

// Fun code
void draw_circle(const int x, const int y, const int r, const uint32_t color) {
#pragma omp parallel for if(r > 64)
    for (int dy = -r; dy <= r; dy++) {
        for (int dx = -r; dx <= r; dx++) {
            if (dx*dx + dy*dy <= r*r) {
                draw_2d_pixel(x + dx, y + dy, color);
            }
        }
    }
}

// Sets
void render_set_buffer(Buffer_t *buffer) {
    if (!buffer) {
        printf("render_set_buffer called with NULL\n");
        return;
    }
    switch (buffer->type) {
        case RAST_VERTEX_BUFFER:
            r_state.vbuffer = buffer;
            break;
        case RAST_INDEX_BUFFER:
            r_state.ibuffer = buffer;
            break;
        default:
            printf("Unknown data type\n");
            break;
    }
}

void render_set_matrix(const mat4 *m) {
    if (!m) return;
    memcpy(r_state.s_matrix, *m, sizeof(mat4)); // NO *, copy from pointer directly
}

void render() {
    if (!r_state.vbuffer || !r_state.vbuffer->vdata) {
        printf("Invalid vertex buffer\n");
        return;
    }
    if (!r_state.ibuffer || !r_state.ibuffer->idata) {
        printf("Invalid index buffer\n");
        return;
    }
    for (int i = 0; i < r_state.ibuffer->size; i++) {
        ivec3 face;
        m_ivec3_copy(r_state.ibuffer->idata[i].index, face);

        vec3 pts[3];
        for (int j = 0; j < 3; j++) {
            vec4 transformed;

            transformed[0] = r_state.vbuffer->vdata[face[j]].position[0];
            transformed[1] = r_state.vbuffer->vdata[face[j]].position[1];
            transformed[2] = r_state.vbuffer->vdata[face[j]].position[2];
            transformed[3] = 1.0f;

            vec4 result;
            glm_mat4_mulv(r_state.s_matrix, transformed, result);

            pts[j][0] = result[0] / result[3];
            pts[j][1] = result[1] / result[3];
            pts[j][2] = result[2] / result[3];

            // Then convert to screen coordinates
            world_to_screen(pts[j], pts[j]);
        }

        switch (r_state.mode) {
            case RAST_LINE_MODE:
                wireframe_triangle(pts, 0xFFFFFFFF);
                break;
            case RAST_FILL_MODE:
                rasterized_triangle(pts, r_state.ibuffer->idata[i].color);
                break;
            default:
                break;
        }
    }
}