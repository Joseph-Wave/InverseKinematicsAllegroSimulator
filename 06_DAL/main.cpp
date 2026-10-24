
#include <allegro.h> //<- recuerdame
#include <fstream>
#include <iostream>
#include <cmath>

#include "Js2xyz.h"
#include "xyz2Js.h"

using namespace std;

float start = 10;
float j_1 = 0; float j_2 = 0; float j_3 = start;
float x_x = 0; float y_y = 0; float z_z = 0;

float altura_de_camara = 0;
float desvio_de_camara = -90;

bool modo_xyz = 0;

void enviar_datos(void);

void set_xyz(float &x, float &y, float &z);

void set_js(void);

void allegro_start(int screen_x , int screen_y){
   allegro_init();
   install_keyboard();
   install_mouse();

   set_color_depth(32);
   set_gfx_mode(GFX_AUTODETECT_WINDOWED, screen_x, screen_y, 0, 0);
}

void caracter(float flotante, int *digit) { /// digit[5]
    /// de un numero "flotante" devuelve los digitos a dos decimales

    float flotante_abs;

    if(flotante < 0) flotante_abs = -1 * flotante;
    else flotante_abs = flotante;

    int int_j1 = flotante_abs*100;
    int num[5];
    num[4] = int_j1 % 10;
    num[3] = ((int_j1 % 100) - (int_j1 % 10)) / 10;
    num[2] = ((int_j1 % 1000) - (int_j1 % 100)) / 100;
    num[1] = ((int_j1 % 10000) - (int_j1 % 1000)) / 1000;
    num[0] = (int_j1 - (int_j1 % 10000)) / 10000;

    int tempo = 0;
    for(int j = 0; j < 5; j++) {
        tempo = 0;
        for(int i = 0; i <= 9; i++) {
            if(tempo < num[j]) tempo++;
        }
        digit[j] = tempo;
    }

}

void imaginar_js(int *local) {

    int digit_1[5]; int digit_2[5]; int digit_3[5];
    int signo_1 = 16; int signo_2 = 16; int signo_3 = 16;

    if(modo_xyz) {
        float ax,by,cz;
        set_xyz(ax,by,cz); /// usado solo para ilustrar
        caracter(ax,digit_1); caracter(by,digit_2); caracter(cz,digit_3);
        if(by < 0) signo_1 = 10; if(cz < 0) signo_2 = 10;
        if(ax < 0) signo_3 = 10;
    } else {
        float temp_j_2 = 0;
        if(j_2 < 0) temp_j_2 = j_2 + 360; else temp_j_2 = j_2;
        caracter(j_1,digit_1); caracter(temp_j_2,digit_2); caracter(j_3-start,digit_3);
        if(j_1 < 0) signo_1 = 10; if(temp_j_2 < 0) signo_2 = 10;
        if(j_3 < 0) signo_3 = 10;
        set_xyz(x_x,y_y,z_z);
    }

    if(modo_xyz) {
        int carg_local[32] = {
            16,13,11,signo_1,
            digit_1[0],digit_1[1],digit_1[2],17,digit_1[3],digit_1[4],16,
            16,14,11,signo_2,
            digit_2[0],digit_2[1],digit_2[2],17,digit_2[3],digit_2[4],16,
            16,15,11,signo_3,
            digit_3[0],digit_3[1],digit_3[2],17,digit_3[3],digit_3[4]
        };
        for(int i = 0; i < 32; i++) local[i] = carg_local[i];
    } else {
        int carg_local[32] = {
            12,1,11,signo_1,
            digit_1[0],digit_1[1],digit_1[2],17,digit_1[3],digit_1[4],16,
            12,2,11,signo_2,
            digit_2[0],digit_2[1],digit_2[2],17,digit_2[3],digit_2[4],16,
            12,3,11,signo_3,
            digit_3[0],digit_3[1],digit_3[2],17,digit_3[3],digit_3[4]
        };
        for(int i = 0; i < 32; i++) local[i] = carg_local[i];
    }
}

int main() {

    allegro_start(600,320);

    BITMAP *cursor = load_bitmap("cursor.bmp",NULL);
    BITMAP *placa = load_bitmap("placa.bmp",NULL);
    BITMAP *numeros = load_bitmap("numeros.bmp",NULL);
    BITMAP *buffer_screen = create_bitmap(626,320);

    clear_to_color(buffer_screen, 0xFFFFFF);
    masked_blit(placa,buffer_screen,11,0,0,0,626,320);
    blit(buffer_screen,screen,0,0,0,0,626,320);
    show_mouse(screen);
    set_mouse_sprite(cursor);
    set_mouse_sprite_focus(8,8);

    int local[32];
    int botones[24] = { // coordenanda x,y de la esquina superior izquierda de cada boton
        28,178,
        88,122,
        90,240,
        148,179,
        205,273,
        272,271,
        350,136,
        350,207,
        451,129,
        451,200,
        555,125,
        555,196
    };
    // acomoda los botones algunos pixeles a los costados
    // para facilitar dar clic
    for(int i = 0; i < 24; i++) botones[i] -= 10;

    int boton = 0xFFFFFF;
    int ubicacion_del_texto_x = 45;
    int ubicacion_del_texto_y = 40;

    float velocidad = 0.1;

    while(!key[KEY_ESC]) {

        /// Limpiar
        clear_to_color(buffer_screen, 0xFFFFFF);
        masked_blit(placa,buffer_screen,11,0,0,0,626,320);

        /// Pintar numeros

        imaginar_js(local);

        for(int i = 0; i < 32; i++)
            masked_blit(numeros,buffer_screen,local[i]*16,0,i*16+ubicacion_del_texto_x,ubicacion_del_texto_y,16,22);
        /// Numero de velocidad
        masked_blit(numeros,buffer_screen,((int)(velocidad*10))*16,0,210,274,16,22);

        /// ¿Cual boton se apachurro?
        for(int i = 0; i < 24; i+=2) {
            if((mouse_x > botones[i]) && (mouse_x < botones[i]+43) && (mouse_b & 1) &&
               (mouse_y > botones[i+1]) && (mouse_y < botones[i+1]+41))
                boton = i/2;
            if(!(mouse_b & 1)) boton = 0xFFFFFF;

            if(!modo_xyz) {
                switch(boton) {
                    case 0: desvio_de_camara -= velocidad; break;
                    case 1: altura_de_camara += velocidad; break;
                    case 2: altura_de_camara -= velocidad; break;
                    case 3: desvio_de_camara += velocidad; break;
                    case 4: if(velocidad > 0.9) velocidad = 0.1;
                            else velocidad += 0.1;
                            rest(100);
                            break;
                    case 5: if(modo_xyz) modo_xyz = 0;
                            else modo_xyz = 1;
                            break;
                    case 6: if(j_1 <= 180) j_1 += velocidad; break;
                    case 7: if(j_1 >= 0) j_1 -= velocidad; break;
                    case 8: if(j_2 <= 134) j_2 += velocidad; break;
                    case 9: if(j_2 >= 0) j_2 -= velocidad; break;
                    case 10: if(j_3 <= 142) j_3 += velocidad; break;
                    case 11: if(j_3 >= 0) j_3 -= velocidad; break;
                };
            } else {
                switch(boton) {
                    case 0: desvio_de_camara -= velocidad; break;
                    case 1: altura_de_camara += velocidad; break;
                    case 2: altura_de_camara -= velocidad; break;
                    case 3: desvio_de_camara += velocidad; break;
                    case 4: if(velocidad > 0.9) velocidad = 0.1;
                            else velocidad += 0.1;
                            rest(100);
                            break;
                    case 5: if(modo_xyz) modo_xyz = 0;
                            else modo_xyz = 1;
                            break;

                    case 6: x_x += velocidad; set_js(); break;
                    case 7: x_x -= velocidad; set_js(); break;
                    case 8: y_y += velocidad; set_js(); break;
                    case 9: y_y -= velocidad; set_js(); break;
                    case 10: z_z += velocidad; set_js(); break;
                    case 11: z_z -= velocidad; set_js(); break;
                };
            }
        }

        /// Enviar datos y mostrar lo que quieras en consola
        enviar_datos();

        //cout << boton << endl;

        /// Pasar todo a pantalla y delay
        blit(buffer_screen,screen,0,0,0,0,626,320);
        rest(50);
    }

    destroy_bitmap(numeros);
    destroy_bitmap(placa);
    destroy_bitmap(buffer_screen);

    return 0;
}
END_OF_MAIN();

void enviar_datos(void) {
    ofstream of;
    of.open("C:/Users/Jojo/Desktop/conectando.dat", std::fstream::out | std::fstream::trunc);
    of << j_1 << endl << j_2 << endl << j_3 << endl << altura_de_camara << endl << desvio_de_camara<< endl;
    of.close();
}

void set_xyz(float &x, float &y, float &z) {
    double *temp;
    coordinates c;

    c.set_joints(j_1*pi/180,j_2*pi/180,j_3*pi/180);
    c.set_lengths(145,110,100);
    temp = c.get_direct();

    x = temp[3]; y = temp[7]; z = temp[11];
}

void set_js(void) {

    float a,b,c;
    angulos(a,b,c,x_x,y_y,z_z);
/*
    if(abs(j_1 - a) <= 10) j_1 = a;
    if(abs(j_2 - b) <= 10) j_2 = b;
    if(abs(j_3 - c) <= 10) j_3 = c;
*/
    j_1 = a; j_2 = b; j_3 = c;
}
