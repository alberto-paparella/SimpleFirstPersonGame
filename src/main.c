/**
 * ------------------------------------------------------------------------------------------------
 * Computer Graphics - A.Y. 2020/2021
 * Teacher:  Antonino Casile
 * Students: Alberto Paparella   Badge number: 144261
 *           Martina Tenani      Badge number: 144739
 * ------------------------------------------------------------------------------------------------
 * Final project:   SIMPLE FIRST PERSON GAME
 *                  An implementation of a simple camera used to navigate into a 3D world.
 *                  It is possible to rotate the camera moving the mouse, and to move around
 *                  using W, A, S and D keys (ESC to close the application).
 *                  It can be used, for example, in first person games or in CAD applications.
 * 
 * Solution:        There are two ways to move to a point in space.
 *                  The first is changing the position of the camera and moving it to that point
 *                  (not possible in OpenGL), the other is changing the position of the point
 *                  and bringing it to the camera.
 * 
 *                  Simply speaking, to produce the effect of moving forward, you either go
 *                  forward yourself, or bring the world backwards.
 * 
 *                  So to produce the effect of going to the point (x,y) in space, it is possible
 *                  to translate the world to the point (-x,-y) instead, and it will be
 *                  indistinguishable.
 * 
 *                  If you wish to look to the right side, rotate the world to the left.
 *                  That is why we use the negative values of the angles while rotating.
 * 
 *                  Important note: always rotate first and then translate.
 * 
 * Useful post about setting a first person camera used for reference:
 *   https://thepentamollisproject.blogspot.com/2018/02/setting-up-first-person-camera-in.html
 * ------------------------------------------------------------------------------------------------
 */
#include <GL/glew.h>
#include <GL/freeglut.h>
#include <cglm/cglm.h>
#include <cglm/types-struct.h>
#include <stdio.h>
#include <stdbool.h>
#include <math.h>

#include "readBMP.h"
#include "shader.h"
#include "shapes.h"
#include "light.h"
#include "material.h"
#include "object.h"

/**
 * Light properties
*/
static const Light light0={
   (vec4){0.0, 0.0, 0.0, 1.0}, 
   (vec4){1.0, 1.0, 1.0, 1.0}, 
   (vec4){1.0, 1.0, 1.0, 1.0},
   (vec4){0.0, 10.0, 0.0, 0.0} //light coords
};

/**
 * Global ambient properties
*/
static const vec4 globAmb ={
   0.2, 0.2, 0.2, 1.0
};

/**
 * Material properties
*/
static const Material floorMatrl =
{
   (vec4){1.0, 1.0, 1.0, 1.0},
   (vec4){1.0, 1.0, 1.0, 1.0},
   (vec4){1.0, 1.0, 1.0, 1.0},
   (vec4){0.0, 0.0, 0.0, 1.0},
   20.0f
};

const unsigned int N_buildings = 4; 

static enum scene_element {FLOOR, WALLS} elements;
static enum scene_element_components {FLOOR_VERTICES, FLOOR_INDICES, WALLS_VERTICES, WALLS_INDICES} element_components;

/**
 * Storing floor data
*/
//TO-DO - mi servono una serie di parametri per poter implementare una collision detection...Volevo provare a 
//Fare delle strutture simil oggetti...
static Vertex squVertices[4];
static unsigned int squIndices[1][4];
static int squCounts[1];
static void* squOffsets[1];

/**
 * Storing elements of the scene
*/
float X_lat=0.0;
float Y_lat=0.0;
float Z_lat=0.0;

CollisionBox objCollBoxes[4];
static unsigned int parIndices[4][4];
static int parCounts[4];
static void* parOffsets[4];

//Cubo 0
static Vertex par0Vertices[4*4];

//Cubo 1
static Vertex par1Vertices[4*4];

//Cubo 2
static Vertex par2Vertices[4*4];

//Cubo 2
static Vertex par3Vertices[4*4];


CollisionBox Player={   //Collision box per il giocatore - Questa deve restare ferma nel mondo
    (vec3){0.0, 0.0, 0.0},
    5.0f,
    5.0f,
    5.0f
};

bool collision;

// Matrices
static mat4 modelViewMat = GLM_MAT4_IDENTITY_INIT;
static mat4 projMat = GLM_MAT4_IDENTITY_INIT;
static mat3 normalMat = GLM_MAT3_IDENTITY_INIT;

// OpenGL global variables
static unsigned int
programId,
vertexShaderId,
fragmentShaderId,
modelViewMatLoc,
normalMatLoc,
projMatLoc,
objectLoc,
floorTexLoc,
wallTexLoc,
buffer[4],
vao[2],
texture[2];

/**
 * Local storage for bmp image data.
 */
struct BitMapFile *image[2];

/**
 * File containing the bmp image.
 */
char *fileName = "textures/grass.bmp";

/**
 * Frame per seconds.
 */
#define FPS 60
/**
 * The trigonometric functions require the angles to be in radian measure.
 * This constant is used to convert degrees to radians on multiplication.
 */
#define TO_RADIANS 3.14/180.0

/**
 * To know which direction to proceed in, we need a structure which will store 4 Booleans
 * called Forward, Backward, Left and Right. All of these will initially be false. 
 * Whenever one of the 4 keys is pressed, the direction to which that key corresponds will be set to true
 * and will remain true until the that key is released. This will be done inside the keyboard() function.
 * The camera() function will keep on checking which of the Booleans are true and will change the position accordingly.
 */
typedef struct
{
    bool Forward,Backward,Left,Right;
} Motion;

/**
 * Width and height of the window (Aspect ratio 16:9).
 */
const int width  = 16*50;
const int height = 9*50;

/**
 * To know which direction to proceed in, we need a structure which will store 4 Booleans 
 * called Forward, Backward, Left and Right. All of these will initially be false. 
 * Whenever one of the 4 keys is pressed, the direction to which that key corresponds will be set to true 
 * and will remain true until the that key is released. This will be done inside the keyboard() function. 
 * The camera() function will keep on checking which of the Booleans are true and will change the position accordingly.
 */
Motion motion = {false, false, false, false};

/**
 * To produce the effect of "looking around" in the 3D world, we will need to use rotation. 
 * We need to store the angles at which the coordinate system is to be rotated, which will be controlled 
 * by the mouse input. The effect of looking right or left will be controlled by rotating the coordinate 
 * system around the Y axis (the angle associated with this is called Yaw), and that of looking up or down 
 * will be produced by rotating around the X axis (the angle associated with this is called Pitch).
 */
float pitch = 0.0, yaw= 0.0;

/**
 * To store the positions of the camera inside the 3D world. 
 * Notice that we have no interest in moving up and down. 
 * TODO this can be scaled in the future, for example to implement jumps.
 */
float camX = 0.0, camZ = 0.0;

/**
 * Display function.
 * @param void
 * @return Nothing.
 */
void display();

/**
 * Reshape function, used to reshape the objects inside the screen when the window width and height change. 
 * @param w The width of the window.
 * @param h The height of the window.
 * @return Nothing.
 */
void reshape(int w, int h);

/**
 * Timer function, used to keep calling the display function periodically at a rate of FPS times in one second. 
 * This will keep updating the screen.
 * @param void
 * @return Nothing.
 */
void timer();

/**
 * This function acts as the callback function for glutPassiveMotionFunc, which
 * we'll use to know the mouse position, even if no button has been pressed on it.
 * We take advantage of the fact that the pointer is not always at the center, 
 * but its position is reset by the timer function every 1/(FPS)th of a second. 
 * To do so, we need to know where the mouse pointer goes between that small interval. 
 * @param x The x coordinate of the mouse.
 * @param y The y coordinate of the mouse
 * @return Nothing.
 */
void passiveMotion(int,int);

/** 
 * This function will be used to apply the rotation to the scene specified by passiveMotion. 
 * It will be called before drawing, and will take care of positioning the camera. * 
 * @param void
 * @return Nothing.
 */
void camera();

/**
 * This function controls keyboard input, used for motion. 
 * The keys W, A, S and D will be used for forward, leftward, backward and rightward motion respectively. 
 * The fundamental concept is that as long as any one of these keys is pressed, the position will keep on changing. 
 * The callback is called only once: when the button is pressed down. 
 * @param key The pressed key.
 * @param x Actual position on the x axis.
 * @param y Actual position on the y axis.
 * @return Nothing.
 */
void keyboard(unsigned char key, int x, int y);

/**
 * This function is used to let the computer know when to stop moving after we pressed a button. 
 * Remember that the keyboard callback is only called once, that is when the button is pressed down. 
 * The motion will be stopped when the button is released.
 * To know when the button is released, we will use another similar callback called glutKeyboardUpFunc. 
 * This callback is identical to the glutKeyboardFunc, except it is called when a key is released. 
 * @param key The pressed key.
 * @param x Actual position on the x axis.
 * @param y Actual position on the y axis.
 * @return Nothing.
 */
void keyboard_up(unsigned char key, int x, int y);

bool checkCollision(CollisionBox Player, CollisionBox *collBoxes);

/**
 * Init function, used to initialize the application.
 */
void init(void)
{ 
    GLenum glErr;

    //PROBLEMA CON SFARFALLAMENTO
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LESS);  
    
    /**
     * Shaders setup
     */
    vertexShaderId = setShader("vertex", "shaders/vertexShader.glsl");
    fragmentShaderId = setShader("fragment", "shaders/fragmentShader.glsl");
    programId = glCreateProgram();
    glAttachShader(programId, vertexShaderId);
    glAttachShader(programId, fragmentShaderId);
    glLinkProgram(programId);  
    glUseProgram(programId);

    /**
     * Initializing elements in the scene
     */
    //floor
    fillSqu(squVertices, squIndices, squCounts, squOffsets);

    //Cubes in the scene
    //CUBE 0
    X_lat = 10.0;
    Y_lat = 20.0;
    Z_lat = 10.0;
    vec3 pos={0.0, 0.0, -50.0};
    fillPar(pos, X_lat, Y_lat, Z_lat, par0Vertices, parIndices, parCounts, parOffsets);
    //inizializza le coordinate
    for(int i = 0; i<3; i++){
        objCollBoxes[0].center_position[i] = pos[i];
    }
    objCollBoxes[0].X_size = X_lat + 2;
    objCollBoxes[0].Y_size = 0.0;
    objCollBoxes[0].Z_size = Z_lat + 2;

    //CUBE 1
    X_lat = 10.0;
    Y_lat = 6.0;
    Z_lat = 5.0;
    pos[0]=-100.0;
    pos[1]=0.0;
    pos[2]=-40.0;
    fillPar(pos, X_lat, Y_lat, Z_lat, par1Vertices, parIndices, parCounts, parOffsets);
    for(int i = 0; i<3; i++){
        objCollBoxes[1].center_position[i] = pos[i];
    }
    objCollBoxes[1].X_size = X_lat + 2;
    objCollBoxes[1].Y_size = 0.0;
    objCollBoxes[1].Z_size = Z_lat + 2;

    //CUBE 2
    X_lat = 20.0;
    Y_lat = 30.0;
    Z_lat = 15.0;

    pos[0]=100.0;
    pos[1]=0.0;
    pos[2]=-40.0;

    fillPar(pos, X_lat, Y_lat, Z_lat, par2Vertices, parIndices, parCounts, parOffsets);
    for(int i = 0; i<3; i++){
        objCollBoxes[2].center_position[i] = pos[i];
    }
    objCollBoxes[2].X_size = X_lat + 2;
    objCollBoxes[2].Y_size = 0.0;
    objCollBoxes[2].Z_size = Z_lat + 2;

    //CUBE 3
    X_lat = 3.0;
    Y_lat = 20.0;
    Z_lat = 10.0;
    pos[0]=-60.0;
    pos[1]=0.0;
    pos[2]=-90.0;

    fillPar(pos, X_lat, Y_lat, Z_lat, par3Vertices, parIndices, parCounts, parOffsets);
    for(int i = 0; i<3; i++){
        objCollBoxes[3].center_position[i] = pos[i];
    }
    objCollBoxes[3].X_size = X_lat + 2;
    objCollBoxes[3].Y_size = 0.0;
    objCollBoxes[3].Z_size = Z_lat + 2;

    /**
     * Setting up VAO's and VBO's
     */
    glGenVertexArrays(2, vao);

    glGenBuffers(4, buffer);

    glBindVertexArray(vao[FLOOR]);
    glBindBuffer(GL_ARRAY_BUFFER, buffer[FLOOR_VERTICES]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(squVertices), squVertices, GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, buffer[FLOOR_INDICES]);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(squIndices), squIndices, GL_STATIC_DRAW);

    /**
     * Storing variables for shaders 
     */
    //Coordinates
    glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, sizeof(squVertices[0]), 0);
    glEnableVertexAttribArray(0);

    //Normals
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(squVertices[0]), (GLvoid*)sizeof(squVertices[0].coords));
    glEnableVertexAttribArray(1);

    //Textures
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(squVertices[0]), (GLvoid*)(sizeof(squVertices[0].coords)+sizeof(squVertices[0].normal)));
    glEnableVertexAttribArray(2);

    /**
     * Getting matrices and object locations
    */
    //Projection matrix
    projMatLoc = glGetUniformLocation(programId, "projMat");
    //setting the viewing frustum
    glm_frustum(-30.0, 30.0, -30.0, 30.0, 0.1, 10, projMat);
    glUniformMatrix4fv(projMatLoc, 1, GL_FALSE, (GLfloat *)projMat);
    
    //ModelView matrix
    modelViewMatLoc = glGetUniformLocation(programId,"modelViewMat");

    //Normal matrix
    normalMatLoc = glGetUniformLocation(programId, "normalMat");

    //object location
    objectLoc = glGetUniformLocation(programId, "object");

    /**
     * Passing light characteristics to the shader
    */
    glUniform4fv(glGetUniformLocation(programId, "light0.ambCols"), 1, &light0.ambCols[0]);
    glUniform4fv(glGetUniformLocation(programId, "light0.difCols"), 1, &light0.difCols[0]);
    glUniform4fv(glGetUniformLocation(programId, "light0.specCols"), 1, &light0.specCols[0]);
    glUniform4fv(glGetUniformLocation(programId, "light0.coords"), 1, &light0.coords[0]);

    /**
     * Passing global ambient parameter uniform location to the shader
    */
    glUniform4fv(glGetUniformLocation(programId, "globAmb"), 1, &globAmb[0]);

    /**
     * Passing material paramenters to the shader
    */
    glUniform4fv(glGetUniformLocation(programId, "floorMatrl.ambRefl"), 1, &floorMatrl.ambRefl[0]);
    glUniform4fv(glGetUniformLocation(programId, "floorMatrl.difRefl"), 1, &floorMatrl.difRefl[0]);
    glUniform4fv(glGetUniformLocation(programId, "floorMatrl.specRefl"), 1, &floorMatrl.specRefl[0]);
    glUniform4fv(glGetUniformLocation(programId, "floorMatrl.emitCols"), 1, &floorMatrl.emitCols[0]);
    glUniform1f(glGetUniformLocation(programId, "floorMatrl.shininess"), floorMatrl.shininess);

    /**
     * Loading the texture image
    */
    image[0] = readBMP("./textures/wild_grass.bmp");
    image[1] = readBMP("./textures/bricks.bmp");

    /**
     * Creating the texture
    */
    glGenTextures(2, texture);//create texture ids

    /**
     * Binding the texture of the floor
    */
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, texture[0]);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, image[0]->sizeX, image[0]->sizeY, 0, GL_RGBA, GL_UNSIGNED_BYTE, image[0]->data);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_MIRRORED_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_MIRRORED_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    //TODO-PROVA A IMPLEMENTARE MIP MAPS PER RISOLVERE SFARFALLIO
/* 
    glActiveTexture(GL_TEXTURE0);
    glGenerateMipmap(GL_TEXTURE_2D);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR); */

    /**
     * Passing the texture to the shader
    */
    floorTexLoc = glGetUniformLocation(programId, "floorTex");
    glUniform1i(floorTexLoc, 0);

    /**
     * Binding the texture of the wall
    */
    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, texture[1]);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, image[1]->sizeX, image[1]->sizeY, 0, GL_RGBA, GL_UNSIGNED_BYTE, image[1]->data);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_MIRRORED_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_MIRRORED_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

    /**
     * Passing the texture to the shader
    */
    wallTexLoc = glGetUniformLocation(programId, "wallTex");

    /**
     * Select clearing color: light blue as the sky.
     */
    glClearColor(0.6, 0.8, 0.9, 0.0);

    /**
     * Hide the cursor.
     */
	glutSetCursor(GLUT_CURSOR_NONE);

	/**
	 * Confine the cursor to the center of the window when the application starts.
	 * glWarpPointer(int x, int y) positions the mouse pointer in the position (x,y)
     * relative to the top left corner of the window, so to place the pointer in the
     * center the parameters should be width/2 and height/2 respectively.
	 */
    glutWarpPointer(width/2, height/2);

    /**
     * ... it does not hurt to check that everything went OK.
     */
    if ((glErr=glGetError()) != 0)
    {
        printf("Errore = %d \n", glErr);
        exit(-1);
    }
}

/**
 * Main function. The computation starts here.
 * @param argc -
 * @param argv -
 * @return 0 if everything went flawless, another integer different from 0 otherwise.
 */
int main(int argc, char**argv)
{
    glutInit(&argc, argv);

    /**
     * Setting OpenGL version
    */
    glutInitContextVersion(4, 3);
    glutInitContextProfile(GLUT_CORE_PROFILE);
    glutInitContextFlags(GLUT_FORWARD_COMPATIBLE);

    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH);
    glutInitWindowSize(width, height);
    glutCreateWindow("Simple First Person Game");

    printf("########################################\n");
    printf("######  Simple First Person Game  ######\n");
    printf("########################################\n");
    printf("# - Use the mouse to rotate the camera #\n");
    printf("# - W, A, S and D to move around       #\n");
    printf("# - ESC to close the application       #\n");
    printf("########################################\n");

    //TESTS
/*     int arr_size = 4;
    int *intPtr = malloc(arr_size * sizeof(int));
    void **voidPtr = malloc(arr_size * sizeof(void*)); //puntatore a un array di puntatori a aree void.
    Vertex 

    intPtr[0] = 3;
    intPtr[1] = 3;
    intPtr[2] = 3;
    intPtr[3] = 3;

    voidPtr[0] = (GLvoid*)(4*0*sizeof(unsigned int));
    voidPtr[1] = (GLvoid*)(4*1*sizeof(unsigned int));
    voidPtr[2] = (GLvoid*)(4*2*sizeof(unsigned int));
    voidPtr[3] = (GLvoid*)(4*3*sizeof(unsigned int));


    Object Cube = {

        intPtr,
        voidPtr
    };

    Cube.objVertices.flex = malloc(3 * sizeof(Vertex));
    Cube.objVertices.flex[0].coords.x = 0.0;
    Cube.objVertices.flex[0].coords.y = 0.0;
    Cube.objVertices.flex[0].coords.z = 0.0;

    for(int i = 0; i<4; i++){
        printf("%d", Cube.objCounts[i]);
    }


    free(intPtr);
    free(*voidPtr);
 */
    /**
     * Here we add support for GLEW.
     */
    GLenum err = glewInit();
    if (GLEW_OK != err) {
        printf("GLEW init failed: %s\n", glewGetErrorString(err));
        exit(1);
    } else {
        printf("GLEW init success\n");
    };

    init();
    glutDisplayFunc(display);
    glutReshapeFunc(reshape);
    glutTimerFunc(0,timer,0);

    /**
     * glutMotionFunc and glutPassiveMotionFunc set the motion and passive motion callback
     * respectively for the current window. The motion callback for a window is called when
     * the mouse moves within the window while one or more mouse buttons are pressed.
     * The passive motion callback for a window is called when the mouse moves within the window
     * while no mouse buttons are pressed.
     */
	glutPassiveMotionFunc(passiveMotion);

    /**
     * glutKeyboardFunc sets the keyboard callback for the current window. When a user types into
     * the window, each key press generating an ASCII character will generate a keyboard callback.
     * The key callback parameter is the generated ASCII character.
     * The state of modifier keys such as Shift cannot be determined directly; their only effect
     * will be on the returned ASCII data. The x and y callback parameters indicate the mouse
     * location in window relative coordinates when the key was pressed.
     * When a new window is created, no keyboard callback is initially registered, and ASCII key
     * strokes in the window are ignored. Passing NULL to glutKeyboardFunc disables the generation
     * of keyboard callbacks.
     */
    glutKeyboardFunc(keyboard);

    /**
     * This callback is identical to the glutKeyboardFunc(), except it is called when a key is released.
     */
    glutKeyboardUpFunc(keyboard_up);

    glutMainLoop();
    return 0;
}

/**
 * This function just draws the scene.
 * @param void
 * @return Nothing.
 */
void draw()
{
    mat4 TMP;
    mat3 TMP_normal;

    /**
     * "Pushing" the matrix
    */
    glm_mat4_copy(modelViewMat,TMP);
    glUniformMatrix4fv(modelViewMatLoc, 1, GL_FALSE, (GLfloat *)(modelViewMat));

    /**
     * Clear color and depth buffer.
     */
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

     /**
     * Calculate and update normal matrix
    */
    glm_mat4_pick3(modelViewMat, TMP_normal);
    glm_mat3_inv(TMP_normal, normalMat);
    glm_mat3_transpose(normalMat);
    glUniformMatrix3fv(normalMatLoc, 1, GL_FALSE, (GLfloat *)normalMat);

    /**
     * Drawing the elements of the scene
    */
   //DRAWING FLOOR
    glUniform1ui(objectLoc, FLOOR); //Passing to shader
    glBindVertexArray(vao[FLOOR]);

    glMultiDrawElements(GL_TRIANGLE_STRIP,squCounts,GL_UNSIGNED_INT,(const void**)squOffsets,1);
    

    //CUBO 0
    glBindVertexArray(vao[WALLS]);
    glBindBuffer(GL_ARRAY_BUFFER, buffer[WALLS_VERTICES]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(par0Vertices), par0Vertices, GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, buffer[WALLS_INDICES]);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(parIndices), parIndices, GL_STATIC_DRAW);

    /**
     * Storing variables for shaders 
     */
    //Coordinates
    glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, sizeof(par0Vertices[0]), 0);
    glEnableVertexAttribArray(0);

    //Normals
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(par0Vertices[0]), (GLvoid*)sizeof(par0Vertices[0].coords));
    glEnableVertexAttribArray(1);

    //Textures
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(par0Vertices[0]), (GLvoid*)(sizeof(par0Vertices[0].coords)+sizeof(par0Vertices[0].normal)));
    glEnableVertexAttribArray(2); 


    glUniform1ui(objectLoc, WALLS); //Passing to shader
    glBindVertexArray(vao[WALLS]);

    glUniform1i(wallTexLoc, 1);

    //glm_translate(modelViewMat, (vec3){0.0, 0.0, -50.0});
    glUniformMatrix4fv(modelViewMatLoc, 1, GL_FALSE, (GLfloat *)(modelViewMat));

    glMultiDrawElements(GL_TRIANGLE_STRIP, parCounts, GL_UNSIGNED_INT, (const void**)parOffsets, 4);

    //CUBO 1
    glBindVertexArray(vao[WALLS]);
    glBindBuffer(GL_ARRAY_BUFFER, buffer[WALLS_VERTICES]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(par1Vertices), par1Vertices, GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, buffer[WALLS_INDICES]);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(parIndices), parIndices, GL_STATIC_DRAW);

    //Coordinates
    glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, sizeof(par1Vertices[0]), 0);
    glEnableVertexAttribArray(0);

    //Normals
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(par1Vertices[0]), (GLvoid*)sizeof(par1Vertices[0].coords));
    glEnableVertexAttribArray(1);

    //Textures
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(par1Vertices[0]), (GLvoid*)(sizeof(par1Vertices[0].coords)+sizeof(par1Vertices[0].normal)));
    glEnableVertexAttribArray(2); 


    glUniform1ui(objectLoc, WALLS); //Passing to shader
    glBindVertexArray(vao[WALLS]);

    glUniform1i(wallTexLoc, 1);

    //glm_translate(modelViewMat, (vec3){0.0, 0.0, -50.0});
    glUniformMatrix4fv(modelViewMatLoc, 1, GL_FALSE, (GLfloat *)(modelViewMat));

    glMultiDrawElements(GL_TRIANGLE_STRIP, parCounts, GL_UNSIGNED_INT, (const void**)parOffsets, 4);

    //CUBO 2
    glBindVertexArray(vao[WALLS]);
    glBindBuffer(GL_ARRAY_BUFFER, buffer[WALLS_VERTICES]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(par2Vertices), par2Vertices, GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, buffer[WALLS_INDICES]);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(parIndices), parIndices, GL_STATIC_DRAW);

    //Coordinates
    glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, sizeof(par2Vertices[0]), 0);
    glEnableVertexAttribArray(0);

    //Normals
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(par2Vertices[0]), (GLvoid*)sizeof(par2Vertices[0].coords));
    glEnableVertexAttribArray(1);

    //Textures
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(par2Vertices[0]), (GLvoid*)(sizeof(par2Vertices[0].coords)+sizeof(par2Vertices[0].normal)));
    glEnableVertexAttribArray(2); 


    glUniform1ui(objectLoc, WALLS); //Passing to shader
    glBindVertexArray(vao[WALLS]);

    glUniform1i(wallTexLoc, 1);

    //glm_translate(modelViewMat, (vec3){0.0, 0.0, -50.0});
    glUniformMatrix4fv(modelViewMatLoc, 1, GL_FALSE, (GLfloat *)(modelViewMat));

    glMultiDrawElements(GL_TRIANGLE_STRIP, parCounts, GL_UNSIGNED_INT, (const void**)parOffsets, 4);

    //CUBO 3
    glBindVertexArray(vao[WALLS]);
    glBindBuffer(GL_ARRAY_BUFFER, buffer[WALLS_VERTICES]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(par3Vertices), par3Vertices, GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, buffer[WALLS_INDICES]);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(parIndices), parIndices, GL_STATIC_DRAW);

    //Coordinates
    glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, sizeof(par3Vertices[0]), 0);
    glEnableVertexAttribArray(0);

    //Normals
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(par3Vertices[0]), (GLvoid*)sizeof(par3Vertices[0].coords));
    glEnableVertexAttribArray(1);

    //Textures
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(par3Vertices[0]), (GLvoid*)(sizeof(par3Vertices[0].coords)+sizeof(par3Vertices[0].normal)));
    glEnableVertexAttribArray(2); 


    glUniform1ui(objectLoc, WALLS); //Passing to shader
    glBindVertexArray(vao[WALLS]);

    glUniform1i(wallTexLoc, 1);

    //glm_translate(modelViewMat, (vec3){0.0, 0.0, -50.0});
    glUniformMatrix4fv(modelViewMatLoc, 1, GL_FALSE, (GLfloat *)(modelViewMat));

    glMultiDrawElements(GL_TRIANGLE_STRIP, parCounts, GL_UNSIGNED_INT, (const void**)parOffsets, 4);

    glFlush();

    /**
     * "Popping" the matrix
    */
    glm_mat4_copy(TMP, modelViewMat);

}

void display()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glm_mat4_identity(modelViewMat);

    /**
     * Here, we call the camera function, will be used to apply the rotation to the scene specified by passiveMotion. 
     * It is important to call it before before drawing, because it will take care of positioning the camera. 
     */
    camera();

    /**
     * Here, we draw the scene.
     */
    draw();

    glutSwapBuffers();

}

void reshape(int w,int h)
{
    glViewport(0,0,w,h);

    glm_mat4_identity(projMat);
    glm_perspective(1.0472, 16.0/9.0, 1, 75, projMat);
    glUniformMatrix4fv(projMatLoc, 1, GL_FALSE, (GLfloat *)(projMat));
}

void timer()
{
    glutPostRedisplay();

    /**
     * Reset pointer position to the center of the screen.
     * Other functions will use the interval of the mouse movement to calculate the camera rotation.
     */
	glutWarpPointer(width/2,height/2);

    glutTimerFunc(1000/FPS,timer,0);
}

void passiveMotion(int x,int y)
{
    /**
     * Two variables to store X and Y coordinates, as observed from the center of the window.
     */
    int dev_x, dev_y;
    dev_x = (width/2)-x;
    dev_y = (height/2)-y;

    /**
     * Modify pitch and yaw according to the mouse pointer displacement within the interval.
     * The displacement of the mouse pointer along Y coordinates (relative to the screen, not
     * the coordinates of the 3D world) will control the pitch, while the displacement along X axis
     * will control the yaw.
     * 
     * If the mouse is moved to the right side, dev_x will be negative, and it will be positive
     * if it is moved to the left. Similarly, dev_y is negative for down and positive for up.
     * These deviations are then used to update the angles.
     * Notice that the values have been divided by 10.
     * This is because the deviations are too large and need to be decreased.
     * It is possible to control the sensitivity of the mouse input by changing this factor.
     * Also, the variables have been cast into float before division.
     * 
     * To apply this rotation to the scene, we will use a separate function which will be called camera().
     * This function will be called before drawing, and will take care of positioning the camera.
     */
    yaw+=(float)dev_x/5.0;
    pitch+=(float)dev_y/5.0;
}

void camera()
{
    /** 
     * It is important to remember to do the rotation along X axis before rotating the world around Y axis.
     * Notice that the value of angle is taken negative. This is because when we want to look right,
     * the world has to be rotated left (since we cannot actually rotate the camera).
     * Similar is the case for pitch (Up and Down).
     * 
     * When the user will press the W key, he will expect the camera to move in the direction it is looking.
     * That means while updating the camX and camZ positions, we will need to consider the value of yaw.
     * Since calculations are going to be based on trigonometry,  we'll need the sin() and cos()
     * functions which are a part of math.h header.
     * 
     * The angle that the arrow makes with the vertical (negative side of Z axis) is yaw.
     * To calculate the position change, we first calculate the angle that the arrow makes with the positive X axis
     * (by adding 90 to yaw it).
     * While moving forward, the component of the arrow along X axis will be cosine of 90 + yaw, and similarly
     * the component along Z axis will be sine of 90 + yaw.
     * If we compare the X-Z plane here with the regular X-Y plane, we will notice that the signs of the Z axis
     * are opposite. That's why we will take the negative value of the component along Z axis.
     * 
     * We divided the whole result by 5. This was to slow down the motion a bit.
     * It is possible to change this value to change the speed of the camera.
     * When moving to the left side, just add 90 more degrees to the angle.
     * Similarly, add 180 for backward motion and subtract 90 for moving right.
     */

    float radius = 5.0f;

    printf("BOX EXTREMITIES: \n X = %f, %f \n Y =  %f, %f \n Z = %f, %f \n ",(objCollBoxes->center_position[0]-objCollBoxes->X_size), (objCollBoxes->center_position[0]+objCollBoxes->X_size),  (objCollBoxes->center_position[1]-objCollBoxes->Y_size),  (objCollBoxes->center_position[1]+objCollBoxes->Y_size),  (objCollBoxes->center_position[2]-objCollBoxes->Z_size),  (objCollBoxes->center_position[2]+objCollBoxes->Z_size));
    printf("PLAYER POSITION: %f,%f,%f\n",Player.center_position[0], Player.center_position[1], Player.center_position[2]);
    printf("YAW: %f\n", yaw);
    printf("COLLISION: %d\n",collision);

    CollisionBox TMP; //Misura quale sarà il prossimo spostamento.


    if(motion.Forward)
    {   

        TMP.center_position[0] = camX + cos((yaw+90)*TO_RADIANS)/radius;
        TMP.center_position[1] = 0.0;
        TMP.center_position[2] = camZ - sin((yaw+90)*TO_RADIANS)/radius;
        collision = checkCollision(TMP, objCollBoxes); //ci sarebbe una collisione
        if(!collision){ 
            camX += cos((yaw+90)*TO_RADIANS)/radius;
            camZ -= sin((yaw+90)*TO_RADIANS)/radius;
        }
    }
    if(motion.Backward)
    {
        TMP.center_position[0] = camX - cos((yaw+90)*TO_RADIANS)/radius;
        TMP.center_position[1] = 0.0;
        TMP.center_position[2] = camZ + sin((yaw+90)*TO_RADIANS)/radius;
        collision = checkCollision(TMP, objCollBoxes);
        if(!collision){
            camX -= cos((yaw+90)*TO_RADIANS)/radius;
            camZ += sin((yaw+90)*TO_RADIANS)/radius;
        }
    }
    if(motion.Left)
    {

        TMP.center_position[0] = camX + cos((yaw+90+90)*TO_RADIANS)/radius;
        TMP.center_position[1] = 0.0;
        TMP.center_position[2] = camZ - sin((yaw+90+90)*TO_RADIANS)/radius;  
        collision = checkCollision(TMP, objCollBoxes);
        if(!collision){
            camX += cos((yaw+90+90)*TO_RADIANS)/radius;
            camZ -= sin((yaw+90+90)*TO_RADIANS)/radius; 
        }     
    }
    if(motion.Right)
    {
        TMP.center_position[0] = camX - cos((yaw+90+90)*TO_RADIANS)/radius;
        TMP.center_position[1] = 0.0;
        TMP.center_position[2] = camZ + sin((yaw+90-90)*TO_RADIANS)/radius; 
        collision = checkCollision(TMP, objCollBoxes);
        if(!collision){
            camX -= cos((yaw+90+90)*TO_RADIANS)/radius;
            camZ += sin((yaw+90+90)*TO_RADIANS)/radius; 
        }
    }

    //tracking player movement
    Player.center_position[0]=camX;
    Player.center_position[2]=camZ;

    /**
     * Limit the values of pitch between -60 and 70.
     */
    if(pitch>=70)
        pitch = 70;
    if(pitch<=-60)
        pitch=-60;
    
    /**
     * ModelView transformations
    */
    glm_rotate(modelViewMat, ((-pitch)*TO_RADIANS), (vec3){1.0, 0.0, 0.0});
    glm_rotate(modelViewMat, ((-yaw)*TO_RADIANS), (vec3){0.0, 1.0, 0.0});

    glm_translate(modelViewMat, (vec3){-camX, 0.0, -camZ});
}

void keyboard(unsigned char key,int x,int y)
{
    /**
     * Whenever one of the 4 keys is pressed, the direction to which that key corresponds will be set to true.
     * The variable key contains the ASCII character value of the key that was pressed, so the switch statement
     * is used to compare its value against the 4 of our keys.
     * We do the comparing for both, uppercase and lowercase. This allows the controls to work regardless of
     * the state of the Caps Lock.
     * Similarly, we will use the keyboard_up() function to set these values to false when the key is released.
     * As always, the ESC button will terminate the execution of the program (freeing memory, e.g. textures).
     */
    switch(key)
    {
        case 27:
            /**
             * ESC
             */
            if (image != NULL)
            {
                /**
                 * Free pixel data.
                 */
                free(image[0]->data);
            }
            exit(0);
            break;
        case 'W':
        case 'w':
            motion.Forward = true;
            break;
        case 'A':
        case 'a':
            motion.Left = true;
            break;
        case 'S':
        case 's':
            motion.Backward = true;
            break;
        case 'D':
        case 'd':
            motion.Right = true;
            break;
    }
}

void keyboard_up(unsigned char key,int x,int y)
{
    /**
     * Whenever one of the 4 keys is released, the direction to which that key corresponds will be set to false.
     */
    switch(key)
    {
        case 'W':
        case 'w':
            motion.Forward = false;
            break;
        case 'A':
        case 'a':
            motion.Left = false;
            break;
        case 'S':
        case 's':
            motion.Backward = false;
            break;
        case 'D':
        case 'd':
            motion.Right = false;
            break;
    }
}

//AABB collision
bool checkCollision(CollisionBox Player, CollisionBox *collBoxes){

    for(int b = 0; b<4; b++){
/*         if((Player.center_position[0]>=(-collBoxes[b].X_size)) && (Player.center_position[0]<=(collBoxes[b].X_size))&&
           (Player.center_position[1]>=(-collBoxes[b].Y_size)) && (Player.center_position[1]<=(collBoxes[b].Y_size))&&
           (Player.center_position[2]>=(-collBoxes[b].Z_size)) && (Player.center_position[2]<=(collBoxes[b].Z_size))){
               return true;
               break;
           } */
        if((Player.center_position[0]>=(collBoxes[b].center_position[0]-collBoxes[b].X_size)) && (Player.center_position[0]<=(collBoxes[b].center_position[0]+collBoxes[b].X_size))&&
           (Player.center_position[1]>=(collBoxes[b].center_position[1]-collBoxes[b].Y_size)) && (Player.center_position[1]<=(collBoxes[b].center_position[1]+collBoxes[b].Y_size))&&
           (Player.center_position[2]>=(collBoxes[b].center_position[2]-collBoxes[b].Z_size)) && (Player.center_position[2]<=(collBoxes[b].center_position[2]+collBoxes[b].Z_size))){
               return true;
               break;
           } 
/* 
        if((Player.center_position[0]>=(-10)) && (Player.center_position[0]<=(10))&&
           (Player.center_position[1]>=(-0)) && (Player.center_position[1]<=(0))&&
           (Player.center_position[2]>=(-40)) && (Player.center_position[2]<=(-60))){
               return true;
               break;
           } */
    }

   return false;
}
