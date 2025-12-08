#include <GL/freeglut.h>
#include <cmath>
#include "jakir-jalan.h"

float jalanOffset = 0.0f; // untuk perubahan posisi kamera di jalan

// inisialisasi objek jalan
void initJalan() {
    // kosong 
}

// update offset jalan
void updateJalan(float speed) {
    jalanOffset += speed;
}

// gambar jalan tanpa batas + garis kuning putus-putus
void drawJalan() {
    glColor3f(0.2f, 0.2f, 0.2f); // abu gelap

    float visibleDist = 300.0f;
    float start = -visibleDist;
    float end   =  visibleDist;

    float baseShift = fmod(jalanOffset, 10.0f);

    glBegin(GL_QUADS);
    for (float z = start; z < end; z += 10.0f) {
        float zShift = z + baseShift;

        glVertex3f(-2.0f, 0.0f, zShift);
        glVertex3f( 2.0f, 0.0f, zShift);
        glVertex3f( 2.0f, 0.0f, zShift + 10);
        glVertex3f(-2.0f, 0.0f, zShift + 10);
    }
    glEnd();

    // garis kuning putus-putus
    glColor3f(1, 1, 0);
    glLineWidth(3);

    glBegin(GL_LINES);
    for (float z = start; z < end; z += 10.0f) {
        float zShift = z + baseShift;

        glVertex3f(0, 0.05f, zShift + 3);
        glVertex3f(0, 0.05f, zShift + 6);
    }
    glEnd();
}

#ifdef STANDALONE
int main(int argc, char** argv) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH);
    glutInitWindowSize(800, 600);
    glutCreateWindow("Test Jalan");

    // Set kamera + proyeksi
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(60.0, 800.0 / 600.0, 0.1, 1000.0);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    gluLookAt(
        0, 5, 20,
        0, 0, 0,
        0, 1, 0
    );

    glEnable(GL_DEPTH_TEST);

    glutDisplayFunc([](){
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        drawJalan();
        glutSwapBuffers();
    });

    glutMainLoop();
    return 0;
}
#endif