#include <GL/freeglut.h>
#include <cmath>
#include <cstdlib>
#include "header/jakir-bintang.h"

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

// =====================
// KONFIGURASI LANGIT
// =====================
const int JUMLAH_BINTANG = 200;

// Ukuran tile langit (diulang)
const float TILE_SIZE = 80.0f;

// Tinggi langit
const float TINGGI_MIN = 10.0f;
const float TINGGI_MAX = 15.0f;

// Zona tengah kosong (bulan)
const float ZONA_TENGAH = 3.0f;

// =====================
struct Bintang {
    float x, y, z;
};

Bintang bintang[JUMLAH_BINTANG];

// Posisi mobil (world)
float mobilXPos = 0.0f;
float mobilZPos = 0.0f;

// =====================
float randRange(float min, float max) {
    return min + (rand() / (float)RAND_MAX) * (max - min);
}

// =====================
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

// =====================
void updateBintang(float mobilX, float mobilZ) {
    mobilXPos = mobilX;
    mobilZPos = mobilZ;
}

// =====================
void drawBintang() {
    glColor3f(1.0f, 1.0f, 1.0f);

    static bool init = false;
    if (!init) {
        for (int i = 0; i < JUMLAH_BINTANG; i++) {

            // ❌ Hindari tengah (bulan)
            do {
                bintang[i].x = randRange(-TILE_SIZE / 2, TILE_SIZE / 2);
            } while (fabs(bintang[i].x) < ZONA_TENGAH);

            bintang[i].y = randRange(TINGGI_MIN, TINGGI_MAX);
            bintang[i].z = randRange(-TILE_SIZE / 2, TILE_SIZE / 2);
        }
        init = true;
    }

    // =====================
    // OFFSET TILE (INFINITE)
    // =====================
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
