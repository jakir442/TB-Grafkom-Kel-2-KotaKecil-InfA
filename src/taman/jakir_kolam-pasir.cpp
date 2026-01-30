#include <cmath>
#include "../header/taman/jakir_kolam-pasir.h"

static const float PI = 3.1415926535f;

// ===============================
// PERMUKAAN PASIR (BERGELOMBANG)
// ===============================
void drawPasir(float radius)
{
    glColor3f(0.85f, 0.75f, 0.45f);

    int slices = 70;
    int rings  = 35;

    float baseHeight = 0.30f;

    for (int i = 0; i < rings; i++) {
        float r1 = radius * i / rings;
        float r2 = radius * (i + 1) / rings;

        glBegin(GL_TRIANGLE_STRIP);
        for (int j = 0; j <= slices; j++) {
            float a = j * 2 * PI / slices;

           // ---- GUNDukan TENGAH (DOMINAN) ----
            float mound1 = 0.22f * exp(-r1 * r1 * 0.9f);
            float mound2 = 0.22f * exp(-r2 * r2 * 0.9f);

            // ---- GELOMBANG BESAR ----
            float waveBig1 = 0.075f * pow(sin(r1 * 2.8f + a * 2.0f), 2);
            float waveBig2 = 0.075f * pow(sin(r2 * 2.8f + a * 2.0f), 2);
            
            // ---- GELOMBANG KECIL (DETAIL PASIR) ----
            float waveSmall1 = 0.03f * pow(sin(r1 * 9.0f + a * 11.0f), 2);
            float waveSmall2 = 0.03f * pow(sin(r2 * 9.0f + a * 11.0f), 2);

            float h1 = baseHeight + mound1 + waveBig1 + waveSmall1;
            float h2 = baseHeight + mound2 + waveBig2 + waveSmall2;

            glVertex3f(cos(a) * r1, h1, sin(a) * r1);
            glVertex3f(cos(a) * r2, h2, sin(a) * r2);
        }
        glEnd();
    }
}

// ===============================
// KOLAM PASIR ANAK-ANAK
// ===============================
void drawKolam(float x, float z)
{
    glPushMatrix();
    glTranslatef(x, 0.0f, z);

    float radius = 2.5f;

    // ---- PASIR ----
    drawPasir(radius);

    // ---- DINDING KOLAM (BIBIR SANDBOX) ----
    glColor3f(0.6f, 0.45f, 0.25f); // kayu / beton coklat
    glBegin(GL_QUAD_STRIP);
    for (int i = 0; i <= 60; i++) {
        float a = i * 2 * PI / 60;
        float x1 = cos(a) * radius;
        float z1 = sin(a) * radius;

        glVertex3f(x1, 0.0f, z1);
        glVertex3f(x1, 0.4f, z1); // tinggi dinding rendah (aman anak)
    }
    glEnd();

    glPopMatrix();
}
