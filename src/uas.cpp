#include <GL/freeglut.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <cmath>

// ---------- Kamera ----------
float camX = 0.0f, camY = 1.0f, camZ = 5.0f;
float yaw = -90.0f;
float pitch = 0.0f;
float lastX = 400, lastY = 300;
bool firstMouse = true;
float roadOffset = 0.0f;

float speed = 0.2f;

// ---------- Hitung arah kamera ----------
void getCameraDirection(float &dirX, float &dirY, float &dirZ) {
    dirX = cosf(glm::radians(yaw)) * cosf(glm::radians(pitch));
    dirY = sinf(glm::radians(pitch));
    dirZ = sinf(glm::radians(yaw)) * cosf(glm::radians(pitch));
}

// ---------- Infinite Road ----------
void drawRoad() {
    glColor3f(0.2, 0.2, 0.2);

    float visibleDist = 300.0f;
    float start = -visibleDist;
    float end   =  visibleDist;

    // offset supaya tidak ada ujung jalan
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

    // Garis putus-putus
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

// ---------- WASD Movement BENAR ----------
void keyboard(unsigned char key, int x, int y) {
    float forwardX = cos(glm::radians(yaw));
    float forwardZ = sin(glm::radians(yaw));

    // right vector
    float rightX = -forwardZ;
    float rightZ =  forwardX;

    switch (key) {

        case 'w':  // maju
            roadOffset += speed * 5;   // jalan bergerak mundur
            break;

        case 's':  // mundur
            roadOffset -= speed * 5;   // jalan bergerak naik
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


// ---------- Mouse Look 360° ----------
void mouseMotion(int x, int y) {
    if (firstMouse) {
        lastX = x;
        lastY = y;
        firstMouse = false;
    }

    float offsetX = x - lastX;
    float offsetY = lastY - y;
    lastX = x;
    lastY = y;

    float sensitivity = 0.15f;
    offsetX *= sensitivity;
    offsetY *= sensitivity;

    yaw += offsetX;
    pitch += offsetY;

    if (pitch > 89.0f) pitch = 89.0f;
    if (pitch < -89.0f) pitch = -89.0f;

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
