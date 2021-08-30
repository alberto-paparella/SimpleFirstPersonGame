#ifndef LIGHT_H_INCLUDED
#define LIGHT_H_INCLUDED

#include <cglm/cglm.h>
#include <cglm/types-struct.h>

typedef struct light{
    vec4 ambCols;
    vec4 difCols;
    vec4 specCols;
    vec4 coords;
}Light;

#endif