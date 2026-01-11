#include <GL/freeglut.h>
#include <FreeImage.h>
#include <cstdio>
#include <algorithm> // std::swap
#include "header/jakir-bulan.h"

GLuint textureBulan;

// =====================
// LOAD TEXTURE DENGAN FREEIMAGE
// =====================
GLuint loadTexture(const char* filename) {
    FREE_IMAGE_FORMAT format = FreeImage_GetFileType(filename, 0);
    if (format == FIF_UNKNOWN)
        format = FreeImage_GetFIFFromFilename(filename);

    if (format == FIF_UNKNOWN) {
        printf("Unknown image format: %s\n", filename);
        return 0;
    }

    FIBITMAP* bitmap = FreeImage_Load(format, filename);
    if (!bitmap) {
        printf("Failed to load image: %s\n", filename);
        return 0;
    }

    FIBITMAP* bitmap32 = FreeImage_ConvertTo32Bits(bitmap);
    FreeImage_Unload(bitmap);

    int width  = FreeImage_GetWidth(bitmap32);
    int height = FreeImage_GetHeight(bitmap32);
    BYTE* data = FreeImage_GetBits(bitmap32);

    // =====================
    // KONVERSI BGRA → RGBA
    // =====================
    for (int i = 0; i < width * height * 4; i += 4) {
        std::swap(data[i + 0], data[i + 2]); // B <-> R
    }

    GLuint textureID;
    glGenTextures(1, &textureID);
    glBindTexture(GL_TEXTURE_2D, textureID);

    glTexImage2D(
        GL_TEXTURE_2D,
        0,
        GL_RGBA,
        width,
        height,
        0,
        GL_RGBA,
        GL_UNSIGNED_BYTE,
        data
    );

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    FreeImage_Unload(bitmap32);
    return textureID;
}


// =====================
// INIT TEXTURE BULAN
// =====================
void initTextureBulan() {
    FreeImage_Initialise();
    textureBulan = loadTexture("C:/Users/taufi/Downloads/TB-Grafkom-Kel-2-KotaKecil-InfA/src/texture/bulan.jpg"); // PNG / JPG / BMP
}

// =====================
// DRAW BULAN DENGAN TEKSTUR
// =====================
void drawBulan(float mobilX, float mobilZ) {
    glPushMatrix();

    float jarakBulan = 300.0f;

    glTranslatef(
        20.0f,          // tengah jalan
        65.0f,         // langit
        mobilZ - jarakBulan
    );

    // UKURAN BULAN (BESAR KECIL)
    glScalef(3.0f, 3.0f, 1.0f);

    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, textureBulan);
    glColor3f(1, 1, 1);

    GLUquadric* quad = gluNewQuadric();
    gluQuadricTexture(quad, GL_TRUE);
    gluQuadricNormals(quad, GL_SMOOTH);

    gluSphere(quad, 10.0, 60, 60);

    gluDeleteQuadric(quad);

    glDisable(GL_TEXTURE_2D);
    glPopMatrix();
}
