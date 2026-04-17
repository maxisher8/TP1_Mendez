#include <time.h> 
#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include "utiles.h"
#include "ascension_magios.h"

const char PARED = 'X';
const char CAMINO = ' ';
const char ESPACIO_VACIO = '-';
const char HOMERO = 'H';
const int PRIMER_POSICION_COL = 0;
const int PRIMER_POSICION_FIL = 0;
const int CANTIDAD_VIDAS_INICIALES = 5;
const int CANTIDAD_HECHIZOS_INICIALES = 5;
const int CANTIDAD_ANTORCHAS_INICIALES = 5;
const char MOVIMIENTO_ARRIBA = 'W';
const char MOVIMIENTO_IZQUIERDA = 'A';
const char MOVIMIENTO_ABAJO = 'S';
const char MOVIMIENTO_DERECHA = 'D';


bool posicion_en_rango(int fil, int col){
    bool en_rango = (fil >= PRIMER_POSICION_FIL && fil < MAX_FILAS && col >= PRIMER_POSICION_COL && col < MAX_COLUMNAS);
    return en_rango;
}

bool es_pared(juego_t *juego, int fil, int col){
    nivel_t *nivel_actual = &juego->niveles[juego->nivel_actual];

    for (int i = 0; i < nivel_actual->tope_paredes; i++) {
        if (nivel_actual->paredes[i].fil == fil && nivel_actual->paredes[i].col == col) {
            return true;
        }
    }
    return false;
}


void inicializar_homero(juego_t *juego){
    juego->homero.posicion.col = juego->niveles[juego->nivel_actual].camino[0].col;
    juego->homero.posicion.fil = juego->niveles[juego->nivel_actual].camino[0].fil;
    //inicializar runa
    //inicializar posicion del pergamino como random
    juego->homero.recolecto_pergamino = false;
    juego->homero.hechizos_reveladores = CANTIDAD_HECHIZOS_INICIALES;
    juego->homero.vidas_restantes = CANTIDAD_VIDAS_INICIALES;
    juego->homero.antorchas = CANTIDAD_ANTORCHAS_INICIALES;
    juego->homero.antorcha_encendida = false;
}

void inicializar_totems(juego_t *juego){
    objeto_t totems[5];
    srand ((unsigned)time(NULL)); 
    for (int i = 0; i < 5; i++) {
        totems[i].tipo = 'T';
        totems[i].posicion.fil = rand() % 20 + 0;
        totems[i].posicion.col = rand() % 30 + 0;
        printf("El valor aleatorio es: fila =  %i\n", totems[i].posicion.fil);
        printf("El valor aleatorio es: columna = %i\n", totems[i].posicion.col);
    }
}

void inicializar_juego(juego_t *juego){
    juego->nivel_actual = 1;
    juego->tope_niveles = 1;
    juego->camino_visible = false;

    obtener_mapa(juego->niveles[1].paredes, &juego->niveles[1].tope_paredes, juego->niveles[1].camino, &juego->niveles[1].tope_camino, 1);
    inicializar_homero(juego);
    inicializar_totems(juego);
    //obstaculos
}

void mostrar_juego(juego_t juego){
    char mapa[MAX_FILAS][MAX_COLUMNAS];
    nivel_t nivel_actual = juego.niveles[juego.nivel_actual];

    for (int i = 0; i < MAX_FILAS; i++) {
        for (int j = 0; j < MAX_COLUMNAS; j++) {
            mapa[i][j] = ESPACIO_VACIO;
        }
    }
    for (int i = 0; i < nivel_actual.tope_camino; i++) {
        mapa[nivel_actual.camino[i].fil][nivel_actual.camino[i].col] = CAMINO;
    }
    for (int i = 0; i < nivel_actual.tope_paredes; i++) {
        mapa[nivel_actual.paredes[i].fil][nivel_actual.paredes[i].col] = PARED;
    }

    mapa[juego.homero.posicion.fil][juego.homero.posicion.col] = HOMERO;
    
    for (int i = 0; i < MAX_FILAS; i++) {
        for (int j = 0; j < MAX_COLUMNAS; j++) {
            printf("%c ", mapa[i][j]);
        }
        printf("\n");
    }
}

bool movimiento_valido(juego_t juego, char movimiento){
    bool es_valido = false;

    es_valido = (movimiento == MOVIMIENTO_ARRIBA || movimiento == MOVIMIENTO_IZQUIERDA || movimiento == MOVIMIENTO_ABAJO || movimiento == MOVIMIENTO_DERECHA);
    if(!es_valido){
        printf("Movimiento no valido, por favor ingrese un movimiento valido (W, A, S ,D)\n");
    }
    return es_valido;
}

void realizar_jugada(juego_t *juego, char movimiento){

    int proxima_fil = juego->homero.posicion.fil;
    int proxima_col = juego->homero.posicion.col;
    
    if(movimiento_valido(*juego, movimiento)){
        if(movimiento == MOVIMIENTO_ARRIBA){
            proxima_fil += -1;
        }
        else if(movimiento == MOVIMIENTO_ABAJO){
            proxima_fil += 1;
        }
        else if(movimiento == MOVIMIENTO_DERECHA){
            proxima_col += 1;
        }
        else if(movimiento == MOVIMIENTO_IZQUIERDA){
            proxima_col += -1;
        }  
        if(posicion_en_rango(proxima_fil, proxima_col) && !(es_pared(juego, proxima_fil, proxima_col))){
            juego->homero.posicion.fil = proxima_fil;
            juego->homero.posicion.col = proxima_col;
        }
        else{
            printf("Movimiento no valido, choca con una pared\n");
        }   
    }
}

int estado_nivel(nivel_t nivel, personaje_t homero){
    int estado = 0;
    return estado;
}
