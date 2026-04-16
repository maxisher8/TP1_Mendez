#include <time.h> 
#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include "utiles.h"
#include "ascension_magios.h"

const char PARED = 'X';
const char CAMINO = ' ';
const int PRIMER_POSICION_COL = 0;
const int PRIMER_POSICION_FIL = 0;
const int CANTIDAD_VIDAS_INICIALES = 5;
const int CANTIDAD_HECHIZOS_INICIALES = 5;
const int CANTIDAD_ANTORCHAS_INICIALES = 5;

bool movimiento_valido(juego_t juego, char movimiento);

bool posicion_en_rango(int fil, int col){
    return (fil >= PRIMER_POSICION_FIL && fil < MAX_FILAS && col >= PRIMER_POSICION_COL && col < MAX_COLUMNAS);
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

/*
 * Pre condiciones: -
 * Post condiciones: Inicializará el juego, cargando toda la información inicial de Homero y de los 3 niveles.
 */
void inicializar_juego(juego_t *juego){
    juego->nivel_actual = 1;
    juego->tope_niveles = 1;
    juego->camino_visible = false;

    obtener_mapa(juego->niveles[1].paredes, &juego->niveles[1].tope_paredes, juego->niveles[1].camino, &juego->niveles[1].tope_camino, 1);
    inicializar_homero(juego);
    //inicializar los 5 totems con posiciones randoms
    //obstaculos
}

void mostrar_juego(juego_t juego){
    char mapa[MAX_FILAS][MAX_COLUMNAS];
    nivel_t nivel_actual = juego.niveles[juego.nivel_actual];

    for (int i = 0; i < MAX_FILAS; i++) {
        for (int j = 0; j < MAX_COLUMNAS; j++) {
            mapa[i][j] = 'L';
        }
    }
    for (int i = 0; i < nivel_actual.tope_camino; i++) {
        mapa[nivel_actual.camino[i].fil][nivel_actual.camino[i].col] = CAMINO;
    }
    for (int i = 0; i < nivel_actual.tope_paredes; i++) {
        mapa[nivel_actual.paredes[i].fil][nivel_actual.paredes[i].col] = PARED;
    }
    //pone la posicion de homero en el mapa
    mapa[juego.homero.posicion.fil][juego.homero.posicion.col] = 'H';
    
    for (int i = 0; i < MAX_FILAS; i++) {
        for (int j = 0; j < MAX_COLUMNAS; j++) {
            printf("%c ", mapa[i][j]);
        }
        printf("\n");
    }
}

void realizar_jugada(juego_t *juego, char movimiento){

    int proxima_fil = juego->homero.posicion.fil;
    int proxima_col = juego->homero.posicion.col;
    
    if(movimiento_valido(*juego, movimiento)){
        if(movimiento == 'w'){
            proxima_fil = juego->homero.posicion.fil += -1;
        }
        else if(movimiento == 's'){
            proxima_fil = juego->homero.posicion.fil += 1;
        }
        else if(movimiento == 'd'){
            proxima_col = juego->homero.posicion.col += 1;
        }
        else if(movimiento == 'a'){
            proxima_col = juego->homero.posicion.col += -1;
        }  
        if(posicion_en_rango(proxima_fil, proxima_col) && !es_pared(juego, proxima_fil, proxima_col)){
            juego->homero.posicion.fil = proxima_fil;
            juego->homero.posicion.col = proxima_col;
        }
        else{
            printf("Movimiento no valido, choca con una pared\n");
        }   
    }
}

bool movimiento_valido(juego_t juego, char movimiento){
    bool es_valido = false;
    
    es_valido = (movimiento == 'w' || movimiento == 'a' || movimiento == 's' || movimiento == 'd');
    if(!es_valido){
        printf("Movimiento no valido, por favor ingrese un movimiento valido (w, a, s, d)\n");
    }
    return es_valido;
    //si el movimiento es valido, actualizar la posicion de homero
    //si el movimiento no es valido, restar una vida a homero
}