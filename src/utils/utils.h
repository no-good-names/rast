#ifndef UTILS_H
#define UTILS_H

#if defined(_MSC_VER)
#define DEPRECATED(msg) __declspec(deprecated(msg))
#elif defined(__GNUC__) || defined(__clang__)
#define DEPRECATED(msg) __attribute__((deprecated(msg)))
#else
#define DEPRECATED(msg)
#endif

#define rast_assert_crash(x) \
	do { \
		if (!x) { \
			fprintf(stderr, "Assertion failed: %d, (%s:%d)\n", x, __FILE__, __LINE__); \
			exit(EXIT_FAILURE); \
		} \
	} while (0);\

#define rast_assert(x) if (!(x)) \
	fprintf(stderr, "Assertion failed: %d, (%s:%d)\n", x, __FILE__, __LINE__); \
	return 0; \

#define rast_assert_msg(msg) if (!(x)) \
	fprintf(stderr, "Assertion failed: %s, (%s:%d)\n", msg, __FILE__, __LINE__); \
	return 0; \


#include <stdint.h>

enum basic_color {
	color_black = 0xFF000000,
	color_white = 0xFFFFFFFF
};

inline uint32_t color_a(uint8_t r, uint8_t g, uint8_t b, uint8_t a) {
    return ((uint32_t)a << 24) | ((uint32_t)r << 16) | ((uint32_t)g << 8) | (uint32_t)b;
}

inline uint32_t color(uint8_t r, uint8_t g, uint8_t b) {
    return ((uint32_t)0xFF << 24) | ((uint32_t)r << 16) | ((uint32_t)g << 8) | (uint32_t)b;
}

inline uint32_t colorf(float r, float g, float b) {
	if (r < 0.0f) r = 0.0f; if (r > 1.0f) r = 1.0f;
	if (g < 0.0f) g = 0.0f; if (g > 1.0f) g = 1.0f;
	if (b < 0.0f) b = 0.0f; if (b > 1.0f) b = 1.0f;

	const uint8_t R = (uint8_t)(r * 255.0f);
	const uint8_t G = (uint8_t)(g * 255.0f);
	const uint8_t B = (uint8_t)(b * 255.0f);

	return (0xFFu << 24) | (R << 16) | (G << 8) | B;
}

#define COLORF(r, g, b) \
	((uint32_t)(0xFFu << 24) | \
	((uint32_t)((r) * 255.0f) << 16) | \
	((uint32_t)((g) * 255.0f) << 8) | \
	(uint32_t)((b) * 255.0f))

#endif
