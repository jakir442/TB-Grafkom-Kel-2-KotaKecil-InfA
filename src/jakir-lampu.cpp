#include <GL/freeglut.h>
#include <GL/glu.h>
#include <cmath>

bool lampuNyala = true;
int  levelLampu = 0;

float rotLampuY = 0.0f; // kiri-kanan
float rotLampuX = 0.0f; // atas-bawah

float rotSceneY = 0.0f; // kiri-kanan
float rotSceneX = 0.0f; // atas-bawah

// inisialisasi lighting
void initLampuLighting() {
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_LIGHTING);

    // ambient malam
    GLfloat ambientGlobal[] = {0.2f, 0.2f, 0.25f, 1.0f};
    glLightModelfv(GL_LIGHT_MODEL_AMBIENT, ambientGlobal);

    glEnable(GL_COLOR_MATERIAL);
    glColorMaterial(GL_FRONT, GL_AMBIENT_AND_DIFFUSE);
    glEnable(GL_NORMALIZE);
    glShadeModel(GL_SMOOTH);

    glEnable(GL_LIGHT0);
    glDisable(GL_LIGHT2);
    glDisable(GL_LIGHT3);
    glDisable(GL_LIGHT4);
    glDisable(GL_LIGHT5);
    glDisable(GL_LIGHT6);
    glDisable(GL_LIGHT7);
}

// gambar 1 lampu jalan
void drawLampuJalan(float x, float z, GLenum lightID) {
    float intensitasDiffuse;
    float intensitasAmbient;

    switch (levelLampu) {
    case 1:
        intensitasDiffuse = 0.6f;
        intensitasAmbient = 0.05f;
        break;
    case 2:
        intensitasDiffuse = 1.0f;
        intensitasAmbient = 0.12f;
        break;
    case 3:
        intensitasDiffuse = 1.6f;
        intensitasAmbient = 0.25f;
        break;
    default:
        intensitasDiffuse = 0.0f;
        intensitasAmbient = 0.0f;
    }

    GLUquadric* quad = gluNewQuadric();
    gluQuadricNormals(quad, GLU_SMOOTH);

    glPushMatrix();
    glTranslatef(x, 0.0f, z);

    // tiang lampu
    glPushMatrix();
        glColor3f(0.45f, 0.45f, 0.45f);
        glTranslatef(0.0f, 0.0f, 0.0f);
        glRotatef(-90, 1, 0, 0);
        gluCylinder(quad, 0.12, 0.12, 2.7, 20, 20);
    glPopMatrix();

    // tangan lampu
    glPushMatrix();
        glColor3f(0.35f, 0.35f, 0.35f);
        glTranslatef(0.0f, 2.7f, 0.3f);
        glScalef(0.12f, 0.12f, 0.8f);
        glutSolidCube(1.0f);
    glPopMatrix();

    // kepala lampu (RUMAH)
    glPushMatrix();
        glTranslatef(0.0f, 2.7f, 0.75f);
        glColor3f(0.18f, 0.18f, 0.18f);
        glScalef(0.7f, 0.28f, 0.9f);
        glutSolidCube(1.0f);
    glPopMatrix();

    // bohlam
    glPushMatrix();
        glTranslatef(0.0f, 2.58f, 0.75f);
        if (lampuNyala)
            glColor3f(1.0f, 1.0f, 0.75f);
        else
            glColor3f(0.2f, 0.2f, 0.2f);
        glutSolidSphere(0.17f, 20, 20);
    glPopMatrix();

    // lighting - setelah objek
    if (!lampuNyala) {
        glDisable(lightID);
        glPopMatrix();
        gluDeleteQuadric(quad);
        return;
    }

    // posisi bohlam (lokal lampu)
    GLfloat pos[] = { 0.0f, 2.58f, 0.75f, 1.0f };

    // arah cahaya ke depan & ke bawah
    GLfloat dir[] = { 0.0f, -0.8f, 0.6f };

    GLfloat diffuse[] = {
        intensitasDiffuse,
        intensitasDiffuse,
        intensitasDiffuse * 0.85f,
        1.0f
    };

    GLfloat ambient[] = {
        intensitasAmbient,
        intensitasAmbient,
        intensitasAmbient,
        1.0f
    };

    glEnable(lightID);
    glLightfv(lightID, GL_POSITION, pos);
    glLightfv(lightID, GL_SPOT_DIRECTION, dir);
    glLightf(lightID, GL_SPOT_CUTOFF, 40.0f);
    glLightf(lightID, GL_SPOT_EXPONENT, 20.0f);
    glLightfv(lightID, GL_DIFFUSE, diffuse);
    glLightfv(lightID, GL_AMBIENT, ambient);

    glPopMatrix();
}

void keyboardLampu(unsigned char key) {
    // tombol L untuk menaikan levelLampu
    if (key == 'l' || key == 'L') {
        if (levelLampu < 3) {
            levelLampu++;
        }
    }

    // tombol O untuk menurunkan levelLampu
    if (key == 'o' || key == 'O') {
        if (levelLampu > 0) {
            levelLampu--;
        }
    }

    lampuNyala = (levelLampu > 0);
}

// AKSES PRIVATE
#ifdef STANDALONE
    void keyboardPrivate(unsigned char key, int x, int y) {
        keyboardLampu(key);

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

    void reshape(int w, int h) {
        glViewport(0, 0, w, h);
        glMatrixMode(GL_PROJECTION);
        glLoadIdentity();
        gluPerspective(60.0, (float)w / h, 0.1f, 100.0f);
        glMatrixMode(GL_MODELVIEW);
    }

    void display() {
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        glMatrixMode(GL_MODELVIEW);
        glLoadIdentity();

        gluLookAt(
            0.0f, 3.0f, 8.0f,
            0.0f, 1.5f, 0.0f,
            0.0f, 1.0f, 0.0f
        );

        GLfloat matDiffuse[] = {1.0f, 1.0f, 1.0f, 1.0f};
        GLfloat matAmbient[] = {0.3f, 0.3f, 0.3f, 1.0f};
        GLfloat matSpec[]    = {0.8f, 0.8f, 0.8f, 1.0f};
        GLfloat matShine[]   = {32.0f};

        glMaterialfv(GL_FRONT, GL_DIFFUSE, matDiffuse);
        glMaterialfv(GL_FRONT, GL_AMBIENT, matAmbient);
        glMaterialfv(GL_FRONT, GL_SPECULAR, matSpec);
        glMaterialfv(GL_FRONT, GL_SHININESS, matShine);

        // arah cahaya ke depan & ke bawah
        GLfloat dir[] = { 0.0f, -0.8f, 0.6f };

        glLightfv(GL_LIGHT0, GL_SPOT_DIRECTION, dir);
        glLightf(GL_LIGHT0, GL_SPOT_CUTOFF, 40.0f);
        glLightf(GL_LIGHT0, GL_SPOT_EXPONENT, 12.0f);
        
        glPushMatrix();
            // rotasi untuk melihat 3D
            glRotatef(rotSceneX, 1.0f, 0.0f, 0.0f);
            glRotatef(rotSceneY, 0.0f, 1.0f, 0.0f);

            drawLampuJalan(0.0f, 0.0f, GL_LIGHT0);
        glPopMatrix();

        drawGrid3D();

        glutSwapBuffers();
    }

    int main(int argc, char** argv) {
        glutInit(&argc, argv);
        glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
        glutInitWindowSize(800, 600);
        glutCreateWindow("Jakir - Lampu Jalan 3D");

        initLampuLighting();

        glClearColor(0.05f, 0.05f, 0.1f, 1.0f);

        glutKeyboardFunc(keyboardPrivate);
        glutSpecialFunc(specialKeyboardRotasi);
        glutReshapeFunc(reshape);
        glutDisplayFunc(display);

        glutMainLoop();
        return 0;
    }
#endif