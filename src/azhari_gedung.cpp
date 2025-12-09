// File: gedung3d.cpp
// Compile: g++ gedung3d.cpp -o gedung3d -lGL -lGLU -lglut
#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glut.h>
#include <cstdio>

// Transform controls
static float angleX = 20.0f;    // rotasi X (derajat)
static float angleY = -25.0f;   // rotasi Y (derajat)
static float scaleFactor = 1.0f;

// Increment settings
const float ROT_STEP = 5.0f;
const float SCALE_STEP = 0.1f;
const float SCALE_MIN = 0.1f;
const float SCALE_MAX = 10.0f;

// Window / door colors
GLfloat wallColor[]   = {0.85f, 0.9f, 0.95f, 1.0f}; // terang putih kebiruan
GLfloat windowColor[] = {0.2f, 0.6f, 1.0f, 1.0f};   // biru jendela
GLfloat doorColor[]   = {1.0f, 0.45f, 0.0f, 1.0f};  // oranye pintu

void initRendering() {
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_NORMALIZE);
    glEnable(GL_COLOR_MATERIAL);
    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);

    // Light0 (simple white)
    GLfloat lightPos[]    = {4.0f, 6.0f, 6.0f, 1.0f};
    GLfloat lightAmbient[]  = {0.15f, 0.15f, 0.15f, 1.0f};
    GLfloat lightDiffuse[]  = {0.85f, 0.85f, 0.85f, 1.0f};
    GLfloat lightSpecular[] = {0.9f, 0.9f, 0.9f, 1.0f};
    glLightfv(GL_LIGHT0, GL_POSITION, lightPos);
    glLightfv(GL_LIGHT0, GL_AMBIENT,  lightAmbient);
    glLightfv(GL_LIGHT0, GL_DIFFUSE,  lightDiffuse);
    glLightfv(GL_LIGHT0, GL_SPECULAR, lightSpecular);

    // Material
    GLfloat spec[] = {0.2f, 0.2f, 0.2f, 1.0f};
    glMaterialfv(GL_FRONT, GL_SPECULAR, spec);
    glMateriali(GL_FRONT, GL_SHININESS, 16);

    glClearColor(0.0f, 0.0f, 0.0f, 1.0f); // background hitam
}

void drawUnitCube() {
    // Draw a unit cube centered at origin with normals
    glutSolidCube(1.0f);
}

// Draw a rectangular prism (width, height, depth) centered on origin
void drawPrism(float width, float height, float depth) {
    glPushMatrix();
    glScalef(width, height, depth);
    drawUnitCube();
    glPopMatrix();
}

// Draw one window as slightly inset rectangle on face
void drawWindow(float w, float h, float depthOffset) {
    // A thin box as window
    glPushMatrix();
    glTranslatef(0.0f, 0.0f, depthOffset);
    glScalef(w, h, 0.02f);
    glutSolidCube(1.0f);
    glPopMatrix();
}

void drawBuilding() {
    // Building main block
    glColor3fv(wallColor);
    drawPrism(2.0f, 3.0f, 1.2f); // width, height, depth

    // Roof (slight overhang)
    glPushMatrix();
    glTranslatef(0.0f, 1.6f, 0.0f);
    glColor3f(0.9f, 0.9f, 0.95f);
    glScalef(2.2f, 0.15f, 1.4f);
    glutSolidCube(1.0f);
    glPopMatrix();

    // Door - bottom center
    glPushMatrix();
    glTranslatef(0.0f, -0.9f, 0.62f + 0.01f); // slightly in front
    glColor3fv(doorColor);
    glScalef(0.5f, 0.8f, 0.02f);
    glutSolidCube(1.0f);
    glPopMatrix();

    // Windows: grid 3x2 on front face
    int rows = 4;
    int cols = 2;
    float startY = 0.9f;
    float startX = -0.6f;
    float stepY = 0.6f;
    float stepX = 0.8f;
    glColor3fv(windowColor);

    for (int r = 0; r < rows; ++r) {
        for (int c = 0; c < cols; ++c) {
            glPushMatrix();
            float x = startX + c * stepX;
            float y = startY - r * stepY;
            // Place window slightly inset on front face (z = depth/2 + small)
            glTranslatef(x, y, 0.62f + 0.011f);
            // Windows are thin boxes
            glScalef(0.45f, 0.35f, 0.02f);
            glutSolidCube(1.0f);
            glPopMatrix();
        }
    }

    // Also put windows on one side for 3D feel (right side)
    for (int r = 0; r < rows; ++r) {
        for (int c = 0; c < cols; ++c) {
            glPushMatrix();
            float x = 0.62f + 0.011f;
            float y = startY - r * stepY;
            float z = -0.4f + c * 0.8f;
            glTranslatef(x, y, z);
            glRotatef(90.0f, 0, 1, 0); // orient to face outward
            glScalef(0.45f, 0.35f, 0.02f);
            glColor3fv(windowColor);
            glutSolidCube(1.0f);
            glPopMatrix();
        }
    }
}

void display() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    // Position the camera to view the scene
    // We'll use a simple static camera (gluLookAt)
    gluLookAt(4.0, 3.0, 6.0,    // eye pos
              0.0, 0.0, 0.0,    // center
              0.0, 1.0, 0.0);   // up

    // Apply global transforms for object
    glPushMatrix();
    glScalef(scaleFactor, scaleFactor, scaleFactor);

    // Rotate object
    glRotatef(angleX, 1.0f, 0.0f, 0.0f);
    glRotatef(angleY, 0.0f, 1.0f, 0.0f);

    // Draw axis helpers (optional subtle)
    // X axis red, Y axis green, Z axis blue (very dim)
    glDisable(GL_LIGHTING);
    glLineWidth(2.0f);
    glBegin(GL_LINES);
      glColor3f(0.3f, 0.0f, 0.0f); glVertex3f(0,0,0); glVertex3f(1.0f,0,0);
      glColor3f(0.0f,0.3f,0.0f); glVertex3f(0,0,0); glVertex3f(0,1.0f,0);
      glColor3f(0.0f,0.0f,0.3f); glVertex3f(0,0,0); glVertex3f(0,0,1.0f);
    glEnd();
    glEnable(GL_LIGHTING);

    // Draw building
    drawBuilding();

    glPopMatrix();

    glutSwapBuffers();
}

void handleSpecialKeypress(int key, int x, int y) {
    switch (key) {
        case GLUT_KEY_RIGHT:
            angleY += ROT_STEP;
            if (angleY >= 360.0f) angleY -= 360.0f;
            break;
        case GLUT_KEY_LEFT:
            angleY -= ROT_STEP;
            if (angleY < 0.0f) angleY += 360.0f;
            break;
        case GLUT_KEY_UP:
            angleX -= ROT_STEP;
            if (angleX < -360.0f) angleX += 360.0f;
            break;
        case GLUT_KEY_DOWN:
            angleX += ROT_STEP;
            if (angleX > 360.0f) angleX -= 360.0f;
            break;
    }
    glutPostRedisplay();
}

void handleKeyboard(unsigned char key, int x, int y) {
    switch (key) {
        case 27: // ESC
            exit(0);

            break;
        case '+':
        case '=': // in case user pressed without shift
            scaleFactor += SCALE_STEP;
            if (scaleFactor > SCALE_MAX) scaleFactor = SCALE_MAX;
            break;
        case '-':
        case '_':
            scaleFactor -= SCALE_STEP;
            if (scaleFactor < SCALE_MIN) scaleFactor = SCALE_MIN;
            break;
        case 'r':
        case 'R':
            // reset view
            angleX = 20.0f;
            angleY = -25.0f;
            scaleFactor = 1.0f;
            break;
    }
    glutPostRedisplay();
}

void reshape(int w, int h) {
    if (h == 0) h = 1;
    float aspect = (float)w / (float)h;
    glViewport(0, 0, w, h);

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    // Perspective projection
    gluPerspective(45.0, aspect, 0.1, 100.0);
    glMatrixMode(GL_MODELVIEW);
}

int main(int argc, char** argv) {
    glutInit(&argc, argv);
    // Double buffering, RGBA, Depth buffer
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH);
    glutInitWindowSize(900, 600);
    glutInitWindowPosition(200, 100);
    glutCreateWindow("Gedung 3D - OpenGL FreeGLUT");

    initRendering();

    glutDisplayFunc(display);
    glutReshapeFunc(reshape);
    glutKeyboardFunc(handleKeyboard);
    glutSpecialFunc(handleSpecialKeypress);

    printf("Kontrol:\n");
    printf("  Panah kanan/kiri: rotasi Y\n");
    printf("  Panah atas/bawah: rotasi X\n");
    printf("  + / = : memperbesar\n");
    printf("  - / _ : memperkecil\n");
    printf("  r : reset posisi dan scale\n");
    printf("  ESC : keluar\n");

    glutMainLoop();
    return 0;
}
