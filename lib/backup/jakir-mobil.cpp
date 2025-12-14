#include "jakir-mobil.h"
#include <GL/glew.h>
#include <GL/glut.h>
#include <cmath>

// Posisi dan rotasi mobil
float mobilX = 0.0f;
float mobilZ = -5.0f;   // letakkan 10 unit ke depan (kamera biasanya menghadap -Z)
float mobilRot = 0.0f;

// --- Objek Primitif 1: Bodi mobil (kubus) ---
void drawCube(float w, float h, float d) {
    glPushMatrix();
    glScalef(w, h, d);
    glutSolidCube(1.0);
    glPopMatrix();
}

// --- Objek Primitif 2: Roda (torus) ---
void drawWheel() {
    glColor3f(0, 0, 0);
    glutSolidTorus(0.1, 0.25, 16, 16);
}

// --- Objek Primitif 3: Kaca depan ---
void drawGlass() {
    glBegin(GL_QUADS);
    glColor3f(0.5f, 0.8f, 1.0f);

    glVertex3f(-0.3f, 0.0f,  0.3f);
    glVertex3f( 0.3f, 0.0f,  0.3f);

    glVertex3f( 0.2f, 0.3f,  0.2f);
    glVertex3f(-0.2f, 0.3f,  0.2f);

    glEnd();
}

// --- Menggambar Mobil ---
void drawMobil() {
    glPushMatrix();
    glTranslatef(mobilX, 0.35f, mobilZ); // naikkan sementara ke 0.6 untuk melihat dengan pasti
    glRotatef(mobilRot, 0, 1, 0);

    // Bodi mobil
    glColor3f(1.0f, 0.0f, 0.0f);
    drawCube(1.2f, 0.4f, 0.6f);

    // Atap
    glPushMatrix();
    glTranslatef(0.0f, 0.35f, 0.0f);
    drawCube(0.7f, 0.3f, 0.6f);
    glPopMatrix();

    // Roda
    glPushMatrix(); glTranslatef( 0.5f, -0.1f,  0.35f); drawWheel(); glPopMatrix();
    glPushMatrix(); glTranslatef( 0.5f, -0.1f, -0.35f); drawWheel(); glPopMatrix();
    glPushMatrix(); glTranslatef(-0.5f, -0.1f,  0.35f); drawWheel(); glPopMatrix();
    glPushMatrix(); glTranslatef(-0.5f, -0.1f, -0.35f); drawWheel(); glPopMatrix();

    // Kaca
    glPushMatrix();
    glTranslatef(0.0f, 0.25f, 0.31f);
    drawGlass();
    glPopMatrix();

    glPopMatrix();
}

// Update posisi mobil
void updateMobil(float speed) {
    float rad = mobilRot * 3.14159f / 180.0f;
    mobilX += sin(rad) * speed;
    mobilZ -= cos(rad) * speed;
}

void initMobil() {
    mobilX = 0.0f;
    mobilZ = -5.0f;
    mobilRot = 0.0f;
}

// --- Standalone Main (hanya aktif jika STANDALONE didefinisikan) ---
#ifdef STANDALONE

void display() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    drawMobil();
    glutSwapBuffers();
}

void handleKeyboard(unsigned char key, int x, int y) {
    if (key == 'w') updateMobil(0.2f);
    if (key == 's') updateMobil(-0.2f);
    if (key == 'a') mobilRot += 4.0f;
    if (key == 'd') mobilRot -= 4.0f;

    glutPostRedisplay();
}

void rotasiKamera(int key, int x, int y) {
    // kosong dulu
}

int main(int argc, char** argv) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH);
    glutInitWindowSize(1000, 700);
    glutCreateWindow("Mobil Standalone");

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(60.0, 1000.0 / 700.0, 0.1, 2000.0);

    glEnable(GL_DEPTH_TEST);

    glutDisplayFunc(display);
    glutKeyboardFunc(handleKeyboard);
    glutSpecialFunc(rotasiKamera);
    glutIdleFunc(display);

    glutMainLoop();
    return 0;
}
#endif
