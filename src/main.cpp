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
#include "header/taman.h"

// rumus variabel phi
#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

// -> konfigurasi 3D (lingkungan kota) <-
const float JARAK_OBJEK  = 8.0f;   // jarak antar objek (gedung, pohon, lampu)
const int   JUMLAH_AKTIF = 14;     // jumlah objek aktif di sekitar kamera

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
    float lebarAlas = 17.0f;  // lebar ke samping
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
            glPushMatrix();
                glTranslatef( POHON_X, -0.6f, z);
                glRotatef(-90, 0, 1, 0);
                glScalef(0.8f, 0.8f, 0.8f);
                drawPohon();
            glPopMatrix();
        }
}

void drawAirDepan(float x, float z) {
    float panjang = 18.1f;      // PANJANG KE SAMPING (X)
    float lebar   = 13.0f;      // LEBAR KE DEPAN-BELAKANG (Z)
    float yAir    = -0.2f;    // AIR NAIK (lebih dekat jembatan)

    glColor3f(0.1f, 0.4f, 0.7f); // warna air

    glBegin(GL_QUADS);
        // kiri bawah
        glVertex3f(x - panjang / 2, yAir, z - lebar / 2);
        // kanan bawah
        glVertex3f(x + panjang / 2, yAir, z - lebar / 2);
        // kanan atas
        glVertex3f(x + panjang / 2, yAir, z + lebar / 2);
        // kiri atas
        glVertex3f(x - panjang / 2, yAir, z + lebar / 2);
    glEnd();
}

void drawPembatasJembatan(float panjang) {
    float tinggi = 0.40f;
    float tebal  = 0.12f;
    float offset = 1.5f;

    glColor3f(0.45f, 0.45f, 0.45f); 

    // kiri
    glPushMatrix();
        glTranslatef(-offset, tinggi / 2, 0);
        glScalef(tebal, tinggi, panjang * 0.95f);
        glutSolidCube(1);
    glPopMatrix();

    // kanan
    glPushMatrix();
        glTranslatef( offset, tinggi / 2, 0);
        glScalef(tebal, tinggi, panjang * 0.95f);
        glutSolidCube(1);
    glPopMatrix();
}

void drawJembatanDepan(float x, float y, float z) {
    float panjang = JARAK_OBJEK * 1.0f;
    float lebar   = 3.0f;
    float tinggi  = 0.22f;

    glPushMatrix();
        glTranslatef(x, y, z);   // Y dari luar (bisa dinaikkan)

        // badan jembatan
        glColor3f(0.55f, 0.38f, 0.20f);   // coklat kayu alami
        glPushMatrix();
            glScalef(lebar, tinggi, panjang);
            glutSolidCube(1);
        glPopMatrix();

        // lapisan atas
        glColor3f(0.35f, 0.23f, 0.12f);   // coklat tua (papan kayu)
        glPushMatrix();
            glTranslatef(0, tinggi / 2 + 0.02f, 0);
            glScalef(lebar * 0.95f, 0.05f, panjang * 0.95f);
            glutSolidCube(1);
        glPopMatrix();

        drawPembatasJembatan(panjang);
    glPopMatrix();
}

void drawPohonUjungJembatan(float xJembatan, float zJembatan) {
    const float offsetXPohon = 2.3f;   // kiri - kanan dari jembatan
    const float offsetZPohon = 4.2f;   // depan & belakang jembatan
    const float yPohon = -0.75f;

    // ===== DEPAN JEMBATAN (Z +) =====
    // kiri
    glPushMatrix();
        glTranslatef(xJembatan - offsetXPohon, yPohon, zJembatan + offsetZPohon);
        glScalef(0.9f, 0.9f, 0.9f);
        drawPohon();
    glPopMatrix();

    // kanan
    glPushMatrix();
        glTranslatef(xJembatan + offsetXPohon, yPohon, zJembatan + offsetZPohon);
        glScalef(0.9f, 0.9f, 0.9f);
        drawPohon();
    glPopMatrix();

    // ===== BELAKANG JEMBATAN (Z -) =====
    // kiri
    glPushMatrix();
        glTranslatef(xJembatan - offsetXPohon, yPohon, zJembatan - offsetZPohon);
        glScalef(0.9f, 0.9f, 0.9f);
        drawPohon();
    glPopMatrix();

    // kanan
    glPushMatrix();
        glTranslatef(xJembatan + offsetXPohon, yPohon, zJembatan - offsetZPohon);
        glScalef(0.9f, 0.9f, 0.9f);
        drawPohon();
    glPopMatrix();
}

void drawPembatasAir() {
    float tinggi = 2.1f;
    float tebal  = 0.25f;

    glColor3f(0.42f, 0.42f, 0.42f); // beton alam

    glPushMatrix();
        glScalef(tebal, tinggi, 6.3f);
        glutSolidCube(1);
    glPopMatrix();
}

void drawJembatanTamanInfinite() {
    const float jarakBelakangGedung = 10.0f;
    const float tamanKananX =  GEDUNG_X + jarakBelakangGedung;
    const float tamanKiriX  = -GEDUNG_X - jarakBelakangGedung;

    const int STEP_TAMAN = 3;

    float tinggiJembatan = -0.24f;

    // OFFSET Z TERPISAH
    float offsetZJembatanKiri  = -6.1f;  // maju
    float offsetZJembatanKanan =  6.1f;  // mundur

    // OFFSET X → MENJAUH DARI GEDUNG
    float offsetXJembatan = 10.0f;

    int baseIndex = (int)floor(mobilPosZ / JARAK_OBJEK);

    for (int i = baseIndex - JUMLAH_AKTIF;
         i <= baseIndex + JUMLAH_AKTIF;
         i++) {

        if (i % STEP_TAMAN != 0) continue;

        float zTaman1 = i * JARAK_OBJEK;
        float zTaman2 = (i + STEP_TAMAN) * JARAK_OBJEK;
        float zJembatan = (zTaman1 + zTaman2) * 0.5f;

        // ===== KANAN =====
        float jembatanKananX = tamanKananX + offsetXJembatan;
        float zKanan = zJembatan + offsetZJembatanKanan;

        // ===== PEMBATAS AIR TAMAN KANAN =====
        glPushMatrix();
            glTranslatef(
                jembatanKananX - 7.9f,  // ujung kiri air ->
                -0.65f,
                zJembatan + offsetZJembatanKanan
            );
            drawPembatasAir();
        glPopMatrix();

        drawAirDepan(jembatanKananX, zKanan);
        drawJembatanDepan(jembatanKananX, tinggiJembatan, zKanan);
        drawPohonUjungJembatan(jembatanKananX, zKanan);   // 🌳

        // ===== KIRI =====
        float jembatanKiriX = tamanKiriX - offsetXJembatan;
        float zKiri = zJembatan + offsetZJembatanKiri;

        // ===== PEMBATAS AIR TAMAN KIRI =====
        glPushMatrix();
            glTranslatef(
                jembatanKiriX + 7.9f,   // ujung kanan air <-
                -0.65f,
                zJembatan + offsetZJembatanKiri
            );
            drawPembatasAir();
        glPopMatrix();

        drawAirDepan(jembatanKiriX, zKiri);
        drawJembatanDepan(jembatanKiriX, tinggiJembatan, zKiri);
        drawPohonUjungJembatan(jembatanKiriX, zKiri);     // 🌳
    }
}

void drawTamanInfinite() {
    const float jarakBelakangGedung = 20.0f;
    const float tamanKananX =  GEDUNG_X + jarakBelakangGedung;
    const float tamanKiriX  = -GEDUNG_X - jarakBelakangGedung;

    const int STEP_TAMAN = 3;

    // TAMAN KIRI
    int baseIndexKiri = (int)floor(mobilPosZ / JARAK_OBJEK);

    for (int i = baseIndexKiri - JUMLAH_AKTIF;
         i <= baseIndexKiri + JUMLAH_AKTIF;
         i++) {

        if (i % STEP_TAMAN != 0) continue;

        float z = i * JARAK_OBJEK;

        glPushMatrix();
            glTranslatef(tamanKiriX, -0.1f, z);
            glRotatef(180, 0, 1, 0);
            glScalef(0.9f, 0.9f, 0.9f);
            drawTaman();
        glPopMatrix();
    }

    // TAMAN KANAN
    int baseIndexKanan = (int)floor((mobilPosZ + JARAK_OBJEK * 0.5f) / JARAK_OBJEK);
    // OFFSET SETENGAH GRID → BIKIN STABIL

    for (int i = baseIndexKanan - JUMLAH_AKTIF;
         i <= baseIndexKanan + JUMLAH_AKTIF;
         i++) {

        if (i % STEP_TAMAN != 0) continue;

        float z = i * JARAK_OBJEK;

        glPushMatrix();
            glTranslatef(tamanKananX, -0.1f, z);
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

void display() {
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