//
// Created by cp176 on 11/6/2025.
//

#ifndef RAST_VERTEX_H
#define RAST_VERTEX_H

#include "math.h"

typedef enum {
	RAST_VERTEX_BUFFER,
	RAST_INDEX_BUFFER,
} BufferType_t;

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

typedef struct Index_t {
	ivec3 index;
	// remove later
	uint32_t color;
} Index_t;

// typedef struct VertexBuffer_t {
// 	Vertex_t *data;
// 	size_t size;
// } VertexBuffer_t;
//
// typedef struct IndexBuffer_t {
// 	ivec3 *data;
// 	size_t size;
// } IndexBuffer_t;

typedef struct Buffer_t{
	BufferType_t type;
	size_t size;
	union {
		Vertex_t *vdata;
		Index_t *idata;
		void *raw;
	};
} Buffer_t;

// VertexBuffer_t *vb_create(size_t size);
// void vb_destroy(VertexBuffer_t *vb);
// void vb_set(VertexBuffer_t *vb, const Vertex_t *data, size_t size);
//
// IndexBuffer_t *ib_create(size_t size);
// void ib_destroy(IndexBuffer_t *ib);
// void ib_set(IndexBuffer_t *ib, const Vertex_t *data, size_t size);

void buffer_create(Buffer_t *buffer, BufferType_t type, size_t size);
void buffer_destroy(const Buffer_t *buffer);
void buffer_set(Buffer_t *buffer, const void *data);

#endif //RAST_VERTEX_H