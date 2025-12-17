#include <GL/freeglut.h> // fungsi utama window OpenGL
#include <cmath> // fungsi matematika untuk floor

#include "header/jakir-jalan.h"
#include "header/putri-mobil.h"
#include "header/azhari-gedung.h"
#include "header/erika-pohon.h"
#include "header/jakir-lampu.h"

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

// =====================
// KONFIGURASI LINGKUNGAN
// =====================
const float JARAK_TILE      = 8.0f;   // satu segmen dunia
const int   JUMLAH_TILE     = 14;     // segmen aktif

const float LAMPU_X         = 3.0f;
const float POHON_X         = 4.5f;
const float GEDUNG_X        = 7.0f;


bool keyState[256] = { false };

// static int lastMouseX = 0;
// static int lastMouseY = 0;
// static bool firstMouse = true;
// static float mouseSensitivity = 0.15f;


// void handleKeyboard(unsigned char key, int x, int y) {
//     controlMobil(key);
//     handleKeyboardGedung(key);
//     glutPostRedisplay();
// }

void keyDown(unsigned char key, int x, int y) {
    keyState[key] = true;

    keyboardLampu(key);

    if (key == 27) exit(0); // ESC
}

void keyUp(unsigned char key, int x, int y) {
    keyState[key] = false;
}


void update() {
    // mobil (translasi)
    if (keyState['w'] || keyState['W']) controlMobil('w');
    if (keyState['s'] || keyState['S']) controlMobil('s');
    if (keyState['a'] || keyState['A']) controlMobil('a');
    if (keyState['d'] || keyState['D']) controlMobil('d');

    // gedung (scaling)
    if (keyState['+'] || keyState['=']) handleKeyboardGedung('+');
    if (keyState['-'] || keyState['_']) handleKeyboardGedung('-');

    glutPostRedisplay();
}

void drawGedungInfinite() {
    int baseIndex = (int)floor(mobilPosZ / JARAK_TILE);

    for (int i = baseIndex - JUMLAH_TILE / 2;
             i <= baseIndex + JUMLAH_TILE / 2; i++) {

        float z = i * JARAK_TILE;

        // kiri
        glPushMatrix();
            glTranslatef(-GEDUNG_X, 1.5f, z);
            glScalef(1.5f * scaleFactor,
                     1.5f * scaleFactor,
                     1.5f * scaleFactor);
            drawGedung();
        glPopMatrix();

        // kanan
        glPushMatrix();
            glTranslatef( GEDUNG_X, 1.5f, z);
            glRotatef(180, 0, 1, 0);
            glScalef(1.5f * scaleFactor,
                     1.5f * scaleFactor,
                     1.5f * scaleFactor);
            drawGedung();
        glPopMatrix();
    }
}


void drawPohonInfinite() {
    int baseIndex = (int)floor(mobilPosZ / JARAK_TILE);

    for (int i = baseIndex - JUMLAH_TILE / 2;
             i <= baseIndex + JUMLAH_TILE / 2; i++) {

        float z = i * JARAK_TILE;

        // kiri
        glPushMatrix();
            glTranslatef(-POHON_X, 0.0f, z);
            glRotatef(90, 0, 1, 0);
            glScalef(0.8f, 0.8f, 0.8f);
            drawPohon();
        glPopMatrix();

        // kanan
        glPushMatrix();
            glTranslatef( POHON_X, 0.0f, z);
            glRotatef(-90, 0, 1, 0);
            glScalef(0.8f, 0.8f, 0.8f);
            drawPohon();
        glPopMatrix();
    }
}


void drawLampuInfinite() {
    int baseIndex = (int)floor(mobilPosZ / JARAK_TILE);

    for (int i = baseIndex - JUMLAH_TILE / 2;
             i <= baseIndex + JUMLAH_TILE / 2; i++) {

        float z = i * JARAK_TILE;

        // ===== LAMPU KIRI (MENGHADAP KE JALAN) =====
        glPushMatrix();
            glTranslatef(-LAMPU_X, 0.0f, z);
            glRotatef( 90, 0, 1, 0);   // hadap ke +X (ke jalan)
            drawLampuJalan(0.0f, 0.0f, GL_LIGHT0);
        glPopMatrix();

        // ===== LAMPU KANAN (MENGHADAP KE JALAN) =====
        glPushMatrix();
            glTranslatef( LAMPU_X, 0.0f, z);
            glRotatef(-90, 0, 1, 0);   // hadap ke -X (ke jalan)
            drawLampuJalan(0.0f, 0.0f, GL_LIGHT1);
        glPopMatrix();
    }
}


void display() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    //  kamera ikut dari belakang (tapi tidak ikut belok) 
    float camX = mobilPosX;
    float camY = 3.0f; // ketinggian kamera dengan mobil
    float camZ = mobilPosZ + 6.0f;   // kedekatan kamera dengan mobil di belakang (sumbu Z)

    gluLookAt(
        camX, camY, camZ,        // posisi kamera
        mobilPosX, 0.0f, mobilPosZ, // lihat ke mobil
        0.0f, 1.0f, 0.0f
    );

    drawJalan();           // dasar
    drawLampuInfinite();   // dekat jalan
    drawPohonInfinite();   // transisi
    drawGedungInfinite();  // background
    drawMobil();           // terakhir (fokus utama)


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

    initMobil();
    initLampuLighting();

    glutDisplayFunc(display);
    // glutKeyboardFunc(handleKeyboard);
    glutKeyboardFunc(keyDown);
    glutKeyboardUpFunc(keyUp);
    glutIdleFunc(update);

    glutMainLoop();
    return 0;
}
