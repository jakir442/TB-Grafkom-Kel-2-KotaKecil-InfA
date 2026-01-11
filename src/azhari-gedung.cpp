#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glut.h>
#include <cstdio>

#include "header/azhari-gedung.h"

// variabel transformasi objek
float angleX = 0.0f;      // rotasi terhadap sumbu X (derajat)
float angleY = 0.0f;      // rotasi terhadap sumbu Y (derajat)
float scaleFactor = 1.0f; // faktor skala objek

// konstanta kontrol transformasi
float ROT_STEP   = 5.0f;   // besar perubahan rotasi
float SCALE_STEP = 0.1f;   // besar perubahan skala
float SCALE_MIN  = 0.1f;   // skala minimum
float SCALE_MAX  = 10.0f;  // skala maksimum

// warna material objek
GLfloat wallColor[]   = {0.85f, 0.9f, 0.95f, 1.0f}; // warna dinding gedung
GLfloat windowColor[] = {0.2f, 0.6f, 1.0f, 1.0f};   // warna jendela
GLfloat doorColor[]   = {1.0f, 0.45f, 0.0f, 1.0f};  // warna pintu

// warna atap helipad
GLfloat roofSideColor[] = {0.60f, 0.72f, 0.62f, 1.0f}; // hijau sage lembut
GLfloat roofTopColor[]  = {0.74f, 0.84f, 0.76f, 1.0f}; // hijau terang pastel

GLfloat helipadRingColor[]= {1.0f, 0.85f, 0.1f, 1.0f};  // kuning helipad
GLfloat helipadHColor[]   = {0.9f, 0.1f, 0.1f, 1.0f};   // merah tegas

void initRendering() {
    // mengaktifkan depth test agar objek memiliki kedalaman
    glEnable(GL_DEPTH_TEST);

    // warna latar belakang
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
}

// fungsi primitive objek 3d
// Kubus satuan sebagai dasar pemodelan
void drawUnitCube() {
    glutSolidCube(1.0f); //objek gedung 1
}

// balok (prisma segi empat) dengan ukuran tertentu
void Prisma(float width, float height, float depth) {
    glPushMatrix();
        glScalef(width, height, depth);
        drawUnitCube();
    glPopMatrix();
}

// fungsi jendela Depan
void jendelaDepan(float x, float y) {
    glPushMatrix();
        glTranslatef(x, y, 0.63f);   // posisi di depan gedung
        glScalef(0.60f, 0.45f, 0.02f); // ukuran jendela
        glutSolidCube(1.0f); 
    glPopMatrix();
}

// fungsi jendela Belakang
void jendelaBelakang(float x, float y) {
    glPushMatrix();
        glTranslatef(x, y, -0.63f);   
        glScalef(0.60f, 0.45f, 0.02f); // ukuran jendela
        glutSolidCube(1.0f);
    glPopMatrix();
}

// atap gedung 
void atapgedung() {
    GLUquadric* quad = gluNewQuadric();

    glPushMatrix();
        // posisi sama seperti atap kotak lama
        glTranslatef(0.0f, 1.40f, 0.0f);

        // warna helipad
        glColor3fv(roofSideColor);

        // badan silinder 
        glPushMatrix();
            glRotatef(-90.0f, 1, 0, 0); // silinder berdiri
            gluCylinder(
                quad,
                1.90f,   // radius bawah (lebar)
                1.40f,   // radius atas
                0.35f,  // tinggi (ketebalan atap)
                40,
                5
            );
        glPopMatrix();

        // tutup atas (biar tidak bolong)
        glColor3fv(roofTopColor);
        glPushMatrix();
            glTranslatef(0.0f, 0.15f, 0.0f);
            glRotatef(-90.0f, 1, 0, 0);
            gluDisk(
                quad,
                0.0f,
                1.65f,
                40,
                1
            );
        glPopMatrix();

        // tutup bawah
        glColor3fv(roofSideColor);
        glPushMatrix();
            glRotatef(-90.0f, 1, 0, 0);
            gluDisk(
                quad,
                0.0f,
                1.65f,
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

        glColor3fv(helipadRingColor); // merah

        glutSolidTorus(
            0.05f,  // radius kecil (ketebalan garis)
            0.8f,   // radius besar (jarak dari pusat)
            40,     // detail kecil
            60      // detail besar
        );
    glPopMatrix();
}
void hurufHHelipad() {
    glColor3fv(helipadHColor); // merah

    // kaki kiri H
    glPushMatrix();
        glTranslatef(-0.3f, 1.60f, 0.0f);
        glScalef(0.15f, 0.02f, 0.6f);
        glutSolidCube(1.0f);
    glPopMatrix();

    // kaki kanan H
    glPushMatrix();
        glTranslatef(0.3f, 1.60f, 0.0f);
        glScalef(0.15f, 0.02f, 0.6f);
        glutSolidCube(1.0f);
    glPopMatrix();

    // garis tengah H
    glPushMatrix();
        glTranslatef(0.0f, 1.60f, 0.0f);
        glScalef(0.6f, 0.02f, 0.15f);
        glutSolidCube(1.0f);
    glPopMatrix();
}

//  pemodelan objek gedung 3d
void drawGedung() {
    // badan utama gedung
    glColor3fv(wallColor);
    Prisma(2.0f, 3.0f, 1.2f);
	
	// atap Gedung
	atapgedung();
	
	// lingkaran helipad
	ringHelipadTorus();
	
	//huruf H
	hurufHHelipad();


    // pintu gedung
    glPushMatrix();
        glTranslatef(0.0f, -0.98f, 0.63f);
        glColor3fv(doorColor);
        glScalef(0.5f, 0.8f, 0.02f);
        glutSolidCube(1.0f);
    glPopMatrix();

    // jendela depan gedung
     glColor3fv(windowColor);

    // baris atas
	jendelaDepan(-0.45f,  0.98f);
	jendelaDepan( 0.45f,  0.98f);
	
	// baris tengah
	jendelaDepan(-0.45f,  0.4f);
	jendelaDepan( 0.45f,  0.4f);
	
	// baris bawah 
	jendelaDepan(-0.45f, -0.2f);
	jendelaDepan( 0.45f, -0.2f);
	
	// jendela depan belakang
     glColor3fv(windowColor);

    // baris atas
	jendelaBelakang(-0.45f,  0.98f);
	jendelaBelakang( 0.45f,  0.98f);
	
	// baris tengah
	jendelaBelakang(-0.45f,  0.4f);
	jendelaBelakang( 0.45f,  0.4f);
	
	// baris bawah 
	jendelaBelakang(-0.45f, -0.2f);
	jendelaBelakang( 0.45f, -0.2f);
	
}

void handleKeyboardGedung(unsigned char key) {
    switch (key) {
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

// akses private
#ifdef STANDALONE   
    // untuk keyboard Arrow
    float rotSceneY = 0.0f; // kiri-kanan
    float rotSceneX = 0.0f; // atas-bawah

    void keyboardPrivate(unsigned char key, int x, int y) {
		
		if (key == 27) { // esc
        exit(0);
    }
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

    // grid 3d (garis background)
    void drawGrid3D() {
        glColor3f(0.3f, 0.3f, 0.3f); // warna line gridnyaa
        glLineWidth(1.0f);     //ketebalan garis

        glTranslatef(0.0f, -2.0f, 0.0f);

        glBegin(GL_LINES); // objek private
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

        // kamera menggunakan proyeksi perspektif
        gluLookAt(4.0, 3.0, 6.0,
                0.0, 0.0, 0.0,
                0.0, 1.0, 0.0);

        // transformasi skala dan rotasi
        glScalef(scaleFactor, scaleFactor, scaleFactor);
        glRotatef(angleX, 1.0f, 0.0f, 0.0f);
        glRotatef(angleY, 0.0f, 1.0f, 0.0f);

        // menggambar objek gedung
        glPushMatrix();
            // rotasi untuk melihat 3D
            glRotatef(rotSceneX, 1.0f, 0.0f, 0.0f);
            glRotatef(rotSceneY, 0.0f, 1.0f, 0.0f);

            drawGedung();
        glPopMatrix();

        drawGrid3D(); // alas kotak-kotak

        glutSwapBuffers();
    }

    //  fungsi utama program
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
