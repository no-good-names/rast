#include "primitives.h"
#include "backend.h"
#include "utils/math.h"

#include <stddef.h>
#include <stdint.h>
#include <sys/types.h>

void draw_2d_pixel(int x, int y, uint32_t color) {	
    if (x < 0 || x >= get_screen_width() || y < 0 || y >= get_screen_height()) return;
    get_pixels()[(y * get_screen_width()) + x] = color;
}

void draw_3d_pixel(int x, int y, float z, uint32_t color) {
    if (x < 0 || x >= get_screen_width() || y < 0 || y >= get_screen_height()) return;
    size_t pos = y * get_screen_width() + x;
    if (z >= get_backend_state()->zbuffer[pos]) return;

    get_pixels()[pos] = color;
    get_backend_state()->zbuffer[pos] = z;
}

void clear_color(uint32_t color) {
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
        iswap(&x0, &y0);
        iswap(&x1, &y1);
        steep = true;
    }
    if (x0 > x1) {
        iswap(&x0, &x1);
        iswap(&y0, &y1);
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
        iswap(&x0, &y0);
        iswap(&x1, &y1);
        steep = true;
    }
    if (x0 > x1) {
        iswap(&x0, &x1);
        iswap(&y0, &y1);
        glm_swapf(&z0, &z1);
    }

    const int dx = x1 - x0;
    const int dy = y1 - y0;
    const int derror2 = abs(dy) * 2;
    int error2 = 0;
    int y = y0;
    for (int x = x0; x <= x1; x++) {
        float t = (dx == 0) ? 0.0f : (float)(x-x0) / (float)dx;
		float z = z0 + (z1 - z0) * t;
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

double signed_triangle_area(int ax, int ay, int bx, int by, int cx, int cy) {
    return .5*((by-ay)*(bx+ax) + (cy-by)*(cx+bx) + (ay-cy)*(ax+cx));
}

static void p_triangle(int ax, int ay, float az,
					   int bx, int by, float bz,
					   int cx, int cy, float cz,
					   uint32_t color) {
    int bbminx = glm_min(glm_min(ax, bx), cx);
    int bbminy = glm_min(glm_min(ay, by), cy);
    int bbmaxx = glm_max(glm_max(ax, bx), cx);
    int bbmaxy = glm_max(glm_max(ay, by), cy);
    double ta = signed_triangle_area(ax, ay, bx, by, cx, cy);
    if (fabs(ta) < 1) return;
    for (int x=bbminx; x<=bbmaxx; x++) {
        for (int y=bbminy; y<=bbmaxy; y++) {
            double alpha = signed_triangle_area(x, y, bx, by, cx, cy) / ta;
            double beta = signed_triangle_area(x, y, cx, cy, ax, ay) / ta;
            double gamma = signed_triangle_area(x, y, ax, ay, bx, by) / ta;
            if (alpha<0 || beta<0 || gamma<0) continue;
            const float z = alpha * az + beta * bz + gamma * cz;
            draw_3d_pixel(x, y, z, color);
        }
    }
}

void triangle(vec3 p0, vec3 p1, vec3 p2, uint32_t color) {
	int ax = p0[0]; int ay = p0[1]; float az = p0[2];
    int bx = p1[0]; int by = p1[1]; float bz = p1[2];
    int cx = p2[0]; int cy = p2[1]; float cz = p2[2];
	p_triangle(ax, ay, az,
			 bx, by, bz,
			 cx, cy, cz,
			 color);
}

void world_to_screen(const vec3 v, vec3 out) {
    out[0] = (v[0] + 1.) * get_screen_width() / 2 + 0.5;
    out[1] = (v[1] + 1.) * get_screen_height() / 2 + 0.5;
    out[2] = v[2];
}

void render_triangle(const vec3 *v, ivec3 *f, const int nFaces, const uint32_t *colors) {
    for (int i = 0; i<nFaces; i++) {
        ivec3 face;
        glm_ivec3_copy(f[i], face);
        vec3 pts[3];
        for (int j = 0; j<3; j++) {
            world_to_screen(v[face[j]], pts[j]);
        }
		triangle(pts[0], pts[1], pts[2], colors[i]);
    }
}


