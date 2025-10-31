#ifndef TRANSFORM_H
#define TRANSFORM_H

#include <cglm/cglm.h>
void mat4_make_translate(mat4 out, const vec3 translate);
void mat4_make_scale(mat4 out, const vec3 scale);
void mat4_make_rotation(mat4 out, float angle, const vec3 axis);

#endif
