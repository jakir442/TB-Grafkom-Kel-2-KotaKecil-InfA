#ifndef JAKIR_drawKolamAir_H
#define JAKIR_drawKolamAir_H

#include <GL/freeglut.h>
#include <GL/glut.h>
#include <GL/glu.h>

// Daun lili kecil
void daun_lili_kecil(float x, float z, float s);

// Kolam air bulat
void kolam_bulat();

// Wrapper kolam air + isi (biar tinggal panggil)
void drawKolamAir();

#endif
