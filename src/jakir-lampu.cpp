#include <GL/freeglut.h>
#include <GL/glu.h>
#include <cmath>


bool lampuNyala = true;
int  levelLampu = 0;

// =================================================
// INIT LIGHTING
// =================================================
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

// =================================================
// GAMBAR 1 LAMPU JALAN
// =================================================
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

    // ======================
    // TIANG LAMPU
    // ======================
    glPushMatrix();
        glColor3f(0.45f, 0.45f, 0.45f);
        glTranslatef(0.0f, 0.0f, 0.0f);
        glRotatef(-90, 1, 0, 0);
        gluCylinder(quad, 0.12, 0.12, 2.7, 20, 20);
    glPopMatrix();

    // ======================
    // LENGAN LAMPU
    // ======================
    glPushMatrix();
        glColor3f(0.35f, 0.35f, 0.35f);
        glTranslatef(0.0f, 2.7f, 0.3f);
        glScalef(0.12f, 0.12f, 0.8f);
        glutSolidCube(1.0f);
    glPopMatrix();

    // ======================
    // KEPALA LAMPU (RUMAH)
    // ======================
    glPushMatrix();
        glTranslatef(0.0f, 2.7f, 0.75f);
        glColor3f(0.18f, 0.18f, 0.18f);
        glScalef(0.7f, 0.28f, 0.9f);
        glutSolidCube(1.0f);
    glPopMatrix();

    // ======================
    // PENUTUP BAWAH
    // ======================
    glPushMatrix();
        glTranslatef(0.0f, 2.6f, 0.75f);
        glColor3f(0.1f, 0.1f, 0.1f);
        glScalef(0.65f, 0.06f, 0.85f);
        glutSolidCube(1.0f);
    glPopMatrix();

    // ======================
    // BOHLAM
    // ======================
    glPushMatrix();
        glTranslatef(0.0f, 2.58f, 0.75f);
        if (lampuNyala)
            glColor3f(1.0f, 1.0f, 0.75f);
        else
            glColor3f(0.2f, 0.2f, 0.2f);
        glutSolidSphere(0.17f, 20, 20);
    glPopMatrix();

    // ======================
    // LIGHTING (SETELAH OBJEK)
    // ======================
    if (!lampuNyala) {
        glDisable(lightID);
        glPopMatrix();
        gluDeleteQuadric(quad);
        return;
    }

    // posisi bohlam (lokal lampu)
    GLfloat pos[] = { 0.0f, 2.58f, 0.75f, 1.0f };

    // arah cahaya → KE DEPAN & KE BAWAH
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

    // ======================
    // L → TAMBAH TERANG
    // ======================
    if (key == 'l' || key == 'L') {
        if (levelLampu < 3) {
            levelLampu++;
        }
    }

    // ======================
    // O → KURANG TERANG
    // ======================
    if (key == 'o' || key == 'O') {
        if (levelLampu > 0) {
            levelLampu--;
        }
    }

    lampuNyala = (levelLampu > 0);
}



#ifdef STANDALONE

// =================================================
// STANDALONE DISPLAY
// =================================================
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


    glLightfv(GL_LIGHT0, GL_SPOT_DIRECTION, dir);
    glLightf(GL_LIGHT0, GL_SPOT_CUTOFF, 40.0f);
    glLightf(GL_LIGHT0, GL_SPOT_EXPONENT, 12.0f);

    // lantai
    glPushMatrix();
        glColor3f(0.1f, 0.1f, 0.1f);
        glTranslatef(0.0f, -0.01f, 0.0f);
        glScalef(6.0f, 0.05f, 6.0f);
        glutSolidCube(1.0);
    glPopMatrix();

    drawLampuJalan(0.0f, 0.0f, GL_LIGHT0);

    glutSwapBuffers();
}


// =================================================
// RESHAPE
// =================================================
void reshape(int w, int h) {
    glViewport(0, 0, w, h);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(60.0, (float)w / h, 0.1f, 100.0f);
    glMatrixMode(GL_MODELVIEW);
}

// =================================================
// MAIN (STANDALONE)
// =================================================
int main(int argc, char** argv) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
    glutInitWindowSize(800, 600);
    glutCreateWindow("Lampu Jalan - Standalone");

    initLampuLighting();

    glClearColor(0.05f, 0.05f, 0.1f, 1.0f);

    glutKeyboardFunc(keyboard);
    glutDisplayFunc(display);
    glutReshapeFunc(reshape);
    glutIdleFunc(display);

    glutMainLoop();
    return 0;
}
#endif