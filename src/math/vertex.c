//
// Created by cp176 on 11/6/2025.
//

#include "vertex.h"

#include <string.h>

// TODO: New Buffer system
typedef struct {
	RastBufferType_t type;
	size_t size_bytes;
	void *data;
	bool in_use;
} BufferObject_t;

typedef struct {
	RastBufferType_t type;
	unsigned int size;
	unsigned int stride;
	unsigned int offset;
} RastAttribute_t;

#define MAX_BUFFERS 1024
static BufferObject_t g_buffers[MAX_BUFFERS];

void RastGenBuffer(size_t size, RastBuffer *out) {
	for (int i = 0; i < size; i++) {
		for (int j = 1; j < MAX_BUFFERS; j++) {
			if (!g_buffers[j].in_use) {
				g_buffers[j].in_use = true;
				g_buffers[j].data = NULL;
				g_buffers[j].size_bytes = 0;
				out[i] = j;
				break;
			}
		}

	}
}

void RastCreateBuffer();
void RastDestroyBuffer();

int rast_create_buffer(Buffer_t *buffer, const RastBufferType_t type, const size_t size_bytes) {
	buffer->type = type;
	buffer->size_bytes = size_bytes;
	buffer->data = malloc(size_bytes);

	if (!buffer->data) {
		fprintf(stderr, "Failed to allocate memory\n");
		return 0;
	}
	return 1;
}

int rast_destroy_buffer(Buffer_t *buffer) {
	if (!buffer || !buffer->data) return 0;
	free(buffer->data);
	buffer->data = NULL;
	buffer->size_bytes = 0;
	return 1;
}

int rast_set_buffer(Buffer_t *buffer, void *data) {
	if (!buffer || !data || !buffer->data) {
		fprintf(stderr, "Invalid parameters\n");
		return 0;
	}
	memcpy(buffer->data, data, buffer->size_bytes);
	return 1;
}

