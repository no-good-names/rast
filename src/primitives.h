#ifndef PRIMITIVE_H
#define PRIMITIVE_H

#include <stdint.h>
#include "math/math.h"
#include "math/vertex.h"

/*
 * TODO:
 */

enum RAST_RENDER_MODE {
	RAST_LINE_MODE = 0x00,
	RAST_FILL_MODE = 0x01,
};

struct rast_render_state {
	// render mode
	uint32_t mode;
	// state matrix
	mat4 s_matrix;

	Buffer_t *vbuffer;
	Buffer_t *ibuffer;
};

struct rast_render_state *get_renderer_state();

void render_init();

void set_render_mode(uint32_t mode);

// Draw Functions
void draw_2d_pixel(int x, int y, uint32_t color);
void draw_3d_pixel(int x, int y, float z, uint32_t color);
void draw_2d_line(const vec2 p0, const vec2 p1, uint32_t color);
void draw_3d_line(const vec2 p0, const vec2 p1, uint32_t color);
void rasterized_triangle(const vec3 *pts, uint32_t color);
void wireframe_triangle(const vec3 *pts, uint32_t color);
// void render_triangle(const vec3 *v, ivec3 *f, int nFaces, const uint32_t *colors);
void render_set_buffer(Buffer_t *buffer);
void render_set_matrix(const mat4 *m);
void render();

void draw_circle(int x, int y, int r, uint32_t color);
// Clearing Functions
void clear_color(uint32_t color);

#endif
