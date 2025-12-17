#ifndef JAKIR_LAMPU_H
#define JAKIR_LAMPU_H

#include <GL/freeglut.h>

// status ON / OFF lampu
extern bool lampuNyala;
extern int  levelLampu; // 0-3

// inisialisasi lighting global
void initLampuLighting();

// gambar 1 lampu jalan
void drawLampuJalan(float x, float z, GLenum lightID);

void keyboardLampu(unsigned char key);

#endif
