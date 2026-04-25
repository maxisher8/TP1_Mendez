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
const char PERGAMINO = 'P';
const char TOTEM = 'T';
const char PIEDRA_CASTIGO = 'R';
const char CATAPULTA = 'F';
const char RUNA = 'U';
const char ALTAR = 'A';
const int PRIMER_POSICION_COL = 0;
const int PRIMER_POSICION_FIL = 0;
const int CANTIDAD_VIDAS_INICIALES = 5;
const int CANTIDAD_HECHIZOS_INICIALES = 5;
const int CANTIDAD_ANTORCHAS_INICIALES = 5;
const int CANTIDAD_TOTEMS = 5;
const int CANTIDAD_PIEDRAS_CASTIGO = 10;
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
    for (int i = 0; i < juego.niveles[juego.nivel_actual - 1].tope_paredes; i++) {
        if (juego.niveles[juego.nivel_actual - 1].paredes[i].fil == fil && juego.niveles[juego.nivel_actual - 1].paredes[i].col == col) {
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

bool es_pergamino(juego_t juego, int fil, int col){
    bool es_pergamino = juego.niveles[juego.nivel_actual - 1].pergamino.fil == fil && juego.niveles[juego.nivel_actual - 1].pergamino.col == col;
    if(es_pergamino){
        return true;
    }
    else{
        return false;
    }
}

bool es_runa(juego_t juego, int fil, int col){
    bool es_runa = (juego.niveles[juego.nivel_actual - 1].camino[0].fil == fil) && (juego.niveles[juego.nivel_actual - 1].camino[0].col == col);
    if(es_runa){
        return true;
    }
    return false;
}

bool es_altar(juego_t juego, int fil, int col){
    bool es_altar = (juego.niveles[juego.nivel_actual - 1].camino[juego.niveles[juego.nivel_actual - 1].tope_camino - 1].fil == fil) && (juego.niveles[juego.nivel_actual - 1].camino[juego.niveles[juego.nivel_actual - 1].tope_camino - 1].col == col);
    if(es_altar){
        return true;
    }
    return false;
}

bool es_camino(juego_t juego, int fil, int col){
    for (int i = 0; i < juego.niveles[juego.nivel_actual - 1].tope_camino; i++) {
        if (juego.niveles[juego.nivel_actual - 1].camino[i].fil == fil && juego.niveles[juego.nivel_actual - 1].camino[i].col == col) {
            return true;
        }
    }
    return false;
}

bool es_totem(juego_t juego, int fil, int col){
    for (int i = 0; i < juego.niveles[juego.nivel_actual - 1].tope_herramientas; i++) {
        if (juego.niveles[juego.nivel_actual - 1].herramientas[i].posicion.fil == fil && juego.niveles[juego.nivel_actual - 1].herramientas[i].posicion.col == col && juego.niveles[juego.nivel_actual - 1].herramientas[i].tipo == TOTEM) {
            return true;
        }
    }
    return false;
}

bool es_piedra_castigo(juego_t juego, int fil, int col){
    for (int i = 0; i < juego.niveles[juego.nivel_actual - 1].tope_obstaculos; i++) {
        if (juego.niveles[juego.nivel_actual - 1].obstaculos[i].posicion.fil == fil && juego.niveles[juego.nivel_actual - 1].obstaculos[i].posicion.col == col) {
            return true;
        }
    }
    return false;
}

//CAMBIAR NOMBRE DE LA FUNCION ES_VALIDO 
bool es_valido(juego_t juego, int fil, int col){
    if(es_homero(juego, fil, col) || es_pared(juego, fil, col) || es_pergamino(juego, fil, col) || es_runa(juego, fil, col) || es_altar(juego, fil, col) || es_totem(juego, fil, col) || es_piedra_castigo(juego, fil, col)){
        return false;
    }
    return true;
}

bool es_vacio(juego_t juego, int fil, int col){
    if(!(es_pared(juego, fil, col)) && !(es_camino(juego, fil, col))){
        return true;
    }
    return false;
}

void restar_vida(juego_t *juego, int fil, int col){
    juego->homero.vidas_restantes--; 
    printf("¡Oh no! Has perdido una vida. Te quedan %d vidas.\n", juego->homero.vidas_restantes);  
}

void sumar_vida(juego_t *juego, int fil, int col){
    juego->homero.vidas_restantes++;
    printf("¡Has ganado una vida! Ahora tienes %d vidas.\n", juego->homero.vidas_restantes);   
}

//inicializaciones

void inicializar_homero(juego_t *juego){
    juego->homero.posicion.col = juego->niveles[juego->nivel_actual - 1].camino[0].col;
    juego->homero.posicion.fil = juego->niveles[juego->nivel_actual - 1].camino[0].fil;
    juego->homero.recolecto_pergamino = false;
    juego->homero.hechizos_reveladores = CANTIDAD_HECHIZOS_INICIALES;
    juego->homero.vidas_restantes = CANTIDAD_VIDAS_INICIALES;
    juego->homero.antorchas = CANTIDAD_ANTORCHAS_INICIALES;
    juego->homero.antorcha_encendida = false;
}

void inicializar_totems(juego_t *juego){
    juego->niveles[juego->nivel_actual - 1].tope_herramientas = 0;
    for (int i = 0; i < CANTIDAD_TOTEMS; i++) {
        juego->niveles[juego->nivel_actual - 1].herramientas[juego->niveles[juego->nivel_actual - 1].tope_herramientas].tipo = TOTEM;
        juego->niveles[juego->nivel_actual - 1].herramientas[juego->niveles[juego->nivel_actual - 1].tope_herramientas].posicion.fil = rand() % 20 + 0;
        juego->niveles[juego->nivel_actual - 1].herramientas[juego->niveles[juego->nivel_actual - 1].tope_herramientas].posicion.col = rand() % 30 + 0;
        if(es_valido(*juego, juego->niveles[juego->nivel_actual - 1].herramientas[juego->niveles[juego->nivel_actual - 1].tope_herramientas].posicion.fil, juego->niveles[juego->nivel_actual - 1].herramientas[juego->niveles[juego->nivel_actual - 1].tope_herramientas].posicion.col)){
            juego->niveles[juego->nivel_actual - 1].tope_herramientas++;
        }
        else{
            i--;
        }
    }
}
//pergamino puede inicializarse en espacio vacio?
void inicializar_pergamino(juego_t *juego){
    int fil_pergamino = -1;
    int col_pergamino = -1;
    do{
        fil_pergamino = rand() % 20 + 0;
        col_pergamino = rand() % 30 + 0;
    } while(!(es_camino(*juego, fil_pergamino, col_pergamino)) || es_homero(*juego, fil_pergamino, col_pergamino) || es_runa(*juego, fil_pergamino, col_pergamino) || es_altar(*juego, fil_pergamino, col_pergamino));
    juego->niveles[juego->nivel_actual - 1].pergamino.fil = fil_pergamino;
    juego->niveles[juego->nivel_actual - 1].pergamino.col = col_pergamino;
}

void inicializar_piedras_castigo(juego_t *juego){
    juego->niveles[juego->nivel_actual - 1].tope_obstaculos = 0;
    for (int i = 0; i < CANTIDAD_PIEDRAS_CASTIGO; i++) {
        juego->niveles[juego->nivel_actual - 1].obstaculos[juego->niveles[juego->nivel_actual - 1].tope_obstaculos].tipo = PIEDRA_CASTIGO;
        juego->niveles[juego->nivel_actual - 1].obstaculos[juego->niveles[juego->nivel_actual - 1].tope_obstaculos].posicion.fil = rand() % 20 + 0;
        juego->niveles[juego->nivel_actual - 1].obstaculos[juego->niveles[juego->nivel_actual - 1].tope_obstaculos].posicion.col = rand() % 30 + 0;
        if((es_valido(*juego, juego->niveles[juego->nivel_actual - 1].obstaculos[juego->niveles[juego->nivel_actual - 1].tope_obstaculos].posicion.fil, juego->niveles[juego->nivel_actual - 1].obstaculos[juego->niveles[juego->nivel_actual - 1].tope_obstaculos].posicion.col))){
            juego->niveles[juego->nivel_actual - 1].tope_obstaculos++;
        }
        else{
            i--;
        }
    }
}

void inicializar_catapulta(juego_t *juego){
    juego->niveles[juego->nivel_actual - 1].obstaculos[juego->niveles[juego->nivel_actual - 1].tope_obstaculos].tipo = CATAPULTA;
    do
    {
        juego->niveles[juego->nivel_actual - 1].obstaculos[juego->niveles[juego->nivel_actual - 1].tope_obstaculos].posicion.fil = rand() % 20 + 0;
        juego->niveles[juego->nivel_actual - 1].obstaculos[juego->niveles[juego->nivel_actual - 1].tope_obstaculos].posicion.col = rand() % 30 + 0;
    } while ((es_camino(*juego, juego->niveles[juego->nivel_actual - 1].obstaculos[juego->niveles[juego->nivel_actual - 1].tope_obstaculos].posicion.fil, juego->niveles[juego->nivel_actual - 1].obstaculos[juego->niveles[juego->nivel_actual - 1].tope_obstaculos].posicion.col)));

    juego->niveles[juego->nivel_actual - 1].tope_obstaculos++;
}

int posicion_camino_buscado(coordenada_t camino[MAX_CAMINO], int tope_camino, int fil_buscada, int col_buscada){
   int i = 0;
   bool coordenada_encontrada = false;
   int posicion_encontrada = -1;
   while(i < tope_camino && !coordenada_encontrada){
      if((camino[i].fil == fil_buscada) && (camino[i].col == col_buscada)){
        coordenada_encontrada = true;
        posicion_encontrada = i;
      }
      i++;
   }
   return posicion_encontrada;
}

void eliminar_camino(coordenada_t camino[MAX_CAMINO], int *tope_camino, int indice_camino){
    for (int i = indice_camino; i < (*tope_camino) - 1; i++) {
        camino[i] = camino[i + 1];
    }
    (*tope_camino)--;
    printf("Camino eliminado en la posición %d.\n", *tope_camino);
}

void eliminar_random(juego_t *juego){
    int fil_a_eliminar = rand() % 20 + 0;
    int col_a_eliminar = rand() % 30 + 0;
    do
    {
        fil_a_eliminar = rand() % 20 + 0;
        col_a_eliminar = rand() % 30 + 0;
    } while (es_homero(*juego, fil_a_eliminar, col_a_eliminar) || es_runa(*juego, fil_a_eliminar, col_a_eliminar) || es_altar(*juego, fil_a_eliminar, col_a_eliminar) || es_pared(*juego, fil_a_eliminar, col_a_eliminar) || es_pergamino(*juego, fil_a_eliminar, col_a_eliminar));
    if(es_camino(*juego, fil_a_eliminar, col_a_eliminar)){
        int indice_camino = posicion_camino_buscado(juego->niveles[juego->nivel_actual - 1].camino, juego->niveles[juego->nivel_actual - 1].tope_camino, fil_a_eliminar, col_a_eliminar);
        eliminar_camino(juego->niveles[juego->nivel_actual - 1].camino, &juego->niveles[juego->nivel_actual - 1].tope_camino, indice_camino);
        printf("¡La bola de fuego destruyó parte del camino en (%d, %d)!\n", fil_a_eliminar, col_a_eliminar);
    }
    else{
        printf("La bola de fuego cayó en (%d, %d) y no destruyó camino.\n", fil_a_eliminar, col_a_eliminar);
    }
}

void activar_catapulta(juego_t *juego, char movimiento){
    if(es_runa(*juego, juego->homero.posicion.fil, juego->homero.posicion.col) || movimiento == MOVIMIENTO_USO_HECHIZO){
        printf("¡Has activado la catapulta! .\n");
        eliminar_random(juego);
    }
}

void inicializar_juego(juego_t *juego){
    juego->nivel_actual = 1;
    juego->tope_niveles = MAX_NIVELES;
    juego->camino_visible = true;

    for(int nivel = 1; nivel < MAX_NIVELES; nivel++){
        juego->nivel_actual = nivel;
        obtener_mapa(juego->niveles[nivel - 1].paredes, &juego->niveles[nivel - 1].tope_paredes, juego->niveles[nivel - 1].camino, &juego->niveles[nivel - 1].tope_camino, nivel);
        inicializar_homero(juego);
        inicializar_pergamino(juego);
        inicializar_totems(juego);
        inicializar_piedras_castigo(juego);
        inicializar_catapulta(juego);
    }

    juego->nivel_actual = 1;
}

void cambiar_nivel(juego_t *juego){

    if(estado_juego(*juego) != 0 || juego->nivel_actual >= MAX_NIVELES){
        return;
    }

    juego->nivel_actual++;

    juego->homero.posicion.col = juego->niveles[juego->nivel_actual - 1].camino[0].col;
    juego->homero.posicion.fil = juego->niveles[juego->nivel_actual - 1].camino[0].fil;
    juego->homero.recolecto_pergamino = false;
    juego->homero.antorcha_encendida = false;
    juego->camino_visible = true;
}

void camino_visible(juego_t *juego){
    if(es_runa(*juego, juego->homero.posicion.fil, juego->homero.posicion.col) || es_pergamino(*juego, juego->homero.posicion.fil, juego->homero.posicion.col)){
        printf("¡Has revelado el camino! Ahora puedes ver el camino completo en el mapa.\n");
        juego->camino_visible = true;
    } 
}

void crear_mapa(char mapa[MAX_FILAS][MAX_COLUMNAS], juego_t juego){
    for (int i = 0; i < MAX_FILAS; i++) {
        for (int j = 0; j < MAX_COLUMNAS; j++) {
            mapa[i][j] = ESPACIO_VACIO;
        }
    }
    for (int i = 0; i < juego.niveles[juego.nivel_actual - 1].tope_camino; i++) {
        mapa[juego.niveles[juego.nivel_actual - 1].camino[i].fil][juego.niveles[juego.nivel_actual - 1].camino[i].col] = CAMINO;
    }
    for (int i = 0; i < juego.niveles[juego.nivel_actual - 1].tope_paredes; i++) {
        mapa[juego.niveles[juego.nivel_actual - 1].paredes[i].fil][juego.niveles[juego.nivel_actual - 1].paredes[i].col] = PARED;
    }

    for (int i = 0; i < juego.niveles[juego.nivel_actual - 1].tope_obstaculos; i++) {
        int fil = juego.niveles[juego.nivel_actual - 1].obstaculos[i].posicion.fil;
        int col = juego.niveles[juego.nivel_actual - 1].obstaculos[i].posicion.col;
        mapa[fil][col] = juego.niveles[juego.nivel_actual - 1].obstaculos[i].tipo;
    }

    for (int i = 0; i < juego.niveles[juego.nivel_actual - 1].tope_herramientas; i++) {
        int fil = juego.niveles[juego.nivel_actual - 1].herramientas[i].posicion.fil;
        int col = juego.niveles[juego.nivel_actual - 1].herramientas[i].posicion.col;
        mapa[fil][col] = juego.niveles[juego.nivel_actual - 1].herramientas[i].tipo;
    }

    mapa[juego.niveles[juego.nivel_actual - 1].camino[0].fil][juego.niveles[juego.nivel_actual - 1].camino[0].col] = RUNA;

    mapa[juego.niveles[juego.nivel_actual - 1].camino[juego.niveles[juego.nivel_actual - 1].tope_camino - 1].fil][juego.niveles[juego.nivel_actual - 1].camino[juego.niveles[juego.nivel_actual - 1].tope_camino - 1].col] = ALTAR;

    mapa[juego.niveles[juego.nivel_actual - 1].pergamino.fil][juego.niveles[juego.nivel_actual - 1].pergamino.col] = PERGAMINO;

    mapa[juego.homero.posicion.fil][juego.homero.posicion.col] = HOMERO;
}
    
void mostrar_juego(juego_t juego){
    char mapa[MAX_FILAS][MAX_COLUMNAS];

    crear_mapa(mapa, juego);

    camino_visible(&juego);

    printf("Nivel actual: %d\n", juego.nivel_actual);
    //juego.camino_visible
    if(juego.camino_visible){
        for (int i = 0; i < MAX_FILAS; i++) {
            for (int j = 0; j < MAX_COLUMNAS; j++) {
                printf("%c ", mapa[i][j]);
            }
            printf("\n");
        }
    }
    else if(juego.homero.antorcha_encendida){
        for (int i = 0; i < MAX_FILAS; i++) {
            for (int j = 0; j < MAX_COLUMNAS; j++) {
                int distancia_manhattan = abs(juego.homero.posicion.fil - i) + abs(juego.homero.posicion.col - j);
                if(distancia_manhattan <= 3){
                    printf("%c ", mapa[i][j]);
                }
                else{
                    printf("%c ", ESPACIO_VACIO);
                }
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

int posicion_totem_buscado(objeto_t herramientas[MAX_ELEMENTOS], int tope_herramientas, int fil_buscada, int col_buscada){
   int i = 0;
   bool coordenada_encontrada = false;
   int posicion_encontrada = -1;
   while(i < tope_herramientas && !coordenada_encontrada){
      if((herramientas[i].posicion.fil == fil_buscada) && (herramientas[i].posicion.col == col_buscada)){
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

int posicion_piedra_buscado(objeto_t obstaculos[MAX_ELEMENTOS], int tope_obstaculos, int fil_buscada, int col_buscada){
   int i = 0;
   bool coordenada_encontrada = false;
   int posicion_encontrada = -1;
   while(i < tope_obstaculos && !coordenada_encontrada){
      if((obstaculos[i].posicion.fil == fil_buscada) && (obstaculos[i].posicion.col == col_buscada)){
        coordenada_encontrada = true;
        posicion_encontrada = i;
      }
      i++;
   }
   return posicion_encontrada;
}

void eliminar_piedra(objeto_t obstaculos[MAX_ELEMENTOS], int *tope_obstaculos, int indice_piedra){
    for (int i = indice_piedra; i < (*tope_obstaculos) - 1; i++) {
        obstaculos[i] = obstaculos[i + 1];
    }
    (*tope_obstaculos)--;
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
    int indice_piedra = -1;
    if(es_pared(*juego, proxima_fil, proxima_col)){
        restar_vida(juego, proxima_fil, proxima_col);
        printf("Has chocado con una pared.\n");
    }
    else if(posicion_en_rango(proxima_fil, proxima_col)){
        juego->homero.posicion.fil = proxima_fil;
        juego->homero.posicion.col = proxima_col;
        if(es_vacio(*juego, proxima_fil, proxima_col)){
            restar_vida(juego, proxima_fil, proxima_col);
        }
        if(es_totem(*juego, proxima_fil, proxima_col)){
            sumar_vida(juego, proxima_fil, proxima_col);
            indice_totem = posicion_totem_buscado(juego->niveles[juego->nivel_actual - 1].herramientas, juego->niveles[juego->nivel_actual - 1].tope_herramientas, proxima_fil, proxima_col);
            eliminar_totem(juego->niveles[juego->nivel_actual - 1].herramientas, &juego->niveles[juego->nivel_actual - 1].tope_herramientas, indice_totem);
        }
        if(es_piedra_castigo(*juego, proxima_fil, proxima_col)){
            inicializar_pergamino(juego);
            printf("¡Has pisado una piedra de castigo! El pergamino cambio de posición.\n");
            juego->homero.recolecto_pergamino = false;
            indice_piedra = posicion_piedra_buscado(juego->niveles[juego->nivel_actual - 1].obstaculos, juego->niveles[juego->nivel_actual - 1].tope_obstaculos, proxima_fil, proxima_col);
            eliminar_piedra(juego->niveles[juego->nivel_actual - 1].obstaculos, &juego->niveles[juego->nivel_actual - 1].tope_obstaculos, indice_piedra);
        }
        if(es_pergamino(*juego, proxima_fil, proxima_col) && !juego->homero.recolecto_pergamino){
            printf("¡Has recolectado el pergamino! Ahora llevalo hasta el altar!.\n");
            juego->homero.recolecto_pergamino = true;
            juego->camino_visible = true;
        }
        if(juego->homero.recolecto_pergamino){
            juego->niveles[juego->nivel_actual - 1].pergamino.fil = juego->homero.posicion.fil;
            juego->niveles[juego->nivel_actual - 1].pergamino.col = juego->homero.posicion.col;
        }
        if(es_runa(*juego, proxima_fil, proxima_col)){
            juego->camino_visible = true;
            activar_catapulta(juego, 'Q');
        }
    }
    else {
        printf("Movimiento fuera de rango. Por favor, intenta un movimiento diferente.\n");
    }
}

bool movimiento_uso_herramienta(juego_t juego, char movimiento){
    bool uso_herramienta = (movimiento == MOVIMIENTO_USO_HECHIZO && juego.homero.hechizos_reveladores > 0) || (movimiento == MOVIMIENTO_USO_ANTORCHA && juego.homero.antorchas > 0);
    
    return uso_herramienta;
}

void uso_herramienta(juego_t *juego, char movimiento){
    if(es_runa(*juego, juego->homero.posicion.fil, juego->homero.posicion.col)){
        return;
    }
    if(movimiento == MOVIMIENTO_USO_HECHIZO && juego->homero.hechizos_reveladores > 0){
        printf("¡Has usado un hechizo revelador! Ahora tienes %d hechizos.\n", juego->homero.hechizos_reveladores - 1);
        juego->homero.hechizos_reveladores--;
        juego->camino_visible = true;
        activar_catapulta(juego, movimiento);
    }
    else if(movimiento == MOVIMIENTO_USO_ANTORCHA && juego->homero.antorchas > 0){
        printf("¡Has usado una antorcha! Ahora tienes %d antorchas.\n", juego->homero.antorchas - 1);
        juego->homero.antorchas--;
        juego->homero.antorcha_encendida = true;
    }
}

void realizar_jugada(juego_t *juego, char movimiento){

    int proxima_fil = juego->homero.posicion.fil;
    int proxima_col = juego->homero.posicion.col;
    
    juego->camino_visible = false;
    juego->homero.antorcha_encendida = false;
    
    if(movimiento_valido(*juego, movimiento)){
        if(movimiento_uso_herramienta(*juego, movimiento)){
            uso_herramienta(juego, movimiento);
        }
        else{
            pre_armar_jugada(juego, movimiento, &proxima_fil, &proxima_col);
            realizar_accion(juego, proxima_fil, proxima_col);
        }
    }
}

int estado_nivel(nivel_t nivel, personaje_t homero){
    int estado = 0;
    printf("Pergamino recolectado: %d\n", homero.recolecto_pergamino);
    if((homero.recolecto_pergamino) && (homero.posicion.fil == nivel.camino[nivel.tope_camino - 1].fil) && (homero.posicion.col == nivel.camino[nivel.tope_camino - 1].col)){
        printf("¡Has llegado al altar con el pergamino! Has completado el nivel.\n");
        estado = 1;
    }
    return estado;
}

int estado_juego(juego_t juego){
    int estado = 0;
    if(juego.nivel_actual == MAX_NIVELES && estado_nivel(juego.niveles[juego.nivel_actual - 1], juego.homero) == 1){
        estado = 1;
    }
    else if(juego.homero.vidas_restantes == 0){
        estado = -1;
    }
    return estado;
}

