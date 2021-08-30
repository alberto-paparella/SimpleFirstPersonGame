#ifndef MATERIAL_H_INCLUDED
#define MATERIAL_H_INCLUDED

#include <GL/glew.h>
#include <cglm/cglm.h>

typedef struct Material
{
   vec4 ambRefl;
   vec4 difRefl;
   vec4 specRefl;
   vec4 emitCols;
   GLfloat shininess;
} Material;

#endif
