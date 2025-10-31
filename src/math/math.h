#ifndef MATH_H
#define MATH_H

static void iswap(int *a, int *b) {
    int tmp;
    tmp = *a;
    *a = *b;
    *b = tmp;
}

#if __has_include(<cglm/cglm.h>)
    #include <cglm/cglm.h>
    #define USE_CGLM 1
#else
	typedef float vec2[2];
	typedef float vec3[3];
	typedef float vec4[4];
    #define USE_CGLM 0
#endif

#endif
