#include <GL/freeglut.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <cmath>

// Kamera 
float camX = 0.0f, camY = 1.0f, camZ = 5.0f;
float cameraYaw = -90.0f; // (-) lihat ke depan, (+) lihat ke belakang
float cameraPitch = 0.0f; // (-) lihat ke bawah, (+) lihat ke atas
float lastMouseX = 0, lastMouseY = 0;
bool firstMouse = true;
float roadOffset = 0.0f;
float speed = 0.2f;

// hitung arah kamera
void getCameraDirection(float &dirX, float &dirY, float &dirZ) {
    dirX = cosf(glm::radians(cameraYaw)) * cosf(glm::radians(cameraPitch));
    dirY = sinf(glm::radians(cameraPitch));
    dirZ = sinf(glm::radians(cameraYaw)) * cosf(glm::radians(cameraPitch));
}

// jalan tanpa batas
void drawRoad() {
    glColor3f(0.2, 0.2, 0.2);

    float visibleDist = 300.0f;
    float start = -visibleDist;
    float end   =  visibleDist;

    // offset agar tidak ada ujung jalan
    float baseShift = fmod(roadOffset, 10.0f);

    glBegin(GL_QUADS);
    for (float z = start; z < end; z += 10.0f) {
        float zShift = z + baseShift;

        glVertex3f(-2.0f, 0.0f, zShift);
        glVertex3f( 2.0f, 0.0f, zShift);
        glVertex3f( 2.0f, 0.0f, zShift + 10);
        glVertex3f(-2.0f, 0.0f, zShift + 10);
    }
    glEnd();

    // garis putus-putus dengan warna kuning
    glColor3f(1, 1, 0);
    glLineWidth(3);

    glBegin(GL_LINES);
    for (float z = start; z < end; z += 10.0f) {
        float zShift = z + baseShift;

        glVertex3f(0, 0.01f, zShift + 3);
        glVertex3f(0, 0.01f, zShift + 6);
    }
    glEnd();
}

void display() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glLoadIdentity();

    float dirX, dirY, dirZ;
    getCameraDirection(dirX, dirY, dirZ);

    // Kamera arah sesuai mouse
    gluLookAt(
        camX, camY, camZ,
        camX + dirX, camY + dirY, camZ + dirZ,
        0, 1, 0
    );

    drawRoad();

    glutSwapBuffers();
}

// WASD pergerakan
void keyboard(unsigned char key, int x, int y) {
    float forwardX = cos(glm::radians(cameraYaw));
    float forwardZ = sin(glm::radians(cameraYaw));

    // kanan vector
    float rightX = -forwardZ;
    float rightZ =  forwardX;

    switch (key) {

        case 'w':  // maju
            roadOffset += speed * 5;
            break;

        case 's':  // mundur
            roadOffset -= speed * 5;
            break;

        case 'a':  // geser kiri
            camX -= rightX * speed;
            camZ -= rightZ * speed;
            break;

        case 'd':  // geser kanan
            camX += rightX * speed;
            camZ += rightZ * speed;
            break;

        case 27:
            exit(0);
    }

    glutPostRedisplay();
}


// penglihatan mouse 360° 
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

    cameraYaw += offsetX;
    cameraPitch += offsetY;

    // if (cameraPitch > 89.0f) cameraPitch = 89.0f;
    // if (cameraPitch < -89.0f) cameraPitch = -89.0f;

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
    glutCreateWindow("Infinite Road - FreeGLUT");

    init();

    glutDisplayFunc(display);
    glutKeyboardFunc(keyboard);
    glutPassiveMotionFunc(mouseMotion);

    glutMainLoop();
    return 0;
}
