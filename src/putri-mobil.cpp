#include <GL/glut.h>
#include <GL/glu.h>
#include <math.h>

#include "header/putri-mobil.h"

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

//var mobil
float mobilPosX = 0.0f;
float mobilPosY = 0.6f;  //posisi sumbu mobil (X,Y,Z), untuk ketinggian mobil dengan jalan
float mobilPosZ = 0.6f;
float mobilYaw = 180.0f;   //arah hadap mobil (sumbu Y)
float mobilWheelAngle = 0.0f;   //sudut roda berputar putar anjay

//init opengl
void initMobil() {
    glEnable(GL_DEPTH_TEST);
}

//Kubus
void drawCube(float r, float g, float b) {
    glColor3f(r, g, b);     
    glutSolidCube(1.0);     //kubus solid uk 1x1x1 (ke diubah di scale) -> Objek Mobil 1 
}

//roda
void drawWheel() {
    GLUquadric *quad = gluNewQuadric();    //buat objek silinder

    glPushMatrix();
        glRotatef(90, 0, 1, 0);      //pergerakan roda
        glColor3f(0.3f, 0.3f, 0.3f); //warna ban bagian luar
        gluCylinder(quad,     //badan roda, quadric -> Objek Mobil 2
		0.2,                 //ats
		0.2,                 //bawah
		0.2,                 //panjang
		20,                  //jumlah melingkar
		20);                //jumlah memanjang

        //garis penanda rotasi ban
        glColor3f(1.0f, 1.0f, 1.0f); //warna ban bagian dalam
        glBegin(GL_LINES); // -> Objek Mobil 3
            glVertex3f(0.2f, 0.0f, 0.0f);
            glVertex3f(0.2f, 0.0f, 0.2f);
        glEnd();

        gluDisk(quad, 0.0, 0.2, 20, 20);    //tutup depan roda -> Objek Mobil 4
        glTranslatef(0, 0, 0.2);            //pergeseran roda
        gluDisk(quad, 0.0, 0.2, 20, 20);
    glPopMatrix();

    gluDeleteQuadric(quad);
}

//mobil
void drawMobil() {
    glPushMatrix();                                      //simpan transformasi mobil
        glTranslatef(mobilPosX, mobilPosY, mobilPosZ);   //posisi mobil (mobil pindah, roda ikut pindah boz)
        glRotatef(mobilYaw, 0, 1, 0);                    //arah hadap mobil (mobil belok, roda ikut belok)

        //atap
        glPushMatrix();
            glTranslatef(0, 0.5, 0);     //geser keatas body (perubahan)
            glScalef(1.0, 0.5, 1.0);     //ukuran atap 
            drawCube(0.0, 0.0, 1.0);   
        glPopMatrix();
        
        //kaca
        glPushMatrix();
        	glColor3f(0.5f, 0.8f, 1.0f);   //warnaaaaaaaa
            glTranslatef(0.0f, 0.55f, 0.55f);     //geser keatas body (perubahan)
            glScalef(0.8f, 0.3f, 0.05f);      //ukuran atap 
            drawCube(1.0f, 1.0f, 1.0f);
        glPopMatrix();

        //body
        glPushMatrix();
            glScalef(1.0, 0.6, 2.0);
            drawCube(0.6f, 0.0f, 0.0f); //warna
        glPopMatrix();

        glPushMatrix();
			glColor3f(1, 1, 0);
			glTranslatef(-0.5f, 0.4f, 0.0f);
    		glScalef(0.1f, 0.002f, 0.002f);
    		glRotatef(-90, 0, 1, 0); //(angle, x, y, z)
    		glutStrokeCharacter(GLUT_STROKE_ROMAN, 'R'); // -> Objek untuk menggambar huruf
		glPopMatrix();

        glPushMatrix();
    		glColor3f(1, 1, 0);
    		glTranslatef(0.5f, 0.4f, 0.0f);
    		glScalef(0.002f, 0.002f, 0.002f);
    		glRotatef(90, 0, 1, 0); 
    	    glutStrokeCharacter(GLUT_STROKE_ROMAN, 'L');
		glPopMatrix();
		
		//rodaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa
		glPushMatrix();
		    glTranslatef(0.7f, -0.4f, -0.85f); //posisi roda kanan
		    glRotatef(mobilWheelAngle, 1, 0, 0); //muter roda na
		    drawWheel();
		glPopMatrix();
		
		glPushMatrix();
		    glTranslatef(-0.85f, -0.4f, -0.85f); //posisi roda kiri
		    glRotatef(mobilWheelAngle, 1, 0, 0); //muter roda na
		    drawWheel();
		glPopMatrix();
		
		glPushMatrix();
		    glTranslatef(0.7f, -0.4f, 0.7f);  //posisi roda belakang kanan
		    glRotatef(mobilWheelAngle, 1, 0, 0); //muter roda na
		    drawWheel();
		glPopMatrix();
		
		glPushMatrix();
		    glTranslatef(-0.85f, -0.4f, 0.7f); //posisi roda belakang kiri
		    glRotatef(mobilWheelAngle, 1, 0, 0); //muter roda na
		    drawWheel();
		glPopMatrix();

        
		//looping pergerakan roda
        //for (int i = 0; i < 2; i++) {               //lopingan buat roda best i(kanan,kiri)j(depan,belakang)
            //for (int j = 0; j < 2; j++) {
               // glPushMatrix();
                   // glTranslatef(x[i], -0.4, z[j]);       //posisi roda biar dibawah di badan na
                    //glRotatef(mobilWheelAngle, 1, 0, 0);  //muter roda na
                    //drawWheel();
                //glPopMatrix();
           // }
        //} 

        //lampu depan (torus)
        glColor3f(1.0f, 1.0f, 0.0f); //warna lampu kuning

        //lampu kanan
        glPushMatrix();
            glTranslatef(0.27f, 0.22f, 1.05f); //posisi depan kanan
            glRotatef(90, 0, 0, 1);            //sudut(x,y,z)
            glutSolidTorus(0.03, 0.12, 20, 30);
        glPopMatrix();

        //lampu kiri
        glPushMatrix();
            glTranslatef(-0.27f, 0.22f, 1.05f); //posisi depan kiri
            glRotatef(90, 0, 0, 1);             //sejajar sumbu X
            glutSolidTorus(0.03, 0.12, 20, 30); //uk lampu
        glPopMatrix();
	
	glPopMatrix();
}         

void updateMobil(float speed) {
    mobilPosX -= sin(mobilYaw * M_PI / 180.0f) * speed;     //sumbu X
    mobilPosZ -= cos(mobilYaw * M_PI / 180.0f) * speed;     //sumbu Z
    mobilWheelAngle -= speed * 300.0f; //roda jalan
}

//(kanan,kiri langsung rawr)void strafeMobil(float speed) {
    //mobilPosX -= cos(mobilYaw * M_PI / 180.0f) * speed;
    //mobilPosZ -= sin(mobilYaw * M_PI / 180.0f) * speed;
//}


//keyword
void controlMobil(unsigned char key) {
    float moveSpeed = 0.35f;       //kecepatan maju mundur
    float turnSpeed = 3.0f;        //kecepatan kanan kiri

    switch (key) {
        case 'w': case 'W': updateMobil(-moveSpeed); break;            //maju
        case 's': case 'S': updateMobil(moveSpeed); break;           //mundur
        case 'd': case 'D': mobilYaw -= turnSpeed; break;             //belok kiri
        case 'a': case 'A': mobilYaw += turnSpeed; break;             //belok kanan                  
		//A': strafeMobil(-moveSpeed); break; // geser kiri
		//D': strafeMobil(moveSpeed); break;  // geser kanan                   
    }
    glutPostRedisplay();
}

//akses privat
#ifdef STANDALONE
    //keyboard 
    float rotSceneY = 0.0f; //kiri-kanan
    float rotSceneX = 0.0f; //atas-bawah

    void keyboardPrivate(unsigned char key, int x, int y) {
        controlMobil(key);
		switch (key){
		    case 27: exit(0);
		}
        glutPostRedisplay();
    }

    void specialKeyboardRotasi(int key, int x, int y) {
        switch (key) {
            case GLUT_KEY_LEFT:
                rotSceneY -= 5.0f;
                break;
            case GLUT_KEY_RIGHT:
                rotSceneY += 5.0f;
                break;
            case GLUT_KEY_UP:
                rotSceneX -= 5.0f;
                break;
            case GLUT_KEY_DOWN:
                rotSceneX += 5.0f;
                break;
            }
        glutPostRedisplay();
    }

    //grid3d
    void drawGrid3D() {
        glColor3f(0.3f, 0.3f, 0.3f); //warna line gridnyaa
        glLineWidth(1.0f);     //ketebalan garis

        glBegin(GL_LINES); //Objek Private
        for (int i = -20; i <= 20; i++) {      //lantai XZ (nu kotak kotak)
            // garis sejajar sumbu Z
            glVertex3f(i, 0, -20);   //titik awal
            glVertex3f(i, 0,  20);   //titik akhir

            //garis sejajar sumbu X
            glVertex3f(-20, 0, i);    
            glVertex3f( 20, 0, i);
        }
        glEnd();
    }

    //fungsi window
    void reshape(int w, int h) {
        glViewport(0, 0, w, h);       //area gambar sesuai uk window
        glMatrixMode(GL_PROJECTION);  //mode proyeksi kamera
        glLoadIdentity();
        gluPerspective(60, (float)w / h, 1, 100);   //sudut pandang kameraa (kek zoom in out bgituloo)1 jarak dekat, 100 terjaoh
        glMatrixMode(GL_MODELVIEW);
    }

    //display
    void display() {
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glClearColor(0.0f, 0.0f, 0.0f, 1.0f); //background hitam

        glLoadIdentity();

        gluLookAt(
            6.0f, 5.0f, 8.0f,  //posisi kamera
            0.0f, 0.0f, 0.0f,  //titik yang dilihat
            0.0f, 1.0f, 0.0f   //arah atas
        );
        
        glPushMatrix();
            //rotasi untuk melihat 3D
            glRotatef(rotSceneX, 1.0f, 0.0f, 0.0f);
            glRotatef(rotSceneY, 0.0f, 1.0f, 0.0f);

            drawMobil();  
        glPopMatrix();
        drawGrid3D(); 
        glutSwapBuffers();
    }

    //main
    int main(int argc, char **argv) {
        glutInit(&argc, argv);
        glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
        glutInitWindowSize(800, 600);
        glutCreateWindow("Putri - Mobil 3D");

        initMobil();

        glutKeyboardFunc(keyboardPrivate);
        glutSpecialFunc(specialKeyboardRotasi);
        glutDisplayFunc(display);
        glutReshapeFunc(reshape);

        glutMainLoop();
        return 0;
    }
#endif
