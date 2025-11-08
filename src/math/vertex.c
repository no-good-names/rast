//
// Created by cp176 on 11/6/2025.
//

#include "vertex.h"

#include <string.h>

// TODO:

VertexBuffer_t *vb_create(const size_t size) {
	VertexBuffer_t *vb = malloc(sizeof(VertexBuffer_t));
	vb->size = size;
	vb->data = calloc(size, sizeof(Vertex_t));
	return vb;
}

void vb_destroy(VertexBuffer_t *vb) {
	free(vb);
}

void vb_set(VertexBuffer_t *vb, const Vertex_t *data, size_t size) {
	if (!vb || !data || size > vb->size) {
		return;
	}
	memcpy(vb->data, data, size * sizeof(Vertex_t));
}
