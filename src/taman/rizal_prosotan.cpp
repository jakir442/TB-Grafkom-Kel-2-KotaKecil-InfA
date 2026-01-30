#include <GL/glut.h>
#include <GL/glu.h>
#include <cmath>

#ifdef STANDALONE 
// Variabel Kamera
float kameraX = 0.0f;
float kameraY = 5.0f;
float kameraZ = 15.0f;
#endif

// -------------------------------------------------------------
// Fungsi untuk membuat Tangga/Tiang
// -------------------------------------------------------------
void buat_tiang(float x, float y, float z, float h) {
    glPushMatrix();
    glTranslatef(x, y, z);
    glRotatef(-90, 1, 0, 0); // Berdiri tegak
    GLUquadric* quad = gluNewQuadric();
    gluCylinder(quad, 0.1, 0.1, h, 10, 10);
    glPopMatrix();
}

// -------------------------------------------------------------
// Fungsi Menggambar Perosotan
// -------------------------------------------------------------
void drawPerosotan() {
    // 1. Tiang Penyangga Platform (4 Tiang)
    glColor3f(0.6f, 0.3f, 0.1f); // Warna Kayu Cokelat
    buat_tiang(-1.0f, -1.0f, -1.0f, 3.0f);
    buat_tiang(1.0f, -1.0f, -1.0f, 3.0f);
    buat_tiang(-1.0f, -1.0f, 1.0f, 3.0f);
    buat_tiang(1.0f, -1.0f, 1.0f, 3.0f);

    // 2. Platform Atas (Tempat Duduk)
    glColor3f(0.8f, 0.4f, 0.1f);
    glPushMatrix();
    glTranslatef(0.0f, 2.0f, 0.0f);
    glScalef(2.2f, 0.2f, 2.2f);
    glutSolidCube(1.0f);
    glPopMatrix();

    // 3. Tangga (Anak Tangga)
    glColor3f(0.5f, 0.25f, 0.1f);
    for(float i = -1.0f; i <= 2.0f; i += 0.6f) {
        glPushMatrix();
        glTranslatef(0.0f, i, 1.0f);
        glScalef(1.8f, 0.1f, 0.2f);
        glutSolidCube(1.0f);
        glPopMatrix();
    }

    // 4. Papan Seluncur (Warna Biru)
    glColor3f(0.0f, 0.5f, 0.8f);
    glPushMatrix();
    glTranslatef(0.0f, 0.8f, -2.5f); // Posisi papan seluncur
    glRotatef(-40, 1, 0, 0);         // Kemiringan seluncur
    glScalef(1.8f, 0.15f, 5.0f);     // Panjang seluncur
    glutSolidCube(1.0f);
    glPopMatrix();

    // Pinggiran Papan Seluncur (Kanan & Kiri)
    glColor3f(0.0f, 0.4f, 0.7f);
    float sidePos[] = {-0.9f, 0.9f};
    for(int i=0; i<2; i++) {
        glPushMatrix();
        glTranslatef(sidePos[i], 0.9f, -2.5f);
        glRotatef(-40, 1, 0, 0);
        glScalef(0.1f, 0.4f, 5.0f);
        glutSolidCube(1.0f);
        glPopMatrix();
    }
}

#ifdef STANDALONE 
// -------------------------------------------------------------
void display() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glLoadIdentity();

    gluLookAt(kameraX, kameraY, kameraZ, 0, 1, 0, 0, 1, 0);

    // Lantai dasar tipis (Rumput)
    glColor3f(0.2f, 0.6f, 0.2f);
    glPushMatrix();
    glTranslatef(0, -1.1f, 0);
    glScalef(10, 0.1, 10);
    glutSolidCube(1.0f);
    glPopMatrix();

    // Gambar Perosotan
    drawPerosotan();

    glutSwapBuffers();
}

void init() {
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_COLOR_MATERIAL);
    glClearColor(0.5f, 0.8f, 1.0f, 1.0f); // Langit Biru
    
    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);
    float lightPos[] = { 5, 10, 5, 1 };
    glLightfv(GL_LIGHT0, GL_POSITION, lightPos);
}

void reshape(int w, int h) {
    if (h == 0) h = 1;
    glViewport(0, 0, w, h);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(45, (float)w / h, 1, 100);
    glMatrixMode(GL_MODELVIEW);
}

int main(int argc, char** argv) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
    glutInitWindowSize(800, 600);
    glutCreateWindow("rizal prosotan");

    init();
    glutDisplayFunc(display);
    glutReshapeFunc(reshape);
    glutMainLoop();
    return 0;
}
#endif