#ifndef SHAPES_H_INCLUDED
#define SHAPES_H_INCLUDED

#include "vertex.h"
//PARALLELEPIPEDO
void fillParVertexArray(float l1, float l2, float h, Vertex parVertices[8]);
void fillParIndices(unsigned int parIndices[1][8]);
void fillParCounts(int parCounts[8]);
void fillParOffsets(void* parOffsets[8]);
void fillPar(float l1, float l2, float h, Vertex parVertices[8], unsigned int parIndices[1][8], int parCounts[8], void* parOffsets[8]);

void fillSquVertexArray(Vertex squVertices[4]);
void fillSquIndices(unsigned int squIndices[1][4]);
void fillSquCounts(int squCounts[1]);
void fillSquOffsets(void* squOffsets[1]); 
void fillSqu(Vertex squVertices[4], unsigned int squIndices[1][4], int squCounts[1], void* squOffsets[1]);

#endif