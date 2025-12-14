#include <GL/glut.h>
#include <math.h>

#include "header/erika-pohon.h"

float angle = 2.3f;

void initLighting() {
    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);
    glEnable(GL_COLOR_MATERIAL);

    GLfloat ambientLight[] = {0.3f, 0.3f, 0.3f, 1.0f};
    glLightModelfv(GL_LIGHT_MODEL_AMBIENT, ambientLight);

    GLfloat lightPos[] = {5.0f, 10.0f, 5.0f, 1.0f};
    glLightfv(GL_LIGHT0, GL_POSITION, lightPos);
}

void drawCylinder(float radius, float height) {
    GLUquadric *quad = gluNewQuadric();
    gluCylinder(quad, radius, radius, height, 32, 32);

    gluDeleteQuadric(quad);
}

void drawTree() {
    glPushMatrix();
    glScalef(0.9f, 0.9f, 0.9f); // mengecilkan ukuran pohon

    // 1. BATANG UTAMA
    glColor3f(0.45f, 0.22f, 0.05f); // lebih gelap sedikit
    glPushMatrix();
        glRotatef(-90, 1, 0, 0);
        drawCylinder(0.22f, 2.0f);
    glPopMatrix();

    // 2. RANTING TENGAH ? DAUN TENGAH
    glColor3f(0.55f, 0.30f, 0.1f); // warna lebih terang agar terlihat
    glPushMatrix();
        glTranslatef(0.0f, 2.0f, 0.0f);
        glRotatef(-90, 1, 0, 0);
        drawCylinder(0.11f, 0.6f);
    glPopMatrix();

    // daun tengah
    glColor3f(0.1f, 0.75f, 0.1f);
    glPushMatrix();
        glTranslatef(0.0f, 2.7f, 0.0f);
        glutSolidSphere(0.80f, 32, 32);
    glPopMatrix();

    // 3. RANTING KIRI
    glColor3f(0.55f, 0.30f, 0.1f);
    glPushMatrix();
        glTranslatef(0.0f, 1.7f, 0.0f);
        glRotatef(55, 0, 0, 1);      // lebih condong ke kiri
        glRotatef(-90, 1, 0, 0);
        drawCylinder(0.10f, 1.1f);  // lebih panjang + lebih besar
    glPopMatrix();

    // daun kiri � digeser lebih keluar
    glColor3f(0.1f, 0.8f, 0.1f);
    glPushMatrix();
        glTranslatef(-1.1f, 2.2f, 0.0f);
        glutSolidSphere(0.73f, 32, 32);
    glPopMatrix();

    // 4. RANTING KANAN
    glColor3f(0.55f, 0.30f, 0.1f);
    glPushMatrix();
        glTranslatef(0.0f, 1.7f, 0.0f);
        glRotatef(-55, 0, 0, 1);    // lebih condong ke kanan
        glRotatef(-90, 1, 0, 0);
        drawCylinder(0.10f, 1.1f);
    glPopMatrix();

    // daun kanan � digeser lebih keluar
    glColor3f(0.1f, 0.8f, 0.1f);
    glPushMatrix();
        glTranslatef(1.1f, 2.2f, 0.0f);
        glutSolidSphere(0.73f, 32, 32);
    glPopMatrix();


    glPopMatrix();
}

#ifdef STANDALONE
void display() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glLoadIdentity();

    gluLookAt(4, 4, 6,   // posisi kamera
              0, 1, 0,   // lihat ke arah
              0, 1, 0);  // up vector

    // rotasi animasi
    glRotatef(angle, 0, 1, 0);

    drawTree();

    glutSwapBuffers();
}

void timer(int value) {
    angle += 0.5f;
    if (angle > 360) angle -= 360;
    glutPostRedisplay();
    glutTimerFunc(16, timer, 0);
}

void reshape(int w, int h) {
    if (h == 0) h = 1;
    float aspect = (float)w / h;

    glViewport(0, 0, w, h);

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(45, aspect, 0.1, 100);

    glMatrixMode(GL_MODELVIEW);
}

int main(int argc, char** argv) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
    glutInitWindowSize(800, 600);
    glutCreateWindow("pohon dede");

    glEnable(GL_DEPTH_TEST);
    initLighting();

    glutDisplayFunc(display);
    glutReshapeFunc(reshape);
    glutTimerFunc(0, timer, 0);

    glutMainLoop();
    return 0;
}
#endif