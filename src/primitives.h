#ifndef PRIMITIVE_H
#define PRIMITIVE_H

#include <stdint.h>
#include "math/math.h"
#include "math/vertex.h"
#include "utils/utils.h"

/*
 * TODO:
 */

enum RAST_RENDER_MODE {
	RAST_LINE_MODE = 0x00,
	RAST_FILL_MODE = 0x01,
};

typedef struct {
	// render mode
	uint32_t mode;
	// state matrix
	mat4 s_matrix;

	Buffer_t *vertex_buffer;
	Buffer_t *index_buffer;
} rast_render_state_t;

extern rast_render_state_t r_state;

void rast_init();

void rast_set_render_mode(uint32_t mode);

// Draw Functions
void rast_draw_2d_pixel(int x, int y, uint32_t color);
void rast_draw_3d_pixel(int x, int y, float z, uint32_t color);
void rast_draw_2d_line(const vec2 p0, const vec2 p1, uint32_t color);
void rast_draw_3d_line(const vec2 p0, const vec2 p1, uint32_t color);
void rast_draw_rasterized_triangle(const vec3 *pts, uint32_t color);
void rast_draw_wireframe_triangle(const vec3 *pts, uint32_t color);
DEPRECATED("Use the object buffers instead")
void rast_draw_triangles(const vec3 *v, ivec3 *f, int nFaces, const uint32_t *colors);
void rast_use_buffer(Buffer_t *buffer);
void rast_set_matrix(const mat4 *m);
void rast_render();

void rast_draw_circle(int x, int y, int r, uint32_t color);

#endif
