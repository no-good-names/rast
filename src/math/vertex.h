//
// Created by cp176 on 11/6/2025.
//

#ifndef RAST_VERTEX_H
#define RAST_VERTEX_H

#include "math.h"

/*
 * All vertex data will be forced to use this struct for now
 * TODO: Complete this
 */

typedef struct Vertex_t {
	vec3 position;
	// NOTE: Use later
	vec3 normal;
	// vec2 uv;
	uint32_t color;
} Vertex_t;

typedef struct VertexBuffer_t {
	Vertex_t *data;
	size_t size;
} VertexBuffer_t;

VertexBuffer_t *vb_create(size_t size);
void vb_destroy(VertexBuffer_t *vb);
void vb_set(VertexBuffer_t *vb, const Vertex_t *data, size_t size);

#endif //RAST_VERTEX_H