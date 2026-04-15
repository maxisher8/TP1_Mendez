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


/*
 * Pre condiciones: -
 * Post condiciones: Inicializará el juego, cargando toda la información inicial de Homero y de los 3 niveles.
 */
void inicializar_juego(juego_t *juego){
    obtener_mapa(juego->niveles->paredes, &juego->niveles->tope_paredes, juego->niveles->camino, &juego->niveles->tope_camino, 1);
    juego->homero.posicion.col = PRIMER_POSICION_COL;
    juego->homero.posicion.fil = PRIMER_POSICION_FIL;
    //inicializar runa
    //inicializar altar
    //inicializar posicion del pergamino como random
    juego->homero.recolecto_pergamino = false;
    juego->homero.hechizos_reveladores = CANTIDAD_HECHIZOS_INICIALES;
    juego->homero.vidas_restantes = CANTIDAD_VIDAS_INICIALES;
    juego->homero.antorchas = CANTIDAD_ANTORCHAS_INICIALES;
    juego->homero.antorcha_encendida = false;
    //srand ((unsigned)time(NULL)); 
	//int numero = rand() % 20 + 0;
    //inicializar los 5 totems con posiciones randoms
    //obstaculos

}

void mostrar_juego(juego_t juego){
    char mapa[MAX_FILAS][MAX_COLUMNAS];
    for (int i = 0; i < MAX_FILAS; i++) {
        for (int j = 0; j < MAX_COLUMNAS; j++) {
            mapa[i][j] = 'L';
        }
    }
    for (int i = 0; i < juego.niveles->tope_camino; i++) {
        mapa[juego.niveles->camino[i].fil][juego.niveles->camino[i].col] = CAMINO;
    }
    for (int i = 0; i < juego.niveles->tope_paredes; i++) {
        mapa[juego.niveles->paredes[i].fil][juego.niveles->paredes[i].col] = PARED;
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
