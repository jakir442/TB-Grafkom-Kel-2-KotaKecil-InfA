#ifndef JAKIR_BINTANG_H
#define JAKIR_BINTANG_H

// Bintang normal
// struct Bintang {
//     float x, y, z;
// };
void drawStar(float x, float y, float z, float r);
void drawBintang();
void updateBintang(float mobilX, float mobilZ);

// Bintang jatuh
// struct BintangJatuh {
//     float x, y, z;
//     float vx, vy, vz;
//     bool aktif;
// };

// maksimal 5 bintang jatuh bersamaan
// extern BintangJatuh bintangJatuh[5];

void spawnBintangJatuh(int value);
void updateBintangJatuh();
void drawBintangJatuh();

// =====================
// KONTROL HUJAN BINTANG
// =====================
extern int jumlahSpawned;
extern bool hujanAktif;

extern const int TOTAL_BINTANG_HUJAN;
extern const int DELAY_ANTAR_BINTANG;
extern const int DELAY_ANTAR_HUJAN;

// =====================
// TIMER HUJAN BINTANG
// =====================
void mulaiHujanBintang(int value);
void spawnBintangJatuh(int value);

#endif
