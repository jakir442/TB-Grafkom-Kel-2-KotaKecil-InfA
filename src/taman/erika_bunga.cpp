#include <GL/glut.h>
#include <math.h>

#include "../header/taman/erika_bunga.h"

//BUNGA SATUAN 
void drawSingleFlower() {

    glPushMatrix();
    	glScalef(0.25f, 0.25f, 0.25f);

        // KELOPAK
        for (int i = 0; i < 8; i++) {
            glPushMatrix();
                glRotatef(i * 45.0f, 0, 0, 1);
                glTranslatef(0.0f, 0.7f, 0.0f);
                glScalef(0.3f, 0.6f, 0.2f);
                glutSolidSphere(1.0, 30, 30);
            glPopMatrix();
        }

        // PUTIK
        glColor3f(0.45f, 0.25f, 0.1f);
        glutSolidSphere(0.2, 30, 30);

        // BATANG
        GLUquadric* quad = gluNewQuadric();
        glColor3f(0.0f, 0.6f, 0.0f);
        glPushMatrix();
            glTranslatef(0.0f, -5.0f, 0.0f);
            glRotatef(-90, 1, 0, 0);
            gluCylinder(quad, 0.05, 0.05, 5.0, 30, 30);
        glPopMatrix();

        // DAUN KIRI
        glColor3f(0.0f, 0.7f, 0.0f);
        glPushMatrix();
            glTranslatef(0.25f, -2.0f, 0.0f);
            glRotatef(30, 0, 0, 1);
            glScalef(0.4f, 0.15f, 0.05f);
            glutSolidSphere(1.0, 20, 20);
        glPopMatrix();

        // DAUN KANAN
        glPushMatrix();
            glTranslatef(-0.25f, -3.0f, 0.0f);
            glRotatef(-30, 0, 0, 1);
            glScalef(0.4f, 0.15f, 0.05f);
            glutSolidSphere(1.0, 20, 20);
        glPopMatrix();
    glPopMatrix();
}

void drawFlower() {

    float jarak = 0.4f;
    float belakang = -1.2f;

    // DEPAN KIRI - PINK
    glPushMatrix();
        glTranslatef(-jarak, 0.0f, 0.0f);
        glColor3f(1.0f, 0.4f, 0.7f);
        drawSingleFlower();
    glPopMatrix();

    // DEPAN KANAN - KUNING
    glPushMatrix();
        glTranslatef(jarak, 0.0f, 0.0f);
        glColor3f(1.0f, 0.9f, 0.2f);
        drawSingleFlower();
    glPopMatrix();

    // BELAKANG KIRI - BIRU
    glPushMatrix();
        glTranslatef(-jarak, 0.0f, belakang);
        glColor3f(0.3f, 0.5f, 1.0f);
        drawSingleFlower();
    glPopMatrix();

    // BELAKANG KANAN - MERAH
    glPushMatrix();
        glTranslatef(jarak, 0.0f, belakang);
        glColor3f(1.0f, 0.2f, 0.2f);
        drawSingleFlower();
    glPopMatrix();
}


#ifdef STANDALONE 
void display() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glLoadIdentity();

    // Kamera
    glTranslatef(0.0f, 0.f, -12.0f);
    
    float jarak = 2.5f;
	float belakang = -3.0f;

    // DEPAN KIRI (PINK)
    glPushMatrix();
        glTranslatef(-1.5f, 0.0f, 0.0f); // kiri - depan
        glColor3f(1.0f, 0.4f, 0.7f);     // pink
        drawFlower();
    glPopMatrix();

    // DEPAN KANAN (KUNING)
    glPushMatrix();
        glTranslatef(1.5f, 0.0f, 0.0f);  // kanan - depan
        glColor3f(1.0f, 0.9f, 0.2f);     // kuning
        drawFlower();
    glPopMatrix();

    // BELAKANG KIRI (BIRU)
    glPushMatrix();
        glTranslatef(-1.5f, 0.0f, -2.5f); // kiri - belakang
        glColor3f(0.3f, 0.5f, 1.0f);      // biru
        drawFlower();
    glPopMatrix();

    // BELAKANG KANAN (MERAH) 
    glPushMatrix();
        glTranslatef(1.5f, 0.0f, -2.5f);  // kanan - belakang
        glColor3f(1.0f, 0.2f, 0.2f);      // merah
        drawFlower();
    glPopMatrix();

    glutSwapBuffers();
}

void reshape(int w, int h) {
    if (h == 0) h = 1;

    float ratio = (float)w / (float)h;

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(60.0, ratio, 0.1, 100.0);
    glMatrixMode(GL_MODELVIEW);
}

void init() {
    glEnable(GL_DEPTH_TEST);
    glClearColor(1.0, 1.0, 1.0, 1.0);
}

int main(int argc, char** argv) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
    glutInitWindowSize(600, 600);
    glutCreateWindow("erika Bunga-3D");

    init();

    glutDisplayFunc(display);
    glutReshapeFunc(reshape); 

    glutMainLoop();
    return 0;
}
#endif