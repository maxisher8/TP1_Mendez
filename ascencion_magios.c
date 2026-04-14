#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <time.h> 
#include "ascension_magios.h"
#include "utiles.h"

/*
 * Pre condiciones: -
 * Post condiciones: Inicializará el juego, cargando toda la información inicial de Homero y de los 3 niveles.
 */
void inicializar_juego(juego_t *juego){

    juego->homero.posicion.col = 0;
    juego->homero.posicion.fil = 0;
    juego->homero.recolecto_pergamino = false;
    juego->homero.hechizos_reveladores = 5;
    juego->homero.vidas_restantes = 5;
    juego->homero.antorchas = 5;
    juego->homero.antorcha_encendida = false;
    //inicializar runa en la posicion de homero (la primer posicion)
    //inicializar altar en la ultima posicion
    //inicializar la posicion del pergamino con random
    srand ((unsigned)time(NULL)); 
	int numero = rand() % 20 + 0;
    //inicializar los 5 totems con posiciones randoms
    //obstaculos

}

void mostrar_juego(juego_t juego){
    coordenada_t paredes[MAX_PAREDES];
    int tope_paredes;
    coordenada_t camino[MAX_CAMINO];
    int tope_camino;
    int nivel;
    obtener_mapa(paredes, tope_paredes, camino, tope_camino, nivel);
}
