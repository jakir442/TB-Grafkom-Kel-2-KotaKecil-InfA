#include <GL/freeglut.h>
#include <cmath>

#include "header/jakir-jalan.h"
#include "header/putri-mobil.h"


#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

// variabel global
float camX = 0, camY = 3, camZ = 15;
float camYaw = 0, camPitch = 0;

float speedMove = 0.15f;
float speedTurn = 1.0f;
bool keys[256] = { false };

float kecepatanGerak = 0.03f;
float kecepatanBelok  = 1.0f;

float jalanOffset = 0.0f;

void drawConeBelang(float x, float z) {
    glPushMatrix();
        glTranslatef(x, 0.01f, z);
        glRotatef(-90, 1, 0, 0); // tegakkan cone

        // BAGIAN BAWAH (ORANGE)
        glTranslatef(0.0f, 0.6f, 0.0f);
        glColor3f(1.0f, 0.5f, 0.0f);
        glutSolidCone(0.25f, 0.5f, 24, 24);

        // bagian tengah STRIP PUTIH
        glTranslatef(0.0f, 0.0f, 0.18f);
        glColor3f(1.0f, 1.0f, 1.0f);
        glutSolidCone(0.20f, 0.25f, 24, 24);

        // BAGIAN ATAS (ORANGE)
        glTranslatef(0.0f, 0.0f, 0.15f);
        glColor3f(1.0f, 0.5f, 0.0f);
        glutSolidCone(0.15f, 0.25f, 24, 24);

    glPopMatrix();
}

void drawQuad(
    float x1,float y1,float z1,
    float x2,float y2,float z2,
    float x3,float y3,float z3,
    float x4,float y4,float z4
) {
    glBegin(GL_QUADS);
        glVertex3f(x1,y1,z1);
        glVertex3f(x2,y2,z2);
        glVertex3f(x3,y3,z3);
        glVertex3f(x4,y4,z4);
    glEnd();
}

// draw jalan infinite
void drawJalan() {
    float lebarJalan = 4.0f;
    float setengahJalan = lebarJalan / 2.0f;
    float ketebalanJalan = 0.3f;
    float lebarTrotoar = 0.4f;
    float jarakTrotoar = 0.0001f; 

    float jarakPandang = 200.0f;

    float jarakConeZ = 20.0f;

    // kamera ngambil dari objek mobil
    float baseZ = std::floor(mobilPosZ  / 10.0f) * 10.0f - jarakPandang;
    // langsung kamera yang gerak
    // float baseZ = std::floor(camZ / 10.0f) * 10.0f - jarakPandang; 
    // std floor digunakan untuk membulatkan nilai kebawah misal 3.9 -> 3.0
    // jika mau build file mobil saja, aktif kan yg camZ
    // Untuk pergerakan kamera agar jalannya tanpa batas saat di running sendiri

    for(float z = baseZ; z < baseZ + jarakPandang*2; z += 10.0f) {
        float z2 = z + 10;

        // badan jalan
        glColor3f(0.15f,0.15f,0.15f);
        drawQuad(
            -setengahJalan, 0, z, setengahJalan, 0, z,
            setengahJalan, 0, z2, -setengahJalan,0, z2
        );

        // sisi jalan (untuk ketebalan trotoar)
        glColor3f(0.1f,0.1f,0.1f);
        drawQuad(
            -setengahJalan, -ketebalanJalan, z, -setengahJalan, 0, z,
            -setengahJalan, 0, z2, -setengahJalan, -ketebalanJalan, z2
        );
        drawQuad( 
            setengahJalan, -ketebalanJalan, z, setengahJalan, 0, z,
            setengahJalan, 0, z2, setengahJalan, -ketebalanJalan, z2
        );

        // trotoar
        glColor3f(0.32f,0.32f,0.32f);
        drawQuad(
            -(setengahJalan + lebarTrotoar), 0, z, -setengahJalan, 0, z,
            -setengahJalan, 0, z2, -(setengahJalan + lebarTrotoar), 0, z2
        );
        drawQuad( 
            setengahJalan, 0, z, (setengahJalan+lebarTrotoar), 0, z,
            (setengahJalan + lebarTrotoar), 0, z2, setengahJalan, 0, z2
        );

        // cone -> tx = translasi X
        float tx = setengahJalan + lebarTrotoar * 0.5f;
        drawConeBelang( tx, z+3);
        drawConeBelang(-tx, z+6);

        // garis putus
        glColor3f(1,1,0);
        glLineWidth(4);
        glBegin(GL_LINES);
            glVertex3f(0,0.02f,z+3);
            glVertex3f(0,0.02f,z+6);
        glEnd();
    }
}

#ifdef STANDALONE
    // Camera Direction(arah)
    void getCameraDir(float &dx, float &dy, float &dz) { // & untuk pointer ke variabel dx untuk merubah nilainya
        float yaw   = camYaw   * M_PI / 180.0f; // yaw(kiri-kanan)
        float pitch = camPitch * M_PI / 180.0f; // pitch(atas-bawah)

        dx = sin(yaw) * cos(pitch);
        dy = sin(pitch);
        dz = -cos(yaw) * cos(pitch);
    }

    // update kamera
    void updateCamera() {
        float dx, dy, dz; // d disini adalah untuk delta(deltaX, deltaY, deltaZ)
        getCameraDir(dx, dy, dz); // delta digunakan untuk selisih perubahan

        
        float rightX = sin((camYaw + 90) * M_PI / 180.0f);
        float rightZ = -cos((camYaw + 90) * M_PI / 180.0f);

        if (keys['w']) { camX += dx*speedMove; camY += dy*speedMove; camZ += dz*speedMove; }
        if (keys['s']) { camX -= dx*speedMove; camY -= dy*speedMove; camZ -= dz*speedMove; }
        if (keys['a']) { camX -= rightX*speedMove; camZ -= rightZ*speedMove; }
        if (keys['d']) { camX += rightX*speedMove; camZ += rightZ*speedMove; }

        if (keys[' ']) camY += speedMove;
        if (keys['c']) camY -= speedMove;
    }

    // keyboard
    void keyboard(unsigned char key, int x, int y) { keys[key] = true; }
    void keyboardUp(unsigned char key, int x, int y) { keys[key] = false; }

    // rotasi kamera
    void specialKeyboardRotasi(int key, int x, int y){
        if (key == GLUT_KEY_LEFT)  camYaw   -= speedTurn;
        if (key == GLUT_KEY_RIGHT) camYaw   += speedTurn;

        if (key == GLUT_KEY_UP)    camPitch += speedTurn;
        if (key == GLUT_KEY_DOWN)  camPitch -= speedTurn;

        // agar kamera tidak terbalik 
        if (camPitch > 89)  camPitch = 89;
        if (camPitch < -89) camPitch = -89;
    }

    void reshape(int w, int h) {
            glViewport(0, 0, w, h);
            glMatrixMode(GL_PROJECTION);
            glLoadIdentity();
            gluPerspective(60.0, (float)w / h, 0.1f, 100.0f);
            glMatrixMode(GL_MODELVIEW);
        }

    // display
    void display(){
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        updateCamera();

        glMatrixMode(GL_MODELVIEW);
        glLoadIdentity();

        float dx, dy, dz;
        getCameraDir(dx, dy, dz);

        gluLookAt(
            camX, camY, camZ,
            camX + dx, camY + dy, camZ + dz,
            0, 1, 0
        );

        drawJalan();
        glutSwapBuffers();
    }

    // main
    int main(int argc,char** argv){
        glutInit(&argc,argv);
        glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH);
        glutInitWindowSize(1000,700);
        glutCreateWindow("Jakir - Jalan Infinite");
        glEnable(GL_DEPTH_TEST);

        glutKeyboardFunc(keyboard);
        glutKeyboardUpFunc(keyboardUp);
        glutSpecialFunc(specialKeyboardRotasi);
        glutReshapeFunc(reshape);
        glutDisplayFunc(display);
        glutIdleFunc(display);
        
        glutMainLoop();
        return 0;
    }
#endif
