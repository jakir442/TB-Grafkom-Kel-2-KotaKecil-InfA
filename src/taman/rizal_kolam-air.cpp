#include <cmath>
#include "../header/taman/rizal_kolam-air.h"

static const float PI = 3.14159265358979323846f;

// ===============================
// DAUN LILI KECIL
// ===============================
void daun_lili_kecil(float x, float z, float s)
{
    glPushMatrix();
        glTranslatef(x, -2.7f, z);
        glScalef(s, s, s);

        // Daun
        glPushMatrix();
            glColor3f(0.0f, 0.5f, 0.0f);
            glScalef(3.0f, 0.05f, 3.0f);
            glutSolidSphere(1.0, 20, 20);
        glPopMatrix();

        // Bunga
        glPushMatrix();
            glColor3f(1.0f, 1.0f, 0.0f);
            glTranslatef(0, 0.2f, 0);
            glutSolidSphere(0.5, 10, 10);
        glPopMatrix();
    glPopMatrix();
}

// ===============================
// KOLAM AIR BULAT
// ===============================
void kolam_bulat()
{
    float radiusX = 2.5f;
    float radiusZ = 2.0f;
    int jumlah_titik = 40;

    // ---- AIR ----
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glColor4f(0.0f, 0.4f, 0.8f, 0.6f);

    glBegin(GL_POLYGON);
    for (int i = 0; i < jumlah_titik; i++) {
        float a = 2.0f * PI * i / jumlah_titik;
        glVertex3f(cos(a) * radiusX, -2.6f, sin(a) * radiusZ);
    }
    glEnd();
    glDisable(GL_BLEND);

    // ---- DINDING KOLAM ----
    glColor3f(0.4f, 0.4f, 0.4f);
    glBegin(GL_QUAD_STRIP);
    for (int i = 0; i <= jumlah_titik; i++) {
        float a = 2.0f * PI * i / jumlah_titik;
        float x = cos(a) * radiusX;
        float z = sin(a) * radiusZ;

        glVertex3f(x, -3.0f, z);
        glVertex3f(x, -2.5f, z);
    }
    glEnd();
}

// ===============================
// WRAPPER: KOLAM AIR LENGKAP
// ===============================
void kolam_air() {
    kolam_bulat();

    // Daun lili
    daun_lili_kecil(0.8f,  0.5f, 0.35f);
    daun_lili_kecil(-1.0f, -0.6f, 0.3f);
    daun_lili_kecil(0.5f, -1.0f, 0.25f);
}
