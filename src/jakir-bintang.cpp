#include <GL/freeglut.h>
#include <cmath>
#include <cstdlib>
#include "header/jakir-bintang.h"
#include "header/putri-mobil.h"

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

// konfigurasi langit
const int JUMLAH_BINTANG = 200;

// ukuran tile langit (diulang)
const float TILE_SIZE = 80.0f;

// tinggi langit
const float TINGGI_MIN = 10.0f;
const float TINGGI_MAX = 15.0f;

// kontrol hujan bintang
int jumlahSpawned = 0;
bool hujanAktif = false;

const int TOTAL_HUJAN_BINTANG = 30;
const int DELAY_ANTAR_BINTANG = 500;    // 0.5 detik
const int DELAY_ANTAR_HUJAN   = 20000;  // 20 detik

struct Bintang {
    float x, y, z;
};

Bintang bintang[JUMLAH_BINTANG];

// Posisi mobil (world)
float mobilXPos = 0.0f;
float mobilZPos = 0.0f;


float randRange(float min, float max);

// bintang jatuh
struct BintangJatuh {
    float x, y, z;
    float vx, vy, vz;      // kecepatan
    float px, py, pz;      // posisi sebelumnya (untuk trail)

    float rot;      // sudut rotasi
    float rotSpeed; // kecepatan rotasi

    bool aktif;
};

const int MAX_BINTANG_JATUH = 30; // jumlah maksimal bintang jatuh bersamaan
BintangJatuh bintangJatuh[MAX_BINTANG_JATUH] = {}; // inisialisasi mati semua

void mulaiHujanBintang(int value) {
    jumlahSpawned = 0;
    hujanAktif = true;

    // mulai spawn bintang pertama
    glutTimerFunc(0, spawnBintangJatuh, 0);
}

// spawn bintang jatuh acak, 1 bintang per spawn, posisi acak unik
void spawnBintangJatuh(int value) {
    // STOP jika sudah cukup
    if (!hujanAktif || jumlahSpawned >= TOTAL_HUJAN_BINTANG) {
        hujanAktif = false;

        // jadwalkan hujan berikutnya
        glutTimerFunc(DELAY_ANTAR_HUJAN, mulaiHujanBintang, 0);
        return;
    }

   // cari index bintang mati
    for (int i = 0; i < MAX_BINTANG_JATUH; i++) {
        if (!bintangJatuh[i].aktif) {
            bintangJatuh[i].rot = randRange(0.0f, 360.0f);
            bintangJatuh[i].rotSpeed = randRange(2.0f, 6.0f); // derajat per frame

            // spawn acak di langit, depan mobil (arah -Z)
            bintangJatuh[i].x = randRange(-TILE_SIZE/2, TILE_SIZE/2);
            bintangJatuh[i].y = randRange(20.0f, 22.0f);
            bintangJatuh[i].z = mobilPosZ + randRange(150.0f, 151.0f); // di depan scene (-Z)


            bintangJatuh[i].px = bintangJatuh[i].x;
            bintangJatuh[i].py = bintangJatuh[i].y;
            bintangJatuh[i].pz = bintangJatuh[i].z;

            // kecepatan jatuh: turun perlahan + maju ke depan (-Z)
            bintangJatuh[i].vx = 0.0f;

            // vy -> turun perlahan (semakin besar nilai negatifnya turun nya akan semakin tajam)
            // semakin kecil nilai vy->(-0.05f) turun nya akan perlahan, semakin besar nilainnya turun nya akan semakin cepat vy->(-1.0f)
            bintangJatuh[i].vy = -0.05f; 

            // vz -> maju ke depan (-Z), jika (+Z) ini akan muncul dari depan dan bergerak ke belakang
            // semakin kecil nilai vz->(-0.5f) semakin pelan dia bergerak ke depan, dan sebaliknya semakin besar nilainya semakin cepat geraknya
            bintangJatuh[i].vz = -3.0f;  


            bintangJatuh[i].aktif = true;
            jumlahSpawned++; 
            break; // spawn satu saja
        }
    }

    // spawn acak tiap 0.8–2 detik
    glutTimerFunc(DELAY_ANTAR_BINTANG, spawnBintangJatuh, 0);
}

// UPDATE BINTANG JATUH
void updateBintangJatuh() {
    for (int i = 0; i < MAX_BINTANG_JATUH; i++) {
        if (!bintangJatuh[i].aktif) continue;

        float trailLength = 4.0f; // ekor bintang lebih panjang
        bintangJatuh[i].px = bintangJatuh[i].x - bintangJatuh[i].vx * trailLength;
        bintangJatuh[i].py = bintangJatuh[i].y - bintangJatuh[i].vy * trailLength;
        bintangJatuh[i].pz = bintangJatuh[i].z - bintangJatuh[i].vz * trailLength;

        // update posisi
        bintangJatuh[i].x += bintangJatuh[i].vx;
        bintangJatuh[i].y += bintangJatuh[i].vy;
        bintangJatuh[i].z += bintangJatuh[i].vz;

        // update rotasi
        bintangJatuh[i].rot += bintangJatuh[i].rotSpeed;

        // matikan jika jatuh terlalu rendah atau terlalu jauh
        if (
            bintangJatuh[i].y < 0.0f ||
            bintangJatuh[i].z < mobilZPos - 290.0f
        ) {
            bintangJatuh[i].aktif = false;
        }
    }
}

float randRange(float min, float max) {
    return min + (rand() / (float)RAND_MAX) * (max - min);
}

void drawStar(float x, float y, float z, float r) {
    glBegin(GL_TRIANGLE_FAN);
    glVertex3f(x, y, z);

    for (int i = 0; i <= 10; i++) {
        float a = i * M_PI / 5;
        float rr = (i % 2 == 0) ? r : r / 3.0f;
        glVertex3f(
            x + cos(a) * rr,
            y + sin(a) * rr,
            z
        );
    }
    glEnd();
}

void drawBintangJatuh() {
    for (int i = 0; i < MAX_BINTANG_JATUH; i++) {
        if (!bintangJatuh[i].aktif) continue;

        glLineWidth(5.0f); // ekor lebih tebal
        glBegin(GL_LINES);
            glColor3f(1.0f, 1.0f, 1.0f);          // kepala bintang putih
            glVertex3f(bintangJatuh[i].x, bintangJatuh[i].y, bintangJatuh[i].z);

            glColor3f(0.4f, 0.6f, 1.0f);         // ekor biru
            glVertex3f(bintangJatuh[i].px, bintangJatuh[i].py, bintangJatuh[i].pz);
        glEnd();

        // kepala bintang
        glPushMatrix();
            glColor3f(1.0f, 1.0f, 1.0f);
            glTranslatef(
                bintangJatuh[i].x,
                bintangJatuh[i].y,
                bintangJatuh[i].z
            );

            glRotatef(bintangJatuh[i].rot, 0.0f, 0.0f, 1.0f); // muter di sumbu Z

            drawStar(0.0f, 0.0f, 0.0f, 0.25f);
        glPopMatrix();
    }
}

void updateBintang(float mobilX, float mobilZ) {
    mobilXPos = mobilX;
    mobilZPos = mobilZ;
}

void drawBintang() {
    glColor3f(1.0f, 1.0f, 1.0f);

    static bool init = false;
    if (!init) {
        for (int i = 0; i < JUMLAH_BINTANG; i++) {

            // Spawn bebas di seluruh langit
            bintang[i].x = randRange(-TILE_SIZE / 2, TILE_SIZE / 2);
            bintang[i].y = randRange(TINGGI_MIN, TINGGI_MAX);
            bintang[i].z = randRange(-TILE_SIZE / 2, TILE_SIZE / 2);
        }
        init = true;
    }


    // OFFSET TILE (INFINITE)

    float baseX = floor(mobilXPos / TILE_SIZE) * TILE_SIZE;
    float baseZ = floor(mobilZPos / TILE_SIZE) * TILE_SIZE;

    // Gambar 3x3 tile agar aman ke segala arah
    for (int tx = -1; tx <= 1; tx++) {
        for (int tz = -1; tz <= 1; tz++) {

            float offsetX = baseX + tx * TILE_SIZE;
            float offsetZ = baseZ + tz * TILE_SIZE;

            for (int i = 0; i < JUMLAH_BINTANG; i++) {
                drawStar(
                    bintang[i].x + offsetX,
                    bintang[i].y,
                    bintang[i].z + offsetZ,
                    0.15f
                );
            }
        }
    }
}