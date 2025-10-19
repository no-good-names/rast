#include <stdio.h>
#include "backend.h"
#include "primitives.h"
#include "utils/utils.h"
#include <SDL3/SDL.h>
#include <cglm/cglm.h>

int main() {
    video_init(600, 600, 300, 300);

	vec3 v[4] = {
		{-0.5f, 0.5f, 0.0f},
        {-0.5f, -0.5f, 0.0f},
        {0.5f, 0.5f, 0.0f},
		{0.5f, -0.5f, 0.0f}
    };
    ivec3 f[2] = {
        {0, 1, 2},
		{1, 2, 3}
    };
	uint32_t colors[2] = {
        color(0, 0, 255),
		color(255, 0, 0)
    };
	
    float angle = 0.0f;
    const float speed = 1.0f;
	while (get_status()) {
        event_update();

        clear_screen();  // clear pixels + z-buffer
		mat4 model;
        glm_mat4_identity(model);
        vec3 axis = {0.0f, 1.0f, 0.0f};
        glm_rotate(model, angle, axis);
		glm_rotate(model, 45.0f, (vec3) {1.0f, 0.0f, 0.0f});

        vec3 pts[4];
        for (int i = 0; i < 4; ++i) {
            vec4 vin = { v[i][0], v[i][1], v[i][2], 1.0f };
            vec4 vout;
            glm_mat4_mulv(model, vin, vout);
            pts[i][0] = vout[0];
            pts[i][1] = vout[1];
            pts[i][2] = vout[2];
        }

        render_triangle(pts, f, 2, colors);
		
        video_update();
		angle += 0.02f * speed;
		// reset angle to 0 if greater than 2pi
        if (angle > M_PI * 2.0f) angle -= (float)(M_PI * 2.0f);
    }
    video_destroy();
    return 0;
}

