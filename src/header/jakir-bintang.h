#ifndef JAKIR_BINTANG_H
#define JAKIR_BINTANG_H

// kontrol hujan bintang
extern const int DELAY_ANTAR_HUJAN;

void drawStar(float x, float y, float z, float r);
void drawBintang();
void updateBintang(float mobilX, float mobilZ);

void spawnBintangJatuh(int value);
void updateBintangJatuh();
void drawBintangJatuh();

// timer hujan bintang
void mulaiHujanBintang(int value);
void spawnBintangJatuh(int value);

#endif