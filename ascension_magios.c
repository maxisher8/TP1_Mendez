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
const char MOVIMIENTO_USO_HECHIZO = 'H';
const char MOVIMIENTO_USO_ANTORCHA = 'L';

//Validaciones

bool posicion_en_rango(int fil, int col){
    bool en_rango = (fil >= PRIMER_POSICION_FIL && fil < MAX_FILAS && col >= PRIMER_POSICION_COL && col < MAX_COLUMNAS);
    return en_rango;
}

bool es_pared(juego_t juego, int fil, int col){
    for (int i = 0; i < juego.niveles[juego.nivel_actual].tope_paredes; i++) {
        if (juego.niveles[juego.nivel_actual].paredes[i].fil == fil && juego.niveles[juego.nivel_actual].paredes[i].col == col) {
            return true;
        }
    }
    return false;
}

bool es_homero(juego_t juego, int fil, int col){
    bool es_homero = juego.homero.posicion.fil == fil && juego.homero.posicion.col == col;
    if(es_homero){
        return true;
    }
    else{
        return false;
    }
}

bool es_pergamino(juego_t *juego, int fil, int col){
    bool es_pergamino = juego->niveles[juego->nivel_actual].pergamino.fil == fil && juego->niveles[juego->nivel_actual].pergamino.col == col;
    if(es_pergamino){
        return true;
    }
    else{
        return false;
    }
}

bool es_runa(juego_t juego, int fil, int col){
    bool es_runa = (juego.niveles[juego.nivel_actual].camino[0].fil == fil) && (juego.niveles[juego.nivel_actual].camino[0].col == col);
    if(es_runa){
        return true;
    }
    return false;
}

bool es_altar(juego_t juego, int fil, int col){
    bool es_altar = (juego.niveles[juego.nivel_actual].camino[juego.niveles[juego.nivel_actual].tope_camino - 1].fil == fil) && (juego.niveles[juego.nivel_actual].camino[juego.niveles[juego.nivel_actual].tope_camino - 1].col == col);
    if(es_altar){
        return true;
    }
    return false;
}

bool es_camino(juego_t juego, int fil, int col){
    for (int i = 0; i < juego.niveles[juego.nivel_actual].tope_camino; i++) {
        if (juego.niveles[juego.nivel_actual].camino[i].fil == fil && juego.niveles[juego.nivel_actual].camino[i].col == col) {
            return true;
        }
    }
    return false;
}
//CAMBIAR NOMBRE DE LA FUNCION ES_VALIDO 
bool es_valido(juego_t juego, int fil, int col){
    if(es_homero(juego, fil, col) || es_pared(juego, fil, col) || es_pergamino(&juego, fil, col) || es_runa(juego, fil, col) || es_altar(juego, fil, col)){
        return false;
    }
    return true;
}

bool es_totem(juego_t juego, int fil, int col){
    for (int i = 0; i < juego.niveles[juego.nivel_actual].tope_herramientas; i++) {
        if (juego.niveles[juego.nivel_actual].herramientas[i].posicion.fil == fil && juego.niveles[juego.nivel_actual].herramientas[i].posicion.col == col && juego.niveles[juego.nivel_actual].herramientas[i].tipo == 'T') {
            return true;
        }
    }
    return false;
}

bool es_vacio(juego_t juego, int fil, int col){
    if(!(es_homero(juego, fil, col)) && !(es_pared(juego, fil, col)) && !(es_pergamino(&juego, fil, col)) && !(es_runa(juego, fil, col)) && !(es_altar(juego, fil, col)) && !(es_totem(juego, fil, col)) && !(es_camino(juego, fil, col)) && !(es_pared(juego, fil, col))){
        return true;
    }
    return false;
}

//restar/sumar vidas segun el movimiento

void restar_vida(juego_t *juego, int fil, int col){
    if(es_pared(*juego, fil, col)){
        juego->homero.vidas_restantes--;
        printf("¡Oh no! Has chocado contra una pared. Te quedan %d vidas.\n", juego->homero.vidas_restantes);
    }
}

void sumar_vida(juego_t *juego, int fil, int col){
    if(es_totem(*juego, fil, col)){
        juego->homero.vidas_restantes++;
        printf("¡Has ganado una vida! Ahora tienes %d vidas.\n", juego->homero.vidas_restantes);
        //ELIMINAR EL TOTEM DEL MAPA Y DEL VECTOR HERRAMIENTAS
    }
}

//inicializaciones

void inicializar_homero(juego_t *juego){
    juego->homero.posicion.col = juego->niveles[juego->nivel_actual].camino[0].col;
    juego->homero.posicion.fil = juego->niveles[juego->nivel_actual].camino[0].fil;
    juego->homero.recolecto_pergamino = false;
    juego->homero.hechizos_reveladores = CANTIDAD_HECHIZOS_INICIALES;
    juego->homero.vidas_restantes = CANTIDAD_VIDAS_INICIALES;
    juego->homero.antorchas = CANTIDAD_ANTORCHAS_INICIALES;
    juego->homero.antorcha_encendida = false;
}

void inicializar_totems(juego_t *juego){
    juego->niveles[juego->nivel_actual].tope_herramientas = 0;
    for (int i = 0; i < 5; i++) {
        juego->niveles[juego->nivel_actual].herramientas[juego->niveles[juego->nivel_actual].tope_herramientas].tipo = 'T';
        juego->niveles[juego->nivel_actual].herramientas[juego->niveles[juego->nivel_actual].tope_herramientas].posicion.fil = rand() % 20 + 0;
        juego->niveles[juego->nivel_actual].herramientas[juego->niveles[juego->nivel_actual].tope_herramientas].posicion.col = rand() % 30 + 0;
        if(es_valido(*juego, juego->niveles[juego->nivel_actual].herramientas[juego->niveles[juego->nivel_actual].tope_herramientas].posicion.fil, juego->niveles[juego->nivel_actual].herramientas[juego->niveles[juego->nivel_actual].tope_herramientas].posicion.col)){
            juego->niveles[juego->nivel_actual].tope_herramientas++;
        }
        else{
            i--;
        }
    }
}

void inicializar_pergamino(juego_t *juego){
    int fil_pergamino = rand() % 20 + 0;
    int col_pergamino = rand() % 30 + 0;
    while((es_camino(*juego, fil_pergamino, col_pergamino)) && !(es_homero(*juego, fil_pergamino, col_pergamino)) && !(es_runa(*juego, fil_pergamino, col_pergamino)) && !(es_altar(*juego, fil_pergamino, col_pergamino)) && !(es_pared(*juego, fil_pergamino, col_pergamino))){
        fil_pergamino = rand() % 20 + 0;
        col_pergamino = rand() % 30 + 0;
    }
    juego->niveles[juego->nivel_actual].pergamino.fil = fil_pergamino;
    juego->niveles[juego->nivel_actual].pergamino.col = col_pergamino;
}

void inicializar_piedras_castigo(juego_t *juego){
    juego->niveles[juego->nivel_actual].tope_obstaculos = 0;
    for (int i = 0; i < 10; i++) {
        juego->niveles[juego->nivel_actual].obstaculos[juego->niveles[juego->nivel_actual].tope_obstaculos].tipo = 'R';
        juego->niveles[juego->nivel_actual].obstaculos[juego->niveles[juego->nivel_actual].tope_obstaculos].posicion.fil = rand() % 20 + 0;
        juego->niveles[juego->nivel_actual].obstaculos[juego->niveles[juego->nivel_actual].tope_obstaculos].posicion.col = rand() % 30 + 0;
        if((es_valido(*juego, juego->niveles[juego->nivel_actual].obstaculos[juego->niveles[juego->nivel_actual].tope_obstaculos].posicion.fil, juego->niveles[juego->nivel_actual].obstaculos[juego->niveles[juego->nivel_actual].tope_obstaculos].posicion.col))){
            i--;
        }
        else{
            juego->niveles[juego->nivel_actual].tope_obstaculos++;
        }
    }
}

void inicializar_catapulta(juego_t *juego){
    juego->niveles[juego->nivel_actual].obstaculos[juego->niveles[juego->nivel_actual].tope_obstaculos].tipo = 'F';
    do
    {
        juego->niveles[juego->nivel_actual].obstaculos[juego->niveles[juego->nivel_actual].tope_obstaculos].posicion.fil = rand() % 20 + 0;
        juego->niveles[juego->nivel_actual].obstaculos[juego->niveles[juego->nivel_actual].tope_obstaculos].posicion.col = rand() % 30 + 0;
    } while (!(es_pared(*juego, juego->niveles[juego->nivel_actual].obstaculos[juego->niveles[juego->nivel_actual].tope_obstaculos].posicion.fil, juego->niveles[juego->nivel_actual].obstaculos[juego->niveles[juego->nivel_actual].tope_obstaculos].posicion.col)));
    juego->niveles[juego->nivel_actual].tope_obstaculos++;
    
}

void inicializar_juego(juego_t *juego){
    juego->nivel_actual = 1;
    juego->tope_niveles = 1;
    juego->camino_visible = false;
    obtener_mapa(juego->niveles[1].paredes, &juego->niveles[1].tope_paredes, juego->niveles[1].camino, &juego->niveles[1].tope_camino, 1);
    inicializar_homero(juego);
    inicializar_pergamino(juego);
    inicializar_totems(juego);
    inicializar_piedras_castigo(juego);
    inicializar_catapulta(juego);
}

void camino_visible(juego_t *juego){
    if(juego->homero.posicion.fil == juego->niveles[juego->nivel_actual].camino[0].fil && juego->homero.posicion.col == juego->niveles[juego->nivel_actual].camino[0].col){
        printf("¡Has revelado el camino! Ahora puedes ver el camino completo en el mapa.\n");
        juego->camino_visible = true;
    } 
}

void mostrar_juego(juego_t juego){
    char mapa[MAX_FILAS][MAX_COLUMNAS];

    for (int i = 0; i < MAX_FILAS; i++) {
        for (int j = 0; j < MAX_COLUMNAS; j++) {
            mapa[i][j] = ESPACIO_VACIO;
        }
    }
    for (int i = 0; i < juego.niveles[juego.nivel_actual].tope_camino; i++) {
        mapa[juego.niveles[juego.nivel_actual].camino[i].fil][juego.niveles[juego.nivel_actual].camino[i].col] = CAMINO;
    }
    for (int i = 0; i < juego.niveles[juego.nivel_actual].tope_paredes; i++) {
        mapa[juego.niveles[juego.nivel_actual].paredes[i].fil][juego.niveles[juego.nivel_actual].paredes[i].col] = PARED;
    }
    //agregar validaciones para que no se pise a homero, runa, altar, etc.

    for (int i = 0; i < juego.niveles[juego.nivel_actual].tope_herramientas; i++) {
        int fil = juego.niveles[juego.nivel_actual].herramientas[i].posicion.fil;
        int col = juego.niveles[juego.nivel_actual].herramientas[i].posicion.col;
        mapa[fil][col] = juego.niveles[juego.nivel_actual].herramientas[i].tipo;
    }

    mapa[juego.niveles[juego.nivel_actual].camino[0].fil][juego.niveles[juego.nivel_actual].camino[0].col] = 'U';

    mapa[juego.niveles[juego.nivel_actual].camino[juego.niveles[juego.nivel_actual].tope_camino - 1].fil][juego.niveles[juego.nivel_actual].camino[juego.niveles[juego.nivel_actual].tope_camino - 1].col] = 'A';

    mapa[juego.niveles[juego.nivel_actual].pergamino.fil][juego.niveles[juego.nivel_actual].pergamino.col] = 'P';

    mapa[juego.homero.posicion.fil][juego.homero.posicion.col] = HOMERO;

    for (int i = 0; i < juego.niveles[juego.nivel_actual].tope_obstaculos; i++) {
        int fil = juego.niveles[juego.nivel_actual].obstaculos[i].posicion.fil;
        int col = juego.niveles[juego.nivel_actual].obstaculos[i].posicion.col;
        mapa[fil][col] = juego.niveles[juego.nivel_actual].obstaculos[i].tipo;
    }
    camino_visible(&juego);

    if(juego.camino_visible){
        for (int i = 0; i < MAX_FILAS; i++) {
            for (int j = 0; j < MAX_COLUMNAS; j++) {
                printf("%c ", mapa[i][j]);
            }
            printf("\n");
        }
    }
}


bool movimiento_valido(juego_t juego, char movimiento){
    bool es_valido = false;

    es_valido = ((movimiento == MOVIMIENTO_ARRIBA) || (movimiento == MOVIMIENTO_IZQUIERDA) || (movimiento == MOVIMIENTO_ABAJO) || (movimiento == MOVIMIENTO_DERECHA) || (movimiento == MOVIMIENTO_USO_HECHIZO && juego.homero.hechizos_reveladores > 0) || (movimiento == MOVIMIENTO_USO_ANTORCHA && juego.homero.antorchas > 0));
    if(!es_valido){
        printf("Movimiento no valido, por favor ingrese un movimiento valido (W, A, S, D, H, L)\n");
    }
    return es_valido;
}

int posicion_totem_buscado(objeto_t herramientas[MAX_ELEMENTOS], int tope_herramientas, coordenada_t coordenada_buscada){
   int i = 0;
   bool coordenada_encontrada = false;
   int posicion_encontrada = -1;
   while(i < tope_herramientas && !coordenada_encontrada){
      if((herramientas[i].posicion.fil == coordenada_buscada.fil) && (herramientas[i].posicion.col == coordenada_buscada.col)){
        coordenada_encontrada = true;
        posicion_encontrada = i;
      }
      i++;
   }
   return posicion_encontrada;
}

void eliminar_totem(objeto_t herramientas[MAX_ELEMENTOS], int *tope_herramientas, int indice_totem){
    for (int i = indice_totem; i < (*tope_herramientas) - 1; i++) {
        herramientas[i] = herramientas[i + 1];
    }
    (*tope_herramientas)--;
}

void pre_armar_jugada(juego_t *juego, char movimiento, int *proxima_fil, int *proxima_col){    
    if(movimiento == MOVIMIENTO_ARRIBA){
        *proxima_fil += -1;
    }
    else if(movimiento == MOVIMIENTO_ABAJO){
        *proxima_fil += 1;
    }
    else if(movimiento == MOVIMIENTO_DERECHA){
        *proxima_col += 1;
    }
    else if(movimiento == MOVIMIENTO_IZQUIERDA){
        *proxima_col += -1;
    }  
}

void realizar_accion(juego_t *juego, int proxima_fil, int proxima_col){
    int indice_totem = -1;
    if(es_pared(*juego, proxima_fil, proxima_col)){
        restar_vida(juego, proxima_fil, proxima_col);
    }
    else if(es_vacio(*juego, proxima_fil, proxima_col)){
        printf("Movimiento no valido\n");
    }
    else if(posicion_en_rango(proxima_fil, proxima_col)){
        if(es_totem(*juego, proxima_fil, proxima_col)){
            sumar_vida(juego, proxima_fil, proxima_col);
            indice_totem = posicion_totem_buscado(juego->niveles[juego->nivel_actual].herramientas, juego->niveles[juego->nivel_actual].tope_herramientas, (coordenada_t){proxima_fil, proxima_col});
            eliminar_totem(juego->niveles[juego->nivel_actual].herramientas, &juego->niveles[juego->nivel_actual].tope_herramientas, indice_totem);
        }
        juego->homero.posicion.fil = proxima_fil;
        juego->homero.posicion.col = proxima_col;
    }
}

bool movimiento_uso_herramienta(juego_t juego, char movimiento){
    bool uso_herramienta = (movimiento == MOVIMIENTO_USO_HECHIZO && juego.homero.hechizos_reveladores > 0) || (movimiento == MOVIMIENTO_USO_ANTORCHA && juego.homero.antorchas > 0);
    
    return uso_herramienta;
}

void uso_herramienta(juego_t *juego, char movimiento){
    if(movimiento == MOVIMIENTO_USO_HECHIZO && juego->homero.hechizos_reveladores > 0){
        printf("¡Has usado un hechizo revelador! Ahora tienes %d hechizos.\n", juego->homero.hechizos_reveladores - 1);
        juego->homero.hechizos_reveladores--;
        juego->camino_visible = true;
    }
    else if(movimiento == MOVIMIENTO_USO_ANTORCHA && juego->homero.antorchas > 0){
        printf("¡Has usado una antorcha! Ahora tienes %d antorchas.\n", juego->homero.antorchas - 1);
        juego->homero.antorchas--;
        // Implementar lógica para mostrar paredes adyacentes
    }
}

void realizar_jugada(juego_t *juego, char movimiento){

    int proxima_fil = juego->homero.posicion.fil;
    int proxima_col = juego->homero.posicion.col;

    /* El efecto de revelado por hechizo dura solo la jugada actual. */
    juego->camino_visible = false;
    
    if(movimiento_valido(*juego, movimiento)){
        if(movimiento_uso_herramienta(*juego, movimiento)){
            uso_herramienta(juego, movimiento);
        }
        pre_armar_jugada(juego, movimiento, &proxima_fil, &proxima_col);
        realizar_accion(juego, proxima_fil, proxima_col);
    }
}

int estado_nivel(nivel_t nivel, personaje_t homero){
    int estado = 0;
    return estado;
}

int estado_juego(juego_t juego){
    int estado = 0;
    if(juego.homero.vidas_restantes == 0){
        estado = -1;
    }
    return estado;
}

