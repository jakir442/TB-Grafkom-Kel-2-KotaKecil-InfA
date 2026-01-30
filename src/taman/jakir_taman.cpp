#include <GL/freeglut.h>
#include <GL/glut.h>
#include <GL/glu.h>
#include <cmath>

#include "../header/taman/jakir_taman.h"
#include "../header/taman/jakir_kolam-pasir.h"
#include "../header/taman/putri_gapura.h"
#include "../header/taman/putri_tong-sampah.h"
#include "../header/taman/assyifa_ayunan.h"
#include "../header/taman/erika_semak.h"
#include "../header/taman/azhari_kursi.h"
#include "../header/taman/rizal_kolam-air.h"
#include "../header/taman/rizal_jungkat-jungkit.h"
#include "../header/taman/rizal_prosotan.h"

#include "../header/erika-pohon.h"

// AIR DI DEPAN JEMBATAN
void drawAirDepan(float x, float z) {
    float panjang = 18.1f;
    float lebar   = 13.0f;
    float yAir    = -0.2f;

    glColor3f(0.1f, 0.4f, 0.7f);

    glBegin(GL_QUADS);
        glVertex3f(x - panjang/2, yAir, z - lebar/2);
        glVertex3f(x + panjang/2, yAir, z - lebar/2);
        glVertex3f(x + panjang/2, yAir, z + lebar/2);
        glVertex3f(x - panjang/2, yAir, z + lebar/2);
    glEnd();
}

// PEMBATAS JEMBATAN
void drawPembatasJembatan(float panjang) {
    float tinggi = 0.40f;
    float tebal  = 0.12f;
    float offset = 1.5f;

    glColor3f(0.45f, 0.45f, 0.45f);

    glPushMatrix();
        glTranslatef(-offset, tinggi/2, 0);
        glScalef(tebal, tinggi, panjang * 0.95f);
        glutSolidCube(1);
    glPopMatrix();

    glPushMatrix();
        glTranslatef(offset, tinggi/2, 0);
        glScalef(tebal, tinggi, panjang * 0.95f);
        glutSolidCube(1);
    glPopMatrix();
}

// BADAN JEMBATAN
void drawJembatanDepan(float x, float y, float z) {
    float panjang = 7.0f;
    float lebar   = 3.0f;
    float tinggi  = 0.22f;

    glPushMatrix();
        glTranslatef(x, y, z);

        glColor3f(0.55f, 0.38f, 0.20f);
        glPushMatrix();
            glScalef(lebar, tinggi, panjang);
            glutSolidCube(1);
        glPopMatrix();

        drawPembatasJembatan(panjang);
    glPopMatrix();
}

// POHON UJUNG JEMBATAN
void drawPohonUjungJembatan(float x, float z) {
    // Ujung belakang jembatan
    glPushMatrix();
        glTranslatef(x - 2.3f, -0.75f, z - 4.2f);
        drawPohon();
    glPopMatrix();

    glPushMatrix();
        glTranslatef(x + 2.3f, -0.75f, z - 4.2f);
        drawPohon();
    glPopMatrix();

    // Ujung depan jembatan
    glPushMatrix();
        glTranslatef(x - 2.3f, -0.75f, z + 4.2f);
        drawPohon();
    glPopMatrix();

    glPushMatrix();
        glTranslatef(x + 2.3f, -0.75f, z + 4.2f);
        drawPohon();
    glPopMatrix();
}

// PEMBATAS AIR
void drawPembatasAir() {
    glColor3f(0.42f, 0.42f, 0.42f);

    glPushMatrix();
        glScalef(0.25f, 2.1f, 6.3f);
        glutSolidCube(1);
    glPopMatrix();
}

void drawJembatanTaman(float x, float z) {
    const float tinggiJembatan = -0.24f;

    drawAirDepan(x, z);
    drawJembatanDepan(x, tinggiJembatan, z);
    drawPohonUjungJembatan(x, z);
}

void drawJalanBatu() {
    // 1. Jalur utama (Z lurus ke belakang)
    for (float z = -11.7f; z > -29.0f; z -= 1.5f) {
        glPushMatrix();
        glTranslatef(0.0f, 0.01f, z);

        glColor3f(0.5f, 0.5f, 0.5f);
        glPushMatrix();
        glScalef(2.0f, 0.08f, 1.2f); // Batu lebih kecil
        glutSolidCube(1.0f);
        glPopMatrix();

        glColor3f(0.3f, 0.3f, 0.3f);
        glPushMatrix();
        glScalef(2.01f, 0.09f, 1.21f);
        glutWireCube(1.0f);
        glPopMatrix();

        glPopMatrix();
    }

    // 2. Jalur kiri (X negatif)
    for (float x = -1.9f; x > -8.0f; x -= 1.5f) {
        glPushMatrix();
        glTranslatef(x, 0.01f, -20.0f); // Z tetap persimpangan

        glColor3f(0.5f, 0.5f, 0.5f);
        glPushMatrix();
        glScalef(1.2f, 0.08f, 2.0f);
        glutSolidCube(1.0f);
        glPopMatrix();

        glColor3f(0.3f, 0.3f, 0.3f);
        glPushMatrix();
        glScalef(1.21f, 0.09f, 2.01f);
        glutWireCube(1.0f);
        glPopMatrix();

        glPopMatrix();
    }
}

void drawSemakKeliling() {
    const float scale = 0.55f;
    const float jarak = 2.4f;

    // ---------------- DEPAN TAMAN (Z = -10) ----------------
    for (float x = -9.0f; x <= 9.0f; x += jarak) {
        // kosongkan area gapura & jalan
        if (x > -2.5f && x < 2.5f) continue;

        glPushMatrix();
            glTranslatef(x, 0.0f, -10.0f);
            glScalef(scale, scale, scale);
            bush();
        glPopMatrix();
    }

    // ---------------- BELAKANG TAMAN (Z = -30) ----------------
    for (float x = -9.0f; x <= 9.0f; x += jarak) {
        glPushMatrix();
            glTranslatef(x, 0.0f, -30.0f);
            glScalef(scale, scale, scale);
            bush();
        glPopMatrix();
    }

    // ---------------- KIRI TAMAN (X = -10) ----------------
    for (float z = -28.0f; z <= -12.0f; z += jarak) {
        glPushMatrix();
            glTranslatef(-10.0f, 0.0f, z);
            glRotatef(90, 0, 1, 0);
            glScalef(scale, scale, scale);
            bush();
        glPopMatrix();
    }

    // ---------------- KANAN TAMAN (X = 10) ----------------
    for (float z = -28.0f; z <= -12.0f; z += jarak) {
        glPushMatrix();
            glTranslatef(10.0f, 0.0f, z);
            glRotatef(90, 0, 1, 0);
            glScalef(scale, scale, scale);
            bush();
        glPopMatrix();
    }
}


void drawTaman() {
    // Lantai rumput
    glColor3f(0.2f, 0.8f, 0.2f);
    glBegin(GL_QUADS);
        glVertex3f(-10, 0, -30);
        glVertex3f( 10, 0, -30);
        glVertex3f( 10, 0, -10);
        glVertex3f(-10, 0, -10);
    glEnd();

    // Gapura
    glPushMatrix();
        glTranslatef(-9.0f, 0.0f, 6.7f); // depan taman
        glScalef(0.8f, 0.8f, 0.8f);
        glRotatef(-90, 0, 1, 0);
        drawGate();
    glPopMatrix();

    // semak kiri depan
    glPushMatrix();
        glTranslatef(-8.7f, 0.0f, -25.5f);
        glScalef(0.6f, 0.6f, 1.2f);
        glRotatef(-90, 0, 1, 0);
        bush();
    glPopMatrix();

    // semak kiri belakang
    glPushMatrix();
        glTranslatef(-8.7f, 0.0f, -14.0f);
        glScalef(0.6f, 0.6f, 1.2f);
        glRotatef(-90, 0, 1, 0);
        bush();
    glPopMatrix();

    // semak Z+ Kiri
    glPushMatrix();
        glTranslatef(-5.5f, 0.0f, -3.0f);
        glScalef(1.0f, 0.6f, 1.1f);
        glRotatef(-180, 0, 1, 0);
        bush();
    glPopMatrix();

    // semak Z- Kiri
    glPushMatrix();
        glTranslatef(-5.5f, 0.0f, -10.0f);
        glScalef(1.0f, 0.6f, 1.1f);
        glRotatef(-180, 0, 1, 0);
        bush();
    glPopMatrix();

    // semak Z+ Kanan
    glPushMatrix();
        glTranslatef(5.5f, 0.0f, -3.0f);
        glScalef(1.1f, 0.6f, 1.0f);
        glRotatef(-180, 0, 1, 0);
        bush();
    glPopMatrix();

    // semak Z- kanan
    glPushMatrix();
        glTranslatef(5.5f, 0.0f, -10.0f);
        glScalef(1.1f, 0.6f, 1.0f);
        glRotatef(-180, 0, 1, 0);
        bush();
    glPopMatrix();

    // semak kanan
    glPushMatrix();
        glTranslatef(10.0f, 0.0f, 6.0f);
        glScalef(1.1f, 0.6f, 3.4f);
        glRotatef(-90, 0, 1, 0);
        bush();
    glPopMatrix();

    // kolam pasir
    glPushMatrix();
        glTranslatef(0.0, -0.1, 0.0); // posisi di taman
        
        // kolam pasir anak-anak
        drawKolam(5.0f, 1.7f);
    glPopMatrix();

    // tong sampah
    glPushMatrix();
        glTranslatef(-7.0, 0, -14.5); // posisi satu SET
        glRotatef(85, 0, 1, 0);
        drawTrashSetOrganik();
        drawTrashSetAnorganik();
        drawTrashSetB3();
    glPopMatrix();

    // jungkat jungkit
    glPushMatrix();
        glTranslatef(7.0, 0.6, -20); // posisi di taman
        glRotatef(-20, 0, 1, 0);
        jungkat_jungkit();
    glPopMatrix();

    // prosotan
    glPushMatrix();
        glTranslatef(7.5f, 0.0f, -15.5f);
        glRotatef(30, 0, 1, 0);
        drawPerosotan();
    glPopMatrix();

    // ayunan
    glPushMatrix();
        glTranslatef(5.5, 3, -13); // posisi ayunan di taman
        ayunan();
    glPopMatrix();

    // --- POSISI KOLAM BARU ---
    glPushMatrix();
        glTranslatef(-5.0f, 3.1f, 2.5f);
        kolam_air();
    glPopMatrix();

    // Panggil jalan batu
    drawJalanBatu();

    // kursi pertama
    glPushMatrix();
        glTranslatef(-5.0f, 0.4f, -18.0f);
        glScalef(0.5f, 0.5f, 0.5f);
        kursiTaman();
    glPopMatrix();

    // kursi kedua
    glPushMatrix();
        glTranslatef(-5.0f, 0.4f, -12.0f);
        glRotatef(180, 0, 1, 0);
        glScalef(0.5f, 0.5f, 0.5f);
        kursiTaman();
    glPopMatrix();
}

// AKSES PRIVATE
#ifdef STANDALONE 
void display() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    // Kamera sederhana
    gluLookAt(
        0.0f, 8.0f, 25.0f,   // posisi kamera
        0.0f, 0.0f, -20.0f,  // arah pandang
        0.0f, 1.0f, 0.0f
    );

    drawTaman();

    glutSwapBuffers();
}

void initGL() {
    glEnable(GL_DEPTH_TEST);

    glClearColor(
        0.53f, 0.81f, 0.92f, 1.0f   // langit biru
    );

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(
        60.0,
        800.0 / 600.0,
        0.1,
        1000.0
    );
}

int main(int argc, char** argv) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
    glutInitWindowSize(800, 600);
    glutCreateWindow("Taman dengan Bangku");

    initGL();

    glutDisplayFunc(display);
    glutIdleFunc(display);

    glutMainLoop();
    return 0;
}
#endif
