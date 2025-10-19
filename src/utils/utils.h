#ifndef UTILS_H
#define UTILS_H

#if defined(_MSC_VER)
#define DEPRECATED(msg) __declspec(deprecated(msg))
#elif defined(__GNUC__) || defined(__clang__)
#define DEPRECATED(msg) __attribute__((deprecated(msg)))
#else
#define DEPRECATED(msg)
#endif

#include <stdint.h>

enum basic_color {
	color_black = 0xFF000000,
	color_white = 0xFFFFFFFF
};

inline uint32_t color_a(uint8_t r, uint8_t g, uint8_t b, uint8_t a) {
    return ((uint32_t)a << 24) | ((uint32_t)r << 16) | ((uint32_t)g << 8) | (uint32_t)b;
}

static uint32_t color(uint8_t r, uint8_t g, uint8_t b) {
    return ((uint32_t)0xFF << 24) | ((uint32_t)r << 16) | ((uint32_t)g << 8) | (uint32_t)b;
}

#endif
