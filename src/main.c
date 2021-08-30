/**
 * ------------------------------------------------------------------------------------------------
 * Computer Graphics - A.Y. 2020/2021
 * Teacher:  Antonino Casile
 * Students: Alberto Paparella   Badge number: 144261
 *           Martina Tenani      Badge number:
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

static enum object {FLOOR} elements;
static enum buffer {FLOOR_VERTICES} element_components;

//light properties
static const Light light0={
   (vec4){0.0, 0.0, 0.0, 1.0},
   (vec4){1.0, 1.0, 1.0, 1.0}, 
   (vec4){1.0, 1.0, 1.0, 1.0},
   (vec4){5.0, 0.0, 0.0, 0.0} //light coords
};

//global ambient
static const vec4 globAmb ={
   0.2, 0.2, 0.2, 1.0
};

//front and back material properties.
static const Material floorMatrl =
{
   (vec4){1.0, 1.0, 1.0, 1.0},
   (vec4){1.0, 1.0, 1.0, 1.0},
   (vec4){1.0, 1.0, 1.0, 1.0},
   (vec4){0.0, 0.0, 0.0, 1.0},
   50.0f
};

// suqare data.
static Vertex squVertices[4];
static vec4 squColors = {1.0, 0.0, 0.0, 1.0};

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
squColorLoc,
floorTexLoc,
buffer[1],
vao[1],
texture[1];

/**
 * Local storage for bmp image data.
 */
struct BitMapFile *image[1];

/**
 * File containing the bmp image.
 */
<<<<<<< HEAD
char *fileName = "grass.bmp"; 

/** 
 * Texture ID objects.
 */
static GLenum textureID[1];
=======
char *fileName = "textures/grass.bmp";
>>>>>>> 4_3_conversion

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

/**
 * Init function, used to initialize the application.
 */
void init(void)
{
    GLenum glErr;

    // Create shader program executable.
    vertexShaderId = setShader("vertex", "shaders/vertexShader.glsl");
    fragmentShaderId = setShader("fragment", "shaders/fragmentShader.glsl");
    
    programId = glCreateProgram();
    glAttachShader(programId, vertexShaderId);
    glAttachShader(programId, fragmentShaderId);
    glLinkProgram(programId);
    glUseProgram(programId);

    // Initialize elements in the scene
    fillSqu(10, 10, squVertices);

    // Create VAOs and VBOs...
    glGenVertexArrays(1, vao);
    glGenBuffers(1, buffer);

    // ...and associate data with vertex shader.
    glBindVertexArray(vao[FLOOR]);
    glBindBuffer(GL_ARRAY_BUFFER, buffer[FLOOR_VERTICES]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(squVertices), squVertices, GL_STATIC_DRAW);

    //coords
    glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, sizeof(squVertices[0]), 0);
    glEnableVertexAttribArray(0);

    //normals
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(squVertices[0]), (void*)sizeof(squVertices[0].coords));
    glEnableVertexAttribArray(1);

    //textures
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(squVertices[0]),(void*)(sizeof(squVertices[0].coords)+sizeof(squVertices[0].normal)));
    glEnableVertexAttribArray(2);

    // Obtain projection matrix uniform location and set value.
    projMatLoc = glGetUniformLocation(programId, "projMat");

    //obtain model view matrix
    modelViewMatLoc = glGetUniformLocation(programId,"modelViewMat");

    //obtain normal matrix
    normalMatLoc = glGetUniformLocation(programId, "normalMat");

/*     // Obtain color uniform locations and set values.
    squColorLoc = glGetUniformLocation(programId, "squColor"); */

    //uniform location of object
    objectLoc = glGetUniformLocation(programId, "object");

    //light parameters uniform locations
    glUniform4fv(glGetUniformLocation(programId, "light0.ambCols"), 1, &light0.ambCols[0]);
    glUniform4fv(glGetUniformLocation(programId, "light0.difCols"), 1, &light0.difCols[0]);
    glUniform4fv(glGetUniformLocation(programId, "light0.specCols"), 1, &light0.specCols[0]);
    glUniform4fv(glGetUniformLocation(programId, "light0.coords"), 1, &light0.coords[0]);

    //global ambient parameter uniform location
    glUniform4fv(glGetUniformLocation(programId, "globAmb"), 1, &globAmb[0]);

    //material paramenters
    glUniform4fv(glGetUniformLocation(programId, "floorMatrl.ambRefl"), 1, &floorMatrl.ambRefl[0]);
    glUniform4fv(glGetUniformLocation(programId, "floorMatrl.difRefl"), 1, &floorMatrl.difRefl[0]);
    glUniform4fv(glGetUniformLocation(programId, "floorMatrl.specRefl"), 1, &floorMatrl.specRefl[0]);
    glUniform4fv(glGetUniformLocation(programId, "floorMatrl.emitCols"), 1, &floorMatrl.emitCols[0]);
    glUniform1f(glGetUniformLocation(programId, "floorMatrl.shininess"), floorMatrl.shininess);

    //load the image
    image[0] = readBMP("./textures/grass.bmp");

    // Create texture ids.
    glGenTextures(1, texture);//create texture ids

    //bind texture
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, texture[0]);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, image[0]->sizeX, image[0]->sizeY, 0, GL_RGBA, GL_UNSIGNED_BYTE, image[0]->data);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    floorTexLoc = glGetUniformLocation(programId, "floorTex");
    glUniform1i(floorTexLoc, 0);

    /**
     * Enable depth test.
     */
    glEnable(GL_DEPTH_TEST);

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

    // set OpenGL version
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
    mat3 TMP;
    
    /**
     * Push initial state on the stack.
     */
    glPushMatrix();

    /**
     * Clear color and depth buffer.
     */
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

/*     glm_mat4_identity(modelViewMat);
    glm_lookat((vec3){0.0, 0.0, 3.0}, (vec3){0.0, 0.0, 0.0},(vec3){0.0, 1.0, 0.0}, modelViewMat);
    glUniformMatrix4fv(modelViewMatLoc, 1, GL_FALSE, (GLfloat *)(modelViewMat));

    // Calculate and update normal matrix.
    glm_mat4_pick3(modelViewMat, TMP);
    glm_mat3_inv(TMP, normalMat);
    glm_mat3_transpose(normalMat);
    glUniformMatrix3fv(normalMatLoc, 1, GL_FALSE, (GLfloat *)normalMat); */

    //draw floor
    glUniform1ui(objectLoc, FLOOR); // Update object name.
    glBindVertexArray(vao[FLOOR]);
    glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
   
    /**
     * Flush graphics objects immediately.
     */
    glFlush();

    /**
     * Pop initial state on the stack.
     */
    glPopMatrix();
}

void display()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glLoadIdentity();

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
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(60,16.0/9.0,1,75);
    glMatrixMode(GL_MODELVIEW);

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
    yaw+=(float)dev_x/10.0;
    pitch+=(float)dev_y/10.0;
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

    if(motion.Forward)
    {
        camX += cos((yaw+90)*TO_RADIANS)/5.0;
        camZ -= sin((yaw+90)*TO_RADIANS)/5.0;
    }
    if(motion.Backward)
    {
        camX += cos((yaw+90+180)*TO_RADIANS)/5.0;
        camZ -= sin((yaw+90+180)*TO_RADIANS)/5.0;
    }
    if(motion.Left)
    {
        camX += cos((yaw+90+90)*TO_RADIANS)/5.0;
        camZ -= sin((yaw+90+90)*TO_RADIANS)/5.0;
    }
    if(motion.Right)
    {
        camX += cos((yaw+90-90)*TO_RADIANS)/5.0;
        camZ -= sin((yaw+90-90)*TO_RADIANS)/5.0;
    }

    /**
     * Limit the values of pitch between -60 and 70.
     */
    if(pitch>=70)
        pitch = 70;
    if(pitch<=-60)
        pitch=-60;

    glRotatef(-pitch,1.0,0.0,0.0);  /* Along X axis. */
    glRotatef(-yaw,0.0,1.0,0.0);    /* Along Y axis. */

    glTranslatef(-camX,0.0,-camZ);
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