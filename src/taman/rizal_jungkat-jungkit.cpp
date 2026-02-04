#include <GL/glut.h>
#include <GL/glu.h>
#include <cmath>

#include "../header/taman/rizal_jungkat-jungkit.h"

// Variabel Kamera
float kameraX = 0.0f;
float kameraY = 0.0f;
float kameraZ = 30.0f;

float rotPapan = 0.0f;
float rotSpeed = 1.0f;
bool naik = true;

// Kecepatan Gerak Kamera
float camSpeed = 1.0f;

// -------------------------------------------------------------
// Kubus Alas Lampu
// -------------------------------------------------------------
void alas_lampu(float tX, float tY, float tZ,
                float angle, float rX, float rY, float rZ,
                float sX, float sY, float sZ)
{
    glPushMatrix();
    glTranslatef(tX, tY, tZ);
    glRotatef(angle, rX, rY, rZ);
    glScalef(sX, sY, sZ);
    glutSolidCube(3);
    glPopMatrix();
}

// -------------------------------------------------------------
// Tiang Lampu
// -------------------------------------------------------------
void tiang_lampu(float tX, float tY, float tZ,
                 float angle, float rX, float rY, float rZ,
                 float sX, float sY, float sZ)
{
    glPushMatrix();
    glTranslatef(tX, tY, tZ);
    glRotatef(angle, rX, rY, rZ);
    glScalef(sX, sY, sZ);

    GLUquadric* quad = gluNewQuadric();
    gluCylinder(quad, 1.0, 1.0, 15.0, 10, 10);

    glPopMatrix();
}

// -------------------------------------------------------------
// Bola
// -------------------------------------------------------------
void bola(float tX, float tY, float tZ,
          float angle, float rX, float rY, float rZ,
          float scaleX, float scaleY, float scaleZ)
{
    glPushMatrix();
    glTranslatef(tX, tY, tZ);
    glRotatef(angle, rX, rY, rZ);

    glScalef(scaleX, scaleY, scaleZ);

    glutSolidSphere(1.5, 40, 40);

    glPopMatrix();
}

void jungkat_jungkit()
{
    glPushMatrix();
        glScalef(0.05f, 0.05f, 0.05f);
        glTranslatef(-7.5f, 0.0f, 0.0f);

        // tiang kiri
        glColor3f(0,0,1);
        alas_lampu(0, -12, 0, 90, 0, 1, 0, 2, 0.3, 2);
        glColor3f(1,0,0);
        tiang_lampu(0, 3, 0, 90, 1, 0, 0, 2, 2, 1);

        // tiang kanan
        glColor3f(0,0,1);
        alas_lampu(15, -12, 0, 90, 0, 1, 0, 2, 0.3, 2);
        glColor3f(1,0,0);
        tiang_lampu(15, 3, 0, 90, 1, 0, 0, 2, 2, 1);

        // poros tengah
        glColor3f(0,0,1);
        tiang_lampu(17, 5, 0, -90, 0, 1, 0, 2, 2, 1.3);

        // ================= PAPAN BERGERAK =================
        glPushMatrix();
            glTranslatef(7.5, 5, 0);
            glRotatef(rotPapan, 1, 0, 0);
            glTranslatef(-7.5, -5, 0);

            // papan
            glColor3f(1,1,0);
            glPushMatrix();
                glTranslatef(7.5, 5, 0);
                glRotatef(90, 0, 1, 0);
                glScalef(25, 0.3, 6.5);
                glutSolidCube(3);
            glPopMatrix();

            // pegangan & baut
            glColor3f(1,0,0);
            bola(-2, 5, 0, 0, 1, 0, 0, 1, 1, 1);
            bola(16.5, 5, 0, 0, 1, 0, 0, 1, 1, 1);
        glPopMatrix();

    glPopMatrix();
}

void keyboardJungkatJungkit(unsigned char key) {
    switch (key) {
        case 'j': 
            if (naik) rotPapan += rotSpeed;
            else      rotPapan -= rotSpeed;
            
            if (rotPapan > 20) naik = false;
            if (rotPapan < -20) naik = true;
            break;
    }
    glutPostRedisplay();
}


// AKSES PRIVATE
#ifdef STANDALONE 
// -------------------------------------------------------------
void display() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glLoadIdentity();

    gluLookAt(
        kameraX, kameraY, kameraZ,
        0, 0, 0,  // Diarahkan ke titik tengah
        0, 1, 0
    );

    // =========================================================
    // GRUP PENYUESUAIAN UKURAN (SCALING)
    // Membungkus seluruh struktur agar mengecil tanpa ubah angka fungsi
    // =========================================================
    glPushMatrix();
        glScalef(0.1f, 0.1f, 0.1f); // Mengecilkan ke 10% ukuran asli
        glTranslatef(-7.5f, 0.0f, 0.0f); // Mengetengahkan objek

        // tiang 1
        glColor3f(0,0,1);
        alas_lampu(0, -12, 0, 90, 0, 1, 0, 2, 0.3, 2);
        glColor3f(1,0,0);
        tiang_lampu(0, 3, 0, 90, 1, 0, 0, 2, 2, 1);

        // tiang 2
        glColor3f(0,0,1);
        alas_lampu(15, -12, 0, 90, 0, 1, 0, 2, 0.3, 2);
        glColor3f(1,0,0);
        tiang_lampu(15, 3, 0, 90, 1, 0, 0, 2, 2, 1);
        
        // tiang tengah (poros)
        glColor3f(0,0,1);
        tiang_lampu(17, 5, 0, -90, 0, 1, 0, 2, 2, 1.3); 

        // AWAL GRUP BERGERAK (Papan + Pegangan + Baut)
        glPushMatrix(); 
            // 1. Tentukan titik tumpu (pivot) di tengah tiang
            glTranslatef(7.5, 5, 0); 
            // 2. Putar seluruh grup
            glRotatef(rotPapan, 1, 0, 0); 
            // 3. Kembalikan posisi koordinat lokal
            glTranslatef(-7.5, -5, 0);

            // --- Papan Kuning ---
            glColor3f(1,1,0);
            glPushMatrix();
                glTranslatef(7.5, 5, 0);
                glRotatef(90, 0, 1, 0);
                glScalef(25, 0.3, 6.5);
                glutSolidCube(3);
            glPopMatrix();

            // --- Pegangan 1 
            glColor3f(0,0,1);
            tiang_lampu(8, 14, 18, 90, 1, 0, 0, 1, 1, 0.6); 
            glColor3f(1,0,0);
            tiang_lampu(3.5, 15, -18, 90, 0, 1, 0, 1, 1, 0.6); 
            
            // --- Pegangan 2 
            glColor3f(0,0,1);
            tiang_lampu(8, 14, -18, 90, 1, 0, 0, 1, 1, 0.6); 
            glColor3f(1,0,0);
            tiang_lampu(3.5, 15, 18, 90, 0, 1, 0, 1, 1, 0.6); 

            // --- Baut Bulat 1 ---
            glColor3f(1, 0, 0);
            bola(-2, 5, 0, 0, 1, 0, 0, 1, 1, 1); 
            
            // --- Baut Bulat 2 ---
            glColor3f(1, 0, 0);
            bola(16.5, 5, 0, 0, 1, 0, 0, 1, 1, 1); 

        glPopMatrix();
    glPopMatrix(); 
    // =========================================================

    glutSwapBuffers();
}

// -------------------------------------------------------------
void init() {
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_COLOR_MATERIAL);
    glClearColor(1,1,1,1);

    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);

    float lightPos[] = { 10, 10, 10, 1 };
    glLightfv(GL_LIGHT0, GL_POSITION, lightPos);
}

// -------------------------------------------------------------
void reshape(int w, int h) {
    if (h == 0) h = 1;
    glViewport(0, 0, w, h);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(60, (float)w / h, 1, 1000);
    glMatrixMode(GL_MODELVIEW);
}

// -------------------------------------------------------------
int main(int argc, char** argv) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
    glutInitWindowSize(800, 600);
    glutCreateWindow("Rizal Septiazi_jungkatjungkit");

    init();

    glutDisplayFunc(display);
    glutIdleFunc(display);
    glutReshapeFunc(reshape);
    glutKeyboardFunc(keyboard);

    glutMainLoop();
    return 0;
}
#endif