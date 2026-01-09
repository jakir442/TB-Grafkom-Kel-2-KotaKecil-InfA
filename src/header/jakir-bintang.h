#ifndef JAKIR_BINTANG_H
#define JAKIR_BINTANG_H


void drawStar(float x, float y, float z, float r);
void drawBintang();
void updateBintang(float mobilX, float mobilZ);

void spawnBintangJatuh(int value);
void updateBintangJatuh();
void drawBintangJatuh();

// kontrol hujan bintang
extern int jumlahSpawned;
extern bool hujanAktif;

extern const int TOTAL_BINTANG_HUJAN;
extern const int DELAY_ANTAR_BINTANG;
extern const int DELAY_ANTAR_HUJAN;

// timer hujan bintang
void mulaiHujanBintang(int value);
void spawnBintangJatuh(int value);

#endif