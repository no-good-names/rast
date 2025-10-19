#ifndef PRIMITIVE_H
#define PRIMITIVE_H

#include "cglm/types.h"
#include <stdint.h>

// Draw Functions
void draw_2d_pixel(int x, int y, uint32_t color);
void draw_3d_pixel(int x, int y, float z, uint32_t color);
void draw_line(vec2 p0, vec2 p1, uint32_t color);
void triangle(vec3 p0, vec3 p1, vec3 p2, uint32_t color);
void render_triangle(const vec3 *v, ivec3 *f, const int nFaces, const uint32_t *colors);
// Clearing Functions
void clear_color(uint32_t color);

#endif
