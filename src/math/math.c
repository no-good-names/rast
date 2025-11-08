#include "math.h"

/*
 * NOTE: This stuff is basically copied and pasted from CGLM
 * TODO: Figure out SSE and stuff
*/

void m_iswap(int *a, int *b) {
	int tmp = *a;
	*a = *b;
	*b = tmp;
}

void m_fswap(float *a, float*b) {
	float tmp = *a;
	*a = *b;
	*b = tmp;
}


float m_min(float a, float b) {
#ifdef USING_CGLM
	return glm_min(a, b);
#else
	if (a < b)
		return a;
	return b;
#endif
}

float m_max(float a, float b) {
#ifdef USING_CGLM
	return glm_max(a, b);
#else
	if (a > b)
		return a;
	return b;
#endif
}

void m_ivec3_copy(ivec3 a, ivec3 dest) {
#ifdef USING_CGLM
	glm_ivec3_copy(a, dest);
#else
	dest[0] = a[0];
	dest[1] = a[1];
	dest[2] = a[2];
#endif
}

void m_mat4_copy(mat4 mat, mat4 dest) {
#ifdef USING_CGLM
	glm_mat4_copy(mat, dest);
#else
	dest[0][0] = mat[0][0];  dest[1][0] = mat[1][0];
	dest[0][1] = mat[0][1];  dest[1][1] = mat[1][1];
	dest[0][2] = mat[0][2];  dest[1][2] = mat[1][2];
	dest[0][3] = mat[0][3];  dest[1][3] = mat[1][3];

	dest[2][0] = mat[2][0];  dest[3][0] = mat[3][0];
	dest[2][1] = mat[2][1];  dest[3][1] = mat[3][1];
	dest[2][2] = mat[2][2];  dest[3][2] = mat[3][2];
	dest[2][3] = mat[2][3];  dest[3][3] = mat[3][3];
#endif
}

void m_mat4_mul(mat4 m1, mat4 m2, mat4 dest) {
#ifdef USING_CGLM
	glm_mat4_mul(m1, m2, dest);
#else
	float a00 = m1[0][0], a01 = m1[0][1], a02 = m1[0][2], a03 = m1[0][3],
		  a10 = m1[1][0], a11 = m1[1][1], a12 = m1[1][2], a13 = m1[1][3],
		  a20 = m1[2][0], a21 = m1[2][1], a22 = m1[2][2], a23 = m1[2][3],
		  a30 = m1[3][0], a31 = m1[3][1], a32 = m1[3][2], a33 = m1[3][3],

		  b00 = m2[0][0], b01 = m2[0][1], b02 = m2[0][2], b03 = m2[0][3],
		  b10 = m2[1][0], b11 = m2[1][1], b12 = m2[1][2], b13 = m2[1][3],
		  b20 = m2[2][0], b21 = m2[2][1], b22 = m2[2][2], b23 = m2[2][3],
		  b30 = m2[3][0], b31 = m2[3][1], b32 = m2[3][2], b33 = m2[3][3];

	dest[0][0] = a00 * b00 + a10 * b01 + a20 * b02 + a30 * b03;
	dest[0][1] = a01 * b00 + a11 * b01 + a21 * b02 + a31 * b03;
	dest[0][2] = a02 * b00 + a12 * b01 + a22 * b02 + a32 * b03;
	dest[0][3] = a03 * b00 + a13 * b01 + a23 * b02 + a33 * b03;
	dest[1][0] = a00 * b10 + a10 * b11 + a20 * b12 + a30 * b13;
	dest[1][1] = a01 * b10 + a11 * b11 + a21 * b12 + a31 * b13;
	dest[1][2] = a02 * b10 + a12 * b11 + a22 * b12 + a32 * b13;
	dest[1][3] = a03 * b10 + a13 * b11 + a23 * b12 + a33 * b13;
	dest[2][0] = a00 * b20 + a10 * b21 + a20 * b22 + a30 * b23;
	dest[2][1] = a01 * b20 + a11 * b21 + a21 * b22 + a31 * b23;
	dest[2][2] = a02 * b20 + a12 * b21 + a22 * b22 + a32 * b23;
	dest[2][3] = a03 * b20 + a13 * b21 + a23 * b22 + a33 * b23;
	dest[3][0] = a00 * b30 + a10 * b31 + a20 * b32 + a30 * b33;
	dest[3][1] = a01 * b30 + a11 * b31 + a21 * b32 + a31 * b33;
	dest[3][2] = a02 * b30 + a12 * b31 + a22 * b32 + a32 * b33;
	dest[3][3] = a03 * b30 + a13 * b31 + a23 * b32 + a33 * b33;
#endif
}

void m_mat4_identity(mat4 mat) {
#ifdef USING_CGLM
	glm_mat4_identity(mat);
#else
	M_ALIGN_MAT mat4 t = M_MAT4_IDENTITY_INIT;
	m_mat4_copy(t, mat);
#endif
}

// Template
#ifdef USING_CGLM
#else
#endif
