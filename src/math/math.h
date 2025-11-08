#ifndef MATH_H
#define MATH_H

void m_iswap(int *a, int *b);
void m_fswap(float *a, float *b);

// NOTE: This is for fall back code. Might remove cglm later on
#if __has_include(<cglm/cglm.h>)
#include <cglm/cglm.h>
// #define USING_CGLM
#endif

#ifndef USING_CGLM

#include <stdalign.h>

#if defined(_MSC_VER)
#  if _MSC_VER < 1913 || _M_ARM /*  Visual Studio 2017 version 15.6  */
#    define M_ALL_UNALIGNED
#    define M_ALIGN(X) /* no alignment */
#  else
#    define M_ALIGN(X) __declspec(align(X))
#  endif
#else
#  define M_ALIGN(X) __attribute((aligned(X)))
#endif

#ifndef M_ALL_UNALIGNED
#  define M_ALIGN_IF(X) CGLM_ALIGN(X)
#else
#  define M_ALIGN_IF(X) /* no alignment */
#endif

#ifdef __AVX__
#  define M_ALIGN_MAT CGLM_ALIGN(32)
#else
#  define M_ALIGN_MAT CGLM_ALIGN(16)
#endif

typedef                 float   vec2[2];
typedef                 float   vec3[3];
typedef M_ALIGN_IF(16)  float   vec4[4];

typedef M_ALIGN_IF(16)  vec2    mat2[2];
typedef                 vec3    mat3[3];
typedef M_ALIGN_MAT     vec4    mat4[4];

#define M_MAT4_IDENTITY_INIT  {{1.0f, 0.0f, 0.0f, 0.0f},                    \
{0.0f, 1.0f, 0.0f, 0.0f},                    \
{0.0f, 0.0f, 1.0f, 0.0f},                    \
{0.0f, 0.0f, 0.0f, 1.0f}}

#define M_MAT4_ZERO_INIT      {{0.0f, 0.0f, 0.0f, 0.0f},                    \
{0.0f, 0.0f, 0.0f, 0.0f},                    \
{0.0f, 0.0f, 0.0f, 0.0f},                    \
{0.0f, 0.0f, 0.0f, 0.0f}}

#endif

float m_min(float a, float b);
float m_max(float a, float b);
void m_ivec3_copy(ivec3 a, ivec3 dest);
void m_mat4_mul(mat4 m1, mat4 m2, mat4 dest);

#endif
