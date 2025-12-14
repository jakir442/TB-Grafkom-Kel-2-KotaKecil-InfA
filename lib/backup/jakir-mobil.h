#ifndef JAKIR_MOBIL_H
#define JAKIR_MOBIL_H

// Variabel posisi dan rotasi mobil
extern float mobilX;
extern float mobilZ;
extern float mobilRot;

// Deklarasi fungsi untuk mobil
void drawCube(float w, float h, float d);
void drawWheel();
void drawMobil();
void updateMobil(float speed);
void initMobil();
void drawMobil();

#endif // JAKIR_MOBIL_H