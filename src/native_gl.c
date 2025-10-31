#include "native_gl.h"

#include <cglm/cglm.h>

// TODO: Make this stuff

static mat4 mode;
static mat4 view;
static mat4 projection;
static mat4 viewport;

void ngl_viewport(const int x, const int y, const int w, const int h) {
	glm_mat4_identity(viewport);
	viewport[0][0] = w/2.;
	viewport[1][1] = h/2.;
	viewport[0][3] = x + w/2.;
	viewport[1][3] = y + h/2.;
	viewport[2][2] = 1.0f;
}

mat4 *ngl_get_viewport() {
	return &viewport;
}

void ngl_perspective(const double f) {}

