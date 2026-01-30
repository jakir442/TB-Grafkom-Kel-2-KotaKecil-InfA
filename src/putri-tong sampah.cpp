#include <GL/glut.h>
#include <GL/glu.h>
#include <math.h>

//#include "header/putri-mobil.h"

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

//init
void init() {
    glEnable(GL_DEPTH_TEST);

    // aktifkan lighting
    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);

    // posisi lampu
    GLfloat lightPos[] = { 5.0f, 8.0f, 5.0f, 1.0f };
    glLightfv(GL_LIGHT0, GL_POSITION, lightPos);

    // warna cahaya
    GLfloat lightAmbient[]  = { 0.3f, 0.3f, 0.3f, 1.0f };
    GLfloat lightDiffuse[]  = { 0.9f, 0.9f, 0.9f, 1.0f };
    GLfloat lightSpecular[] = { 1.0f, 1.0f, 1.0f, 1.0f };

    glLightfv(GL_LIGHT0, GL_AMBIENT,  lightAmbient);
    glLightfv(GL_LIGHT0, GL_DIFFUSE,  lightDiffuse);
    glLightfv(GL_LIGHT0, GL_SPECULAR, lightSpecular);

    glEnable(GL_COLOR_MATERIAL);
    glColorMaterial(GL_FRONT, GL_AMBIENT_AND_DIFFUSE);
}

//tong sampah
void drawTrashBin(float r, float g, float b) {
    GLUquadric* quad = gluNewQuadric();
    gluQuadricNormals(quad, GLU_SMOOTH);
    
    //bagian dalam tong
	glDisable(GL_LIGHTING);     // biar hitam pekat
	glColor3f(0.0f, 0.0f, 0.0f);

	glPushMatrix();
    	glTranslatef(0.0f, 0.02f, 0.0f); // sedikit naik biar tidak z-fighting
    	glRotatef(-90, 1, 0, 0);
    	gluCylinder(quad, 0.38f, 0.38f, 1.0f, 30, 30);
	glPopMatrix();

	glEnable(GL_LIGHTING);
	
	//sisi tong
	glDisable(GL_LIGHTING);
	glColor3f(0.0f, 0.0f, 0.0f);
	
	glPushMatrix();
	    glTranslatef(0.0f, 1.19f, 0.0f);
	    glRotatef(-90, 1, 0, 0);
	    gluDisk(quad, 0.38f, 0.5f, 40, 1);
	glPopMatrix();
	
	glEnable(GL_LIGHTING);


    // material warna tong
    GLfloat matDiffuse[]  = { r, g, b, 1.0f };
    GLfloat matSpecular[] = { 1.0f, 1.0f, 1.0f, 1.0f }; // kilap
    GLfloat matShine[]    = { 80.0f };                 // tingkat kilap

    glMaterialfv(GL_FRONT, GL_DIFFUSE,  matDiffuse);
    glMaterialfv(GL_FRONT, GL_SPECULAR, matSpecular);
    glMaterialfv(GL_FRONT, GL_SHININESS, matShine);

    // badan tong
    glColor3f(r, g, b);
    glPushMatrix();
        glRotatef(-90, 1, 0, 0);
        gluCylinder(quad, 0.5, 0.5, 1.2, 40, 40);
    glPopMatrix();

    // tutup tong
    glColor3f(r - 0.1f, g - 0.1f, b - 0.1f);
    glPushMatrix();
        glTranslatef(0, 1.2f, 0);
        glRotatef(-90, 1, 0, 0);
        gluDisk(quad, 0.0, 0.55, 40, 1);
    glPopMatrix();

    gluDeleteQuadric(quad);
}

void drawGrid() {
    glColor3f(0.3f, 0.3f, 0.3f);
    glBegin(GL_LINES);
    for (int i = -10; i <= 10; i++) {
        glVertex3f(i, 0, -10);
        glVertex3f(i, 0,  10);
        glVertex3f(-10, 0, i);
        glVertex3f( 10, 0, i);
    }
    glEnd();
}

void drawText3D(const char* text) {
    glScalef(0.002f, 0.002f, 0.002f);
    for (int i = 0; text[i] != '\0'; i++) {
        glutStrokeCharacter(GLUT_STROKE_ROMAN, text[i]);
    }
}

void logoOrganik() {
    glColor3f(0.0f, 0.0f, 0.0f);
    glBegin(GL_TRIANGLE_FAN);
        glVertex3f(0, 0, 0);
        for (int i = 0; i <= 360; i++) {
            float rad = i * M_PI / 180;
            glVertex3f(cos(rad) * 0.15f, sin(rad) * 0.3f, 0);
        }
    glEnd();

    glColor3f(0, 0, 0); //tulang daun
    glBegin(GL_LINES);
        glVertex3f(0, -0.3f, 0);
        glVertex3f(0,  0.3f, 0);
    glEnd();
    
    glColor3f(1, 1, 1); //tulang daun
    glBegin(GL_LINES);
        glVertex3f(0, -0.2f, 0);
        glVertex3f(0,  0.2f, 0);
    glEnd();
}

void drawAnorganikLogo() {
    glDisable(GL_LIGHTING); // biar warna tajam

    glColor3f(1, 1, 1); // putih kontras

    // botol
    glBegin(GL_QUADS);
        glVertex3f(-0.08f, 0.15f, 0.01f);
        glVertex3f( 0.08f, 0.15f, 0.01f);
        glVertex3f( 0.08f,-0.15f, 0.01f);
        glVertex3f(-0.08f,-0.15f, 0.01f);
    glEnd();

    // leher botol
    glBegin(GL_QUADS);
        glVertex3f(-0.03f, 0.22f, 0.01f);
        glVertex3f( 0.03f, 0.22f, 0.01f);
        glVertex3f( 0.03f, 0.15f, 0.01f);
        glVertex3f(-0.03f, 0.15f, 0.01f);
    glEnd();

    // kaleng
    glBegin(GL_QUADS);
        glVertex3f(0.12f, 0.10f, 0.01f);
        glVertex3f(0.22f, 0.10f, 0.01f);
        glVertex3f(0.22f,-0.10f, 0.01f);
        glVertex3f(0.12f,-0.10f, 0.01f);
    glEnd();

    glEnable(GL_LIGHTING);
}


void logoB3() {
    glColor3f(1, 1, 0);
    glBegin(GL_TRIANGLES);
        glVertex3f(0.0f, 0.3f, 0);
        glVertex3f(-0.3f, -0.3f, 0);
        glVertex3f(0.3f, -0.3f, 0);
    glEnd();

    glColor3f(0, 0, 0);
    glBegin(GL_LINES);
        glVertex3f(0, 0.15f, 0);
        glVertex3f(0, -0.1f, 0);
    glEnd();

    glPointSize(5);
    glBegin(GL_POINTS);
        glVertex3f(0, -0.2f, 0);
    glEnd();
}

void drawMetalBar(float length) {
    GLUquadric* quad = gluNewQuadric();

    glColor3f(0.6f, 0.6f, 0.6f); // warna besi

    glPushMatrix();
        glRotatef(90, 0, 1, 0);  // arahkan silinder horizontal
        gluCylinder(quad, 0.05f, 0.05f, length, 20, 5);
    glPopMatrix();

    gluDeleteQuadric(quad);
}

//display
void display() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glLoadIdentity();

    gluLookAt(6, 5, 8,  0, 0, 0,  0, 1, 0);

    drawGrid();
    
	//besi
glPushMatrix();
    glTranslatef(-2.0f, 0.6f, -0.55f); //posisi di belakang tong
    drawMetalBar(2.6f);               //panjang besi
glPopMatrix();

    //organik
    glPushMatrix();
        glTranslatef(-1.3f, 0, 0);
        drawTrashBin(0.0f, 0.8f, 0.0f);
    glPopMatrix();

    glPushMatrix();
        glTranslatef(-1.8f, 1.6f, 0);
        glColor3f(1, 1, 1);
        drawText3D("ORGANIK");
    glPopMatrix();

    glPushMatrix();
        glTranslatef(-1.3f, 0.6f, 0.62f);
        glScalef(1.1f, 1.1f, 1.1f);
        logoOrganik();
    glPopMatrix();

    //anorganik
    glPushMatrix();
        drawTrashBin(0.0f, 0.0f, 1.0f);
    glPopMatrix();

    glPushMatrix();
        glTranslatef(-0.60f, 1.6f, 0);
        glColor3f(1, 1, 1);
        drawText3D("ANORGANIK");
    glPopMatrix();

    glPushMatrix();
        glTranslatef(0.0f, 0.6f, 0.51f);
        glScalef(1.2f, 1.2f, 1.2f);
        drawAnorganikLogo();
    glPopMatrix();

    //B3
    glPushMatrix();
        glTranslatef(1.1f, 0, 0);
        drawTrashBin(1.0f, 0.0f, 0.0f);
    glPopMatrix();

    glPushMatrix();
        glTranslatef(1.0f, 1.6f, 0);
        glColor3f(1, 1, 1);
        drawText3D("B3");
    glPopMatrix();

    glPushMatrix();
        glTranslatef(1.3f, 0.6f, 0.52f); //posisi logo
        glScalef(1.0f, 1.0f, 1.0f);
        logoB3();
    glPopMatrix();

    glutSwapBuffers();
}

void reshape(int w, int h) {
    glViewport(0, 0, w, h);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(60, (float)w / h, 1, 100);
    glMatrixMode(GL_MODELVIEW);
}

//main
int main(int argc, char** argv) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
    glutInitWindowSize(800, 600);
    glutCreateWindow("Putri - 3 Tong Sampah 3D");

    init();
    glutDisplayFunc(display);
    glutReshapeFunc(reshape);
    glutMainLoop();
    return 0;
}
//#endif 
