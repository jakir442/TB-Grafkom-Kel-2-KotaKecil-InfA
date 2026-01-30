#ifndef PUTRI_TRASHBIN_H
#define PUTRI_TRASHBIN_H

#include <GL/glut.h>
#include <GL/glu.h>

// objek dasar
void drawTrashBin(float r, float g, float b);
void drawText3D(const char* text);
void logoOrganik();
void drawAnorganikLogo();
void logoB3();
void drawMetalBar(float length);

// wrapper SET
void drawTrashSetOrganik();
void drawTrashSetAnorganik();
void drawTrashSetB3();

#endif
