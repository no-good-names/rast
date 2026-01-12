//
// Created by cp176 on 11/6/2025.
//

#ifndef RAST_VERTEX_H
#define RAST_VERTEX_H

#include "math.h"

typedef enum {
	RAST_VERTEX_BUFFER,
	RAST_INDEX_BUFFER,
} RastBufferType_t;

/*
 * TODO: Remake to be like OpenGL
 */

typedef struct Buffer_t{
	RastBufferType_t type;
	size_t size_bytes;
	void *data;
} Buffer_t;

typedef unsigned int RastBuffer;

void RastGenBuffer(size_t size, RastBuffer *out);

int rast_create_buffer(Buffer_t *buffer, RastBufferType_t type, size_t size_bytes);
int rast_destroy_buffer(Buffer_t *buffer);

int rast_set_buffer(Buffer_t *buffer, void *data);



#endif //RAST_VERTEX_H