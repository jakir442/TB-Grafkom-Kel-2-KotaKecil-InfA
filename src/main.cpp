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
#include "header/taman/jakir_taman.h"
#include "header/taman/rizal_jungkat-jungkit.h"
#include "header/taman/assyifa_ayunan.h"

// rumus variabel phi
#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

// -> konfigurasi 3D (lingkungan kota) <-
const float JARAK_OBJEK  = 8.0f;   // jarak antar objek (gedung, pohon, lampu)
const int   JUMLAH_AKTIF = 10;     // jumlah objek aktif di sekitar kamera

// -> posisi objek di sumbu X <-
const float LAMPU_X  = 3.0f;       // posisi lampu jalan
const float POHON_X  = 4.5f;       // posisi pohon
const float GEDUNG_X = 7.0f;       // posisi gedung

// -> input keyboard <-
bool keyState[256] = { false };    // status tombol keyboard (pressed / released)

// -> sistem kamera <-
float driverViewYaw = 180.0f;      // arah pandang kamera driver

// -> mode kamera <-
enum ModeCamera {
    CAM_BELAKANG = 1,
    CAM_DEPAN,
    CAM_KANAN,
    CAM_KIRI,
    CAM_ATAS,
    CAM_DRIVER
};

ModeCamera kameraAktif     = CAM_BELAKANG; // kamera yang sedang aktif
ModeCamera prevModeCamera = CAM_BELAKANG; // kamera sebelum mode acak

bool randomActiveCam = false;      // status kamera acak
bool qButton         = false;      // debounce tombol Q

// -> transisi kamera (smooth movement) <-
float cameraSmooth = 0.08f;

float smoothCamX = 0.0f;
float smoothCamY = 0.0f;
float smoothCamZ = 0.0f;

float smoothTargetX = 0.0f;
float smoothTargetY = 0.0f;
float smoothTargetZ = 0.0f;

// -> timer & index kamera <-
int waktuTerakhirGantiCam = 0;
const int DURASI_kAMERA = 2000; // 2 detik per titik kamera
int camIndex = 0;

// -> sistem jumpsacre mobil <-
float posisiMobilJumpscareZ = 0.0f; // posisi mobil penabrak
bool  mobilJumpscareSiap   = false; // status mobil jumpscare

// -> mode scane <-
enum SceneMode {
    SCENE_KOTA_NORMAL,
    SCENE_JUMPSCARE
};
SceneMode sceneAktif = SCENE_KOTA_NORMAL;

// -> tahapan jumpscare <-
enum TahapJumpscare {
    JUMPSCARE_LIHAT_KANAN_KIRI,
    JUMPSCARE_DIAM,
    JUMPSCARE_MUNCUL
};
TahapJumpscare tahapAktifJumpscare = JUMPSCARE_LIHAT_KANAN_KIRI;

enum ProjectionMode {
    PROJ_PERSPECTIVE,
    PROJ_ORTHOGRAPHIC
};

ProjectionMode projectionMode = PROJ_PERSPECTIVE;

// -> timer jumpscare <-
int   waktuMulaiJumpscare = 0;
float sceneTimer          = 0.0f;
float offsetYawJumpscare  = 0.0f;  // offset rotasi kamera saat panik

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

    // toggle proyeksi
    if (key == 'o' || key == 'O') {
        projectionMode = PROJ_ORTHOGRAPHIC;
    }

    if (key == 'p' || key == 'P') {
        projectionMode = PROJ_PERSPECTIVE;
    }

    keyState[key] = true;

    keyboardJungkatJungkit(key);
    keyboardAyunan(key);

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

    if (key == 27 && sceneAktif == SCENE_KOTA_NORMAL) {
        sceneAktif = SCENE_JUMPSCARE;
        kameraAktif = CAM_DRIVER;

        waktuMulaiJumpscare = glutGet(GLUT_ELAPSED_TIME);
        tahapAktifJumpscare = JUMPSCARE_LIHAT_KANAN_KIRI;

        mobilJumpscareSiap = false; // reset mobil penabrak
        posisiMobilJumpscareZ = 0.0f;
    }
}

void keyUp(unsigned char key, int x, int y) {
    keyState[key] = false;

    if (key == 'q' || key == 'Q') {
        qButton = false;
    }
}

void update() {
    if (sceneAktif == SCENE_JUMPSCARE) {
        glutPostRedisplay();
        // return; // SEMUA GERAK BERHENTI
    }

    // mobil (translasi)
    if (keyState['w'] || keyState['W']) controlMobil('w');
    if (keyState['s'] || keyState['S']) controlMobil('s');
    if (keyState['a'] || keyState['A']) controlMobil('a');
    if (keyState['d'] || keyState['D']) controlMobil('d');

    // gedung (scaling)
    if (keyState['+'] || keyState['=']) handleKeyboardGedung('+');
    if (keyState['-'] || keyState['_']) handleKeyboardGedung('-');


    if (keyState['j'] || keyState['J']) keyboardJungkatJungkit('j');
    if (keyState['k'] || keyState['K']) keyboardAyunan('k');

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
    float lebarAlas = 9.0f;  // lebar ke samping
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
    int baseIndex = (int)floor(mobilPosZ / JARAK_OBJEK);

    float kiriX  = -GEDUNG_X;
    float kananX =  GEDUNG_X;

    //  TUKAR SAAT KAMERA DEPAN
    if (kameraAktif == CAM_DEPAN) {
        std::swap(kiriX, kananX);
    }

    for (int i = baseIndex - JUMLAH_AKTIF / 2;
        i <= baseIndex + JUMLAH_AKTIF / 2; i++) {

        float z = i * JARAK_OBJEK;

        // gedung kiri (tampilan depan jika kamera belakang)
        glPushMatrix();
            glTranslatef(kiriX, 2.16f, z);
            glScalef(1.5f * scaleFactor,
                     1.5f * scaleFactor,
                     1.5f * scaleFactor);
            drawGedung();
        glPopMatrix();
    }
}

void drawPohonInfinite() {
    int baseIndex = (int)floor(mobilPosZ / JARAK_OBJEK);

    for (int i = baseIndex - JUMLAH_AKTIF / 2;
            i <= baseIndex + JUMLAH_AKTIF / 2; i++) {

            float z = i * JARAK_OBJEK;

            // pohon kiri
            glPushMatrix();
                glTranslatef(-POHON_X, -0.6f, z);
                glRotatef(90, 0, 1, 0);
                glScalef(0.8f, 0.8f, 0.8f);
                drawPohon();
            glPopMatrix();

            // pohon kanan
            // glPushMatrix();
            //     glTranslatef( POHON_X, -0.6f, z);
            //     glRotatef(-90, 0, 1, 0);
            //     glScalef(0.8f, 0.8f, 0.8f);
            //     drawPohon();
            // glPopMatrix();
        }
}

// JEMBATAN TAMAN INFINITE
void drawJembatanTamanInfinite() {
    const int STEP = 3;
    int baseIndex = (int)floor(mobilPosZ / JARAK_OBJEK);

    for (int i = baseIndex - JUMLAH_AKTIF;
         i <= baseIndex + JUMLAH_AKTIF; i++) {

        if (i % STEP != 0) continue;

        float z = i * JARAK_OBJEK + 18.0f;
        drawJembatanTaman(19.0f, z);
    }
}

void drawTamanInfinite() {
    const float jarakBelakangGedung = 16.0f;
    const float tamanKananX =  GEDUNG_X + jarakBelakangGedung;
    const float tamanKiriX  = -GEDUNG_X - jarakBelakangGedung;

    const int STEP_TAMAN = 3;

    // TAMAN KIRI
    // int baseIndexKiri = (int)floor(mobilPosZ / JARAK_OBJEK);

    // for (int i = baseIndexKiri - JUMLAH_AKTIF;
    //      i <= baseIndexKiri + JUMLAH_AKTIF;
    //      i++) {

    //     if (i % STEP_TAMAN != 0) continue;

    //     float z = i * JARAK_OBJEK;

    //     glPushMatrix();
    //         glTranslatef(tamanKiriX, -0.1f, z);
    //         glRotatef(180, 0, 1, 0);
    //         glScalef(0.9f, 0.9f, 0.9f);
    //         drawTaman();
    //     glPopMatrix();
    // }

    // TAMAN KANAN
    int baseIndexKanan = (int)floor((mobilPosZ + JARAK_OBJEK * 0.5f) / JARAK_OBJEK);
    // OFFSET SETENGAH GRID → BIKIN STABIL

    for (int i = baseIndexKanan - JUMLAH_AKTIF;
         i <= baseIndexKanan + JUMLAH_AKTIF;
         i++) {

        if (i % STEP_TAMAN != 0) continue;

        float z = i * JARAK_OBJEK;

        glPushMatrix();
            glTranslatef(19.0f, -0.1f, z);
            glScalef(0.9f, 0.9f, 0.9f);
            drawTaman();
        glPopMatrix();
    }
}

void drawLampuInfinite() {
    int baseIndex = (int)floor(mobilPosZ / JARAK_OBJEK);

    for (int i = baseIndex - JUMLAH_AKTIF / 2;
            i <= baseIndex + JUMLAH_AKTIF / 2; i++) {

        float z = i * JARAK_OBJEK;

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

void drawJumpscareCar() {
    if (tahapAktifJumpscare != JUMPSCARE_MUNCUL) return;

    const float JARAK_DEPAN = 70.0f;
    const float KECEPATAN_NABRAK = 4.8f;

    static bool posisiAwalDitetapkan = false;
    static float posisiAwalZ; 
    static float targetMobilZ; // posisi pemain saat jumpscare mulai

    // Tentukan posisi awal jumpscare hanya sekali saat mulai muncul
    if (!posisiAwalDitetapkan && tahapAktifJumpscare == JUMPSCARE_MUNCUL) {
        targetMobilZ = mobilPosZ;          // simpan posisi pemain saat jumpscare mulai
        posisiAwalZ = targetMobilZ - JARAK_DEPAN; // mobil muncul 70 unit di depan
        posisiMobilJumpscareZ = posisiAwalZ;
        posisiAwalDitetapkan = true;
    }

    // Gerakkan mobil jumpscare maju ke mobil pemain
    if (tahapAktifJumpscare == JUMPSCARE_MUNCUL) {
        posisiMobilJumpscareZ += KECEPATAN_NABRAK;

        // cek tabrakan dengan posisi pemain saat jumpscare mulai
        if (posisiMobilJumpscareZ >= targetMobilZ - 1.2f) {
            exit(0);
        }
    }

    // gambar mobil
    glPushMatrix();
        glTranslatef(mobilPosX, mobilPosY, posisiMobilJumpscareZ);
        glRotatef(180, 0, 1, 0);
        drawMobil();
    glPopMatrix();

    // reset saat jumpscare berakhir
    if (tahapAktifJumpscare != JUMPSCARE_MUNCUL) {
        posisiAwalDitetapkan = false;
    }
}

void setProjection() {
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();

    if (projectionMode == PROJ_PERSPECTIVE) {
        gluPerspective(
            60.0,
            1280.0 / 720.0,
            0.1,
            2000.0
        );
    } else {
        // Orthographic view (tampak teknis / denah)
        glOrtho(
            -25.0, 25.0,   // kiri, kanan
            -15.0, 15.0,   // bawah, atas
            -2000.0, 2000.0
        );
    }

    glMatrixMode(GL_MODELVIEW);
}

void display() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // SET PROJECTION SESUAI MODE
    setProjection();

    // reset model view
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    if (sceneAktif == SCENE_JUMPSCARE) {
        int now = glutGet(GLUT_ELAPSED_TIME);
        sceneTimer = (now - waktuMulaiJumpscare) / 1000.0f;

        if (sceneTimer < 5.5f) {
            // lihat kiri kanan secara berulang
            tahapAktifJumpscare = JUMPSCARE_LIHAT_KANAN_KIRI;
            offsetYawJumpscare = sin(sceneTimer * 1.6f) * 35.0f;

            // 💡 lampu berkedip otomatis: level 1 ↔ 0
            if ((now / 250) % 2 == 0) {
                levelLampu = 1;
                lampuNyala = true;
            } else {
                levelLampu = 0;
                lampuNyala = false;
            }
        }
        else if (sceneTimer < 6.2f) {
            // ❄ FREEZE
            tahapAktifJumpscare = JUMPSCARE_DIAM;
            offsetYawJumpscare = 0.0f;

            // 💡 lampu nyala stabil
            levelLampu = 2; 
            lampuNyala = true;
        }
        else {
            // 👻 APPEAR
            tahapAktifJumpscare = JUMPSCARE_MUNCUL;
            levelLampu = 2; 
            lampuNyala = true;
        }
    }

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

    float effectiveYaw = mobilYaw;

    if (sceneAktif == SCENE_JUMPSCARE &&
        tahapAktifJumpscare == JUMPSCARE_LIHAT_KANAN_KIRI) {

        effectiveYaw += offsetYawJumpscare;
    }

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
            float rad = (effectiveYaw  + 180.0f) * M_PI / 180.0f;

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
    drawTamanInfinite();
    drawJembatanTamanInfinite();
    drawMobil();
    if (sceneAktif == SCENE_JUMPSCARE) {
        drawJumpscareCar();
    }
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