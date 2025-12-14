#include <GL/freeglut.h>
#include <cmath>

#include "header/jakir-jalan.h"

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

// ---------------------------------- variabel global
float cameraPosX = 0.0f, cameraPosY = 3.0f, cameraPosZ = 15.0f;
float cameraYaw = 0.0f, cameraPitch = 0.0f;
bool keys[256] = { false };

float moveSpeed = 0.03f;
float rotSpeed  = 1.0f;

float jalanOffset = 0.0f;

// ---------------------------------- draw jalan infinite
void drawJalan() {
    float roadWidth  = 4.0f;
    float roadHalf   = roadWidth / 2.0f;
    float thickness  = 0.3f;
    float curbWidth  = 0.3f;
    float curbOffset = 0.0001f;

    float visibleDist = 200.0f;
    float baseZ = std::floor(mobilPosZ  / 10.0f) * 10.0f - visibleDist;

    for (float z = baseZ; z < baseZ + visibleDist*2; z += 10.0f) {
        float z1 = z;
        float z2 = z1 + 10.0f;

        // badan jalan
        glColor3f(0.15f,0.15f,0.15f);
        glBegin(GL_QUADS);
            glVertex3f(-roadHalf,0,z1); glVertex3f( roadHalf,0,z1);
            glVertex3f( roadHalf,0,z2); glVertex3f(-roadHalf,0,z2);
        glEnd();

        // sisi jalan
        glColor3f(0.1f,0.1f,0.1f);
        glBegin(GL_QUADS);
            glVertex3f(-roadHalf,-thickness,z1); glVertex3f(-roadHalf,0,z1);
            glVertex3f(-roadHalf,0,z2); glVertex3f(-roadHalf,-thickness,z2);
            glVertex3f( roadHalf,-thickness,z1); glVertex3f( roadHalf,0,z1);
            glVertex3f( roadHalf,0,z2); glVertex3f( roadHalf,-thickness,z2);
        glEnd();

        // trotoar
        glColor3f(0.32f,0.32f,0.32f);
        glBegin(GL_QUADS);
            glVertex3f(-(roadHalf+curbWidth),0,z1); glVertex3f(-roadHalf+curbOffset,0,z1);
            glVertex3f(-roadHalf+curbOffset,0,z2); glVertex3f(-(roadHalf+curbWidth),0,z2);
            glVertex3f( roadHalf-curbOffset,0,z1); glVertex3f( roadHalf+curbWidth,0,z1);
            glVertex3f( roadHalf+curbWidth,0,z2); glVertex3f( roadHalf-curbOffset,0,z2);
        glEnd();

        // garis putus-putus
        glColor3f(1,1,0); glLineWidth(4);
        glBegin(GL_LINES); glVertex3f(0,0.02f,z1+3); glVertex3f(0,0.02f,z1+6); glEnd();
    }
}

#ifdef STANDALONE
// ---------------------------------- update kamera
void updateCamera() {
    float yawRad   = cameraYaw * M_PI / 180.0f;
    float pitchRad = cameraPitch * M_PI / 180.0f;

    // forward / right vector
    float forwardX = sinf(yawRad) * cosf(pitchRad);
    float forwardY = sinf(pitchRad);
    float forwardZ = -cosf(yawRad) * cosf(pitchRad);
    float rightX   = sinf(yawRad + M_PI/2.0f);
    float rightZ   = -cosf(yawRad + M_PI/2.0f);

    if (keys['w'] || keys['W']) { 
    cameraPosX += forwardX * moveSpeed; 
    cameraPosY += forwardY * moveSpeed; 
    cameraPosZ += forwardZ * moveSpeed; // <- tambah sesuai forwardZ
    jalanOffset += moveSpeed*3; 
    }
    if (keys['s'] || keys['S']) { 
        cameraPosX -= forwardX * moveSpeed; 
        cameraPosY -= forwardY * moveSpeed; 
        cameraPosZ -= forwardZ * moveSpeed; // <- kurang sesuai forwardZ
        jalanOffset -= moveSpeed*3; 
    }
    if (keys['a'] || keys['A']) { cameraPosX -= rightX * moveSpeed; cameraPosZ -= rightZ * moveSpeed; }
    if (keys['d'] || keys['D']) { cameraPosX += rightX * moveSpeed; cameraPosZ += rightZ * moveSpeed; }

    if (keys[' ']) cameraPosY += moveSpeed;
    if (keys['c'] || keys['C']) cameraPosY -= moveSpeed;
}

// ---------------------------------- keyboard
void keyboard(unsigned char key, int x, int y) { keys[key] = true; }
void keyboardUp(unsigned char key, int x, int y) { keys[key] = false; }


// ---------------------------------- rotasi kamera
void rotasiKamera(int key, int x, int y){
    switch(key){
        case GLUT_KEY_LEFT: cameraYaw -= rotSpeed; break;
        case GLUT_KEY_RIGHT: cameraYaw += rotSpeed; break;
        case GLUT_KEY_UP: cameraPitch -= rotSpeed; break;
        case GLUT_KEY_DOWN: cameraPitch += rotSpeed; break;
    }
    if(cameraPitch>89) cameraPitch=89;
    if(cameraPitch<-89) cameraPitch=-89;
    glutPostRedisplay();
}

// ---------------------------------- display
void display(){
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    updateCamera();

    glMatrixMode(GL_PROJECTION); glLoadIdentity();
    gluPerspective(60.0, 1000.0/700.0, 0.1, 2000.0);

    glMatrixMode(GL_MODELVIEW); glLoadIdentity();

    // --- forward vector sama seperti di updateCamera ---
    float yawRad   = cameraYaw * M_PI / 180.0f;
    float pitchRad = cameraPitch * M_PI / 180.0f;
    float forwardX = sinf(yawRad) * cosf(pitchRad);
    float forwardY = sinf(pitchRad);
    float forwardZ = -cosf(yawRad) * cosf(pitchRad);

    float lookX = cameraPosX + forwardX;
    float lookY = cameraPosY + forwardY;
    float lookZ = cameraPosZ + forwardZ;

    gluLookAt(cameraPosX,cameraPosY,cameraPosZ,lookX,lookY,lookZ,0,1,0);

    drawJalan();

    glutSwapBuffers();
}

// ---------------------------------- main
int main(int argc,char** argv){
    glutInit(&argc,argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH);
    glutInitWindowSize(1000,700);
    glutCreateWindow("Jalan Infinite + Kamera Bebas");
    glEnable(GL_DEPTH_TEST);

    glutDisplayFunc(display);
    glutKeyboardFunc(keyboard);
    glutKeyboardUpFunc(keyboardUp);
    glutSpecialFunc(rotasiKamera);
    glutIdleFunc(display);
    
    glutMainLoop();
    return 0;
}
#endif
