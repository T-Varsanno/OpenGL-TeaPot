#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <GL/glut.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <vector>
#include <cstdlib>
#include <cstdio>
#include <iostream>
#include "globals.h"
#include "drawFunctions.h"
#include "cameraUtils.h"
#include "renderUtils.h"
#include "keyCallback.h"


// Function declarations
void setupScene();

//void mouseButtonCallback(int button, int state, int x, int y);


std::vector<glm::vec3> points;


int main(int argc, char** argv) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
    glutInitWindowSize(WIDTH, HEIGHT);
    glutCreateWindow("OpenGL Project");

    GLenum err = glewInit();
    if (err != GLEW_OK) {
        std::cerr << "Error: GLEW initialization failed: " << glewGetErrorString(err) << std::endl;
        return -1;
    }

    setupScene();
    

    glutDisplayFunc(renderScene);

    //handling keypresses for glut input
    glutKeyboardFunc(updateKeyState);
    glutKeyboardUpFunc(updateKeyUp);
    glutSpecialFunc(updateSpecialKeyState);
    glutSpecialUpFunc(updateSpecialKeyUp);

    // Set the idle callback to continuously update the camera
    glutIdleFunc([]() {
        updateCamera();
        glutPostRedisplay(); // Request a redraw after updating the camera
        });

    glutMainLoop();

    return 0;
}

void setupScene() {
    glEnable(GL_DEPTH_TEST);

    // Enable lighting
    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);

    // Define light properties
    GLfloat lightPos[] = { 1.0f, 1.0f, 1.0f, 0.0f };
    GLfloat lightAmbient[] = { 0.2f, 0.2f, 0.2f, 1.0f };
    GLfloat lightDiffuse[] = { 0.8f, 0.8f, 0.8f, 1.0f };
    GLfloat lightSpecular[] = { 1.0f, 1.0f, 1.0f, 1.0f };

    glLightfv(GL_LIGHT0, GL_POSITION, lightPos);
    glLightfv(GL_LIGHT0, GL_AMBIENT, lightAmbient);
    glLightfv(GL_LIGHT0, GL_DIFFUSE, lightDiffuse);
    glLightfv(GL_LIGHT0, GL_SPECULAR, lightSpecular);

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(45.0, 800.0 / 600.0, 0.1, 100.0);
    glMatrixMode(GL_MODELVIEW);

    glClearColor(0.2f, 0.2f, 0.2f, 1.0f);
}

