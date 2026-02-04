#include <GL/glut.h>

#include "../header/taman/erika_semak.h"

void leaf(float x, float y, float r) {
    glPushMatrix();
        glTranslatef(x, y, 0);
        glColor3f(0.0, 0.4, 0.0); 
        glutSolidSphere(r, 20, 20);
    glPopMatrix();
}

void drawSemak() {
    // bawah
    leaf(-3.0, 0.0, 0.6);
    leaf(-2.2, 0.0, 0.7);
    leaf(-1.4, 0.0, 0.65);
    leaf(-0.6, 0.0, 0.7);
    leaf( 0.2, 0.0, 0.65);
    leaf( 1.0, 0.0, 0.7);
    leaf( 1.8, 0.0, 0.65);
    leaf( 2.6, 0.0, 0.6);

    // tengah
    leaf(-2.6, 0.5, 0.6);
    leaf(-1.8, 0.6, 0.7);
    leaf(-1.0, 0.5, 0.65);
    leaf(-0.2, 0.6, 0.7);
    leaf( 0.6, 0.5, 0.65);
    leaf( 1.4, 0.6, 0.7);
    leaf( 2.2, 0.5, 0.6);

    // atas
    leaf(-2.2, 1.0, 0.6);
    leaf(-1.2, 1.1, 0.7);
    leaf(-0.2, 1.0, 0.65);
    leaf( 0.8, 1.1, 0.7);
    leaf( 1.8, 1.0, 0.6);
}

#ifdef STANDALONE 
void display() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glLoadIdentity();
    gluLookAt(0, 2, 8, 0, 0.6, 0, 0, 1, 0);
    drawSemak();
    glutSwapBuffers();
}

void reshape(int w, int h) {
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(60, (float)w/h, 1, 100);
    glMatrixMode(GL_MODELVIEW);
}

int main(int argc, char** argv) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
    glutInitWindowSize(800,600);
    glutCreateWindow("erika-Semak 3D");

    glEnable(GL_DEPTH_TEST);
    glClearColor(1,1,1,1);

    glutDisplayFunc(display);
    glutReshapeFunc(reshape);
    glutMainLoop();
    return 0;
}
#endif