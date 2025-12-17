#ifndef PUTRI_MOBIL_H
#define PUTRI_MOBIL_H

// Variabel Mobil (extern agar bisa diakses dari file lain)
extern float mobilPosX;
extern float mobilPosY;
extern float mobilPosZ;
extern float mobilYaw;
extern float mobilWheelAngle;

// Fungsi Mobil
void initMobil();
void updateMobil(float speed);
void drawMobil();
void controlMobil(unsigned char key);

#endif
