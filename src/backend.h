#ifndef BACKEND_H
#define BACKEND_H

#include <stdbool.h>
#include <stdint.h>
#include <SDL3/SDL.h>

#define RAST_VIDEO_FLIP (1 << 0)

struct rast_backend_state {
	uint32_t *pixels;
	float *zbuffer;
	SDL_Window *window;
	SDL_Renderer *renderer;
	SDL_Texture *texture;
	bool running;
	int window_width, window_height;
	int screen_width, screen_height;

	uint32_t flags;
};

struct rast_backend_state *get_backend_state();

void rast_video_init(int window_width, int window_height, int screen_width, int screen_height);
void rast_video_update();
void rast_video_destroy();

// Flags
void rast_video_set_flags(uint32_t flags);
void rast_video_enable_flags(uint32_t flags);
void rast_video_disable_flags(uint32_t flags);
int rast_video_check_flag(uint32_t flags);

void rast_event_update();

void rast_clear_pixel_buffer();
void rast_clear_z_buffer();
void rast_clear_screen();

void rast_set_clear_color(float r, float g, float b, float a);

int get_window_width();
int get_window_height();
int get_screen_width();
int get_screen_height();
uint32_t *get_pixels();
bool get_status();

#endif
