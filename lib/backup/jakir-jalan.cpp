#include <GL/freeglut.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

// variabel global
float jalanOffset = 0.0f;

// kamera
glm::vec3 camPos  = glm::vec3(0.0f, 3.0f, 15.0f);
glm::vec3 camFront = glm::vec3(0.0f, 0.0f, -1.0f);
glm::vec3 camUp    = glm::vec3(0.0f, 1.0f, 0.0f);

float camYaw   = 0.0f;
float camPitch = 0.0f;


float moveSpeed = 0.4f;
float rotSpeed  = 2.0f;

void updateCameraDirection() {
    glm::vec3 dir;
    dir.x = sin(glm::radians(camYaw)) * cos(glm::radians(camPitch));
    dir.y = sin(glm::radians(camPitch));
    dir.z = cos(glm::radians(camYaw)) * cos(glm::radians(camPitch));

    // camFront = glm::normalize(dir);
}


void initJalan() {}


void updateJalan(float speed) {
    jalanOffset += speed;
}

// gambar jalan tanpa batas
void drawJalan() {
    float visibleDist = 300.0f;
    float start = -visibleDist;
    float end   = visibleDist;

    float baseShift = fmod(jalanOffset, 10.0f);

    float roadWidth  = 4.0f;
    float roadHalf   = roadWidth / 2.0f;
    float thickness  = 0.30f;
    float curbWidth  = 0.30f;
    float curbOffset = 0.0001f;

    for (float z = start; z < end; z += 10.0f) {

        float z1 = z + baseShift;
        float z2 = z1 + 10.0f;

        // body/badan jalan
        glColor3f(0.15f, 0.15f, 0.15f);
        glBegin(GL_QUADS);
        glVertex3f(-roadHalf, 0.0f, z1);
        glVertex3f( roadHalf, 0.0f, z1);
        glVertex3f( roadHalf, 0.0f, z2);
        glVertex3f(-roadHalf, 0.0f, z2);
        glEnd();

        // samping 3D
        glColor3f(0.10f, 0.10f, 0.10f);
        glBegin(GL_QUADS);

        // kiri
        glVertex3f(-roadHalf, -thickness, z1);
        glVertex3f(-roadHalf,  0.0f,      z1);
        glVertex3f(-roadHalf,  0.0f,      z2);
        glVertex3f(-roadHalf, -thickness, z2);

        // kanan
        glVertex3f( roadHalf, -thickness, z1);
        glVertex3f( roadHalf,  0.0f,      z1);
        glVertex3f( roadHalf,  0.0f,      z2);
        glVertex3f( roadHalf, -thickness, z2);

        glEnd();

        // trotoar
        glColor3f(0.32f, 0.32f, 0.32f);
        glBegin(GL_QUADS);

        // kiri
        glVertex3f(-(roadHalf + curbWidth), 0.0f, z1);
        glVertex3f(-roadHalf + curbOffset,  0.0f, z1);
        glVertex3f(-roadHalf + curbOffset,  0.0f, z2);
        glVertex3f(-(roadHalf + curbWidth), 0.0f, z2);

        // kanan
        glVertex3f( roadHalf - curbOffset,  0.0f, z1);
        glVertex3f( roadHalf + curbWidth,   0.0f, z1);
        glVertex3f( roadHalf + curbWidth,   0.0f, z2);
        glVertex3f( roadHalf - curbOffset,  0.0f, z2);

        glEnd();

        // garis putuh-putus warna kuning
        glColor3f(1, 1, 0);
        glLineWidth(4);
        glBegin(GL_LINES);
        glVertex3f(0, 0.02f, z1 + 3);
        glVertex3f(0, 0.02f, z1 + 6);
        glEnd();
    }
}

// update kamera (lookAt)
void updateCamera() {
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    glm::vec3 lookAtPoint = camPos + camFront;

    gluLookAt(
        camPos.x, camPos.y, camPos.z,
        lookAtPoint.x, lookAtPoint.y, lookAtPoint.z,
        camUp.x, camUp.y, camUp.z
    );
}

// translasi (W A S D)
void handleKeyboard(unsigned char key, int x, int y) {
    glm::vec3 right = glm::normalize(glm::cross(camFront, camUp));

    switch (key) {

    case 'w':   // maju
        camPos += camFront * moveSpeed;
        updateJalan(7.5f);
        break;

    case 's':   // mundur
        camPos -= camFront * moveSpeed;
        updateJalan(-7.5f);
        break;

    case 'a':   // kiri
        camPos -= right * moveSpeed;
        break;

    case 'd':   // kanan
        camPos += right * moveSpeed;
        break;

    case 27:
        exit(0);
    }

    glutPostRedisplay();
}

// keyboard untuk tombol kiri,kanan,atas,bawah (rotasi kamera)
void rotasiKamera(int key, int x, int y) {
    switch (key) {
    case GLUT_KEY_LEFT:
        camYaw -= rotSpeed;
        break;

    case GLUT_KEY_RIGHT:
        camYaw += rotSpeed;
        break;

    case GLUT_KEY_UP:
        camPitch += rotSpeed;
        if (camPitch > 89) camPitch = 89;
        break;

    case GLUT_KEY_DOWN:
        camPitch -= rotSpeed;
        if (camPitch < -89) camPitch = -89;
        break;
    }

    updateCameraDirection();
    glutPostRedisplay();
}


#ifdef STANDALONE
// display
void display() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    updateCamera();
    drawJalan();

    glutSwapBuffers();
}

int main(int argc, char** argv) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH);
    glutInitWindowSize(1000, 700);
    glutCreateWindow("Jalan Tak Berujung + Kamera Bebas");

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(60.0, 1000.0 / 700.0, 0.1, 2000.0);

    glEnable(GL_DEPTH_TEST);

    updateCameraDirection();
    glutDisplayFunc(display);
    glutKeyboardFunc(handleKeyboard);
    glutSpecialFunc(rotasiKamera);

    glutIdleFunc(display);

    glutMainLoop();
    return 0;
}
#endif