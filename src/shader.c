#include <GL/glew.h>
#include <GL/freeglut.h>
#include <stdio.h>
#include <string.h>


/**
 * Function to read text file.
 * @param aTextFile Pointer to the file.
 * @return Pointer to the array where the content of the file is stored.
 */
char* readTextFile(char* aTextFile) {
    FILE* filePointer = fopen(aTextFile, "rb");
    char* content = NULL;
    long numVal = 0;

    fseek(filePointer, 0L, SEEK_END);
    numVal = ftell(filePointer);
    fseek(filePointer, 0L, SEEK_SET);
    content = (char*) malloc((numVal+1) * sizeof(char));
    fread(content, 1, numVal, filePointer);
    content[numVal] = '\0';
    fclose(filePointer);
    return content;
}

/**
 * Function to initialize shaders.
 * @param shaderType Type of the shader (i.e. vertex, fragment, etc.).
 * @param shaderFile Pointer to the file containing the shader.
 * @return The shader internal given id.
 */
int setShader(char* shaderType, char* shaderFile) {
    int shaderId;
    char* shader = readTextFile(shaderFile);

    /* Set shaderId based on the shader type. */
    if (strcmp(shaderType, "vertex") == 0) shaderId = glCreateShader(GL_VERTEX_SHADER);
    if (strcmp(shaderType, "tessControl") == 0) shaderId = glCreateShader(GL_TESS_CONTROL_SHADER);
    if (strcmp(shaderType, "tessEvaluation") == 0) shaderId = glCreateShader(GL_TESS_EVALUATION_SHADER);
    if (strcmp(shaderType, "geometry") == 0) shaderId = glCreateShader(GL_GEOMETRY_SHADER);
    if (strcmp(shaderType, "fragment") == 0) shaderId = glCreateShader(GL_FRAGMENT_SHADER);

    glShaderSource(shaderId, 1, (const char**) &shader, NULL);
    glCompileShader(shaderId);

    return shaderId;
}


