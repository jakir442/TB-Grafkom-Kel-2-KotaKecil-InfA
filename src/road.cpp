#include <GL/freeglut.h>
#include <cmath>
#include "road.h"

float roadOffset = 0.0f;

// Inisialisasi khusus objek jalan
void initRoad() {
    // kosong jika tidak diperlukan
}

// Update offset jalan
void updateRoad(float speed) {
    roadOffset += speed;
}

// Gambar jalan tanpa batas + garis kuning putus-putus
void drawRoad() {
    glColor3f(0.2f, 0.2f, 0.2f);

    float visibleDist = 300.0f;
    float start = -visibleDist;
    float end   =  visibleDist;

    float baseShift = fmod(roadOffset, 10.0f);

    glBegin(GL_QUADS);
    for (float z = start; z < end; z += 10.0f) {
        float zShift = z + baseShift;

        glVertex3f(-2.0f, 0.0f, zShift);
        glVertex3f( 2.0f, 0.0f, zShift);
        glVertex3f( 2.0f, 0.0f, zShift + 10);
        glVertex3f(-2.0f, 0.0f, zShift + 10);
    }
    glEnd();

    // Garis kuning putus-putus
    glColor3f(1, 1, 0);
    glLineWidth(3);

    glBegin(GL_LINES);
    for (float z = start; z < end; z += 10.0f) {
        float zShift = z + baseShift;

        glVertex3f(0, 0.05f, zShift + 3);
        glVertex3f(0, 0.05f, zShift + 6);
    }
    glEnd();
}
