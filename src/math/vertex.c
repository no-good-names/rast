//
// Created by cp176 on 11/6/2025.
//

#include "vertex.h"

#include <string.h>

// TODO:

// VertexBuffer_t *vb_create(const size_t size) {
// 	VertexBuffer_t *vb = malloc(sizeof(VertexBuffer_t));
// 	vb->size = size;
// 	vb->data = calloc(size, sizeof(Vertex_t));
// 	return vb;
// }
//
// void vb_destroy(VertexBuffer_t *vb) {
// 	free(vb);
// }
//
// void vb_set(VertexBuffer_t *vb, const Vertex_t *data, const size_t size) {
// 	if (!vb || !data || size > vb->size) {
// 		return;
// 	}
// 	memcpy(vb->data, data, size * sizeof(Vertex_t));
// }
//
// IndexBuffer_t *ib_create(const size_t size) {
// 	IndexBuffer_t *ib = malloc(sizeof(VertexBuffer_t));
// 	ib->size = size;
// 	ib->data = calloc(size, sizeof(Vertex_t));
// 	return ib;
// }
//
// void ib_destroy(IndexBuffer_t *ib) {
// 	free(ib);
// }
//
// void ib_set(IndexBuffer_t *ib, const Vertex_t *data, const size_t size) {
// 	if (!ib || !data || size > ib->size) {
// 		return;
// 	}
// 	memcpy(ib->data, data, size * sizeof(Vertex_t));
// }


void buffer_create(Buffer_t *buffer, const BufferType_t type, const size_t size) {
	buffer->type = type;
	buffer->size = size;

	switch (type) {
		case RAST_VERTEX_BUFFER:
			buffer->vdata = malloc(sizeof(Vertex_t) * size);
			break;
		case RAST_INDEX_BUFFER:
			buffer->idata = malloc(sizeof(Index_t) * size);
			break;
		default:
			buffer->raw = NULL;
			printf("Unknown buffer type to be created\n");
			break;
	}
}
void buffer_destroy(const Buffer_t *buffer) {
	switch (buffer->type) {
		case RAST_VERTEX_BUFFER:
			free(buffer->vdata);
			break;
		case RAST_INDEX_BUFFER:
			free(buffer->idata);
			break;
		default:
			printf("No buffer to be destroyed\n");
			break;
	}
}

void buffer_set(Buffer_t *buffer, const void *data) {
	if (!buffer || !data) {
		fprintf(stderr, "Invalid parameters\n");
		return;
	}
	switch (buffer->type) {
		case RAST_VERTEX_BUFFER:
			buffer->vdata = realloc(buffer->vdata, sizeof(Vertex_t) * buffer->size);
			if (!buffer->vdata) {
				fprintf(stderr, "buffer_set: vertex buffer allocation failed\n");
				return;
			}
			memcpy(buffer->vdata, data, sizeof(Vertex_t) * buffer->size);
			break;
		case RAST_INDEX_BUFFER:
			buffer->idata = realloc(buffer->idata, sizeof(Index_t) * buffer->size);
			if (!buffer->idata) {
				fprintf(stderr, "buffer_set: vertex buffer allocation failed\n");
				return;
			}
			memcpy(buffer->idata, data, sizeof(Index_t) * buffer->size);
			break;
		default:
			buffer->raw = NULL;
			fprintf(stderr, "Unknown buffer type\n");
			break;
	}
}

