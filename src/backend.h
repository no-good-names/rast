#ifndef BACKEND_H
#define BACKEND_H

#include <stdbool.h>
#include <stdint.h>
#include <SDL3/SDL.h>

struct rast_backend_state {
	uint32_t *pixels;
	float *zbuffer;
	SDL_Window *window;
	SDL_Renderer *renderer;
	SDL_Texture *texture;
	bool running;
	int window_width, window_height;
	int screen_width, screen_height;
};

struct rast_backend_state *get_backend_state();

void video_init(int window_width, int window_height, int screen_width, int screen_height);
void video_update();
void video_destroy();

void event_update();

void clear_pixelbuffer();
void clear_zbuffer();
void clear_screen();

int get_window_width();
int get_window_height();
int get_screen_width();
int get_screen_height();
uint32_t *get_pixels();
bool get_status();

#endif
