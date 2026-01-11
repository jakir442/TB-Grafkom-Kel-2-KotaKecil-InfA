#include <GL/glew.h>
#include <GL/glut.h>
#include <FreeImage.h>
#include <stdio.h>

using namespace std;

// Menyimpan ID texture
// GLuint texture_Earth_ID;
GLuint texture_Moon_ID;
// GLuint texture_Extra_ID;

// Fungsi Quadric untuk membuat object Sphere dengan UV mapping texture
void createObject(float radius) {
    GLUquadric* object = gluNewQuadric();
    gluQuadricTexture(object, GL_TRUE);
    gluQuadricNormals(object, GLU_SMOOTH);
    gluSphere(object, radius, 100, 100);
}

// Variabel rotasi
float tickEarth = 0.0f;
float tickMoonSelf = 0.0f;   // Rotasi bulan sendiri
float tickMoonOrbit = 0.0f;  // Rotasi orbit bulan


// Variabel global untuk ID texture loader
GLuint textureID = 0;

// Fungsi untuk memuat texture
int loadTexture(const char* path) {
    glGenTextures(1, &textureID);
    void* imgData;
    int imgWidth, imgHeight;
    FREE_IMAGE_FORMAT format = FreeImage_GetFIFFromFilename(path);
    if (format == FIF_UNKNOWN) {
        printf("Unknown file type for texture image file %s\n", path);
        return -1;
    }
    FIBITMAP* bitmap = FreeImage_Load(format, path, 0);
    if (!bitmap) {
        printf("Failed to load image %s\n", path);
        return -1;
    }
    FIBITMAP* bitmap2 = FreeImage_ConvertTo24Bits(bitmap);
    FreeImage_Unload(bitmap);
    imgData = FreeImage_GetBits(bitmap2);
    imgWidth = FreeImage_GetWidth(bitmap2);
    imgHeight = FreeImage_GetHeight(bitmap2);
    if (imgData) {
        int formatColor = (FI_RGBA_RED == 0) ? GL_RGB : GL_BGR;
        glBindTexture(GL_TEXTURE_2D, textureID);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, imgWidth, imgHeight, 0,
                    formatColor, GL_UNSIGNED_BYTE, imgData);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        return textureID++;
    } else {
        printf("Failed to get texture data from %s\n", path);
        return -1;
    }
}

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
    glBindTexture(GL_TEXTURE_2D, texture_Moon_ID); // texture bulan
    glColor3f(1, 1, 1);

    GLUquadric* quad = gluNewQuadric();
    gluQuadricTexture(quad, GL_TRUE);
    gluQuadricNormals(quad, GL_SMOOTH);

    gluSphere(quad, 10.0, 60, 60);

    gluDeleteQuadric(quad);

    glDisable(GL_TEXTURE_2D);
    glPopMatrix();
}

// Setup kamera
void initTextureBulan() {
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_POLYGON_SMOOTH);
    glShadeModel(GL_SMOOTH);

    // Enable texture
    glEnable(GL_TEXTURE_2D);

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(45.0, 1800.0 / 900.0, 1.0, 100.0);
    glMatrixMode(GL_MODELVIEW);

    // Load textures
    texture_Moon_ID  = loadTexture("D:/2406004-KULIAH-TEKNIK-INFORMATIKA-A/semester_3/Praktikum-Grafik-Komputer-1-SKS/UAS/UAS-KotaKecil/src/texture/bulan.jpg"); // Bulan
}

#ifdef standalone 
void display() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glLoadIdentity();

    // Kamera
    gluLookAt(
        20.0, 10.0, 20.0,   // Posisi kamera
        0.0,  0.0,  0.0,    // Titik pandang
        0.0,  1.0,  0.0     // Arah atas
    );

    // Render Bulan (Orbit)
    glPushMatrix();
        glRotatef(tickMoonOrbit, 0.0f, 1.0f, 0.0f);
        glTranslatef(12.0f, 0.0f, 0.0f);
        glRotatef(tickMoonSelf, 0.0f, 1.0f, 0.0f);

        glEnable(GL_TEXTURE_2D);
        glBindTexture(GL_TEXTURE_2D, texture_Moon_ID);
        createSphere(2.0f);
        glDisable(GL_TEXTURE_2D);
    glPopMatrix();

    glutSwapBuffers();

    // Update animasi
    tickEarth     += 0.2f;
    tickMoonSelf  += 0.5f;
    tickMoonOrbit += 0.1f;

    glutPostRedisplay();
}


// Main Program
int main(int argc, char** argv) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH);
    glutInitWindowSize(1800, 900);
    glutInitWindowPosition(100, -50);

    glutCreateWindow("2406004 - Jakir: Bumi & Bulan");

    glewInit();
    initTextureBulan();
    glutDisplayFunc(display);
    glutMainLoop();
    return 0;
}
#endif