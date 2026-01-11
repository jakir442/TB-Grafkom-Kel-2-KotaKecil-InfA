#include <GL/glut.h>
#include <math.h>

#include "header/erika-pohon.h"

float angle = 2.3f;

float sudutDaunX = 0.0f;
float sudutDaunY = 0.0f;
float sudutDaunZ = 0.0f;

void drawSilinder(float radius, float tinggi) { // ini untuk menggambar silinder
    GLUquadric *objek = gluNewQuadric();
    gluCylinder(objek, radius, radius, tinggi, 32, 32); // -> Objek Pohon 1
    gluDeleteQuadric(objek);
}

void drawApel() { 
	glColor3f(1.0f, 0.0f, 0.0f); // merah 
	glutSolidSphere(0.15f, 30, 30); // -> Objek Pohon 2
}

void drawPohon() {
    glPushMatrix();

    float tinggiBatang = 1.4f;

    // BATANG UTAMA
    glColor3f(0.45f, 0.22f, 0.05f);
    glPushMatrix();
        glTranslatef(0.0f, 2.1f, 0.0f);
        glRotatef(90, 1, 0, 0);
        drawSilinder(0.22f, tinggiBatang);
    glPopMatrix();

    // RANTING TENGAH
    glColor3f(0.55f, 0.30f, 0.1f);
    glPushMatrix();
        glTranslatef(0.0f, 2.0f, 0.0f);
        glRotatef(-90, 1, 0, 0);
        drawSilinder(0.11f, 0.6f);
    glPopMatrix();
	
    // DAUN TENGAH
    glColor3f(0.1f, 0.75f, 0.1f);
	glPushMatrix();
        glTranslatef(0.0f, 2.78f, 0.0f);
        glRotatef(sudutDaunX, 1, 0, 0);
        glRotatef(sudutDaunY, 0, 1, 0);
        glRotatef(sudutDaunZ, 0, 0, 1);
        glScalef(0.55f, 0.55f, 0.55f);

        // ISI DAUN
        glEnable(GL_POLYGON_OFFSET_FILL);
        glPolygonOffset(1.0f, 1.0f);
        glColor3f(0.1f, 0.75f, 0.1f);
        glutSolidDodecahedron(); // -> Objek Pohon 3
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
        drawSilinder(0.10f, 1.1f);
    glPopMatrix();
    
    // ================= APEL KIRI ================= 
    // buahnya ke atas
	glPushMatrix(); 
		glTranslatef(-1.2f, 2.0f, 0.5f); // geser ke kiri, atas, ke depan 
		drawApel(); 
	glPopMatrix();

    // buahnya ke bawah
	glPushMatrix(); 
		glTranslatef(-1.3f, 1.8f, -0.1f); // geser ke kiri, atas, ke depan 
		drawApel(); 
	glPopMatrix();

    // DAUN KIRI
    glColor3f(0.1f, 0.8f, 0.1f);
	glPushMatrix();
        glTranslatef(-1.0f, 2.4f, 0.0f);
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
        drawSilinder(0.10f, 1.1f);
    glPopMatrix();
	
	// ================= APEL KANAN ================= 
    // buahnya ke atas
	glPushMatrix(); 
		glTranslatef(1.2f, 2.2f, 0.6f); 
		drawApel(); 
	glPopMatrix();

    // buahnya ke bawah
    glPushMatrix(); 
		glTranslatef(1.3f, 1.9f, 0.1f); // geser ke kiri, atas, ke depan 
		drawApel(); 
	glPopMatrix();
	
    // DAUN KANAN
    glColor3f(0.1f, 0.8f, 0.1f);
    glPushMatrix();
        glTranslatef(1.0f, 2.4f, 0.0f);
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
        case 'g': 
			sudutDaunX += 5; 
			break;
        case 't': 
			sudutDaunX -= 5; 
			break;

        case 'h': 
			sudutDaunY += 5; 
			break;
        case 'f': 
			sudutDaunY -= 5; 
			break;

        case 'r': 
			sudutDaunZ += 5; 
			break;
        case 'y': 
			sudutDaunZ -= 5; 
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
        keyboardPohon(key);
        
        switch (key){
        	case 27: exit(0); //esc
		}
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

    void reshape(int lebar, int tinggi) {
        if (tinggi == 0) tinggi = 1;
        float rasio = (float)lebar / tinggi;

        glViewport(0, 0, lebar, tinggi);
        glMatrixMode(GL_PROJECTION);
        glLoadIdentity();
        gluPerspective(45, rasio, 0.1, 100);
        glMatrixMode(GL_MODELVIEW);
    }

    void display() {
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glLoadIdentity();

        gluLookAt(4, 4, 6,   // posisi kamera
                0, 1, 0,   // lihat ke arah
                0, 1, 0);  // up vector

        // rotasi animasi
        glRotatef(angle, 0, 1, 0);

        glPushMatrix();
             // rotasi untuk melihat 3D
            glRotatef(rotSceneX, 1.0f, 0.0f, 0.0f);
            glRotatef(rotSceneY, 0.0f, 1.0f, 0.0f);

            drawPohon();
        glPopMatrix();

        drawGrid3D();

        glutSwapBuffers();
    }

    int main(int argc, char** argv) {
        glutInit(&argc, argv);
        glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
        glutInitWindowSize(800, 600);
        glutCreateWindow("Erika - Pohon 3D");

        glEnable(GL_NORMALIZE);

        glutKeyboardFunc(keyboardPrivate);
        glutSpecialFunc(specialKeyboardRotasi);
        glutDisplayFunc(display); // tampilan layar
        glutReshapeFunc(reshape); // ubah ukuran

        glEnable(GL_NORMALIZE);
        glEnable(GL_DEPTH_TEST);
        glEnable(GL_POLYGON_OFFSET_FILL);

        glutMainLoop();
        return 0;
    }
#endif
