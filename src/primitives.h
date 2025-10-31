#ifndef PRIMITIVE_H
#define PRIMITIVE_H

#include <stdint.h>
#include "math/math.h"

enum RAST_RENDER_MODE {
	RAST_LINE_MODE = 0x00,
	RAST_FILL_MODE = 0x01,
};

void set_render_mode(uint32_t mode);

// Draw Functions
void draw_2d_pixel(int x, int y, uint32_t color);
void draw_3d_pixel(int x, int y, float z, uint32_t color);
void draw_line(vec2 p0, vec2 p1, uint32_t color);
void rasterized_triangle(vec3 *pts, uint32_t color);
void wireframe_triangle(vec3 *pts, uint32_t color);
void render_triangle(const vec3 *v, ivec3 *f, const int nFaces, const uint32_t *colors);
// Clearing Functions
void clear_color(uint32_t color);

#endif
