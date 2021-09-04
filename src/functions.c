/**
 * Functions to initialize variables.
 *
 */

#include <GL/glew.h>
#include <GL/freeglut.h>
#include <cglm/cglm.h>
#include <cglm/types-struct.h>
#include <stdio.h>
#include <math.h>

#include "functions.h"

void VAO_associate(unsigned int vao_arr, unsigned int buff_arr[2], Vertex vertexArr, int mem_start, int elements){
    // ...and associate data with vertex shader.
    glBindVertexArray(vao_arr);
    glBindBuffer(GL_ARRAY_BUFFER, buff_arr[0]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(squVertices), squVertices, GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, buffer[FLOOR_INDICES]);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(squIndices), squIndices, GL_STATIC_DRAW);

    //coords
    glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, sizeof(squVertices[0]), 0);
    glEnableVertexAttribArray(0);

    //normals
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(squVertices[0]), (GLvoid*)sizeof(squVertices[0].coords));
    glEnableVertexAttribArray(1);

    //textures
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(squVertices[0]), (GLvoid*)(sizeof(squVertices[0].coords)+sizeof(squVertices[0].normal)));
    glEnableVertexAttribArray(2);

}