#include <GL/freeglut.h>

#include "../header/taman/assyifa_ayunan.h"

// ================= VARIABEL AYUNAN =================
float rAyunan = 0.0f;
float kAyunan = 1.5f;

// ================= FUNGSI AYUNAN =================
void ayunan() {
    // Tiang Kiri
    glPushMatrix();
        glColor3f(0.6f, 0.35f, 0.1f);
        glTranslatef(-2, -4.5, 0);
        glRotatef(-90, 1, 0, 0);
        glutSolidCylinder(0.15f, 5.0f, 32, 8);
    glPopMatrix();

    // Tiang Kanan
    glPushMatrix();
        glTranslatef(2, -4.5, 0);
        glRotatef(-90, 1, 0, 0);
        glutSolidCylinder(0.15f, 5.0f, 32, 8);
    glPopMatrix();

    // Palang Atas
    glPushMatrix();
        glTranslatef(-2, 0.5, 0);
        glRotatef(90, 0, 1, 0);
        glutSolidCylinder(0.15f, 4.0f, 32, 8);
    glPopMatrix();

    // Sendi
    glPushMatrix();
        glTranslatef(-2, 0.5, 0);
        glutSolidSphere(0.16f, 20, 20);
    glPopMatrix();

    glPushMatrix();
        glTranslatef(2, 0.5, 0);
        glutSolidSphere(0.16f, 20, 20);
    glPopMatrix();

    // ================= AYUNAN (BERPUTAR) =================
    glPushMatrix();
        glTranslatef(0, 2, 0);
        glRotatef(rAyunan, 1, 0, 0);

        // Tali Kiri
        glPushMatrix();
            glColor3f(0.9f, 0.9f, 0.8f);
            glTranslatef(-0.8f, -1.5, 0);
            glRotatef(90, 1, 0, 0);
            glutSolidCylinder(0.03f, 2.8f, 16, 4);
        glPopMatrix();

        // Tali Kanan
        glPushMatrix();
            glTranslatef(0.8f, -1.5, 0);
            glRotatef(90, 1, 0, 0);
            glutSolidCylinder(0.03f, 2.8f, 16, 4);
        glPopMatrix();

        // Dudukan
        glPushMatrix();
            glColor3f(0.6f, 0.35f, 0.1f);
            glTranslatef(0, -4.3f, 0);
            glScalef(2.0f, 0.2f, 0.8f);
            glutSolidCube(1.0f);
        glPopMatrix();
    glPopMatrix();
}

// ================= KEYBOARD =================
void keyboardAyunan(unsigned char key) {
    if (key == 'k') {
        rAyunan += kAyunan;
        if (rAyunan > 8 || rAyunan < -8)
            kAyunan = -kAyunan;
    }
    glutPostRedisplay();
}

// AKSES PRIVATE
#ifdef STANDALONE 
// ================= DISPLAY =================
void display() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glLoadIdentity();

    gluLookAt(
        0, 5, 15,
        0, 0, 0,
        0, 1, 0
    );

    ayunan();

    glutSwapBuffers();
}

// ================= INIT =================
void init() {
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_COLOR_MATERIAL);
    glClearColor(0.5f, 0.8f, 1.0f, 1.0f); // langit
}

// ================= RESHAPE =================
void reshape(int w, int h) {
    if (h == 0) h = 1;
    glViewport(0, 0, w, h);

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(60, (float)w / h, 1, 100);
    glMatrixMode(GL_MODELVIEW);
}

// ================= MAIN =================
int main(int argc, char** argv) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
    glutInitWindowSize(800, 600);
    glutCreateWindow("Ayunan OpenGL");

    init();

    glutDisplayFunc(display);
    glutReshapeFunc(reshape);
    glutKeyboardFunc(keyboard);

    glutMainLoop();
    return 0;
}
#endif
