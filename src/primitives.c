#include "primitives.h"
#include "backend.h"
#include "math/math.h"

#include <stdint.h>
#include <stdlib.h>

rast_render_state_t rast_render_state_g = { 0 };

void rast_init() {
    rast_render_state_g.mode = RAST_LINE_MODE;
    glm_mat4_identity(rast_render_state_g.s_matrix);
    rast_render_state_g.index_buffer = 0;
    rast_render_state_g.vertex_buffer = 0;
}

void rast_set_render_mode(const uint32_t mode) {
	if(mode > 0x01) {
		return;
	}
	rast_render_state_g.mode = mode;
}

void rast_draw_2d_pixel(const int x, const int y, const uint32_t color) {
    if (x < 0 || x >= get_screen_width() || y < 0 || y >= get_screen_height()) return;
    get_pixels()[y * get_screen_width() + x] = color;
}

// Draw a pixel with a z value
void rast_draw_3d_pixel(const int x, const int y, const float z, const uint32_t color) {
    if (x < 0 || x >= get_screen_width() || y < 0 || y >= get_screen_height() || z < 0.01f) return;
    const size_t pos = y * get_screen_width() + x;
    if (z > get_backend_state()->zbuffer[pos]) return;

    get_pixels()[pos] = color;
    get_backend_state()->zbuffer[pos] = z;
}


void rast_draw_2d_line(const vec2 p0, const vec2 p1, const uint32_t color) {
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
            rast_draw_2d_pixel(y, x, color);
        else
            rast_draw_2d_pixel(x, y, color);

        error2 += derror2;
        if (error2 > dx) {
            y += (y1 > y0) ? 1 : -1;
            error2 -= dx * 2;
        }
    }
}

void rast_draw_3d_line(const vec3 p0, const vec3 p1, const uint32_t color) {
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
            rast_draw_3d_pixel(y, x, z, color);
        else
            rast_draw_3d_pixel(x, y, z, color);

        error2 += derror2;
        if (error2 > dx) {
            y += (y1 > y0) ? 1 : -1;
            error2 -= dx * 2;
        }
    }
}

static double signed_triangle_area(const int ax, const int ay, const int bx,
                            const int by, const int cx, const int cy) {
    return .5*((by-ay)*(bx+ax) + (cy-by)*(cx+bx) + (ay-cy)*(ax+cx));
}

void rast_draw_rasterized_triangle(const vec3 *pts, const uint32_t color) {
	const int ax = (int)pts[0][0]; const int ay = (int)pts[0][1]; const float az = pts[0][2];
    const int bx = (int)pts[1][0]; const int by = (int)pts[1][1]; const float bz = pts[1][2];
    const int cx = (int)pts[2][0]; const int cy = (int)pts[2][1]; const float cz = pts[2][2];
	const int bbminx = (int)m_min(m_min(ax, bx), cx);
    const int bbminy = (int)m_min(m_min(ay, by), cy);
    const int bbmaxx = (int)m_max(m_max(ax, bx), cx);
    const int bbmaxy = (int)m_max(m_max(ay, by), cy);
    double ta = signed_triangle_area(ax, ay, bx, by, cx, cy);
#pragma omp parallel for
    if (fabs(ta) < 1) return;
    for (int x=bbminx; x<=bbmaxx; x++) {
        for (int y=bbminy; y<=bbmaxy; y++) {
            const double alpha = signed_triangle_area(x, y, bx, by, cx, cy) / ta;
            const double beta = signed_triangle_area(x, y, cx, cy, ax, ay) / ta;
            const double gamma = signed_triangle_area(x, y, ax, ay, bx, by) / ta;
            if (alpha<0 || beta<0 || gamma<0) continue;
            const double z = alpha * az + beta * bz + gamma * cz;
            rast_draw_3d_pixel(x, y, (float)z, color);
        }
    }
}

static void world_to_screen(const float v[3], float out[3]) {
    out[0] = (v[0] + 1.0f) * (float) get_screen_width() / 2;
    out[1] = (v[1] + 1.0f) * (float) get_screen_height() / 2;
    out[2] = (v[2] + 1.0f) * 255.0f/2;
}

void rast_draw_wireframe_triangle(const vec3 *pts, const uint32_t color) {
    rast_draw_3d_line(pts[0],
              pts[1],  0xFFFFFFFF);
    rast_draw_3d_line(pts[1],
              pts[2], 0xFFFFFFFF);

    rast_draw_3d_line(pts[2],
              pts[0], 0xFFFFFFFF);
}

void rast_draw_triangles(const vec3 *v, ivec3 *f, const int nFaces, const uint32_t *colors) {
    for (int i = 0; i<nFaces; i++) {
        const ivec3 *face = &f[i];
        vec3 pts[3];
        for (int j = 0; j<3; j++) {
            world_to_screen(v[*face[j]], pts[j]);
        }
        if (signed_triangle_area(pts[0][0], pts[0][1], pts[1][0], pts[1][1], pts[2][0], pts[2][1]))
        switch(rast_render_state_g.mode) {
            case RAST_LINE_MODE:
                rast_draw_wireframe_triangle(pts, colors[i]);
                break;
            case RAST_FILL_MODE:
                rast_draw_rasterized_triangle(pts, colors[i]);
                break;
            default:
                break;
        }
    }
}

// Fun code
void rast_draw_circle(const int x, const int y, const int r, const uint32_t color) {
#pragma omp parallel for if(r > 64)
    for (int dy = -r; dy <= r; dy++) {
        for (int dx = -r; dx <= r; dx++) {
            if (dx*dx + dy*dy <= r*r) {
                rast_draw_2d_pixel(x + dx, y + dy, color);
            }
        }
    }
}

// Sets
void rast_use_buffer(Buffer_t *buffer) {
    if (buffer == 0) return;
    switch (buffer->type) {
        case RAST_VERTEX_BUFFER:
            rast_render_state_g.vertex_buffer = buffer;
            break;
        case RAST_INDEX_BUFFER:
            rast_render_state_g.index_buffer = buffer;
            break;
        default:
            printf("Unknown data type\n");
            break;
    }
}

void rast_set_matrix(const mat4 *m) {
    if (!m) return;
    memcpy(rast_render_state_g.s_matrix, m, sizeof(mat4));
}

void rast_render() {
    // If buffers = 0 then there is no buffer binded
    if (rast_render_state_g.index_buffer == 0 || rast_render_state_g.vertex_buffer == 0) {
        return;
    }
    const size_t index_count = rast_render_state_g.index_buffer->size_bytes / sizeof(ivec3);
    const float *vertices = rast_render_state_g.vertex_buffer->data;
    const unsigned int *indices = rast_render_state_g.index_buffer->data;
    for (int i = 0; i < index_count; i++) {
        const uint32_t face[3] = {
            indices[i * 3 + 0],
            indices[i * 3 + 1],
            indices[i * 3 + 2]
        };
        vec3 pts[3];

        bool valid_triangle = true;

        for (int j = 0; j < 3; j++) {
            const uint32_t vi = face[j] * 3;
            vec4 transformed = {
                vertices[vi + 0],
                vertices[vi + 1],
                vertices[vi + 2],
                1.0f
            };

            vec4 result;
            glm_mat4_mulv(rast_render_state_g.s_matrix, transformed, result);

            if (result[3] <= 0.1f) {
                valid_triangle = false;
                break;
            };
            float inv_w = 1.0f / result[3];
            pts[j][0] = result[0] * inv_w;
            pts[j][1] = result[1] * inv_w;
            pts[j][2] = result[2] * inv_w;

            // Then convert to screen coordinates
            world_to_screen(pts[j], pts[j]);
        }
        // Simple fix for now
        if (!valid_triangle) continue;

        switch (rast_render_state_g.mode) {
            case RAST_LINE_MODE:
                rast_draw_wireframe_triangle(pts, 0xFFFFFFFF);
                break;
            case RAST_FILL_MODE:
                rast_draw_rasterized_triangle(pts, 0xFFFFFFFF);
                break;
            default:
                break;
        }
    }
}