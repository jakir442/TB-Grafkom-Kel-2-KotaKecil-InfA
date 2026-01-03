#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glut.h>
#include <cstdio>

#include "header/azhari-gedung.h"

// Variabel transformasi objek
float angleX = 0.0f;      // Rotasi terhadap sumbu X (derajat)
float angleY = 0.0f;      // Rotasi terhadap sumbu Y (derajat)
float scaleFactor = 1.0f; // Faktor skala objek

// Konstanta kontrol transformasi
float ROT_STEP   = 5.0f;   // Besar perubahan rotasi
float SCALE_STEP = 0.1f;   // Besar perubahan skala
float SCALE_MIN  = 0.1f;   // Skala minimum
float SCALE_MAX  = 10.0f;  // Skala maksimum

// Warna material objek
GLfloat wallColor[]   = {0.85f, 0.9f, 0.95f, 1.0f}; // Warna dinding gedung
GLfloat windowColor[] = {0.2f, 0.6f, 1.0f, 1.0f};   // Warna jendela
GLfloat doorColor[]   = {1.0f, 0.45f, 0.0f, 1.0f};  // Warna pintu

void initRendering() {
    // Mengaktifkan depth test agar objek memiliki kedalaman
    glEnable(GL_DEPTH_TEST);

    // Warna latar belakang
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
}

// FUNGSI PRIMITIF OBJEK 3D
// Kubus satuan sebagai dasar pemodelan
void drawUnitCube() {
    glutSolidCube(1.0f); // -> Objek Gedung 1
}

// Balok (prisma segi empat) dengan ukuran tertentu
void Prisma(float width, float height, float depth) {
    glPushMatrix();
        glScalef(width, height, depth);
        drawUnitCube();
    glPopMatrix();
}

// Fungsi jendela Depan
void jendelaDepan(float x, float y) {
    glPushMatrix();
        glTranslatef(x, y, 0.63f);   // Posisi di depan gedung
        glScalef(0.60f, 0.45f, 0.02f); // Ukuran jendela
        glutSolidCube(1.0f); 
    glPopMatrix();
}

// Fungsi jendela Belakang
void jendelaBelakang(float x, float y) {
    glPushMatrix();
        glTranslatef(x, y, -0.63f);   // Posisi di depan gedung
        glScalef(0.60f, 0.45f, 0.02f); // Ukuran jendela
        glutSolidCube(1.0f);
    glPopMatrix();
}

// Atap gedung 
void atapgedung() {
    GLUquadric* quad = gluNewQuadric();

    glPushMatrix();
        // POSISI SAMA seperti atap kotak lama
        glTranslatef(0.0f, 1.45f, 0.0f);

        // Warna helipad
        glColor3f(0.9f, 0.9f, 0.95f);

        // BADAN SILINDER 
        glPushMatrix();
            glRotatef(-90.0f, 1, 0, 0); // silinder berdiri
            gluCylinder(
                quad,
                1.40f,   // radius bawah (lebar)
                1.40f,   // radius atas
                0.15f,  // tinggi (ketebalan atap)
                40,
                5
            );
        glPopMatrix();

        // TUTUP ATAS (BIAR TIDAK BOLONG)
        glPushMatrix();
            glTranslatef(0.0f, 0.15f, 0.0f);
            glRotatef(-90.0f, 1, 0, 0);
            gluDisk(
                quad,
                0.0f,
                1.40f,
                40,
                1
            );
        glPopMatrix();

        // TUTUP BAWAH
        glPushMatrix();
            glRotatef(-90.0f, 1, 0, 0);
            gluDisk(
                quad,
                0.0f,
                1.40f,
                40,
                1
            );
        glPopMatrix();

    glPopMatrix();
    gluDeleteQuadric(quad);
}

void ringHelipadTorus() {
    glPushMatrix();
        
        glTranslatef(0.0f, 1.60f, 0.0f);

        
        glRotatef(90.0f, 1.0f, 0.0f, 0.0f);

        glColor3f(1.0f, 0.0f, 0.0f); // merah

        glutSolidTorus(
            0.05f,  // radius kecil (ketebalan garis)
            0.8f,   // radius besar (jarak dari pusat)
            40,     // detail kecil
            60      // detail besar
        );
    glPopMatrix();
}
void hurufHHelipad() {
    glColor3f(1.0f, 0.0f, 0.0f); // merah

    // Kaki kiri H
    glPushMatrix();
        glTranslatef(-0.3f, 1.60f, 0.0f);
        glScalef(0.15f, 0.02f, 0.6f);
        glutSolidCube(1.0f);
    glPopMatrix();

    // Kaki kanan H
    glPushMatrix();
        glTranslatef(0.3f, 1.60f, 0.0f);
        glScalef(0.15f, 0.02f, 0.6f);
        glutSolidCube(1.0f);
    glPopMatrix();

    // Garis tengah H
    glPushMatrix();
        glTranslatef(0.0f, 1.60f, 0.0f);
        glScalef(0.6f, 0.02f, 0.15f);
        glutSolidCube(1.0f);
    glPopMatrix();
}

//  PEMODELAN OBJEK GEDUNG 3D
void drawGedung() {
    // Badan utama gedung
    glColor3fv(wallColor);
    Prisma(2.0f, 3.0f, 1.2f);
	
	// Atap Gedung
	atapgedung();
	
	// Lingkaran Helipad
	ringHelipadTorus();
	
	//huruf H
	hurufHHelipad();


    // Pintu gedung
    glPushMatrix();
        glTranslatef(0.0f, -0.98f, 0.63f);
        glColor3fv(doorColor);
        glScalef(0.5f, 0.8f, 0.02f);
        glutSolidCube(1.0f);
    glPopMatrix();

    // Jendela depan gedung
     glColor3fv(windowColor);

    // Baris atas
	jendelaDepan(-0.45f,  0.98f);
	jendelaDepan( 0.45f,  0.98f);
	
	// Baris tengah
	jendelaDepan(-0.45f,  0.4f);
	jendelaDepan( 0.45f,  0.4f);
	
	// Baris bawah 
	jendelaDepan(-0.45f, -0.2f);
	jendelaDepan( 0.45f, -0.2f);
	
	// Jendela depan belakang
     glColor3fv(windowColor);

    // Baris atas
	jendelaBelakang(-0.45f,  0.98f);
	jendelaBelakang( 0.45f,  0.98f);
	
	// Baris tengah
	jendelaBelakang(-0.45f,  0.4f);
	jendelaBelakang( 0.45f,  0.4f);
	
	// Baris bawah 
	jendelaBelakang(-0.45f, -0.2f);
	jendelaBelakang( 0.45f, -0.2f);
	
}

void handleKeyboardGedung(unsigned char key) {
    switch (key) {
        case 27: exit(0); break; // Tombol ESC
        case '+': case '=':
            scaleFactor += SCALE_STEP;
            if (scaleFactor > SCALE_MAX) scaleFactor = SCALE_MAX;
            break;
        case '-': case '_':
            scaleFactor -= SCALE_STEP;
            if (scaleFactor < SCALE_MIN) scaleFactor = SCALE_MIN;
            break;
        case 'r': case 'R':
            angleX = 0.0f;
            angleY = 0.0f;
            scaleFactor = 1.0f;
            break;
    }
    glutPostRedisplay();
}

// AKSES PRIVATE
#ifdef STANDALONE   
    // untuk keyboard Arrow
    float rotSceneY = 0.0f; // kiri-kanan
    float rotSceneX = 0.0f; // atas-bawah

    void keyboardPrivate(unsigned char key, int x, int y) {
        handleKeyboardGedung(key);

        glutPostRedisplay();
    }

    void specialKeyboardRotasi(int key, int x, int y) {
        switch (key) {
            case GLUT_KEY_LEFT:
                rotSceneY -= 5.0f;
                break;
            case GLUT_KEY_RIGHT:
                rotSceneY += 5.0f;
                break;
            case GLUT_KEY_UP:
                rotSceneX -= 5.0f;
                break;
            case GLUT_KEY_DOWN:
                rotSceneX += 5.0f;
                break;
            }
        glutPostRedisplay();
    }

    // grid 3D (GARIS BACKGROUND)
    void drawGrid3D() {
        glColor3f(0.3f, 0.3f, 0.3f); // warna line gridnyaa
        glLineWidth(1.0f);     //ketebalan garis

        glTranslatef(0.0f, -2.0f, 0.0f);

        glBegin(GL_LINES); // Objek Private
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

    void reshape(int w, int h) {
        if (h == 0) h = 1;
        float aspect = (float)w / (float)h;

        glViewport(0, 0, w, h);
        glMatrixMode(GL_PROJECTION);
        glLoadIdentity();
        gluPerspective(45.0, aspect, 0.1, 100.0);
        glMatrixMode(GL_MODELVIEW);
    }

    void display() {
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        glMatrixMode(GL_MODELVIEW);
        glLoadIdentity();

        // Kamera menggunakan proyeksi perspektif
        gluLookAt(4.0, 3.0, 6.0,
                0.0, 0.0, 0.0,
                0.0, 1.0, 0.0);

        // Transformasi skala dan rotasi
        glScalef(scaleFactor, scaleFactor, scaleFactor);
        glRotatef(angleX, 1.0f, 0.0f, 0.0f);
        glRotatef(angleY, 0.0f, 1.0f, 0.0f);

        // Menggambar objek gedung
        glPushMatrix();
            // rotasi untuk melihat 3D
            glRotatef(rotSceneX, 1.0f, 0.0f, 0.0f);
            glRotatef(rotSceneY, 0.0f, 1.0f, 0.0f);

            drawGedung();
        glPopMatrix();

        drawGrid3D(); // alas kotak-kotak

        glutSwapBuffers();
    }

    //  FUNGSI UTAMA PROGRAM
    int main(int argc, char** argv) {
        glutInit(&argc, argv);
        glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH);
        glutInitWindowSize(900, 600);
        glutInitWindowPosition(200, 100);
        glutCreateWindow("Azhari - Gedung 3D");

        initRendering();

        glutKeyboardFunc(keyboardPrivate);
        glutSpecialFunc(specialKeyboardRotasi);
        glutReshapeFunc(reshape);
        glutDisplayFunc(display);
        
        glutMainLoop();
        return 0;
    }
#endif
