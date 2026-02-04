#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glut.h>
#include <cstdlib>

#include "../header/taman/azhari_kursi.h"

//transform objek
float rotX = 0;
float rotY = 0;
float scaleF = 1.0f;

//warna
GLfloat wood[] = {0.7f, 0.5f, 0.3f, 1};
GLfloat legC[] = {0.5f, 0.35f, 0.2f, 1};

//balok dari cube
void balok(float w,float h,float d){
    glPushMatrix();
        glScalef(w,h,d);
        glutSolidCube(1);
    glPopMatrix();
}


void drawKursiTaman(){

    glColor3fv(wood);

    //dudukan
    glPushMatrix();
        balok(3.5,0.1,1.2);
    glPopMatrix();

    //sandaran
    glPushMatrix();
        glTranslatef(0,0.6,-0.7);
        glRotatef(80,1,0,0);
        balok(3.5,0.1,1.2);
    glPopMatrix();

    glColor3fv(legC);

    //kaki kiri belakang
    glPushMatrix();
        glTranslatef(-1.6,-0.5,-0.5);
        balok(0.2,1,0.2);
    glPopMatrix();

    //kaki kanan belakang
    glPushMatrix();
        glTranslatef(1.6,-0.5,-0.5);
        balok(0.2,1,0.2);
    glPopMatrix();

    //kaki kiri depan
    glPushMatrix();
        glTranslatef(-1.6,-0.5,0.4);
        balok(0.2,1,0.2);
    glPopMatrix();

    //kaki kanan depan
    glPushMatrix();
        glTranslatef(1.6,-0.5,0.4);
        balok(0.2,1,0.2);
    glPopMatrix();
}

#ifdef STANDALONE 
void init() {
    glEnable(GL_DEPTH_TEST);
    glClearColor(0,0,0,1); // background hitam
}

//keyboard
void key(unsigned char k,int,int){
    if(k==27) exit(0);
    if(k=='r'){ rotX=rotY=0; scaleF=1; }

    if(k=='=') scaleF += 0.1f;
    if(k=='-') scaleF -= 0.1f;

    glutPostRedisplay();
}

void special(int k,int,int){
    if(k==GLUT_KEY_LEFT)  rotY -= 5;
    if(k==GLUT_KEY_RIGHT) rotY += 5;
    if(k==GLUT_KEY_UP)    rotX -= 5;
    if(k==GLUT_KEY_DOWN)  rotX += 5;

    glutPostRedisplay();
}


void display(){
    glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
    glLoadIdentity();

    //kamera tetap
    gluLookAt(5,4,7, 0,0,0, 0,1,0);

    //transform hanya objek
    glPushMatrix();
        glScalef(scaleF,scaleF,scaleF);
        glRotatef(rotX,1,0,0);
        glRotatef(rotY,0,1,0);
        kursi();
    glPopMatrix();

    glutSwapBuffers();
}


void reshape(int w,int h){
    glViewport(0,0,w,h);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(45,(float)w/h,0.1,100);
    glMatrixMode(GL_MODELVIEW);
}


int main(int argc,char** argv){
    glutInit(&argc,argv);
    glutInitDisplayMode(GLUT_DOUBLE|GLUT_RGB|GLUT_DEPTH);
    glutInitWindowSize(800,600);
    glutCreateWindow("Azhari_Kursi Taman ");

    init();

    glutDisplayFunc(display);
    glutReshapeFunc(reshape);
    glutKeyboardFunc(key);
    glutSpecialFunc(special);

    glutMainLoop();
    return 0;
}
#endif
