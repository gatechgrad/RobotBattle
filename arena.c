/* 2003 Levi D. Smith */

#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <string.h>
#include <GL/glx.h>
#include <GL/gl.h>
#include <GL/glu.h>
/*
#include <X11/extensions/xf86vmode.h>
#include <X11/keysym.h>
*/
#include "SDL.h"
#include <math.h>

#define PI 3.14159265359
#define SCREEN_WIDTH 640
#define SCREEN_HEIGHT 480 
#define SCREEN_BPP 16 

#define TRUE 1
#define FALSE 0
SDL_Surface *surface;



XFontStruct *font;
GLuint  base;

extern Bool loadGLTextures(GLuint *myTex, char *strFileName);
extern void loadModel(GLfloat x, GLfloat y, GLfloat z, GLfloat rot);
void drawRobotOne();
GLvoid printGLf(const char *fmt, ...);

GLfloat flCameraPosition[3];
GLfloat flCameraVelocity[3];
GLfloat flCameraAngle[3];


GLfloat LightAmbient[] = { 1.0f, 1.0f, 1.0f, 1.0f };

GLfloat RedLightDiffuse[] = { 1.0f, 0.5f, 0.5f, 1.0f };
GLfloat RedLightPosition[] = { -24.0f, 0.0f, 0.0f, 1.0f };


GLfloat BlueLightDiffuse[] = { 1.0f, 1.0f, 1.0f, 1.0f };
GLfloat BlueLightPosition[] = { 0.0f, 0.0f, -2.0f, 1.0f };


GLfloat rotY = 0.0f;
GLuint *imgArena;
GLuint *imgFloor;
GLuint *imgRamp;
GLuint *imgSide;
GLuint *imgRedPanel;
GLuint *imgBluePanel;
GLuint *imgWall;

GLfloat robotOneYRot = 0.0f; 
GLfloat robotOnePosition[3] = { 0.0f, 2.0f, 0.0f};
GLfloat robotOneVelocity[3] = { 0.0f, 0.0f, 0.0f};

  GLfloat fZoom = -100.0f;

Bool done;
GLuint  *texture[1];  /* Storage For One Texture */
GLuint  *bot00088[2];  /* Storage For One Texture */
GLuint  *bot00182[2];  /* Storage For One Texture */

/* function called when our window is resized (should only happen in window mode) */
void resizeGLScene(unsigned int width, unsigned int height)
{
    if (height == 0)    /* Prevent A Divide By Zero If The Window Is Too Small */
        height = 1;
    /* Reset The Current Viewport And Perspective Transformation */
    glViewport(0, 0, width, height);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(45.0f, (GLfloat)width / (GLfloat)height, 0.1f, 200.0f);
    glMatrixMode(GL_MODELVIEW);
}

/* general OpenGL initialization function */
int initGL(GLvoid)
{

    imgArena = malloc(sizeof(GLuint));
    loadGLTextures(imgArena, "arena.bmp");
 
    imgFloor = malloc(sizeof(GLuint));
    loadGLTextures(imgFloor, "floor.bmp");

    imgRamp = malloc(sizeof(GLuint));
    loadGLTextures(imgRamp, "ramp.bmp");
 
    imgSide = malloc(sizeof(GLuint));
    loadGLTextures(imgSide, "side.bmp");
 
    imgRedPanel = malloc(sizeof(GLuint));
    loadGLTextures(imgRedPanel, "redpanel.bmp");
 
    imgBluePanel = malloc(sizeof(GLuint));
    loadGLTextures(imgBluePanel, "bluepanel.bmp");

    imgWall = malloc(sizeof(GLuint));
    loadGLTextures(imgWall,  "wall.bmp");

    bot00088[0] = malloc(sizeof(GLuint));
    loadGLTextures(bot00088[0],  "bot00088-001.bmp");

    bot00088[1] = malloc(sizeof(GLuint));
    loadGLTextures(bot00088[1],  "bot00088-002.bmp");

    bot00182[0] = malloc(sizeof(GLuint));
    loadGLTextures(bot00182[0],  "bot00182-001.bmp");

    bot00182[1] = malloc(sizeof(GLuint));
    loadGLTextures(bot00182[1],  "bot00182-002.bmp");

/****************************************************/



/****************************************************/
    glEnable(GL_TEXTURE_2D);       /* Enable Texture Mapping */
    glShadeModel(GL_SMOOTH);
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    glClearDepth(1.0f);
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LEQUAL);
    glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);

    glBlendFunc(GL_SRC_ALPHA, GL_ONE);
/*
    glDisable(GL_DEPTH_TEST);
*/

/*
    glLightfv(GL_LIGHT1, GL_AMBIENT, LightAmbient);
    glLightfv(GL_LIGHT1, GL_DIFFUSE, RedLightDiffuse);
    glLightfv(GL_LIGHT1, GL_POSITION, RedLightPosition);
    glEnable(GL_LIGHT1); 
*/
/*
    glLightfv(GL_LIGHT2, GL_AMBIENT, LightAmbient);
    glLightfv(GL_LIGHT2, GL_DIFFUSE, BlueLightDiffuse);
    glLightfv(GL_LIGHT2, GL_POSITION, BlueLightPosition);
    glEnable(GL_LIGHT2); 
*/


    /* we use resizeGLScene once to set up our initial perspective */
    resizeGLScene(SCREEN_WIDTH, SCREEN_HEIGHT);
    glFlush();


    flCameraPosition[0] = 0.0f;
    flCameraPosition[1] = 10.0f;
    flCameraPosition[2] = 40.0f;

    flCameraVelocity[0] = 0.0f;
    flCameraVelocity[1] = 0.0f;
    flCameraVelocity[2] = 0.0f;
 
    flCameraAngle[0] = 0.0f;
    flCameraAngle[1] = 0.0f;
    flCameraAngle[2] = 0.0f;
 

    return True;
}

/* Here goes our drawing code */
int drawGLScene(GLvoid) {
  int i;

  glColor4f(1.0f, 1.0f, 1.0f, 1.0f);
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);


/*
    glRotatef(20.0f, 1.0f, 0.0f, 0.0f);
*/

    loadModel(robotOnePosition[0], robotOnePosition[1], robotOnePosition[2], robotOneYRot);

    glLoadIdentity();
/*
    glTranslatef(0.0f, 0.0f, fZoom);
    glRotatef(45.0f, 1.0f, 0.0f, 0.0f);
    glRotatef(rotY, 0.0f, 1.0f, 0.0f);
*/


    glTranslatef(-flCameraPosition[0], -flCameraPosition[1], -flCameraPosition[2]);
    glRotatef(flCameraAngle[0], 1.0f, 0.0f, 0.0f);
    glRotatef(flCameraAngle[1], 0.0f, 1.0f, 0.0f);
    glRotatef(flCameraAngle[2], 0.0f, 0.0f, 1.0f);

    /* Floor */
    glBindTexture(GL_TEXTURE_2D, *imgFloor);
    glBegin(GL_POLYGON);
        glColor4f(1.0f, 1.0f, 1.0f, 0.5f);
        glNormal3f(0.0f, 1.0f, 0.0f);
        glTexCoord2f(0.0f, 0.0f); glVertex3f(-100.0f, 0.0f, -100.0f); 
        glTexCoord2f(0.0f, 1.0f); glVertex3f(-100.0f, 0.0f, 100.0f);
        glTexCoord2f(1.0f, 1.0f); glVertex3f(100.0f, 0.0f, 100.0f);
        glTexCoord2f(1.0f, 0.0f); glVertex3f(100.0f, 0.0f, -100.0f);
    glEnd();


    /* Cage Main Surface */
    glBindTexture(GL_TEXTURE_2D, *imgArena);
    glBegin(GL_POLYGON);
        glColor4f(1.0f, 1.0f, 1.0f, 1.0f);
        glTexCoord2f(0.0f, 0.0f); glVertex3f(-24.0f, 2.0f, 24.0f); 
        glTexCoord2f(1.0f, 0.0f); glVertex3f(24.0f, 2.0f, 24.0f);
        glTexCoord2f(1.0f, 1.0f); glVertex3f(24.0f, 2.0f, -24.0f);
        glTexCoord2f(0.0f, 1.0f); glVertex3f(-24.0f, 2.0f, -24.0f);
    glEnd();




    /* Cage Sides */
    glBindTexture(GL_TEXTURE_2D, *imgSide);
    glBegin(GL_POLYGON);
        glColor4f(0.25f, 0.25f, 0.25f, 1.0f);
        glNormal3f(0.0f, 1.0f, 0.0f);
        glTexCoord2f(0.0f, 1.0f); glVertex3f(-24.0f, 2.0f, -24.0f); 
        glTexCoord2f(1.0f, 1.0f); glVertex3f(24.0f, 2.0f, -24.0f);
        glTexCoord2f(1.0f, 0.0f); glVertex3f(24.0f, 0.0f, -24.0f);
        glTexCoord2f(0.0f, 0.0f); glVertex3f(-24.0f, 0.0f, -24.0f);
    glEnd();

    glBegin(GL_POLYGON);
        glColor4f(0.25f, 0.25f, 0.25f, 1.0f);
        glVertex3f(-24.0f, 2.0f, 24.0f); 
        glVertex3f(24.0f, 2.0f, 24.0f);
        glVertex3f(24.0f, 0.0f, 24.0f);
        glVertex3f(-24.0f, 0.0f, 24.0f);
    glEnd();

    glBegin(GL_POLYGON);
        glColor4f(0.25f, 0.25f, 0.25f, 1.0f);
        glVertex3f(-24.0f, 2.0f, 24.0f); 
        glVertex3f(-24.0f, 2.0f, -24.0f);
        glVertex3f(-24.0f, 0.0f, -24.0f);
        glVertex3f(-24.0f, 0.0f, 24.0f);
    glEnd();

    glBegin(GL_POLYGON);
        glColor4f(0.25f, 0.25f, 0.25f, 1.0f);
        glVertex3f(24.0f, 2.0f, 24.0f); 
        glVertex3f(24.0f, 2.0f, -24.0f);
        glVertex3f(24.0f, 0.0f, -24.0f);
        glVertex3f(24.0f, 0.0f, 24.0f);
    glEnd();

    /* Ring Bounds */
/*
    glBegin(GL_POLYGON);
        glColor3f(1.0f, 1.0f, 0.25f);
        glVertex3f(-24.0f, 2.0f + 0.1f, 24.0f - 0.25f); 
        glVertex3f(24.0f, 2.0f + 0.1, 24.0f - 0.25f);
        glVertex3f(24.0f, 2.0f + 0.1, 24.0f);
        glVertex3f(-24.0f, 2.0f + 0.1, 24.0f);
    glEnd();

    glBegin(GL_POLYGON);
        glColor3f(1.0f, 1.0f, 0.25f);
        glVertex3f(-24.0f, 2.0f + 0.1f, -24.0f + 0.25f); 
        glVertex3f(24.0f, 2.0f + 0.1, -24.0f + 0.25f);
        glVertex3f(24.0f, 2.0f + 0.1, -24.0f);
        glVertex3f(-24.0f, 2.0f + 0.1, -24.0f);
    glEnd();

    glBegin(GL_POLYGON);
        glColor3f(1.0f, 1.0f, 0.25f);
        glVertex3f(-24.0f + 0.25f, 2.0f + 0.1f, -24.0f); 
        glVertex3f(-24.0f + 0.25f, 2.0f + 0.1, 24.0f);
        glVertex3f(-24.0f, 2.0f + 0.1, 24.0f);
        glVertex3f(-24.0f, 2.0f + 0.1, -24.0f);
    glEnd();

    glBegin(GL_POLYGON);
        glColor3f(1.0f, 1.0f, 0.25f);
        glVertex3f(24.0f - 0.25, 2.0f + 0.1f, -24.0f); 
        glVertex3f(24.0f - 0.25f, 2.0f + 0.1, 24.0f);
        glVertex3f(24.0f, 2.0f + 0.1, 24.0f);
        glVertex3f(24.0f, 2.0f + 0.1, -24.0f);
    glEnd();

*/

/*
    glBegin(GL_POLYGON);
        glColor3f(1.0000f, 0.5f, 0.5000f);
        glVertex3f(-4.0f, 2.0f + 0.1f, 24.0f); 
        glVertex3f(4.0f, 2.0f + 0.1, 24.0f);
        glVertex3f(4.0f, 2.0f + 0.1, 20.0f);
        glVertex3f(-4.0f, 2.0f + 0.1, 20.0f);
    glEnd();

    glBegin(GL_POLYGON);
        glColor3f(0.5f, 0.5f, 1.0f);
        glVertex3f(-4.0f, 2.0f + 0.1, -24.0f); 
        glVertex3f(4.0f, 2.0f + 0.1, -24.0f);
        glVertex3f(4.0f, 2.0f + 0.1, -20.0f);
        glVertex3f(-4.0f, 2.0f + 0.1, -20.0f);
    glEnd();
*/

    /* Contestant Stands */
    glBegin(GL_POLYGON);
        glNormal3f(0.0f, 1.0f, 0.0f);
        glColor4f(1.0f, 0.5f, 0.5f, 1.0f);
        glVertex3f(24.0f, 2.0f + 0.01f, 16.0f); 
        glVertex3f(32.0f, 2.0f + 0.01, 16.0f);
        glVertex3f(32.0f, 2.0f + 0.01, 4.0f);
        glVertex3f(24.0f, 2.0f + 0.01, 4.0f);
    glEnd();

    glBegin(GL_POLYGON);
        glColor4f(0.25f, 0.25f, 0.25f, 1.0f);
        glVertex3f(24.0f, 0.0f, 16.0f); 
        glVertex3f(24.0f, 2.0f, 16.0f);
        glVertex3f(32.0f, 2.0f, 16.0f);
        glVertex3f(32.0f, 0.0f, 16.0f);
    glEnd();

    glBegin(GL_POLYGON);
        glColor4f(0.25f, 0.25f, 0.25f, 1.0f);
        glVertex3f(24.0f, 0.0f, 4.0f); 
        glVertex3f(24.0f, 2.0f, 4.0f);
        glVertex3f(32.0f, 2.0f, 4.0f);
        glVertex3f(32.0f, 0.0f, 4.0f);
    glEnd();

    glBegin(GL_POLYGON);
        glColor4f(0.25f, 0.25f, 0.25f, 1.0f);
        glVertex3f(32.0f, 2.0f, 4.0f); 
        glVertex3f(32.0f, 2.0f, 16.0f);
        glVertex3f(32.0f, 0.0f, 16.0f);
        glVertex3f(32.0f, 0.0f, 4.0f);
    glEnd();




    glBegin(GL_POLYGON);
        glColor4f(0.5f, 0.5f, 1.0f, 1.0f);
        glVertex3f(24.0f, 2.0f + 0.01f, -16.0f); 
        glVertex3f(32.0f, 2.0f + 0.01, -16.0f);
        glVertex3f(32.0f, 2.0f + 0.01, -4.0f);
        glVertex3f(24.0f, 2.0f + 0.01, -4.0f);
    glEnd();

    glBegin(GL_POLYGON);
        glColor4f(0.25f, 0.25f, 0.25f, 1.0f);
        glVertex3f(24.0f, 0.0f, -16.0f); 
        glVertex3f(24.0f, 2.0f, -16.0f);
        glVertex3f(32.0f, 2.0f, -16.0f);
        glVertex3f(32.0f, 0.0f, -16.0f);
    glEnd();

    glBegin(GL_POLYGON);
        glColor4f(0.25f, 0.25f, 0.25f, 1.0f);
        glVertex3f(24.0f, 0.0f, -4.0f); 
        glVertex3f(24.0f, 2.0f, -4.0f);
        glVertex3f(32.0f, 2.0f, -4.0f);
        glVertex3f(32.0f, 0.0f, -4.0f);
    glEnd();

    glBegin(GL_POLYGON);
        glColor4f(0.25f, 0.25f, 0.25f, 1.0f);
        glVertex3f(32.0f, 2.0f, -4.0f); 
        glVertex3f(32.0f, 2.0f, -16.0f);
        glVertex3f(32.0f, 0.0f, -16.0f);
        glVertex3f(32.0f, 0.0f, -4.0f);
    glEnd();

    /* Draw Posts */
    for (i = 0; i <= 12; i++) {
      glBegin(GL_LINES);
        glColor4f(0.25f, 0.25f, 0.25f, 1.0f);
        glVertex3f((i * 4.0f) + -24.0f, 2.0f, -24.0f); 
        glVertex3f((i * 4.0f) + -24.0f, 26.0f, -24.0f); 
      glEnd();

      glBegin(GL_LINES);
        glColor4f(0.25f, 0.25f, 0.25f, 1.0f);
        glVertex3f((i * 4.0f) + -24.0f, 2.0f, 24.0f); 
        glVertex3f((i * 4.0f) + -24.0f, 26.0f, 24.0f); 
      glEnd();

      glBegin(GL_LINES);
        glColor4f(0.25f, 0.25f, 0.25f, 1.0f);
        glVertex3f(-24.0f, 2.0f, (i * 4.0f) - 24.0f); 
        glVertex3f(-24.0f, 26.0f, (i * 4.0f) - 24.0f); 
      glEnd();

      glBegin(GL_LINES);
        glColor4f(0.25f, 0.25f, 0.25f, 1.0f);
        glVertex3f(24.0f, 2.0f, (i * 4.0f) - 24.0f); 
        glVertex3f(24.0f, 26.0f, (i * 4.0f) - 24.0f); 
      glEnd();

    }

    /* Draw Posts */
    for (i = 1; i < 4; i++) {
      glBegin(GL_LINES);
        glColor4f(0.25f, 0.25f, 0.25f, 1.0f);
        glVertex3f(-24.0f, 2.0f + (i * 8.0f), -24.0f); 
        glVertex3f(24.0f, 2.0f + (i * 8.0f), -24.0f); 
      glEnd();

      glBegin(GL_LINES);
        glColor4f(0.25f, 0.25f, 0.25f, 1.0f);
        glVertex3f(-24.0f, 2.0f + (i * 8.0f), 24.0f); 
        glVertex3f(24.0f, 2.0f + (i * 8.0f), 24.0f); 
      glEnd();

      glBegin(GL_LINES);
        glColor4f(0.25f, 0.25f, 0.25f, 1.0f);
        glVertex3f(-24.0f, 2.0f + (i * 8.0f), 24.0f); 
        glVertex3f(-24.0f, 2.0f + (i * 8.0f), -24.0f); 
      glEnd();

      glBegin(GL_LINES);
        glColor4f(0.25f, 0.25f, 0.25f, 1.0f);
        glVertex3f(24.0f, 2.0f + (i * 8.0f), 24.0f); 
        glVertex3f(24.0f, 2.0f + (i * 8.0f), -24.0f); 
      glEnd();

    }

/* drawRobotOne(); 
    drawRobotTwo(); 
    glRotatef(-robotOneYRot, 0.0f, 1.0f, 0.0f);    
    glLoadIdentity();
    glTranslatef(0.0f, 0.0f, fZoom);
    glRotatef(45.0f, 1.0f, 0.0f, 0.0f);
    glRotatef(rotY, 0.0f, 1.0f, 0.0f);
*/
 
    /* Draw Plexiglass */
    glEnable(GL_BLEND);

    glBindTexture(GL_TEXTURE_2D, *imgWall);
    glBegin(GL_POLYGON);
      glColor4f(1.0f, 1.0f, 1.0f, 0.25f);
      glVertex3f(-24.0f, 2.0f, -24.0f); 
      glVertex3f(24.0f, 2.0f, -24.0f);
      glVertex3f(24.0f, 26.0f, -24.0f);
      glVertex3f(-24.0f, 26.0f, -24.0f);
    glEnd();

    glBindTexture(GL_TEXTURE_2D, *imgWall);
    glBegin(GL_POLYGON);
      glColor4f(1.0f, 1.0f, 1.0f, 0.25f);
      glVertex3f(-24.0f, 2.0f, 24.0f); 
      glVertex3f(24.0f, 2.0f, 24.0f);
      glVertex3f(24.0f, 26.0f, 24.0f);
      glVertex3f(-24.0f, 26.0f, 24.0f);
    glEnd();

    glBindTexture(GL_TEXTURE_2D, *imgWall);
    glBegin(GL_POLYGON);
      glColor4f(1.0f, 1.0f, 1.0f, 0.25f);
      glVertex3f(-24.0f, 2.0f, 24.0f); 
      glVertex3f(-24.0f, 2.0f, -24.0f);
      glVertex3f(-24.0f, 26.0f, -24.0f);
      glVertex3f(-24.0f, 26.0f, 24.0f);
    glEnd();

    glBindTexture(GL_TEXTURE_2D, *imgWall);
    glBegin(GL_POLYGON);
      glColor4f(1.0f, 1.0f, 1.0f, 0.25f);
      glVertex3f(24.0f, 2.0f, 24.0f); 
      glVertex3f(24.0f, 2.0f, -24.0f);
      glVertex3f(24.0f, 26.0f, -24.0f);
      glVertex3f(24.0f, 26.0f, 24.0f);
    glEnd();

    glDisable(GL_BLEND);

    /* Draw Ramps */ 
    glBegin(GL_POLYGON);
        glColor4f(0.5f, 0.5f, 0.5f, 1.0f);
        glVertex3f(24.0f, 2.0f, -16.0f); 
        glVertex3f(24.0f, 2.0f, -24.0f);
        glVertex3f(40.0f, 0.0f, -24.0f);
        glVertex3f(40.0f, 0.0f, -16.0f);
    glEnd();

    glBegin(GL_TRIANGLES);
        glColor4f(0.25f, 0.25f, 0.25f, 1.0f);
        glVertex3f(24.0f, 2.0f, -24.0f); 
        glVertex3f(24.0f, 0.0f, -24.0f);
        glVertex3f(40.0f, 0.0f, -24.0f);
    glEnd();

    glBegin(GL_TRIANGLES);
        glColor4f(0.25f, 0.25f, 0.25f, 1.0f);
        glVertex3f(24.0f, 2.0f, -16.0f); 
        glVertex3f(24.0f, 0.0f, -16.0f);
        glVertex3f(40.0f, 0.0f, -16.0f);
    glEnd();




    glBegin(GL_POLYGON);
        glColor4f(0.5f, 0.5f, 0.5f, 1.0f);
        glVertex3f(24.0f, 2.0f, 16.0f); 
        glVertex3f(24.0f, 2.0f, 24.0f);
        glVertex3f(40.0f, 0.0f, 24.0f);
        glVertex3f(40.0f, 0.0f, 16.0f);
    glEnd();

    glBegin(GL_TRIANGLES);
        glColor4f(0.25f, 0.25f, 0.25f, 1.0f);
        glVertex3f(24.0f, 2.0f, 24.0f); 
        glVertex3f(24.0f, 0.0f, 24.0f);
        glVertex3f(40.0f, 0.0f, 24.0f);
    glEnd();

    glBegin(GL_TRIANGLES);
        glColor4f(0.25f, 0.25f, 0.25f, 1.0f);
        glVertex3f(24.0f, 2.0f, 16.0f); 
        glVertex3f(24.0f, 0.0f, 16.0f);
        glVertex3f(40.0f, 0.0f, 16.0f);
    glEnd();




    rotY += 0.1f;

  glColor4f(1.0f, 0.0f, 0.0f, 1.0f);
 glRasterPos2f(5.0f, 5.0f);
/*
    printGLf("Battle Bots");
*/

  SDL_GL_SwapBuffers();
    return True;    
}

/*
void drawRobotOne() {

  glLoadIdentity();
    glTranslatef(0.0f, 0.0f, fZoom);
    glRotatef(45.0f, 1.0f, 0.0f, 0.0f);
    glRotatef(rotY, 0.0f, 1.0f, 0.0f);

  glTranslatef(robotOnePosition[0], robotOnePosition[1], robotOnePosition[2]);  
  glRotatef(robotOneYRot, 0.0f, 1.0f, 0.0f);

  glBindTexture(GL_TEXTURE_2D, *bot00088[0]);
  glBegin(GL_POLYGON);
      glColor4f(1.0f, 1.0f, 1.0f, 0.5f);
      glTexCoord2f(0.0f, 0.0f); glVertex3f(-1.0f, 1.0f, 2.0f); 
      glTexCoord2f(1.0f, 0.0f); glVertex3f(1.0f, 1.0f, 2.0f);
      glTexCoord2f(1.0f, 1.0f); glVertex3f(1.0f, 1.0f, -2.0f);
      glTexCoord2f(0.0f, 1.0f); glVertex3f(-1.0f, 1.0f, -2.0f);
  glEnd();

  glBindTexture(GL_TEXTURE_2D, *bot00088[1]);
  glBegin(GL_POLYGON);
      glColor4f(1.0f, 1.0f, 1.0f, 0.5f);
      glTexCoord2f(0.0f, 0.0f); glVertex3f(-1.0f, 0.0f, 2.0f); 
      glTexCoord2f(1.0f, 0.0f); glVertex3f(1.0f, 0.0f, 2.0f);
      glTexCoord2f(1.0f, 1.0f); glVertex3f(1.0f, 0.0f, -2.0f);
      glTexCoord2f(0.0f, 1.0f); glVertex3f(-1.0f, 0.0f, -2.0f);
  glEnd();

  glBindTexture(GL_TEXTURE_2D, *bot00088[1]);
  glBegin(GL_POLYGON);
      glColor4f(1.0f, 1.0f, 1.0f, 0.5f);
      glTexCoord2f(0.0f, 0.0f); glVertex3f(-1.0f, 0.0f, 2.0f); 
      glTexCoord2f(1.0f, 0.0f); glVertex3f(1.0f, 0.0f, 2.0f);
      glTexCoord2f(1.0f, 1.0f); glVertex3f(1.0f, 1.0f, 2.0f);
      glTexCoord2f(0.0f, 1.0f); glVertex3f(-1.0f, 1.0f, 2.0f);
  glEnd();

  glBindTexture(GL_TEXTURE_2D, *bot00088[1]);
  glBegin(GL_POLYGON);
      glColor4f(1.0f, 1.0f, 1.0f, 0.5f);
      glTexCoord2f(0.0f, 0.0f); glVertex3f(-1.0f, 0.0f, -2.0f); 
      glTexCoord2f(1.0f, 0.0f); glVertex3f(1.0f, 0.0f, -2.0f);
      glTexCoord2f(1.0f, 1.0f); glVertex3f(1.0f, 1.0f, -2.0f);
      glTexCoord2f(0.0f, 1.0f); glVertex3f(-1.0f, 1.0f, -2.0f);
  glEnd();

  glBindTexture(GL_TEXTURE_2D, *bot00088[1]);
  glBegin(GL_POLYGON);
      glColor4f(1.0f, 1.0f, 1.0f, 0.5f);
      glTexCoord2f(0.0f, 0.0f); glVertex3f(1.0f, 0.0f, -2.0f); 
      glTexCoord2f(1.0f, 0.0f); glVertex3f(1.0f, 0.0f, 2.0f);
      glTexCoord2f(1.0f, 1.0f); glVertex3f(1.0f, 1.0f, 2.0f);
      glTexCoord2f(0.0f, 1.0f); glVertex3f(1.0f, 1.0f, -2.0f);
  glEnd();

  glBindTexture(GL_TEXTURE_2D, *bot00088[1]);
  glBegin(GL_POLYGON);
      glColor4f(1.0f, 1.0f, 1.0f, 0.5f);
      glTexCoord2f(0.0f, 0.0f); glVertex3f(-1.0f, 0.0f, -2.0f); 
      glTexCoord2f(1.0f, 0.0f); glVertex3f(-1.0f, 0.0f, 2.0f);
      glTexCoord2f(1.0f, 1.0f); glVertex3f(-1.0f, 1.0f, 2.0f);
      glTexCoord2f(0.0f, 1.0f); glVertex3f(-1.0f, 1.0f, -2.0f);
  glEnd();

}

void drawRobotTwo() {

  glLoadIdentity();
    glTranslatef(0.0f, 0.0f, fZoom);
    glRotatef(45.0f, 1.0f, 0.0f, 0.0f);
    glRotatef(rotY, 0.0f, 1.0f, 0.0f);

  glTranslatef(r1x, r1y, r1z);  
  glRotatef(robotOneYRot, 0.0f, 1.0f, 0.0f);

  glBindTexture(GL_TEXTURE_2D, *bot00182[1]);
  glBegin(GL_POLYGON);
      glColor4f(1.0f, 1.0f, 1.0f, 0.5f);
      glTexCoord2f(0.0f, 0.0f); glVertex3f(-1.0f, 1.0f, 2.0f); 
      glTexCoord2f(1.0f, 0.0f); glVertex3f(1.0f, 1.0f, 2.0f);
      glTexCoord2f(1.0f, 1.0f); glVertex3f(1.0f, 1.0f, -2.0f);
      glTexCoord2f(0.0f, 1.0f); glVertex3f(-1.0f, 1.0f, -2.0f);
  glEnd();

  glBindTexture(GL_TEXTURE_2D, *bot00182[0]);
  glBegin(GL_POLYGON);
      glColor4f(1.0f, 1.0f, 1.0f, 0.5f);
      glTexCoord2f(0.0f, 0.0f); glVertex3f(-1.0f, 0.0f, 2.0f); 
      glTexCoord2f(1.0f, 0.0f); glVertex3f(1.0f, 0.0f, 2.0f);
      glTexCoord2f(1.0f, 1.0f); glVertex3f(1.0f, 0.0f, -2.0f);
      glTexCoord2f(0.0f, 1.0f); glVertex3f(-1.0f, 0.0f, -2.0f);
  glEnd();

  glBindTexture(GL_TEXTURE_2D, *bot00182[0]);
  glBegin(GL_POLYGON);
      glColor4f(1.0f, 1.0f, 1.0f, 0.5f);
      glTexCoord2f(0.0f, 0.0f); glVertex3f(-1.0f, 0.0f, 2.0f); 
      glTexCoord2f(1.0f, 0.0f); glVertex3f(1.0f, 0.0f, 2.0f);
      glTexCoord2f(1.0f, 1.0f); glVertex3f(1.0f, 1.0f, 2.0f);
      glTexCoord2f(0.0f, 1.0f); glVertex3f(-1.0f, 1.0f, 2.0f);
  glEnd();

  glBindTexture(GL_TEXTURE_2D, *bot00182[0]);
  glBegin(GL_POLYGON);
      glColor4f(1.0f, 1.0f, 1.0f, 0.5f);
      glTexCoord2f(0.0f, 0.0f); glVertex3f(-1.0f, 0.0f, -2.0f); 
      glTexCoord2f(1.0f, 0.0f); glVertex3f(1.0f, 0.0f, -2.0f);
      glTexCoord2f(1.0f, 1.0f); glVertex3f(1.0f, 1.0f, -2.0f);
      glTexCoord2f(0.0f, 1.0f); glVertex3f(-1.0f, 1.0f, -2.0f);
  glEnd();

  glBindTexture(GL_TEXTURE_2D, *bot00182[0]);
  glBegin(GL_POLYGON);
      glColor4f(1.0f, 1.0f, 1.0f, 0.5f);
      glTexCoord2f(0.0f, 0.0f); glVertex3f(1.0f, 0.0f, -2.0f); 
      glTexCoord2f(1.0f, 0.0f); glVertex3f(1.0f, 0.0f, 2.0f);
      glTexCoord2f(1.0f, 1.0f); glVertex3f(1.0f, 1.0f, 2.0f);
      glTexCoord2f(0.0f, 1.0f); glVertex3f(1.0f, 1.0f, -2.0f);
  glEnd();

  glBindTexture(GL_TEXTURE_2D, *bot00182[0]);
  glBegin(GL_POLYGON);
      glColor4f(1.0f, 1.0f, 1.0f, 0.5f);
      glTexCoord2f(0.0f, 0.0f); glVertex3f(-1.0f, 0.0f, -2.0f); 
      glTexCoord2f(1.0f, 0.0f); glVertex3f(-1.0f, 0.0f, 2.0f);
      glTexCoord2f(1.0f, 1.0f); glVertex3f(-1.0f, 1.0f, 2.0f);
      glTexCoord2f(0.0f, 1.0f); glVertex3f(-1.0f, 1.0f, -2.0f);
  glEnd();

}
*/


GLvoid printGLf(const char *fmt, ...) {
    va_list ap;     /* our argument pointer */
    char text[256];
    if (fmt == NULL)    /* if there is no string to draw do nothing */
        return;
    va_start(ap, fmt);  /* make ap point to first unnamed arg */
    /* FIXME: we *should* do boundschecking or something to prevent buffer
     * overflows/segmentations faults
     */
    vsprintf(text, fmt, ap);
    va_end(ap);
    glPushAttrib(GL_LIST_BIT);
    glListBase(base - 32);
    glCallLists(strlen(text), GL_UNSIGNED_BYTE, text);
    glPopAttrib();
}

void robotOneMoveForward(int isMoving) {
  if (isMoving == TRUE) {
    robotOneVelocity[0] = 0.5f;
  } else {
    robotOneVelocity[0] = 0.0f;
  }
}

void robotOneMoveBackward(int isMoving) {
  if (isMoving == TRUE) {
    robotOneVelocity[0] = -0.5f;
  } else {
    robotOneVelocity[0] = 0.0f;
  }
}


void keyPressed(SDL_Event *event, int isDown) {
    switch (event->key.keysym.sym) {

        case SDLK_ESCAPE:
            done = TRUE;
            /* SDL_Quit(); */
            break;
        case SDLK_F1:
          if (isDown == TRUE) {
            SDL_WM_ToggleFullScreen(surface);
          }
          break;
        case SDLK_F2:
            glEnable(GL_LIGHTING);
            break;
        case SDLK_F3:
            glDisable(GL_LIGHTING);
            break;
        case SDLK_LEFT:
          if (isDown == TRUE) {
            flCameraVelocity[0] = -0.1f;
          } else {
            flCameraVelocity[0] = 0.0f;
          }
            break;
        case SDLK_RIGHT:
          if (isDown == TRUE) {
            flCameraVelocity[0] = 0.1f;
          } else {
            flCameraVelocity[0] = 0.0f;
          }
            break;
        case SDLK_UP:
          if (isDown == TRUE) {
            flCameraVelocity[2] = -0.1f;
          } else {
            flCameraVelocity[2] = 0.0f;
          }

            break;
        case SDLK_DOWN:
          if (isDown == TRUE) {
            flCameraVelocity[2] = 0.1f;
          } else {
            flCameraVelocity[2] = 0.0f;
          }
            break;
        case SDLK_w:
          if (isDown == TRUE) {
            robotOneMoveForward(TRUE);
          } else {
            robotOneMoveForward(FALSE);
          }
          break;
        case SDLK_s:
          if (isDown == TRUE) {
            robotOneMoveBackward(TRUE);
          } else {
            robotOneMoveBackward(FALSE);
          }
          break;
        case SDLK_a:
          robotOneYRot += 10;      
          break;
        case SDLK_d:
          robotOneYRot -= 10;      
          break;
        default:
            break;
    }
}

void mouseMotion(SDL_Event *event) {
  int x, y;

  x = event->motion.xrel;
  y = event->motion.yrel;
  if ((event->motion.x != (SCREEN_WIDTH / 2)) ||
      (event->motion.y != (SCREEN_HEIGHT / 2)) ) { 
  if ((x != 0) || (y != 0)) {
/*
    flCameraPosition[0] += ((GLfloat) x) * 0.1f;
    flCameraPosition[1] += ((GLfloat) y) * 0.1f;
*/
    flCameraAngle[1] += ((GLfloat) x) * 2.0f;
    flCameraPosition[1] += ((GLfloat) y) * 0.1f;
    

    if (flCameraPosition[1] < 2.5f) {
      flCameraPosition[1] = 2.5f;
    } 

    SDL_WarpMouse((SCREEN_WIDTH / 2), (SCREEN_HEIGHT / 2)); 
    drawGLScene(); 
  }
  }
}

void mouseButtonDown(SDL_Event *ev) {
  SDL_MouseButtonEvent *mev = (SDL_MouseButtonEvent *) ev;
  switch(mev->button) {
    case SDL_BUTTON_LEFT:
      break;
    
    case SDL_BUTTON_MIDDLE:
      break;
  
    case SDL_BUTTON_RIGHT:
      break;

    default:
      break;
  }
}


void resizeWindow(int Width, int Height) {
  if (Height == 0) {
    Height = 1;
  }

  glViewport(0, 0, Width, Height);
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();

  gluPerspective(45.0f, (GLfloat)Width/(GLfloat)Height,
                 0.1f, 100.0f);
  glMatrixMode(GL_MODELVIEW);
}

void moveCamera() {
  flCameraPosition[0] += flCameraVelocity[0]; 
  flCameraPosition[1] += flCameraVelocity[1]; 
  flCameraPosition[2] += flCameraVelocity[2]; 

}

void moveRobotOne() {
  robotOnePosition[0] += robotOneVelocity[0]; 
  robotOnePosition[1] += robotOneVelocity[1]; 
  robotOnePosition[2] += robotOneVelocity[2]; 

}

int main(int argc, char **argv) {
  int videoFlags;
  /* int done = FALSE; */
  SDL_Event event;
  SDL_VideoInfo *videoInfo;
  int isActive = TRUE;
  
  done = FALSE;

  if (SDL_Init(SDL_INIT_VIDEO) < 0) {
    printf("*** Error\n");
    exit(1);
  }
 
  videoInfo = (SDL_VideoInfo *) SDL_GetVideoInfo();
  if (!videoInfo) {
    printf("*** Error\n");
  }

  videoFlags = SDL_OPENGL;
  videoFlags |= SDL_GL_DOUBLEBUFFER;
  videoFlags |= SDL_HWPALETTE;
  videoFlags |= SDL_RESIZABLE;

  if (videoInfo->hw_available) {
    videoFlags |= SDL_HWSURFACE;
  } else {
    videoFlags |= SDL_SWSURFACE;
  }

  if (videoInfo->blit_hw) {
    videoFlags |= SDL_HWACCEL;
  }

  SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
  surface = SDL_SetVideoMode(SCREEN_WIDTH, SCREEN_HEIGHT,
                             SCREEN_BPP, videoFlags);

  if (!surface) {
    printf("*** Error\n");
  }

  initGL();

  resizeWindow(SCREEN_WIDTH, SCREEN_HEIGHT);

  while (!done) {
    while(SDL_PollEvent(&event)) {
      switch(event.type) {

        case SDL_ACTIVEEVENT:
          if (event.active.gain == 0) {
            isActive = FALSE;
          } else {
            isActive = TRUE;
          }
          break;

        case SDL_VIDEORESIZE:
          surface = SDL_SetVideoMode(event.resize.w,
                                     event.resize.h,
                                     16, videoFlags);
          if (!surface) {
            printf("*** Error\n");
            exit(1);
          }
          resizeWindow(event.resize.w, event.resize.h);
          break;
          
        case SDL_KEYDOWN:
          keyPressed(&event, TRUE);
          break;
        
        case SDL_KEYUP:
          keyPressed(&event, FALSE);
          break;
        
        case SDL_MOUSEMOTION:
          mouseMotion(&event);
          break;

        case SDL_MOUSEBUTTONDOWN:
          /* mouseButtonDown(event); */
          break;
         
        case SDL_MOUSEBUTTONUP:
          /* mouseButtonUp(event); */
          break;
         
        case SDL_QUIT:
          done = TRUE;
          break;
        default:
          break;
      }
    }          

    moveCamera();
    moveRobotOne();

    if (isActive) {
      drawGLScene();
    }
  }
  /*SDL_Quit(); */
  return 0;
}
