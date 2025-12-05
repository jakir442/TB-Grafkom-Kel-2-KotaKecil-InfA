#include <GL/freeglut.h>

// Fungsi untuk menggambar gedung
void drawBuilding() {
    // Badan gedung - kubus
    glPushMatrix();
    glColor3f(0.6f, 0.6f, 0.8f); // Warna biru muda
    glScalef(2.0f, 3.0f, 2.0f);   // Skala kubus untuk menjadi gedung
    glutSolidCube(1.0f);
    glPopMatrix();

    // Atap gedung - kerucut
    glPushMatrix();
    glColor3f(0.8f, 0.3f, 0.3f); // Warna merah
    glTranslatef(0.0f, 1.5f + 0.5f, 0.0f); // Pindah ke atas kubus
    glutSolidCone(1.5f, 1.0f, 20, 20);
    glPopMatrix();

    // Tiang di depan gedung - silinder
    glPushMatrix();
    glColor3f(0.3f, 0.3f, 0.3f); // Warna abu
    glTranslatef(-1.0f, -1.5f + 1.0f, 1.5f); // Pindah ke depan
    glRotatef(-90, 1, 0, 0); // Orientasi silinder tegak
    GLUquadric* quad = gluNewQuadric();
    gluCylinder(quad, 0.1f, 0.1f, 2.0f, 20, 20);
    gluDeleteQuadric(quad);
    glPopMatrix();
}

// Fungsi display
void display() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glLoadIdentity();

    // Atur kamera
    gluLookAt(5.0, 5.0, 10.0,  // Posisi kamera
              0.0, 0.0, 0.0,   // Titik fokus
              0.0, 1.0, 0.0);  // Arah atas

    drawBuilding();

    glutSwapBuffers();
}

// Fungsi untuk inisialisasi
void init() {
    glEnable(GL_DEPTH_TEST);
    glClearColor(0.5f, 0.8f, 0.9f, 1.0f); // Warna langit biru muda
}

// Fungsi reshape
void reshape(int w, int h) {
    glViewport(0, 0, w, h);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(45, (float)w / h, 1, 100);
    glMatrixMode(GL_MODELVIEW);
}

// Fungsi utama
int main(int argc, char** argv) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
    glutInitWindowSize(800, 600);
    glutCreateWindow("Gedung 3 Primitive");

    init();

    glutDisplayFunc(display);
    glutReshapeFunc(reshape);

    glutMainLoop();
    return 0;
}
