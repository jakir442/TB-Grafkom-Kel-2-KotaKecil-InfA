#include <GL/freeglut.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "jakir-jalan.h"

// kamera 
float camX = 0.0f, camY = 1.0f, camZ = 5.0f;
float cameraYaw = -90.0f;
float cameraPitch = 0.0f;
float lastMouseX = 0, lastMouseY = 0;
bool firstMouse = true;

float speed = 0.2f;

void getCameraDirection(float &dirX, float &dirY, float &dirZ) {
    dirX = cosf(glm::radians(cameraYaw)) * cosf(glm::radians(cameraPitch));
    dirY = sinf(glm::radians(cameraPitch));
    dirZ = sinf(glm::radians(cameraYaw)) * cosf(glm::radians(cameraPitch));
}

void display() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glLoadIdentity();

    float dirX, dirY, dirZ;
    getCameraDirection(dirX, dirY, dirZ);

    gluLookAt(
        camX, camY, camZ,
        camX + dirX, camY + dirY, camZ + dirZ,
        0, 1, 0
    );

    drawJalan();

    glutSwapBuffers();
}

// WASD untuk translasi
void keyboard(unsigned char key, int x, int y) {
    float forwardX = cos(glm::radians(cameraYaw));
    float forwardZ = sin(glm::radians(cameraYaw));

    float rightX = -forwardZ;
    float rightZ =  forwardX;

    switch (key) {
        case 'w':
            updateJalan(speed * 5);
            break;

        case 's':
            updateJalan(-speed * 5);
            break;

        case 'a':
            camX -= rightX * speed;
            camZ -= rightZ * speed;
            break;

        case 'd':
            camX += rightX * speed;
            camZ += rightZ * speed;
            break;

        case 27:
            exit(0);
    }

    glutPostRedisplay();
}

// Mouse
void mouseMotion(int x, int y) {
    if (firstMouse) {
        lastMouseX = x;
        lastMouseY = y;
        firstMouse = false;
    }

    float offsetX = x - lastMouseX;
    float offsetY = lastMouseY - y;

    lastMouseX = x;
    lastMouseY = y;

    float sensitivity = 0.15f;
    offsetX *= sensitivity;
    offsetY *= sensitivity;

    cameraYaw   += offsetX;
    cameraPitch += offsetY;

    glutPostRedisplay();
}

void init() {
    glEnable(GL_DEPTH_TEST);
    glClearColor(0.3f, 0.6f, 1.0f, 1.0f);

    glMatrixMode(GL_PROJECTION);
    gluPerspective(60.0, 1280.0/720.0, 0.1, 2000.0);

    glMatrixMode(GL_MODELVIEW);
}

int main(int argc, char** argv) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
    glutInitWindowSize(1280, 720);
    glutCreateWindow("Kota Kecil - Jalan Tanpa Batas");

    init();
    initJalan();

    glutDisplayFunc(display);
    glutKeyboardFunc(keyboard);
    glutPassiveMotionFunc(mouseMotion);

    glutMainLoop();
}
