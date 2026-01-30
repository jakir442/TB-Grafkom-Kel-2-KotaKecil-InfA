#include <GL/glut.h>
#include <GL/glu.h>

void init() {
    glEnable(GL_DEPTH_TEST);

    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);

    GLfloat lightPos[] = { 5.0f, 8.0f, 5.0f, 1.0f };
    GLfloat lightAmbient[]  = { 0.3f, 0.3f, 0.3f, 1.0f };
    GLfloat lightDiffuse[]  = { 0.9f, 0.9f, 0.9f, 1.0f };
    GLfloat lightSpecular[] = { 1.0f, 1.0f, 1.0f, 1.0f };

    glLightfv(GL_LIGHT0, GL_POSITION, lightPos);
    glLightfv(GL_LIGHT0, GL_AMBIENT,  lightAmbient);
    glLightfv(GL_LIGHT0, GL_DIFFUSE,  lightDiffuse);
    glLightfv(GL_LIGHT0, GL_SPECULAR, lightSpecular);

    glEnable(GL_COLOR_MATERIAL);
    glColorMaterial(GL_FRONT, GL_AMBIENT_AND_DIFFUSE);
}

//teks
void drawText(const char* text) {
    glScalef(0.003f, 0.003f, 0.003f);
    for (int i = 0; text[i] != '\0'; i++) {
        glutStrokeCharacter(GLUT_STROKE_ROMAN, text[i]);
    }
}

//pilar
void drawPillar(float height) {
    GLUquadric* quad = gluNewQuadric();
    gluQuadricNormals(quad, GLU_SMOOTH);

    // pondasi
    glColor3f(0.4f, 0.4f, 0.4f);
    
    glPushMatrix();
        glTranslatef(0, 0.15f, 0);
        glScalef(0.7f, 0.3f, 0.7f);
        glutSolidCube(1);
    glPopMatrix();

    //tiang utama
    glColor3f(0.75f, 0.75f, 0.75f);
    glPushMatrix();
        glTranslatef(0, 0.3f, 0);
        glRotatef(-90, 1, 0, 0);
        gluCylinder(quad, 0.25f, 0.25f, height, 40, 40);
    glPopMatrix();
    
    //tutup atas
    glColor3f(0.7f, 0.7f, 0.7f);
    glPushMatrix();
        glTranslatef(0, 0.3f + height, 0); // ke ujung atas cylinder
        glRotatef(-90, 1, 0, 0);
        gluDisk(quad, 0.0f, 0.25f, 40, 1);
    glPopMatrix();
    
    //tutup bawah
    glPushMatrix();
        glTranslatef(0, 0.3f, 0); // bawah cylinder
        glRotatef(90, 1, 0, 0);
        gluDisk(quad, 0.0f, 0.25f, 40, 1);
    glPopMatrix();

    gluDeleteQuadric(quad);
}

//Balok atas
void drawTopBeam(float width) {
    glColor3f(0.55f, 0.27f, 0.07f); // coklat kayu
    glPushMatrix();
        glScalef(width, 0.35f, 0.5f);
        glutSolidCube(1);
    glPopMatrix();
}

//Atap segitiga
void drawRoof() {
    glColor3f(0.6f, 0.1f, 0.1f); // merah genteng
    glBegin(GL_TRIANGLES);
        // depan
        glVertex3f(-2.2f, 0,  0.3f);
        glVertex3f( 2.2f, 0,  0.3f);
        glVertex3f( 0.0f, 0.8f, 0.0f);

        // belakang
        glVertex3f(-2.2f, 0, -0.3f);
        glVertex3f( 2.2f, 0, -0.3f);
        glVertex3f( 0.0f, 0.8f, 0.0f);
    glEnd();
}

//Gapura
void drawGate() {
    float pillarHeight = 3.0f;
    float width = 4.0f;

    //tiang kiri
    glPushMatrix();
        glTranslatef(-width / 2, 0, 0);
        drawPillar(pillarHeight);
    glPopMatrix();

    //tiang kanan
    glPushMatrix();
        glTranslatef(width / 2, 0, 0);
        drawPillar(pillarHeight);
    glPopMatrix();

    // balok utama
    glPushMatrix();
        glTranslatef(0, pillarHeight + 0.2f, 0);
        drawTopBeam(width);
    glPopMatrix();

    //atap
    glPushMatrix();
        glTranslatef(0, pillarHeight + 0.4f, 0);
        drawRoof();
    glPopMatrix();

    //papan tulisan
    glColor3f(0.3f, 0.15f, 0.05f);
    glPushMatrix();
        glTranslatef(0, pillarHeight + 0.2f, 0.35f);
        glScalef(3.0f, 0.6f, 0.1f);
        glutSolidCube(1);
    glPopMatrix();

    //tulisan
    glDisable(GL_LIGHTING);
    glColor3f(1, 1, 1);
    glPushMatrix();
        glTranslatef(-0.9f, pillarHeight + 0.10f, 0.42);
        drawText("KEL 2");
    glPopMatrix();
    glEnable(GL_LIGHTING);
}

void drawGrid() {
    glDisable(GL_LIGHTING);
    glColor3f(0.3f, 0.3f, 0.3f);
    glBegin(GL_LINES);
    for (int i = -10; i <= 10; i++) {
        glVertex3f(i, 0, -10);
        glVertex3f(i, 0,  10);
        glVertex3f(-10, 0, i);
        glVertex3f( 10, 0, i);
    }
    glEnd();
    glEnable(GL_LIGHTING);
}

void display() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glLoadIdentity();

    gluLookAt(6, 5, 9,  0, 2, 0,  0, 1, 0);

    drawGrid();

    glPushMatrix();
        drawGate();
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
    glutCreateWindow("Gapura Taman");

    init();
    glutDisplayFunc(display);
    glutReshapeFunc(reshape);
    glutMainLoop();
    return 0;
}
