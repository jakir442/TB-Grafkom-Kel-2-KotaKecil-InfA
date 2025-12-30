// File: gedung3d.cpp
// Compile: g++ gedung3d.cpp -o gedung3d -lGL -lGLU -lglut
#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glut.h>
#include <cstdio>

#include "header/azhari-gedung.h"


// Variabel transformasi objek
float angleX = 0.0f;    // Rotasi terhadap sumbu X (derajat)
float angleY = 0.0f;   // Rotasi terhadap sumbu Y (derajat)
float scaleFactor = 1.0f; // Faktor skala objek

// Konstanta kontrol transformasi
float ROT_STEP   = 5.0f;   // Besar perubahan rotasi
float SCALE_STEP = 0.1f;   // Besar perubahan skala
float SCALE_MIN  = 0.1f;   // Skala minimum
float SCALE_MAX  = 10.0f;  // Skala maksimum

// Warna material objek
GLfloat wallColor[]   = {0.85f, 0.9f, 0.95f, 1.0f}; // Warna dinding gedung
GLfloat windowColor[] = {0.2f, 0.6f, 1.0f, 1.0f};  // Warna jendela
GLfloat doorColor[]   = {1.0f, 0.45f, 0.0f, 1.0f}; // Warna pintu

void initRendering() {
    // Mengaktifkan depth test agar objek memiliki kedalaman
    glEnable(GL_DEPTH_TEST);

    // Warna latar belakang
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
}

//  FUNGSI PRIMITIF OBJEK 3D

// Kubus satuan sebagai dasar pemodelan

void drawUnitCube() {
    glutSolidCube(1.0f);
}

// Balok (prisma segi empat) dengan ukuran tertentu
void Prisma(float width, float height, float depth) {
    glPushMatrix();
    glScalef(width, height, depth);
    drawUnitCube();
    glPopMatrix();
}

// Fungsi jendela Depan
void jendelaDepan(float x, float y) {
    glPushMatrix();
    glTranslatef(x, y, 0.63f);   // Posisi di depan gedung
    glScalef(0.60f, 0.45f, 0.02f); // Ukuran jendela
    glutSolidCube(1.0f);
    glPopMatrix();
}

// Fungsi jendela Belakang
void jendelaBelakang(float x, float y) {
    glPushMatrix();
    glTranslatef(x, y, -0.63f);   // Posisi di depan gedung
    glScalef(0.60f, 0.45f, 0.02f); // Ukuran jendela
    glutSolidCube(1.0f);
    glPopMatrix();
}

void Tabung() {
    GLUquadric* quad = gluNewQuadric();

    glPushMatrix();
    // Posisi cerobong di atas atap (kanan)
    glTranslatef(0.0f, 1.50f, 0.0f);
    // Putar agar tabung berdiri ke atas
    glRotatef(-90.0f, 1, 0, 0);
    // Warna cerobong
    glColor3f(0.6f, 0.6f, 0.6f);

    // Gambar tabung
    gluCylinder(
        quad,
        0.12f,  // radius bawah
        0.12f,  // radius atas
        0.6f,   // tinggi
        20,     
        20     
    );

    glPopMatrix();
    gluDeleteQuadric(quad);
}

//  PEMODELAN OBJEK GEDUNG 3D

void drawGedung() {
    // Badan utama gedung
    glColor3fv(wallColor);
    Prisma(2.0f, 3.0f, 1.2f);

    // Atap gedung
    glPushMatrix();
    glTranslatef(0.0f, 1.6f, 0.0f);
    glColor3f(0.9f, 0.9f, 0.95f);
    glScalef(2.2f, 0.15f, 1.4f);
    glutSolidCube(1.0f);
    glPopMatrix();

    // Pintu gedung
    glPushMatrix();
    glTranslatef(0.0f, -0.98f, 0.63f);
    glColor3fv(doorColor);
    glScalef(0.5f, 0.8f, 0.02f);
    glutSolidCube(1.0f);
    glPopMatrix();

    // Jendela depan gedung
     glColor3fv(windowColor);

    // Baris atas
	jendelaDepan(-0.45f,  0.98f);
	jendelaDepan( 0.45f,  0.98f);
	
	// Baris tengah
	jendelaDepan(-0.45f,  0.4f);
	jendelaDepan( 0.45f,  0.4f);
	
	// Baris bawah 
	jendelaDepan(-0.45f, -0.2f);
	jendelaDepan( 0.45f, -0.2f);
	
	// Jendela depan belakang
     glColor3fv(windowColor);

    // Baris atas
	jendelaBelakang(-0.45f,  0.98f);
	jendelaBelakang( 0.45f,  0.98f);
	
	// Baris tengah
	jendelaBelakang(-0.45f,  0.4f);
	jendelaBelakang( 0.45f,  0.4f);
	
	// Baris bawah 
	jendelaBelakang(-0.45f, -0.2f);
	jendelaBelakang( 0.45f, -0.2f);
	
	Tabung();
}

void rotasi(int key, int x, int y) {
    switch (key) {
        case GLUT_KEY_RIGHT: angleY += ROT_STEP; break;
        case GLUT_KEY_LEFT:  angleY -= ROT_STEP; break;
        case GLUT_KEY_UP:    angleX -= ROT_STEP; break;
        case GLUT_KEY_DOWN:  angleX += ROT_STEP; break;
    }
    glutPostRedisplay();
}

void handleKeyboardGedung(unsigned char key) {
    switch (key) {
        case 27: exit(0); break; // Tombol ESC
        case '+': case '=':
            scaleFactor += SCALE_STEP;
            if (scaleFactor > SCALE_MAX) scaleFactor = SCALE_MAX;
            break;
        case '-': case '_':
            scaleFactor -= SCALE_STEP;
            if (scaleFactor < SCALE_MIN) scaleFactor = SCALE_MIN;
            break;
        case 'r': case 'R':
            angleX = 0.0f;
            angleY = 0.0f;
            scaleFactor = 1.0f;
            break;
    }
    glutPostRedisplay();
}

void reshape(int w, int h) {
     if (h == 0) h = 1;
    float aspect = (float)w / (float)h;

    glViewport(0, 0, w, h);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(45.0, aspect, 0.1, 100.0);
    glMatrixMode(GL_MODELVIEW);
}

#ifdef STANDALONE
void keyboard (unsigned char key, int x, int y) {
    handleKeyboardGedung(key);

    glutPostRedisplay();
}

void display() {
     glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    // Kamera menggunakan proyeksi perspektif
    gluLookAt(4.0, 3.0, 6.0,
              0.0, 0.0, 0.0,
              0.0, 1.0, 0.0);

    glPushMatrix();

    // Transformasi skala dan rotasi
    glScalef(scaleFactor, scaleFactor, scaleFactor);
    glRotatef(angleX, 1.0f, 0.0f, 0.0f);
    glRotatef(angleY, 0.0f, 1.0f, 0.0f);

    // Menggambar objek gedung
    drawGedung();

    glPopMatrix();
    glutSwapBuffers();
}

//  FUNGSI UTAMA PROGRAM
int main(int argc, char** argv) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH);
    glutInitWindowSize(900, 600);
    glutInitWindowPosition(200, 100);
    glutCreateWindow("Azhari - Gedung 3D");

    initRendering();

    glutDisplayFunc(display);
    glutReshapeFunc(reshape);
    glutKeyboardFunc(keyboard);
    glutSpecialFunc(rotasi);
    
    glutMainLoop();
    return 0;
}
#endif