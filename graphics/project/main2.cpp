#include <stdlib.h>
#include <iostream>
// OSX systems need their own headers
#ifdef __APPLE__
#include <OpenGL/gl3.h>
#include <OpenGL/glext.h>
#include <GLUT/glut.h>
#else
#include <GL/glew.h>
#include <GL/glut.h>
#endif
// Use of degrees is deprecated. Use radians for GLM functions
#define GLM_FORCE_RADIANS
#include <glm/glm.hpp>
#include "Screenshot.h"
#include "Scene.h"


static const int width = 800;
static const int height = 600;
static const char* title = "Scene viewer";
static const glm::vec4 background(0.1f, 0.2f, 0.3f, 1.0f);
static Scene scene;

#include "hw3AutoScreenshots.h"

void printHelp(){
    std::cout << R"(
    Available commands:
      press 'H' to print this message again.
      press Esc to quit.
      press 'P' to save a screenshot.
      press the arrow keys to rotate camera/light.
      press 'A'/'Z' to zoom camera/light.
      press ','/'.' to adjust FOV of camera/light.
      press 'R' to reset scene.
      press 'M' to cycle mode to camera/light.
    
      press Spacebar to generate images.
    
)";
}

void initialize(void){
    printHelp();
    glClearColor(background[0], background[1], background[2], background[3]); // background color
    glViewport(0,0,width,height);
    
    // Initialize scene
    scene.init();

    // Enable depth test
    glEnable(GL_DEPTH_TEST);
}

/** 
    Display method:
    // 1: depth pass
	bind frame buffer
	call glClear()
	call glViewport(shadow map's dimensions)
	call scene.draw(your depth shader)
	unbind frame buffer

	// 2: color pass
	call glClear()
	call glViewport(screen's dimensions)
	activate GL's texture slot 0
	bind shadow map texture to activated texture slot
	call scene.draw(your surface shader)

	do buffer swaps
*/
void display(void){

    Light* li = scene.lightzero;
    glViewport(0, 0, li->SHADOW_WIDTH, li->SHADOW_HEIGHT);
    glBindFramebuffer(GL_FRAMEBUFFER, li->depthMapFBO);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    scene.drawDepth();   // pass 1
    glBindFramebuffer(GL_FRAMEBUFFER, 0);

    glViewport(0, 0, width, height);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, li->depthMap);
    scene.drawMain();           // pass 2
    
    glutSwapBuffers();
    glFlush();
}

void saveScreenShot(const char* filename = "test.png"){
    int currentwidth = glutGet(GLUT_WINDOW_WIDTH);
    int currentheight = glutGet(GLUT_WINDOW_HEIGHT);
    Screenshot imag = Screenshot(currentwidth,currentheight);
    imag.save(filename);
}

void keyboard(unsigned char key, int x, int y){
    switch(key){
        case 27: // Escape to quit
            exit(0);
            break;
        case 'h': // print help
            printHelp();
            break;
        case 'p': // save screenshot
            saveScreenShot();
            break;
        case 'r':
            scene.camera -> aspect_default = float(glutGet(GLUT_WINDOW_WIDTH))/float(glutGet(GLUT_WINDOW_HEIGHT));
            scene.camera -> reset();
            scene.lightzero -> reset();
            glutPostRedisplay();
            break;
        case 'a':
            scene.main_camera -> zoom(0.9f);
            glutPostRedisplay();
            break;
        case 'z':
            scene.main_camera-> zoom(1.1f);
            glutPostRedisplay();
            break;
        case 'm':
            scene.cycleMode();
            glutPostRedisplay();
            break;
        case ',':
            scene.main_camera->zoomfov(0.9f);
            glutPostRedisplay();
            break;
        case '.':
            scene.main_camera->zoomfov(1.1f);
            glutPostRedisplay();
            break;
        case ' ':
            hw3AutoScreenshots();
            glutPostRedisplay();
            break;
        default:
            glutPostRedisplay();
            break;
        /*
        * Possible move controls
        case 'i':
            scene.lightMain->move(glm::vec3(0.0f, 0.1f, 0.0f));
            glutPostRedisplay();
            break;
        case 'k':
            scene.lightMain->move(glm::vec3(0.0f, -0.1f, 0.0f));
            glutPostRedisplay();
            break;
        case 'j':
            scene.lightMain->move(glm::vec3(-0.1f, 0.0f, 0.0f));
            glutPostRedisplay();
            break;
        case 'l':
            scene.lightMain->move(glm::vec3(0.1f, 0.0f, 0.0f));
            glutPostRedisplay();
            break;
        case 'u':
            scene.lightMain->move(glm::vec3(0.0f, 0.0f, -0.1f));
            glutPostRedisplay();
            break;
        case 'o':
            scene.lightMain->move(glm::vec3(0.0f, 0.0f, 0.1f));
            glutPostRedisplay();
            break;
        */
    }
}
void specialKey(int key, int x, int y){
    switch (key) {
        case GLUT_KEY_UP: // up
            scene.main_camera-> rotateUp(-10.0f);
            glutPostRedisplay();
            break;
        case GLUT_KEY_DOWN: // down
            scene.main_camera-> rotateUp(10.0f);
            glutPostRedisplay();
            break;
        case GLUT_KEY_RIGHT: // right
            scene.main_camera-> rotateRight(-10.0f);
            glutPostRedisplay();
            break;
        case GLUT_KEY_LEFT: // left
            scene.main_camera-> rotateRight(10.0f);
            glutPostRedisplay();
            break;
    }
}

int main2(int argc, char** argv)
{
    // BEGIN CREATE WINDOW
    glutInit(&argc, argv);
    
#ifdef __APPLE__
    glutInitDisplayMode( GLUT_3_2_CORE_PROFILE | GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH);
#else
    glutInitDisplayMode( GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH );
#endif
    glutInitWindowSize(width, height);
    glutCreateWindow(title);
#ifndef __APPLE__
    glewExperimental = GL_TRUE;
    GLenum err = glewInit() ;
    if (GLEW_OK != err) {
        std::cerr << "Error: " << glewGetErrorString(err) << std::endl;
    }
#endif
    std::cout << "OpenGL Version: " << glGetString(GL_VERSION) << std::endl;
    // END CREATE WINDOW
    
    initialize();
    glutDisplayFunc(display);
    glutKeyboardFunc(keyboard);
    glutSpecialFunc(specialKey);
    
    glutMainLoop();
	return 0;   /* ANSI C requires main to return int. */
}
