#ifndef JUNGKAT_JUNGKIT_H
#define JUNGKAT_JUNGKIT_H

#include <GL/glut.h>
#include <GL/glu.h>

// ================= VARIABEL ANIMASI =================
extern float rotPapan;
extern float rotSpeed;
extern bool naik;

// ================= FUNGSI OBJEK =================
void alas_lampu(float tX, float tY, float tZ,
                float angle, float rX, float rY, float rZ,
                float sX, float sY, float sZ);

void tiang_lampu(float tX, float tY, float tZ,
                 float angle, float rX, float rY, float rZ,
                 float sX, float sY, float sZ);

void bola(float tX, float tY, float tZ,
          float angle, float rX, float rY, float rZ,
          float scaleX, float scaleY, float scaleZ);

// ================= FUNGSI UTAMA =================
void jungkat_jungkit();

#endif
