/**
 * File to generate basic shapes.
 *
 * IN DETAIL: the functions in this file fill the arrays given
 * with point coordinates.
 */

#include <GL/glew.h>
#include <GL/freeglut.h>
#include <cglm/cglm.h>
#include <cglm/types-struct.h>
#include <stdio.h>
#include <math.h>

#include "shapes.h"

//FLOOR
/**
 * Fills the arrays for coordinates, normals and texture-coordinates to generate a rectangle.
 * @return Nothing.
 */
void fillSquVertexArray(Vertex squVertices[4]){

    //VERTEX 0
    //coordinate
    squVertices[0].coords.x = -200.0;
    squVertices[0].coords.y = -5.0;
    squVertices[0].coords.z = -200.0;
    squVertices[0].coords.w = 1.0;
    //normali
    squVertices[0].normal.x = 0.0;
    squVertices[0].normal.y = 1.0;
    squVertices[0].normal.z = 0.0;
    //texture
    squVertices[0].texCoords.x = 0.0;
    squVertices[0].texCoords.y = 0.0;

    squVertices[1].coords.x = -200.0;
    squVertices[1].coords.y = -5.0;
    squVertices[1].coords.z = 200.0;
    squVertices[1].coords.w = 1.0;
    squVertices[1].normal.x = 0.0;
    squVertices[1].normal.y = 1.0;
    squVertices[1].normal.z = 0.0;
    squVertices[1].texCoords.x = 100.0;
    squVertices[1].texCoords.y = 0.0;

    squVertices[2].coords.x = 200.0;
    squVertices[2].coords.y = -5.0;
    squVertices[2].coords.z = -200.0;
    squVertices[2].coords.w = 1.0;
    squVertices[2].normal.x = 0.0;
    squVertices[2].normal.y = 1.0;
    squVertices[2].normal.z = 0.0;
    squVertices[2].texCoords.x = 100.0;
    squVertices[2].texCoords.y = 100.0;

    squVertices[3].coords.x = -200.0;
    squVertices[3].coords.y = -5.0;
    squVertices[3].coords.z = -200.0;
    squVertices[3].coords.w = 1.0;
    squVertices[3].normal.x = 0.0;
    squVertices[3].normal.y = 1.0;
    squVertices[3].normal.z = 0.0;
    squVertices[3].texCoords.x = 0.0;
    squVertices[3].texCoords.y = 100.0;

}

void fillSquIndices(unsigned int squIndices[1][4]){
    squIndices[0][0] = 0;
    squIndices[0][1] = 1;
    squIndices[0][2] = 2;
    squIndices[0][3] = 3;
}

void fillSquCounts(int squCounts[1]){ //one triangle strip with 4 vertices
    squCounts[0] = 4;
}

// Fill the array of buffer offsets
void fillSquOffsets(void* squOffsets[1])
{
    squOffsets[0] = 0;
}

/**
 * Fills the coordinates arrays and the pointers
 * @param squVertices[4] The array that will contain the vertex coordinates data.
 */
void fillSqu(Vertex squVertices[4], unsigned int squIndices[1][4], int squCounts[1], void* squOffsets[1])
{
    fillSquVertexArray(squVertices);
    fillSquIndices(squIndices);
    fillSquOffsets(squOffsets);
    fillSquCounts(squCounts);
}

//WALLS
/**
 * Fills the arrays for coordinates, normals and texture-coordinates to generate a parallelepiped.
 * @return Nothing.
 */
void fillParVertexArray(float l1, float l2, float h, Vertex parVertices[8]){
    
    //counters
    int v;

    float l = l1/2;
    float startPos = 0.0;
    for (v = 0; v < 8; v++) {
        parVertices[v].coords.x = (v & 4) != 0 ? l : -l + startPos;
        parVertices[v].coords.y = (v & 2) != 0 ? l : -l + startPos;
        parVertices[v].coords.z = (v & 1) != 0 ? l : -l + startPos;
        parVertices[v].coords.w = 1.0;
    }

}
void fillParIndices(unsigned int parIndices[1][8]){
    for(int v = 0; v < 8; v++)
    {
        parIndices[1][v] = v;
    }
}
void fillParCounts(int parCounts[8]){
    for(int v = 0; v <8; v++) 
        parCounts[v] = v;
}
void fillParOffsets(void* parOffsets[8]){
    for(int v = 0; v < 8; v++) 
      parOffsets[v] = (GLvoid*)(v*sizeof(unsigned int));
}

/**
 * Fills the coordinates arrays and the pointers
 * @param l1 The length of a side of the base rectangle.
 * @param l2 The length of a side of the base rectangle.
 * @param h The height of the parallelepiped.
 * @param parVertices[8] The array that will contain the vertex coordinates data.
 */
void fillPar(float l1, float l2, float h, Vertex parVertices[8], unsigned int parIndices[1][8], int parCounts[8], void* parOffsets[8]){
    fillParVertexArray(l1, l2, h, parVertices);
    fillParIndices(parIndices);
    fillParCounts(parCounts);
    fillParOffsets(parOffsets);
}

