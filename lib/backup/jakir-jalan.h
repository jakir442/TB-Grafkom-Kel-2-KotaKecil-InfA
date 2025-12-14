#ifndef JAKIR_JALAN_H
#define JAKIR_JALAN_H

// ================================
// EKSPOR VARIABEL GLOBAL
// ================================
extern float jalanOffset;

extern float camX;
extern float camY;
extern float camZ;

extern float camYaw;
extern float camPitch;

extern float moveSpeed;
extern float rotSpeed;

// ================================
// EKSPOR FUNGSI
// ================================
void initJalan();
void updateJalan(float speed);
void drawJalan();
void updateCamera();
void handleKeyboard(unsigned char key, int x, int y);
void rotasiKamera(int key, int x, int y);
void display();

#endif
