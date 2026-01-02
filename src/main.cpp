#include <GL/freeglut.h> // fungsi utama window OpenGL
#include <cmath> // fungsi matematika untuk floor
#include <ctime>   // untuk time()
#include <cstdlib> // untuk rand(), srand()

#include "header/jakir-jalan.h"
#include "header/putri-mobil.h"
#include "header/azhari-gedung.h"
#include "header/erika-pohon.h"
#include "header/jakir-lampu.h"
#include "header/jakir-bulan.h"
#include "header/jakir-bintang.h"

// rumus variabel phi
#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

// konfigurasi dunia 3D
const float JARAK_TILE      = 8.0f;   // satu segmen dunia
const int   JUMLAH_TILE     = 14;     // segmen aktif

const float LAMPU_X         = 3.0f;
const float POHON_X         = 4.5f;
const float GEDUNG_X        = 7.0f;

float driverViewYaw = 180.0f; 

bool keyState[256] = { false };

// mode kamera
enum ModeCamera {
    CAM_BELAKANG = 1,
    CAM_DEPAN    = 2,
    CAM_KANAN    = 3,
    CAM_KIRI     = 4,
    CAM_ATAS     = 5,
    CAM_DRIVER   = 6
};

ModeCamera kameraAktif = CAM_BELAKANG;
ModeCamera prevModeCamera = CAM_BELAKANG;

bool randomActiveCam = false;
bool qButton = false;

float cameraSmooth = 0.08f;

int waktuTerakhirGantiCam = 0;
const int DURASI_kAMERA = 2000; // 2 detik per titik kamera

// variabel transisi kamera
float smoothCamX = 0.0f;
float smoothCamY = 0.0f;
float smoothCamZ = 0.0f;

float smoothTargetX = 0.0f;
float smoothTargetY = 0.0f;
float smoothTargetZ = 0.0f;

int camIndex = 0;


// static int lastMouseX = 0;
// static int lastMouseY = 0;
// static bool firstMouse = true;
// static float mouseSensitivity = 0.15f;


// void handleKeyboard(unsigned char key, int x, int y) {
//     controlMobil(key);
//     handleKeyboardGedung(key);
//     glutPostRedisplay();
// }

void keyDown(unsigned char key, int x, int y) {
    if (key == 'q' || key == 'Q') {
        if (!qButton) {
            qButton = true;

            randomActiveCam = !randomActiveCam;

            if (randomActiveCam) {
                prevModeCamera = kameraAktif;
                waktuTerakhirGantiCam = glutGet(GLUT_ELAPSED_TIME); // menghitung waktu dengan satuan MS
            } else {
                kameraAktif = prevModeCamera;
            }
        }
    }

    keyState[key] = true;

    keyboardLampu(key);

    keyboardPohon(key);

    // perpindahan kamera
    if (!randomActiveCam) {
        if (key == '1') kameraAktif = CAM_BELAKANG;
        if (key == '2') kameraAktif = CAM_DEPAN;
        if (key == '3') kameraAktif = CAM_KANAN;
        if (key == '4') kameraAktif = CAM_KIRI;
        if (key == '5') kameraAktif = CAM_ATAS;
        if (key == '6') kameraAktif = CAM_DRIVER;
    }

    if (key == 27) exit(0); // ESC
}

void keyUp(unsigned char key, int x, int y) {
    keyState[key] = false;

    if (key == 'q' || key == 'Q') {
        qButton = false;
    }
}

void update() {
    // mobil (translasi)
    if (keyState['w'] || keyState['W']) controlMobil('w');
    if (keyState['s'] || keyState['S']) controlMobil('s');
    if (keyState['a'] || keyState['A']) controlMobil('a');
    if (keyState['d'] || keyState['D']) controlMobil('d');

    // gedung (scaling)
    if (keyState['+'] || keyState['=']) handleKeyboardGedung('+');
    if (keyState['-'] || keyState['_']) handleKeyboardGedung('-');

    // auto random kamera (per 2 detik)
    if (randomActiveCam) {
        int sekarang = glutGet(GLUT_ELAPSED_TIME); // waktu berjalan sejak program dimulai

        if (sekarang - waktuTerakhirGantiCam >= DURASI_kAMERA) {
            ModeCamera camList[] = {
                CAM_BELAKANG, 
                CAM_KANAN,
                CAM_ATAS,
                CAM_KIRI,
                CAM_DEPAN,
                CAM_DRIVER
            };

            const int JUMLAH_CAM = 6;

            kameraAktif = camList[camIndex];
            camIndex = (camIndex + 1) % JUMLAH_CAM;

            waktuTerakhirGantiCam = sekarang;
        }
    }
    updateBintang(mobilPosX, mobilPosZ);

    glutPostRedisplay();
}

void drawAlasBidang(float width, float depth) {
    float w = width / 2.0f;
    float d = depth / 2.0f;
    float y = -0.02; // sedikit di bawah jalan (nempel)

    glColor3f(0.75f, 0.75f, 0.75f); // abu-abu muda

    glBegin(GL_QUADS);
        glVertex3f(-w, y, -d);
        glVertex3f( w, y, -d);
        glVertex3f( w, y,  d);
        glVertex3f(-w, y,  d);
    glEnd();
}

void drawAlasKotaGlobal() {
    float panjangZ = 3000.0f; // super panjang
    float lebarAlas = 10.0f;  // lebar ke samping
    float overlap = 0.9f;    // MASUK ke jalan (anti-celah)

    // alas kiri
    glPushMatrix();
        glTranslatef(-(3.0f - overlap) - lebarAlas / 2.0f, 0.0f, mobilPosZ);
        drawAlasBidang(lebarAlas, panjangZ);
    glPopMatrix();

    // alas kanan
    glPushMatrix();
        glTranslatef( (3.0f - overlap) + lebarAlas / 2.0f, 0.0f, mobilPosZ);
        drawAlasBidang(lebarAlas, panjangZ);
    glPopMatrix();
}

void drawGedungInfinite() {
    int baseIndex = (int)floor(mobilPosZ / JARAK_TILE);

    float kiriX  = -GEDUNG_X;
    float kananX =  GEDUNG_X;

    //  TUKAR SAAT KAMERA DEPAN
    if (kameraAktif == CAM_DEPAN) {
        std::swap(kiriX, kananX);
    }

    for (int i = baseIndex - JUMLAH_TILE / 2;
         i <= baseIndex + JUMLAH_TILE / 2; i++) {

        float z = i * JARAK_TILE;

        // gedung kiri (tampilan depan jika kamera belakang)
        glPushMatrix();
            glTranslatef(kiriX, 2.16f, z);
            glScalef(1.5f * scaleFactor,
                     1.5f * scaleFactor,
                     1.5f * scaleFactor);
            drawGedung();
        glPopMatrix();

        // gedung kanan (tampilan belakang jika kamera belakang)
        glPushMatrix();
            glTranslatef(kananX, 2.16f, z);
            glRotatef(180, 0, 1, 0);
            glScalef(1.5f * scaleFactor,
                     1.5f * scaleFactor,
                     1.5f * scaleFactor);
            drawGedung();
        glPopMatrix();
    }
}

void drawPohonInfinite() {
    int baseIndex = (int)floor(mobilPosZ / JARAK_TILE);

    for (int i = baseIndex - JUMLAH_TILE / 2;
             i <= baseIndex + JUMLAH_TILE / 2; i++) {

        float z = i * JARAK_TILE;

        // pohon kiri
        glPushMatrix();
            glTranslatef(-POHON_X, -0.6f, z);
            glRotatef(90, 0, 1, 0);
            glScalef(0.8f, 0.8f, 0.8f);
            drawPohon();
        glPopMatrix();

        // pohon kanan
        glPushMatrix();
            glTranslatef( POHON_X, -0.6f, z);
            glRotatef(-90, 0, 1, 0);
            glScalef(0.8f, 0.8f, 0.8f);
            drawPohon();
        glPopMatrix();
    }
}

void drawLampuInfinite() {
    int baseIndex = (int)floor(mobilPosZ / JARAK_TILE);

    for (int i = baseIndex - JUMLAH_TILE / 2;
            i <= baseIndex + JUMLAH_TILE / 2; i++) {

        float z = i * JARAK_TILE;

        // lampu kiri (menghadap ke jalan)
        glPushMatrix();
            glTranslatef(-LAMPU_X, 0.0f, z);
            glRotatef( 90, 0, 1, 0);   // hadap ke +X (ke jalan)
            drawLampuJalan(0.0f, 0.0f, GL_LIGHT0);
        glPopMatrix();

        // lampu kanan (menghadap ke jalan)
        glPushMatrix();
            glTranslatef( LAMPU_X, 0.0f, z);
            glRotatef(-90, 0, 1, 0);   // hadap ke -X (ke jalan)
            drawLampuJalan(0.0f, 0.0f, GL_LIGHT1);
        glPopMatrix();
    }
}

void display() {
    // clear buffer
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // reset model view
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    // konfigurasi kamera
    float jarakKamera = 10.0f;

    float camX = mobilPosX;
    float camY = 3.0f;  // tinggi kamera tetap
    float camZ = mobilPosZ;

    // target kamera
    float targetX = mobilPosX;
    float targetY = 1.0f;
    float targetZ = mobilPosZ;

    // posisi kamera berdasarkan mode
    switch (kameraAktif) {
        case CAM_BELAKANG:
            camZ = mobilPosZ + jarakKamera;
            break;

        case CAM_DEPAN:
            camZ = mobilPosZ - jarakKamera;
            break;

        case CAM_KANAN:
            camX = mobilPosX + jarakKamera;
            targetZ += 5.0f;   // lihat ke arah depan mobil
            break;

        case CAM_KIRI:
            camX = mobilPosX - jarakKamera;
            targetZ += 5.0f;   // lihat ke arah depan mobil
            break;
        
        case CAM_ATAS:
            camY = mobilPosY + 8.0f;      // kamera naik ke atas
            camZ = mobilPosZ + jarakKamera; // tetap di belakang
            break;
        
        case CAM_DRIVER: {
            float rad = (mobilYaw + 180.0f) * M_PI / 180.0f;

            // arah depan mobil (sama dengan logika gerak)
            float dirX = -sin(rad);
            float dirZ = -cos(rad);

            // posisi kamera (di depan dashboard)
            camX = mobilPosX + dirX * 0.8f; // maju ke depan (kap mobil terlihat)
            camY = mobilPosY + 0.34f; // tinggi kamera (setara mata pengemudi)
            camZ = mobilPosZ + dirZ * 0.8f; // maju ke depan mengikuti arah mobil

            // arah pandang (ke depan jalan)
            targetX = camX + dirX * 10.0f; // arah pandang jauh ke depan
            targetY = camY - 0.10f;  // sedikit menunduk (kap mesin terlihat)
            targetZ = camZ + dirZ * 10.0f; // arah pandang lurus ke depan
            break;
        }
    }

    // transisi kamera
    if (kameraAktif == CAM_DRIVER) {
        // driver kamera: posisi langsung (NO LAG)
        smoothCamX = camX;
        smoothCamY = camY;
        smoothCamZ = camZ;

        // arah pandang smooth (biar halus lihat jalan)
        smoothTargetX += (targetX - smoothTargetX) * cameraSmooth;
        smoothTargetY += (targetY - smoothTargetY) * cameraSmooth;
        smoothTargetZ += (targetZ - smoothTargetZ) * cameraSmooth;
    } else {
        // kamera lain: full smooth
        smoothCamX += (camX - smoothCamX) * cameraSmooth;
        smoothCamY += (camY - smoothCamY) * cameraSmooth;
        smoothCamZ += (camZ - smoothCamZ) * cameraSmooth;

        smoothTargetX += (targetX - smoothTargetX) * cameraSmooth;
        smoothTargetY += (targetY - smoothTargetY) * cameraSmooth;
        smoothTargetZ += (targetZ - smoothTargetZ) * cameraSmooth;
    }

    // aktifkan kamera ini untuk tanpa transisi
    // Kamera tidak smooth
    // gluLookAt(
    //     camX, camY, camZ,          // posisi kamera
    //     targetX, targetY, targetZ, // arah pandang
    //     0.0f, 1.0f, 0.0f            // UP vector (TIDAK MIRING)
    // );

    // kamera smooth
    gluLookAt(
        smoothCamX, smoothCamY, smoothCamZ,
        smoothTargetX, smoothTargetY, smoothTargetZ,
        0.0f, 1.0f, 0.0f
    );


    drawJalan();           // dasar
    drawAlasKotaGlobal();
    drawPohonInfinite();   // transisi
    drawGedungInfinite();  // background  
    drawMobil();
    drawBulan(mobilPosX, mobilPosZ);

    drawBintang();
    updateBintangJatuh();
    drawBintangJatuh();

    drawLampuInfinite();   // dekat jalan

    glutSwapBuffers();
}

void initGL() {
    glEnable(GL_DEPTH_TEST);
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f); // background hitam

    initTextureBulan();

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(60.0, 1280.0 / 720.0, 0.1, 2000.0);
}

int main(int argc, char** argv) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
    glutInitWindowSize(1280, 720);
    glutCreateWindow("Kelompok 2 - Kota Kecil");

    initGL();

    initMobil();
    initLampuLighting();

    glutTimerFunc(DELAY_ANTAR_HUJAN, mulaiHujanBintang, 0);

    glutDisplayFunc(display);
    // glutKeyboardFunc(handleKeyboard);
    glutKeyboardFunc(keyDown);
    glutKeyboardUpFunc(keyUp);
    glutIdleFunc(update);

    srand(time(NULL));

    glutMainLoop();
    return 0;
}
