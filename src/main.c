#include <stdint.h>
#include <stdio.h>
#include "backend.h"
#include "cglm/mat4.h"
#include "primitives.h"
#include "utils/utils.h"
#include <SDL3/SDL.h>
#include <cglm/cglm.h>

int main() {
    video_init(600, 600, 600, 600);

	vec3 v[8] = {
        {-1.0f, 1.0f, -1.0f}, // ftl 0
        {1.0f, 1.0f, -1.0f}, // ftr 1
        {-1.0f, -1.0f, -1.0f}, // fbl 2
        {1.0f, -1.0f, -1.0f}, // fbr 3
        {-1.0f, 1.0f, 1.0f}, // btl 4
        {1.0f, 1.0f, 1.0f}, // btr 5
        {-1.0f, -1.0f, 1.0f}, // bbl 6
        {1.0f, -1.0f, 1.0f} // bbr 7
    };

    ivec3 f[12] = {
        {0, 1, 2},
        {1, 2, 3},
        {4, 6, 7},
        {4, 5, 7},
        {5, 7, 3},
        {5, 1, 3},
        {4, 0, 6},
        {0, 2, 6},
        {4, 5, 1},
        {4, 0, 1},
        {6, 7, 2},
        {7, 2, 3}
    };

    uint32_t colors[12] = {
		color(255, 0, 0),
		color(255, 0, 0),
		color(0, 255, 0),
		color(0, 255, 0),
		color(0, 0, 255),
		color(0, 0, 255),
		color(255, 255, 0),
		color(255, 255, 0),
		color(0, 255, 255),
		color(0, 255, 255),
		color(255, 0, 255),
		color(255, 0, 255)
    };
	
    mat4 projection;
    glm_perspective(glm_rad(60.0f), 1, 0.1f, 100.0f, projection);
	
    float angle = 0.0f;
    const float speed = 1.0f;
	set_render_mode(RAST_FILL_MODE);
	float i = 0.0f;
	while (get_status()) {
        event_update();
        clear_screen();  // clear pixels + z-buffer

		// Render Code:
		mat4 model, R, T;
        glm_mat4_identity(model);
		glm_mat4_identity(R);
		glm_mat4_identity(T);
		glm_rotate(R, angle, (vec3) {1.0f, 0.0f, 1.0f});
		glm_translate(T, (vec3){0, 0, -5});
		glm_mat4_mul(T, R, model);

		mat4 view;
		glm_mat4_identity(view);

		mat4 mv, mvp;
		glm_mat4_mul(view, model, mv);
		glm_mat4_mul(projection, mv, mvp);

        vec3 pts[8];
        for (int i = 0; i < 8; ++i) {
            vec4 vin = { v[i][0], v[i][1], v[i][2], 1.0f };
            vec4 vout;
            glm_mat4_mulv(mvp, vin, vout);
            pts[i][0] = vout[0] / vout[3];
            pts[i][1] = vout[1] / vout[3];
            pts[i][2] = vout[2] / vout[3];
        }

        render_triangle(pts, f, 12, colors);
		
        video_update();
		angle += 0.02f * speed;
		i+=0.01;
    }
    video_destroy();
    return 0;
}

