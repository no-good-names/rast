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

void rast_video_init(const int window_width, const int window_height,
	const int screen_width, const int screen_height) {
#ifdef USE_OPENCL
	printf("OpenCL is on\n");
#endif
    g_backend.screen_width = screen_width;
    g_backend.screen_height = screen_height;
    g_backend.window_width = window_width;
    g_backend.window_height = window_height;
	g_backend.flags = 0x0;
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
	rast_clear_pixel_buffer();
	rast_clear_z_buffer();
}

void rast_video_update() {
    SDL_UpdateTexture(g_backend.texture, NULL, g_backend.pixels, g_backend.screen_width * (int) sizeof(uint32_t));
	if (rast_video_check_flag(RAST_VIDEO_FLIP))
		SDL_RenderTextureRotated(g_backend.renderer, g_backend.texture, NULL, NULL, 0.0f, NULL, SDL_FLIP_VERTICAL);
	else
		SDL_RenderTexture(g_backend.renderer, g_backend.texture, NULL, NULL);
	SDL_RenderPresent(g_backend.renderer);
}

void rast_video_destroy() {
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

void rast_video_set_flags		(const uint32_t flags) { g_backend.flags = flags;				}
void rast_video_enable_flags	(const uint32_t flags) { g_backend.flags |= flags;				}
void rast_video_disable_flags	(const uint32_t flags) { g_backend.flags &= ~flags;				}
int  rast_video_check_flag		(const uint32_t flags) { return (g_backend.flags & flags) != 0; }

void rast_event_update() {
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

static uint32_t screen_clear_color = 0xFF000000;

void rast_clear_pixel_buffer() {
	// [ 0, max_width * screen_height - 1 ]
	for (size_t i = 0; i < (size_t)get_screen_width() * (size_t)get_screen_height(); ++i) {
		g_backend.pixels[i] = screen_clear_color;
    }
}

void rast_clear_z_buffer() {
	// [ 0, max_width * screen_height - 1 ]=
    for (size_t i = 0; i < (size_t)get_screen_width() * (size_t)get_screen_height(); ++i) {
        g_backend.zbuffer[i] = FLT_MAX;
	}
}

void rast_clear_screen() {
	rast_clear_pixel_buffer();
	rast_clear_z_buffer();
}

void rast_set_clear_color(float r, float g, float b, float a) {
	if (r < 0.0f) r = 0.0f; if (r > 1.0f) r = 1.0f;
	if (g < 0.0f) g = 0.0f; if (g > 1.0f) g = 1.0f;
	if (b < 0.0f) b = 0.0f; if (b > 1.0f) b = 1.0f;
	if (a < 0.0f) a = 0.0f; if (a > 1.0f) a = 1.0f;

	const uint8_t R = (uint8_t)(r * 255.0f);
	const uint8_t G = (uint8_t)(g * 255.0f);
	const uint8_t B = (uint8_t)(b * 255.0f);
	const uint8_t A = (uint8_t)(a * 255.0f);

	screen_clear_color = (A << 24) | (R << 16) | (G << 8) | B;
}

int get_window_width() {
	return g_backend.window_width;
}
int get_window_height() { return g_backend.window_height; }
int get_screen_width() { return g_backend.screen_width; }
int get_screen_height() { return g_backend.screen_height; }
uint32_t *get_pixels() { return g_backend.pixels; }
bool get_status() { return g_backend.running; }
