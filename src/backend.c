#include "backend.h"
#include <float.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>
#include <SDL3/SDL.h>

#ifdef USE_OPENCL
#include <CL/cl.h>
#endif

struct rast_backend_state g_backend;

struct rast_backend_state *get_backend_state() {
	return &g_backend;
}

void video_init(const int window_width, const int window_height,
	const int screen_width, const int screen_height) {
#ifdef USE_OPENCL
	printf("OpenCL is on\n");
	cl_int x;

#endif
    g_backend.screen_width = screen_width;
    g_backend.screen_height = screen_height;
    g_backend.window_width = window_width;
    g_backend.window_height = window_height;
    if (SDL_Init(SDL_INIT_VIDEO) == false) {
		printf("SDL_Init Error: %s\n", SDL_GetError());
		exit(1);
	}
    g_backend.window = SDL_CreateWindow("Demo", window_width, window_height, 0);
    if (g_backend.window == NULL) {
		printf("SDL_CreateWindow Error: %s\n", SDL_GetError());
		exit(1);
	}
    g_backend.renderer = SDL_CreateRenderer(g_backend.window, 0);
    if (g_backend.renderer == NULL) {
		printf("SDL_CreateRenderer Error: %s\n", SDL_GetError());
		exit(1);
	}
    g_backend.texture = SDL_CreateTexture(g_backend.renderer, SDL_PIXELFORMAT_ARGB8888, SDL_TEXTUREACCESS_STREAMING,
        g_backend.screen_width, g_backend.screen_height);
    if (g_backend.texture == NULL) {
		printf("SDL_CreateTexture Error: %s\n", SDL_GetError());
		exit(1);
	}
	SDL_SetTextureScaleMode(g_backend.texture, SDL_SCALEMODE_NEAREST);
    fprintf(stderr, "Video initialized successfully\n");
    fprintf(stderr, "Window size: %d x %d\n", g_backend.window_width, g_backend.window_height);
    fprintf(stderr, "Screen size: %d x %d\n", g_backend.screen_width, g_backend.screen_height);
    g_backend.running = true;
    g_backend.pixels = malloc(screen_width * screen_height * sizeof(uint32_t));
    if (g_backend.pixels == NULL) {
        printf("Failed to allocate memory for pixels\n");
        exit(1);
    }
    g_backend.zbuffer = malloc(screen_width * screen_height * sizeof(float));
    if (g_backend.zbuffer == NULL) {
        printf("Failed to allocate memory for zbuffer\n");
        exit(1);
    }
	clear_pixelbuffer();
	clear_zbuffer();
}
void video_update() {
    SDL_UpdateTexture(g_backend.texture, NULL, g_backend.pixels, g_backend.screen_width * sizeof(uint32_t));
	// SDL_RenderTexture(g_backend.renderer, g_backend.texture, NULL, NULL);
    // Use this to flip/rotate
	SDL_RenderTextureRotated(g_backend.renderer, g_backend.texture, NULL, NULL, 0.0f, NULL, SDL_FLIP_NONE);
    SDL_RenderPresent(g_backend.renderer);
}
void video_destroy() {
    fprintf(stderr, "Destroying video\n");
    free(g_backend.pixels);
    free(g_backend.zbuffer);
    SDL_DestroyTexture(g_backend.texture);
    SDL_DestroyRenderer(g_backend.renderer);
    SDL_DestroyWindow(g_backend.window);
    fprintf(stderr, "Video destroyed successfully\n");
    fprintf(stderr, "Exiting...\n");
    SDL_Quit();
    exit(0);
}

void event_update() {
	SDL_Event event;
	while(SDL_PollEvent(&event)) {
		switch(event.type) {
			case SDL_EVENT_QUIT:
				g_backend.running = false;
				break;
			default:
				break;
		}
	}
}

void clear_pixelbuffer() {
	// [ 0, max_width * screen_height - 1 ]
    const size_t area =  (size_t)get_screen_width() * (size_t)get_screen_height();
    uint32_t color = 0xFF000000;
    for (size_t i = 0; i < area; ++i) {
        g_backend.pixels[i] = color;
    }
}
void clear_zbuffer() {
	// [ 0, max_width * screen_height - 1 ]
    size_t width = (size_t)get_screen_width();
    size_t height = (size_t)get_screen_height();
    size_t area = width * height;
    for (size_t i = 0; i < area; ++i) {
        g_backend.zbuffer[i] = FLT_MAX;
	}
}

void clear_screen() {
	clear_pixelbuffer();
	clear_zbuffer();
}

int get_window_width() {
	return g_backend.window_width;
}
int get_window_height() {
	return g_backend.window_height;
}

int get_screen_width() {
	return g_backend.screen_width;
}

int get_screen_height() {
	return g_backend.screen_height;
}

uint32_t *get_pixels() {
	return g_backend.pixels;
}

bool get_status() {
	return g_backend.running;
}
