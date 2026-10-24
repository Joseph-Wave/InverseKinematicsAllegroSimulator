
#define GLEW_STATIC
#include <GL/glew.h>
#include <GL/freeglut.h>

#include <cmath>
#include <iostream>
#include <fstream>
using namespace std;

float j_1 = 0; float j_2 = 180; float j_3 = 0;

float delta = 0.01;

float altura_de_camara = 0;
float desvio_de_camara = 0;

//////////////////UBICACION Y ESCALAMIENTO/////////////////////
float escalamiento = 0.04; // 0.07 maximo, 0.05 -> OPTIMO

float ubicar_robot_x = 0; /// toma como referencia el origen 0
float ubicar_robot_y = -0.3;
float ubicar_robot_z = 0.3;

float grosor_de_linea_robot = 1;
float grosor_de_linea_plano = 1;
///////////////////////////////////////////////////////////////

const int num_de_box = 7;

void recibe_datos(void);

float longitudes[3*num_de_box] = { /// para cuando los uses ya estaran escalados
    //largo, ancho, alto
    //caja 1
    20,10,3.5, //20,10,6,
    //caja 2
    2.7,4,15, //2.7,4,15
    //caja 3
    3,3,10, //3,3,10
    //caja 4 izq
    2.7,3,8.5, //2.7,3,5,
    //caja 5 der
    2.7,3,8.5, //2.7,3,5
    //tubillo
    1.5,0.1,0.1,
    //tubillo final
    0.1,0.1,0.75
};

class box {
private:
    GLfloat *CUBO, *CUBO_linea;
    //float ancho, largo, alto, R, G, B;
    float a, l, al, R, G, B;
    float p_x, p_y, p_z;
public:
    box() {
        a = l = al = 0;
        R = G = B = 0;
        p_x = p_y = p_z = 0;
        actualizar(); }
    //////////////////////////////////////////////////////////
    void dar_medidas(float ancho, float largo, float alto) {
        a = ancho; l = largo; al = alto;
        actualizar();
    }
    void dar_color(float Rx, float Gx, float Bx) {
        R = Rx; G = Gx; B = Bx;
        actualizar();
    }
    //////////////////////////////////////////////////////////
    GLfloat *get_CUBO() {return CUBO;}
    GLfloat *get_CUBO_linea() {return CUBO_linea;}
    //////////////////////////////////////////////////////////
    void dar_nuevo_origen(float x, float y, float z) {
        p_x = x; p_y = y; p_z = z;
    }
    float get_p_x() { return p_x; }
    float get_p_y() { return p_y; }
    float get_p_z() { return p_z; }
    ///////////////////////////////////////////////////////////
    void pantalla();
    void actualizar();

};

void box::actualizar() {

    CUBO = new GLfloat[6*4*6];
    CUBO_linea = new GLfloat[15*2*6];

    GLfloat CUBO_temp[] = {
//x     y     z    R  G  B
 a/2, -al/2, -l/2, R, G, B,
a/2, al/2, -l/2,  R, G, B,
 -a/2, al/2, -l/2,  R, G, B,
 -a/2, -al/2, -l/2,  R, G, B,

 a/2, -al/2, l/2,  R, G, B,
 a/2, al/2, l/2,  R, G, B,
 a/2, al/2, -l/2,  R, G, B,
 a/2, -al/2,-l/2,  R, G, B,

 a/2, -al/2, l/2,  R, G, B,
 a/2, al/2, l/2,  R, G, B,
-a/2, al/2, l/2,  R, G, B,
-a/2, -al/2, l/2,  R, G, B,

 -a/2, -al/2, l/2,  R, G, B,
 -a/2, al/2, l/2,  R, G, B,
 -a/2, al/2, -l/2,  R, G, B,
 -a/2, -al/2, -l/2,  R, G, B,

 a/2, al/2, -l/2,  R, G, B,
 a/2, al/2, l/2,  R, G, B,
 -a/2, al/2, l/2,  R, G, B,
 -a/2, al/2, -l/2,  R, G, B,

 a/2, -al/2, -l/2,  R, G, B,
 a/2, -al/2, l/2,  R, G, B,
 -a/2, -al/2, l/2,  R, G, B,
 -a/2, -al/2, -l/2,  R, G, B

};

    GLfloat CUBO_linea_temp[] = {
//x    y    z    R    G    B
 -(a+delta)/2, (al+delta)/2, -(l+delta)/2, 0, 0, 0,
 -(a+delta)/2, (al+delta)/2, (l+delta)/2, 0, 0, 0, //1-2

-(a+delta)/2, (al+delta)/2, (l+delta)/2, 0, 0, 0,
(a+delta)/2, (al+delta)/2, (l+delta)/2, 0, 0, 0, //2-3

(a+delta)/2, (al+delta)/2, (l+delta)/2, 0, 0, 0,
(a+delta)/2, (al+delta)/2, -(l+delta)/2, 0, 0, 0, //3-4

(a+delta)/2, (al+delta)/2, -(l+delta)/2, 0, 0, 0,
-(a+delta)/2, (al+delta)/2, -(l+delta)/2, 0, 0, 0, //4-5

-(a+delta)/2, (al+delta)/2, -(l+delta)/2, 0, 0, 0,
-(a+delta)/2, -(al+delta)/2, -(l+delta)/2, 0, 0, 0, //5-6

-(a+delta)/2, -(al+delta)/2, -(l+delta)/2, 0, 0, 0,
-(a+delta)/2, -(al+delta)/2, (l+delta)/2, 0, 0, 0, //6-7

-(a+delta)/2, -(al+delta)/2, (l+delta)/2, 0, 0, 0,
(a+delta)/2, -(al+delta)/2, (l+delta)/2, 0, 0, 0, //7-8

(a+delta)/2, -(al+delta)/2, (l+delta)/2, 0, 0, 0,
(a+delta)/2, -(al+delta)/2, -(l+delta)/2, 0, 0, 0, //8-9

(a+delta)/2, -(al+delta)/2, -(l+delta)/2, 0, 0, 0,
-(a+delta)/2, -(al+delta)/2, -(l+delta)/2, 0, 0, 0, //9-10

-(a+delta)/2, -(al+delta)/2, -(l+delta)/2, 0, 0, 0,
(a+delta)/2, -(al+delta)/2, -(l+delta)/2, 0, 0, 0, //10-11

(a+delta)/2, -(al+delta)/2, -(l+delta)/2, 0, 0, 0,
(a+delta)/2, (al+delta)/2, -(l+delta)/2, 0, 0, 0, //11-12

(a+delta)/2, (al+delta)/2, -(l+delta)/2, 0, 0, 0,
(a+delta)/2, (al+delta)/2, (l+delta)/2, 0, 0, 0, //12-13

(a+delta)/2, (al+delta)/2, (l+delta)/2, 0, 0, 0,
(a+delta)/2, -(al+delta)/2, (l+delta)/2, 0, 0, 0, //13-14

(a+delta)/2, -(al+delta)/2, (l+delta)/2, 0, 0, 0,
-(a+delta)/2, -(al+delta)/2, (l+delta)/2, 0, 0, 0, //14-15

-(a+delta)/2, -(al+delta)/2, (l+delta)/2, 0, 0, 0,
-(a+delta)/2, (al+delta)/2, (l+delta)/2, 0, 0, 0, //15-16

};

    for(int i = 0; i < 6*4*6; i++)
        CUBO[i] = CUBO_temp[i];
    for(int i = 0; i < 15*2*6; i++)
        CUBO_linea[i] = CUBO_linea_temp[i];
}

void box::pantalla() {
    glEnableClientState(GL_VERTEX_ARRAY);
    glEnableClientState(GL_COLOR_ARRAY);
    //------------------------------------------------------

    glVertexPointer(3, GL_FLOAT, 6*sizeof(GLfloat), CUBO_linea);
    glColorPointer(3, GL_FLOAT, 24, CUBO_linea+3); //3*sizeof(GLfloat)

    glDrawArrays(GL_LINES,0,15*2);
    //-------------------------------------------------------

    glVertexPointer(3, GL_FLOAT, 6*sizeof(GLfloat), CUBO);
    glColorPointer(3, GL_FLOAT, 24, CUBO+3); //3*sizeof(GLfloat)

    glDrawArrays(GL_QUADS,0,4*6);
    //-------------------------------------------------------

    glDisableClientState(GL_VERTEX_ARRAY);
    glDisableClientState(GL_COLOR_ARRAY);
}

box zzz[num_de_box];

void eje_medio(box *XEL, int i, float &p_m_x, float &p_m_y, float &p_m_z) {

    //static GLfloat *ubic = XEL[3].get_CUBO_linea(); // <- una buena forma de hacerlo para un i

    float a0, a1, a2, c18, c19, c20;
    GLfloat *temp;

        temp = XEL[i].get_CUBO_linea();
        a0 = temp[0]; a1 = temp[1]; a2 = temp[2];
        c18 = temp[18]; c19 = temp[19]; c20 = temp[20];
        p_m_x = (a0+c18)/2; p_m_y = (a1+c19)/2; p_m_z = (a2+c20)/2;
}

void dibujar_plano(float grosor_de_linea, float escalamiento_cuadricula) {

GLfloat cuadricula[] = {
1,0,1,      0,0,0,
-0.98,0,1,      0,0,0,

1,0,0.82,   0,0,0,
-0.98,0,0.82,   0,0,0,

1,0,0.64,   0,0,0,
-0.98,0,0.64,   0,0,0,

1,0,0.46,   0,0,0,
-0.98,0,0.46,   0,0,0,

1,0,0.28,   0,0,0,
-0.98,0,0.28,   0,0,0,

1,0,0.1,    0,0,0,
-0.98,0,0.1,    0,0,0,

1,0,-0.08,  0,0,0,
-0.98,0,-0.08,  0,0,0,

1,0,-0.26,  0,0,0,
-0.98,0,-0.26,  0,0,0,

1,0,-0.44,  0,0,0,
-0.98,0,-0.44,  0,0,0,

1,0,-0.64,  0,0,0,
-0.98,0,-0.64,  0,0,0,

1,0,-0.8,   0,0,0,
-0.98,0,-0.8,   0,0,0,

1,0,-0.98,  0,0,0,
-0.98,0,-0.98,  0,0,0,

/////////////////

1,0,1,      0,0,0,
1,0,-0.98,      0,0,0,

0.82,0,1,   0,0,0,
0.82,0,-0.98,   0,0,0,

0.64,0,1,   0,0,0,
0.64,0,-0.98,   0,0,0,

0.46,0,1,   0,0,0,
0.46,0,-0.98,   0,0,0,

0.28,0,1,   0,0,0,
0.28,0,-0.98,   0,0,0,

0.1,0,1,    0,0,0,
0.1,0,-0.98,    0,0,0,

-0.08,0,1,  0,0,0,
-0.08,0,-0.98,  0,0,0,

-0.26,0,1,  0,0,0,
-0.26,0,-0.98,  0,0,0,

-0.44,0,1,  0,0,0,
-0.44,0,-0.98,  0,0,0,

-0.64,0,1,  0,0,0,
-0.64,0,-0.98,  0,0,0,

-0.8,0,1,   0,0,0,
-0.8,0,-0.98,   0,0,0,

-0.98,0,1,  0,0,0,
-0.98,0,-0.98,  0,0,0,

};

for(int i = 0; i < 12*2*6*2; i++) cuadricula[i] *= escalamiento_cuadricula;

    glLineWidth(grosor_de_linea);

    glEnableClientState(GL_VERTEX_ARRAY);
    glEnableClientState(GL_COLOR_ARRAY);

    glVertexPointer(3, GL_FLOAT, 6*sizeof(GLfloat), cuadricula);
    glColorPointer(3, GL_FLOAT, 24, cuadricula+3); //3*sizeof(GLfloat)

    glDrawArrays(GL_LINES,0,24*2);

    glDisableClientState(GL_VERTEX_ARRAY);
    glDisableClientState(GL_COLOR_ARRAY);
}

void pintar( box *XEL, int numero_de_elementos) {

    float p_medio_x, p_medio_y, p_medio_z;

    recibe_datos();

    for (int i = 0; i < numero_de_elementos; i++) {

        glLoadIdentity();

        glTranslatef(ubicar_robot_x,ubicar_robot_y,ubicar_robot_z); /// ubicar el robot
        glRotatef(-desvio_de_camara,0,1,0);
        glRotatef(-altura_de_camara,1,0,0);

        glTranslatef(0,-longitudes[2]/2-1*delta,0);
        dibujar_plano(grosor_de_linea_plano,escalamiento*20);
        glTranslatef(0,longitudes[2]/2+1*delta,0);

        glLineWidth(grosor_de_linea_robot);

        /// Nuevo origen
        glTranslatef(XEL[i].get_p_x(),XEL[i].get_p_y(),XEL[i].get_p_z());
        eje_medio(XEL, i, p_medio_x, p_medio_y, p_medio_z);

        switch(i) {
        case 0: XEL[i].pantalla(); break; /// Base
        case 1: /// Primer eslabon
            glRotatef(j_1,p_medio_x,p_medio_y,p_medio_z);
            //////////////////////////////////////////////////
            glTranslatef(-p_medio_x,-p_medio_y + 10*escalamiento,-p_medio_z); /// da la distancia de la base...
            glRotatef(j_2,1,0,0);
            glTranslatef(p_medio_x,p_medio_y - 10*escalamiento,p_medio_z); /// a la union
            /// esa medida es la altura donde se encuentra la union
            //aunque alrevez
            //////////////////////////////////////////////////

            XEL[i].pantalla();

            /// Segundo eslabon
            glTranslatef(p_medio_x-(longitudes[6]+longitudes[3])/2-2*delta,p_medio_y - 15*escalamiento,p_medio_z); /// caja 3
            glRotatef(j_3,1,0,0);
            XEL[i+1].pantalla();

            /// Tubillo

            glTranslatef(p_medio_x - 0.8*escalamiento+(longitudes[6]+longitudes[3])/2+2*delta,p_medio_y - 15*escalamiento + 3*escalamiento,p_medio_z); /// tubillo
            XEL[5].pantalla();
            glTranslatef(p_medio_x + longitudes[15]/2,p_medio_y - 15*escalamiento + 7*escalamiento,p_medio_z); /// tubillo
            XEL[6].pantalla();

            break;

        case 2: break;

        case 3: /// Adorno izquierdo
            glRotatef(j_1,p_medio_x,p_medio_y,p_medio_z);
            ///equivalente a dar_nuevo_origen
            glTranslatef(-((longitudes[9]+longitudes[3])/2)-2*delta,(longitudes[2]+longitudes[11])/2+2*delta,0);
            XEL[i].pantalla();
            break;
        case 4: /// adorno derecho
            glRotatef(j_1,p_medio_x,p_medio_y,p_medio_z);
            ///equivalente a dar_nuevo_origen
            glTranslatef(((longitudes[9]+longitudes[3])/2)+2*delta,(longitudes[2]+longitudes[11])/2+2*delta,0);
            XEL[i].pantalla();
            break;
        }
    }
}

void display(void) {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    pintar(zzz,num_de_box);

    glutSwapBuffers();
}

void reshape(int width, int high) { glViewport(0,0,width,high);}

void initOpenGL() {
    glClearColor(255,255,255,0);
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LEQUAL);
}

static void key(unsigned char key, int x, int y) {

    switch (key) {
        case '1': j_1+=10; break;
        case '2': j_2+=10; break;
        case '3': j_3+=10; break;
        case '4': j_1-=10; break;
        case '5': j_2-=10; break;
        case '6': j_3-=10; break;
        case '7': desvio_de_camara+=10; break;
        case '8':
            desvio_de_camara = 0;
            altura_de_camara = 0; break;
        case '9': altura_de_camara+=10; break;
    }
    glutPostRedisplay();
}

int main(int argc, char **argv) {

    for(int i = 0; i < 3*num_de_box; i++) longitudes[i] *= escalamiento;

    /// Caja base
    zzz[0].dar_medidas(longitudes[0],longitudes[1],longitudes[2]);
    zzz[0].dar_color(255,0,0);

    /// Primer eslabon
    zzz[1].dar_medidas(longitudes[3],longitudes[4],longitudes[5]);
    zzz[1].dar_color(0,255,0);
    zzz[1].dar_nuevo_origen(0,8*escalamiento,0); /// da la altura de la union

    /// Segundo eslabon
    zzz[2].dar_medidas(longitudes[6],longitudes[7],longitudes[8]);
    zzz[2].dar_color(0,255,0);

    /// Adorno izquiedo
    zzz[3].dar_medidas(longitudes[9],longitudes[10],longitudes[11]);
    zzz[3].dar_color(0,0,255);

    /// Adorno derecho
    zzz[4].dar_medidas(longitudes[12],longitudes[13],longitudes[14]);
    zzz[4].dar_color(0,0,255);

    /// Tubillo
    zzz[5].dar_medidas(longitudes[15],longitudes[16],longitudes[17]);
    zzz[5].dar_color(0,0,255);

    /// Tubillo Final
    zzz[6].dar_medidas(longitudes[18],longitudes[19],longitudes[20]);
    zzz[6].dar_color(0,0,255);

    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH);
    glutInitWindowSize(500,500);
    glutCreateWindow("pp");

    glutDisplayFunc(display);
    glutIdleFunc(display);
    glutKeyboardFunc(key);
    glutReshapeFunc(reshape);

    initOpenGL();

    glutMainLoop();

    return 0;
}

void recibe_datos(void) {

    // el programa no esta sincronizado con el programa de control
    // por eso esta parte, toma nume_ruido muestras y hasta que se mantengan constantes
    // actualiza los datos; esto para prevenir la lectura de algun un numero "medio" escrito

    const int nume_ruido = 3;
    float mega[nume_ruido][5];

    for(int i = 0; i < nume_ruido; i++) {
        ifstream op;
        op.open((char *)"C:/Users/Jojo/Desktop/conectaundo.dat");
        for(int j = 0; j < 5; j++) {
            op >> mega[i][j];
        }
        op.close();
    }

    float meta1[5];
    meta1[0] = mega[0][0];
    meta1[1] = mega[0][1];
    meta1[2] = mega[0][2];
    meta1[3] = mega[0][3];
    meta1[4] = mega[0][4];

    int cou = 0;
    for(int i = 0; i < nume_ruido; i++) {
        if(meta1[0] == mega[i][0])
            if(meta1[1] == mega[i][1])
                if(meta1[2] == mega[i][2])
                    if(meta1[3] == mega[i][3])
                        if(meta1[4] == mega[i][4]) cou++;
    }

    if(cou == nume_ruido) {
        j_1 = meta1[0]; j_2 = meta1[1] + 180; j_3 = meta1[2];
        altura_de_camara = meta1[3]; desvio_de_camara = meta1[4];
        cout << j_1 << "\t" << j_2 << "\t" << j_3 << endl;
    }

}

