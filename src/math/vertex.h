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
 * TODO: Remake to be like OpenGL
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

typedef struct Buffer_t{
	BufferType_t type;
	size_t size_bytes;
	union {
		Vertex_t *vertex_data;
		Index_t *index_data;
		void *raw;
	};
} Buffer_t;

void rast_create_buffer(Buffer_t *buffer, BufferType_t type, size_t size);
void rast_destroy_buffer(const Buffer_t *buffer);
void rast_set_buffer(Buffer_t *buffer, const void *data);

#endif //RAST_VERTEX_H