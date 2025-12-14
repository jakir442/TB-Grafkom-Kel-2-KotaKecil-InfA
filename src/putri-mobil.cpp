#include <GL/glut.h>
#include <GL/glu.h>
#include <math.h>

#include "header/putri-mobil.h"

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

// ================= VARIABEL MOBIL =================
float mobilPosX = 0.0f;
float mobilPosY = 0.6f;  //posisi sumbu mobil (X,Y,Z), untuk ketinggian mobil dengan jalan
float mobilPosZ = 0.0f;
float mobilYaw = 180.0f;   //arah hadap mobil (sumbu Y)
float mobilWheelAngle = 0.0f;   //sudut roda berputar putar anjay

// ================= INIT OPENGL =================
void initMobil() {
    glEnable(GL_DEPTH_TEST);
}

// ================= KUBUS =================
void drawCube(float r, float g, float b) {
    glColor3f(r, g, b);     //warna objek
    glutSolidCube(1.0);     //kubus solid uk 1x1x1 (ke diubah di scale)
}

// ================= RODA =================
void drawWheel() {
    GLUquadric *quad = gluNewQuadric();    //buat objek silinder

    glPushMatrix();
        glRotatef(90, 0, 1, 0);      //pergerakan roda
        glColor3f(1.0f, 1.0f, 1.0f); // ban putih
        gluCylinder(quad, 0.2, 0.2, 0.2, 20, 20);     //buat badan roda (objek quadric,atas,bawah,panjang,jumlah melingkar,jumlah mmemanjang)
        gluDisk(quad, 0.0, 0.2, 20, 20);          //tutup depan roda
        glTranslatef(0, 0, 0.2);                  //pergeseran roda
        gluDisk(quad, 0.0, 0.2, 20, 20);
    glPopMatrix();

    gluDeleteQuadric(quad);
}

// ================= MOBIL =================
void drawMobil() {
    glPushMatrix();                                      //simpan ttransformasi mobil
        glTranslatef(mobilPosX, mobilPosY, mobilPosZ);   //posisi mobil
        glRotatef(mobilYaw, 0, 1, 0);                    //arah hadap mobil

        // body
        glPushMatrix();
            glScalef(1.0, 0.6, 2.0);
            drawCube(0.2, 0.6, 1.0);
        glPopMatrix();

        // atap
        glPushMatrix();
            glTranslatef(0, 0.5, 0);       //geser keatas body
            glScalef(1.0, 0.5, 1.0);      //ukuran atap
            drawCube(0.3, 0.8, 1.0);
        glPopMatrix();

        // posisi roda
        float x[2] = {0.7f, -0.85f};   //kanan, kiri
        float z[2] = {-0.85f, 0.7f};   //depan, belakang
        
		//looping pergerakan roda
        for (int i = 0; i < 2; i++) {
            for (int j = 0; j < 2; j++) {
                glPushMatrix();
                    glTranslatef(x[i], -0.4, z[j]);       //posisi roda
                    glRotatef(mobilWheelAngle, 1, 0, 0);  //putar roda
                    drawWheel();
                glPopMatrix();
            }
        } 
		// ================= LAMPU DEPAN (TORUS) =================
		glColor3f(1.0f, 1.0f, 0.0f); // warna lampu kuning

		// Lampu depan kanan
		glPushMatrix();
    		glTranslatef(0.4f, 0.0f, 1.05f); // kanan - depan
    		glRotatef(90, 0, 0, 1);          // PUTAR AGAR SEJAJAR SUMBU X
    		glutSolidTorus(0.03, 0.12, 20, 30);
		glPopMatrix();

		// Lampu depan kiri
		glPushMatrix();
    		glTranslatef(-0.4f, 0.0f, 1.05f); // kiri - depan
    		glRotatef(90, 0, 0, 1);           // PUTAR AGAR SEJAJAR SUMBU X
    		glutSolidTorus(0.03, 0.12, 20, 30);
		glPopMatrix();
        
	glPopMatrix();
}

// ================= UPDATE =================
void updateMobil(float speed) {
    mobilPosX -= sin(mobilYaw * M_PI / 180.0f) * speed;     //sumbu X
    mobilPosZ -= cos(mobilYaw * M_PI / 180.0f) * speed;     //sumbu Z
    mobilWheelAngle -= speed * 300.0f; //roda jalan ea
}

// ================= KEYBOARD =================
void controlMobil(unsigned char key) {
    float moveSpeed = 0.15f;       //kecepatan maju mundur
    float turnSpeed = 3.0f;        //kecepatan kanan kiri

    switch (key) {
        case 'w': case 'W': updateMobil(-moveSpeed); break;            //maju
        case 's': case 'S': updateMobil(moveSpeed); break;           //mundur
        case 'd': case 'D': mobilYaw -= turnSpeed; break;             //belok kiri
        case 'a': case 'A': mobilYaw += turnSpeed; break;             //belok kanan
        case 27: exit(0);                                             //esc
    }
    glutPostRedisplay();
}

#ifdef STANDALONE
// ================= GRID 3D (GARIS BACKGROUND) =================
void drawGrid3D() {
    glColor3f(0.3f, 0.3f, 0.3f); // warna garis abu-abu
    glLineWidth(1.0f);     //ketebalan garis

    glBegin(GL_LINES);
    for (int i = -20; i <= 20; i++) {      //lantai XZ (nu kotak kotak)
        // garis sejajar sumbu Z
        glVertex3f(i, 0, -20);   //titik awal
        glVertex3f(i, 0,  20);   //titik akhir

        // garis sejajar sumbu X
        glVertex3f(-20, 0, i);    
        glVertex3f( 20, 0, i);
    }
    glEnd();
}

// ================= FUNGSI WINDOW =================
void reshape(int w, int h) {
    glViewport(0, 0, w, h);       //area gambar sesuai uk window
    glMatrixMode(GL_PROJECTION);  //mode proyeksi kamera
    glLoadIdentity();
    gluPerspective(60, (float)w / h, 1, 100);   //sudut pandang kameraa (kek zoom in out bgituloo)1 jarak dekat, 100 terjaoh
    glMatrixMode(GL_MODELVIEW);
}

// ================= DISPLAY =================
void display() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f); // background hitam

    glLoadIdentity();

    gluLookAt(
        6.0f, 5.0f, 8.0f,  // posisi kamera
        0.0f, 0.0f, 0.0f,  // titik yang dilihat
        0.0f, 1.0f, 0.0f   // arah atas
    );
    
    drawGrid3D();  // BACKGROUND GARIS 3D
    drawMobil();   // MOBIL

    glutSwapBuffers();
}

// ================= MAIN =================
int main(int argc, char **argv) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
    glutInitWindowSize(800, 600);
    glutCreateWindow("Mobil 3D + Background Grid");

    initMobil();

    glutDisplayFunc(display);
    glutReshapeFunc(reshape);
    glutKeyboardFunc(controlMobil);

    glutMainLoop();
    return 0;
}
#endif