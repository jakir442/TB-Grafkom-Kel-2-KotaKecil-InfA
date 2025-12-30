#include <GL/glut.h>
#include <math.h>

#include "header/erika-pohon.h"

float angle = 2.3f;

float sudutDaunX = 0.0f;
float sudutDaunY = 0.0f;
float sudutDaunZ = 0.0f;

void gambarSilinder(float radius, float tinggi) {
    GLUquadric *objek = gluNewQuadric();
    gluCylinder(objek, radius, radius, tinggi, 32, 32);
    gluDeleteQuadric(objek);
}

void drawPohon() {
    glPushMatrix();

    float tinggiBatang = 1.4f;

    // BATANG UTAMA
    glColor3f(0.45f, 0.22f, 0.05f);
    glPushMatrix();
        glTranslatef(0.0f, 2.1f, 0.0f);
        glRotatef(90, 1, 0, 0);
        gambarSilinder(0.22f, tinggiBatang);
    glPopMatrix();

    // RANTING TENGAH
    glColor3f(0.55f, 0.30f, 0.1f);
    glPushMatrix();
        glTranslatef(0.0f, 2.0f, 0.0f);
        glRotatef(-90, 1, 0, 0);
        gambarSilinder(0.11f, 0.6f);
    glPopMatrix();

    // DAUN TENGAH
    glColor3f(0.1f, 0.75f, 0.1f);
	glPushMatrix();
    glTranslatef(0.0f, 2.7f, 0.0f);
    glRotatef(sudutDaunX, 1, 0, 0);
    glRotatef(sudutDaunY, 0, 1, 0);
    glRotatef(sudutDaunZ, 0, 0, 1);
    glScalef(0.55f, 0.55f, 0.55f);

    // ISI DAUN
    glEnable(GL_POLYGON_OFFSET_FILL);
    glPolygonOffset(1.0f, 1.0f);
    glColor3f(0.1f, 0.75f, 0.1f);
    glutSolidDodecahedron();
    glDisable(GL_POLYGON_OFFSET_FILL);

    // GARIS DAUN
    glLineWidth(2.0f);
    glColor3f(0.0f, 0.4f, 0.0f);
    glutWireDodecahedron();
	glPopMatrix();

    // RANTING KIRI
    glColor3f(0.55f, 0.30f, 0.1f);
    glPushMatrix();
        glTranslatef(0.0f, 1.7f, 0.0f);
        glRotatef(55, 0, 0, 1);
        glRotatef(-90, 1, 0, 0);
        gambarSilinder(0.10f, 1.1f);
    glPopMatrix();

    // DAUN KIRI
    glColor3f(0.1f, 0.8f, 0.1f);
	glPushMatrix();
    glTranslatef(-1.0f, 2.2f, 0.0f);
    glRotatef(sudutDaunX, 1, 0, 0);
    glRotatef(sudutDaunY, 0, 1, 0);
    glRotatef(sudutDaunZ, 0, 0, 1);
    glScalef(0.45f, 0.45f, 0.45f);

    // ISI DAUN
    glEnable(GL_POLYGON_OFFSET_FILL);
    glPolygonOffset(1.0f, 1.0f);
    glColor3f(0.1f, 0.8f, 0.1f);
    glutSolidDodecahedron();
    glDisable(GL_POLYGON_OFFSET_FILL);

    // GARIS DAUN
    glLineWidth(2.0f);
    glColor3f(0.0f, 0.4f, 0.0f);
    glutWireDodecahedron();
	glPopMatrix();

    // RANTING KANAN
    glColor3f(0.55f, 0.30f, 0.1f);
    glPushMatrix();
        glTranslatef(0.0f, 1.7f, 0.0f);
        glRotatef(-55, 0, 0, 1);
        glRotatef(-90, 1, 0, 0);
        gambarSilinder(0.10f, 1.1f);
    glPopMatrix();

    // DAUN KANAN
    glColor3f(0.1f, 0.8f, 0.1f);
    glPushMatrix();
    glTranslatef(1.0f, 2.2f, 0.0f);
    glRotatef(sudutDaunX, 1, 0, 0);
    glRotatef(sudutDaunY, 0, 1, 0);
    glRotatef(sudutDaunZ, 0, 0, 1);
    glScalef(0.45f, 0.45f, 0.45f);

    // ISI DAUN
    glEnable(GL_POLYGON_OFFSET_FILL);
    glPolygonOffset(1.0f, 1.0f);
    glColor3f(0.1f, 0.8f, 0.1f);
    glutSolidDodecahedron();
    glDisable(GL_POLYGON_OFFSET_FILL);

    // GARIS DAUN
    glLineWidth(2.0f);
    glColor3f(0.0f, 0.4f, 0.0f);
    glutWireDodecahedron();
	glPopMatrix();

    glPopMatrix();
}

void keyboardPohon(unsigned char key) {
    switch (key) {
        case 'f': 
			sudutDaunX += 5; 
			break;
        case 'h': 
			sudutDaunX -= 5; 
			break;

        case 't': 
			sudutDaunY += 5; 
			break;
        case 'g': 
			sudutDaunY -= 5; 
			break;

        case 'r': 
			sudutDaunZ += 5; 
			break;
        case 'y': 
			sudutDaunZ -= 5; 
			break;

        case 27: exit(0);
    }
    glutPostRedisplay();
}

#ifdef STANDALONE
void keyboard(unsigned char key, int x, int y) {
    keyboardPohon(key);

    glutPostRedisplay();
}

void display() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glLoadIdentity();

    gluLookAt(4, 4, 6,   // posisi kamera
              0, 1, 0,   // lihat ke arah
              0, 1, 0);  // up vector

    // rotasi animasi
    glRotatef(angle, 0, 1, 0);

    drawPohon();

    glutSwapBuffers();
}

void reshape(int lebar, int tinggi) {
    if (tinggi == 0) tinggi = 1;
    float rasio = (float)lebar / tinggi;

    glViewport(0, 0, lebar, tinggi);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(45, rasio, 0.1, 100);
    glMatrixMode(GL_MODELVIEW);
}

int main(int argc, char** argv) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
    glutInitWindowSize(800, 600);
    glutCreateWindow("pohon dede");

    glEnable(GL_NORMALIZE);

    glEnable(GL_NORMALIZE);
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_POLYGON_OFFSET_FILL);
    glutDisplayFunc(display); // tampilan layar
    glutReshapeFunc(reshape); // ubah ukuran
    glutKeyboardFunc(keyboard);

    glutMainLoop();
    return 0;
}
#endif