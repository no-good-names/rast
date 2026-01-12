/*
 * RASTERIZER
 * by no-good-names
 * https://github.com/no-good-names/rast
 */


#include <stdint.h>
#include <stdio.h>
#include "backend.h"
#include "cglm/mat4.h"
#include "primitives.h"
#include "utils/utils.h"
#include <SDL3/SDL.h>
#include <cglm/cglm.h>

#include "cglm/clipspace/persp_lh_no.h"
#include "cglm/clipspace/persp_lh_zo.h"
#include "math/vertex.h"

static float pyramid_vs[15] = {
	 0.0f,	 1.0f,	0.0f,
	-1.0f,	-1.0f, -1.0f,
	 1.0f,	-1.0f, -1.0f,
	-1.0f,	-1.0f,	1.0f,
	 1.0f,	-1.0f,	1.0f,
};

static float pyramid_is[18] = {
	0, 1, 2,
	0, 3, 4,
	0, 1, 3,

	0, 2, 4,
	1, 3, 4,
	1, 2, 4,
};

static float cube_vs[24] = {
	-1.0f,  1.0f, -1.0f,
	 1.0f,  1.0f, -1.0f,
	-1.0f, -1.0f, -1.0f,
	 1.0f, -1.0f, -1.0f,
	-1.0f,  1.0f,  1.0f,
	 1.0f,  1.0f,  1.0f,
	-1.0f, -1.0f,  1.0f,
	 1.0f, -1.0f,  1.0f
};

static unsigned int cube_is[36] = {
	0, 1, 2,
	1, 2, 3,
	4, 6, 7,
	4, 5, 7,
	5, 7, 3,
	5, 1, 3,
	4, 0, 6,
	0, 2, 6,
	4, 5, 1,
	4, 0, 1,
	6, 7, 2,
	7, 2, 3,
};

int main() {
    rast_video_init(600, 600, 600, 600);
	rast_init();
	
    mat4 projection;
    glm_perspective_lh_no(glm_rad(60.0f), 1, 0.1f, 100.0f, projection);

    float angle = 0.0f;
    rast_set_render_mode(RAST_LINE_MODE);
	float currentTime = (float)SDL_GetTicks() / 1000.0f;
	float lastTime = currentTime;

	rast_video_enable_flags(RAST_VIDEO_FLIP);

	// IMPORTANT: Make sure to set the size correctly
	Buffer_t vbo, ibo;
	rast_create_buffer(&vbo, RAST_VERTEX_BUFFER, sizeof(float) * 24);
	rast_set_buffer(&vbo, cube_vs);
	rast_create_buffer(&ibo, RAST_INDEX_BUFFER, sizeof(unsigned int) * 36);
	rast_set_buffer(&ibo, cube_is);

	Buffer_t vbo1, ibo1;
	rast_create_buffer(&vbo1, RAST_VERTEX_BUFFER, sizeof(float) * 15);
	rast_set_buffer(&vbo1, pyramid_vs);
	rast_create_buffer(&ibo1, RAST_INDEX_BUFFER, sizeof(unsigned int) * 18);
	rast_set_buffer(&ibo1, pyramid_is);

	rast_use_buffer(&vbo);
	rast_use_buffer(&ibo);

	while (get_status()) {
		const float speed = 1.0f;
		currentTime = (float)SDL_GetTicks() / 1000.0f;
		const float dt = (currentTime - lastTime);
		lastTime = currentTime;
        rast_event_update();
        rast_clear_screen();

		rast_set_clear_color(0.2f, 0.3f, 0.3f, 1.0f);

		mat4 model, view, proj;
		glm_mat4_identity(model);
		glm_translate(model, (vec3){0,0,5});

		glm_rotate(model, angle, (vec3) {1.0f, 0.75f, 0.25f});
		glm_mat4_identity(view);
		glm_perspective_lh_no(glm_rad(60.0f), 1.0f, 0.1f, 100.0f, proj);

		mat4 mvp;
		glm_mat4_mul(proj, view, mvp);
		glm_mat4_mul(mvp, model, mvp);

		rast_set_matrix(&mvp);
		rast_render();


        rast_video_update();
		angle += 1.0f * speed * dt;
    }
	rast_destroy_buffer(&vbo);
	rast_destroy_buffer(&ibo);

	rast_destroy_buffer(&vbo1);
	rast_destroy_buffer(&ibo1);

    rast_video_destroy();
    return 0;
}

