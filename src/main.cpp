#include <GL/freeglut.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <cmath>

#include "header/jakir-jalan.h"
#include "header/putri-mobil.h"
#include "header/azhari-gedung.h"
#include "header/erika-pohon.h"


#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

// float camYaw = 0.0f;
// float camPitch = 0.0f;

bool keyState[256] = { false };

static int lastMouseX = 0;
static int lastMouseY = 0;
static bool firstMouse = true;
static float mouseSensitivity = 0.15f;

static float clampf(float v, float lo, float hi) {
    if (v < lo) return lo;
    if (v > hi) return hi;
    return v;
}

// void keyboardDispatcher(unsigned char key, int x, int y) {
//     controlMobil(key);
//     handleKeyboardGedung(key);
//     glutPostRedisplay();
// }

void keyDown(unsigned char key, int x, int y) {
    keyState[key] = true;

    if (key == 27) exit(0); // ESC
}

void keyUp(unsigned char key, int x, int y) {
    keyState[key] = false;
}


void update() {

    // ===== MOBIL =====
    if (keyState['w'] || keyState['W']) controlMobil('w');
    if (keyState['s'] || keyState['S']) controlMobil('s');
    if (keyState['a'] || keyState['A']) controlMobil('a');
    if (keyState['d'] || keyState['D']) controlMobil('d');

    // ===== GEDUNG =====
    if (keyState['+'] || keyState['=']) handleKeyboardGedung('+');
    if (keyState['-'] || keyState['_']) handleKeyboardGedung('-');

    glutPostRedisplay();
}


void mouseMotion(int x, int y) {
    if (firstMouse) {
        lastMouseX = x;
        lastMouseY = y;
        firstMouse = false;
    }

    int dx = x - lastMouseX;
    int dy = lastMouseY - y;

    lastMouseX = x;
    lastMouseY = y;

    float offsetX = dx * mouseSensitivity;
    float offsetY = dy * mouseSensitivity;

    // camYaw += offsetX;
    // camPitch += offsetY;

    // camPitch = clampf(camPitch, -89.0f, 89.0f);

    glutPostRedisplay();
}

void drawGedungInfinite() {
    const float jarakSamping = 6.0f;   // kiri / kanan jalan
    const float jarakAntar   = 8.0f;   // jarak antar gedung
    const int   jumlahTile   = 14;     // jumlah segmen aktif

    int baseIndex = (int)floor(mobilPosZ / jarakAntar);

    for (int i = baseIndex - jumlahTile / 2;
             i <= baseIndex + jumlahTile / 2; i++) {

        float z = i * jarakAntar;

        // ===== GEDUNG KIRI =====
        glPushMatrix();
            glTranslatef(-jarakSamping, 1.5f, z);
            glScalef( // zoom in/out gedung kiri
                1.5f * scaleFactor,
                1.5f * scaleFactor,
                1.5f * scaleFactor
            );
            drawBuilding();
        glPopMatrix();

        // ===== GEDUNG KANAN =====
        glPushMatrix();
            glTranslatef(jarakSamping, 1.5f, z);
            glRotatef(180, 0, 1, 0);
            glScalef( // zoom in/out gedung kanan
                1.5f * scaleFactor,
                1.5f * scaleFactor,
                1.5f * scaleFactor
            );
            drawBuilding();
        glPopMatrix();
    }
}

void drawTreeInfinite() {
    const float jarakSamping = 3.0f;   // posisi pohon lebih jauh dari jalan
    const float jarakAntar   = 6.0f;  // jarak antar pohon
    const int jumlahTile     = 14;     // jumlah pohon aktif di layar

    int baseIndex = (int)floor(mobilPosZ / jarakAntar);

    for (int i = baseIndex - jumlahTile / 2;
             i <= baseIndex + jumlahTile / 2; i++) {

        float z = i * jarakAntar;

        // ===== POHON KIRI =====
        glPushMatrix();
            glTranslatef(-jarakSamping, 0.0f, z);
            glRotatef(90, 0, 1, 0);   // pohon menghadap jalan
            glScalef(0.8f, 0.8f, 0.8f);
            drawTree();
        glPopMatrix();

        // ===== POHON KANAN =====
        glPushMatrix();
            glTranslatef(jarakSamping, 0.0f, z);
            glRotatef(-90, 0, 1, 0);  // pohon menghadap jalan
            glScalef(0.8f, 0.8f, 0.8f);
            drawTree();
        glPopMatrix();
    }
}

void display() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    // ===== KAMERA FOLLOW DARI BELAKANG (TIDAK IKUT BELOK) =====
    float camX = mobilPosX;
    float camY = 3.0f; // ketinggian kamera dengan mobil
    float camZ = mobilPosZ + 6.0f;   // kedekatan kamera dengan mobil di belakang (sumbu Z)

    gluLookAt(
        camX, camY, camZ,        // posisi kamera
        mobilPosX, 0.0f, mobilPosZ, // lihat ke mobil
        0.0f, 1.0f, 0.0f
    );

    drawJalan();
    drawMobil();
    drawGedungInfinite();
    drawTreeInfinite();


    glutSwapBuffers();
}



void initGL() {
    glEnable(GL_DEPTH_TEST);
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f); // background hitam

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(60.0, 1280.0 / 720.0, 0.1, 2000.0);
}

int main(int argc, char** argv) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
    glutInitWindowSize(1280, 720);
    glutCreateWindow("Kota Kecil");

    initGL();

    // initJalan();   // WAJIB
    initMobil();   // WAJIB
    // initLighting();

    glutDisplayFunc(display);
    // glutKeyboardFunc(keyboardDispatcher);
    glutKeyboardFunc(keyDown);
    glutKeyboardUpFunc(keyUp);
    glutIdleFunc(update);
    glutPassiveMotionFunc(mouseMotion);

    glutMainLoop();
    return 0;
}
