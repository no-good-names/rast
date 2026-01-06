//
// Created by cp176 on 11/6/2025.
//

#include "vertex.h"

#include <string.h>

void rast_create_buffer(Buffer_t *buffer, const BufferType_t type, const size_t size) {
	buffer->type = type;
	buffer->size_bytes = size;

	switch (type) {
		case RAST_VERTEX_BUFFER:
			buffer->vertex_data = malloc(sizeof(Vertex_t) * size);
			break;
		case RAST_INDEX_BUFFER:
			buffer->index_data = malloc(sizeof(Index_t) * size);
			break;
		default:
			buffer->raw = NULL;
			printf("Unknown buffer type to be created\n");
			break;
	}
}

void rast_destroy_buffer(const Buffer_t *buffer) {
	switch (buffer->type) {
		case RAST_VERTEX_BUFFER:
			free(buffer->vertex_data);
			break;
		case RAST_INDEX_BUFFER:
			free(buffer->index_data);
			break;
		default:
			printf("No buffer to be destroyed\n");
			break;
	}
}

void rast_set_buffer(Buffer_t *buffer, const void *data) {
	if (!buffer || !data) {
		fprintf(stderr, "Invalid parameters\n");
		return;
	}
	switch (buffer->type) {
		case RAST_VERTEX_BUFFER:
			buffer->vertex_data = realloc(buffer->vertex_data, sizeof(Vertex_t) * buffer->size_bytes);
			if (!buffer->vertex_data) {
				fprintf(stderr, "buffer_set: vertex buffer allocation failed\n");
				return;
			}
			memcpy(buffer->vertex_data, data, sizeof(Vertex_t) * buffer->size_bytes);
			break;
		case RAST_INDEX_BUFFER:
			buffer->index_data = realloc(buffer->index_data, sizeof(Index_t) * buffer->size_bytes);
			if (!buffer->index_data) {
				fprintf(stderr, "buffer_set: vertex buffer allocation failed\n");
				return;
			}
			memcpy(buffer->index_data, data, sizeof(Index_t) * buffer->size_bytes);
			break;
		default:
			buffer->raw = NULL;
			fprintf(stderr, "Unknown buffer type\n");
			break;
	}
}

