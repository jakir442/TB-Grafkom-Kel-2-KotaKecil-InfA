#include <GL/freeglut.h>
#include "header/taman.h"

void drawBangku(float x, float z) {
    glPushMatrix();
    glTranslatef(x, 0.3f, z);

    // Dudukan
    glColor3f(0.6f, 0.4f, 0.2f);
    glPushMatrix();
    glScalef(2.0f, 0.2f, 0.6f);
    glutSolidCube(1);
    glPopMatrix();

    // Kaki kiri
    glPushMatrix();
    glTranslatef(-0.8f, -0.3f, 0);
    glScalef(0.2f, 0.6f, 0.2f);
    glutSolidCube(1);
    glPopMatrix();

    // Kaki kanan
    glPushMatrix();
    glTranslatef(0.8f, -0.3f, 0);
    glScalef(0.2f, 0.6f, 0.2f);
    glutSolidCube(1);
    glPopMatrix();

    glPopMatrix();
}

void drawTaman() {
    glPushMatrix();

    glColor3f(0.2f, 0.8f, 0.2f);
    glBegin(GL_QUADS);
        glVertex3f(-10, 0, -30);
        glVertex3f( 10, 0, -30);
        glVertex3f( 10, 0, -10);
        glVertex3f(-10, 0, -10);
    glEnd();

    drawBangku(-4, -15);

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