#version 430 core

#define FLOOR 0
#define WALL 1

// samplers for the textures
uniform sampler2D floorTex;
uniform sampler2D wallTex;

// material properties
in vec4 frontAmbDiffExport, frontSpecExport, backAmbDiffExport, backSpecExport;
// texture
in vec2 texCoordsExport;

uniform uint object;
vec4 texColor;

out vec4 colorsOut;

void main(void)
{
   if (object == FLOOR){
      texColor = texture(floorTex, texCoordsExport);
   }else if (object == WALL){
      texColor = texture(wallTex, texCoordsExport);
   }

   colorsOut = frontAmbDiffExport * texColor + frontSpecExport;
}