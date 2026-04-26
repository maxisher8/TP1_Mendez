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
const char MOVIMIENTO_INVALIDO = 'Q';
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
const int DISTANCIA_MANHATTAN_MAXIMA = 3;

/*------funciones a implementar------
1. mover_homero
2. realizar_efecto_totem
3. realizar_efecto_piedra
4. verificar_cambio_nivel
5. imprimir_mapa_completo
6. imprimir_mapa_antorcha
7. generar_posicion_random
8. impacto_camino_bola_de_fuego
9. detectar_tipo_entrada
*/

bool posicion_en_rango(int fil, int col){
    bool en_rango = (fil >= PRIMER_POSICION_FIL && fil < MAX_FILAS && col >= PRIMER_POSICION_COL && col < MAX_COLUMNAS);
    return en_rango;
}


bool es_pared(coordenada_t paredes[MAX_PAREDES], int tope_paredes, int fil, int col){
    int i = 0;
    bool es_posicion_igual = false;
    while((i < tope_paredes) && !(es_posicion_igual)){
        if((paredes[i].fil == fil) && (paredes[i].col == col)){
            es_posicion_igual = true;
        }
        else{
            i++;
        }
    }
    return es_posicion_igual;
}

bool es_homero(personaje_t homero, int fil, int col){
    bool es_homero = homero.posicion.fil == fil && homero.posicion.col == col;
    if(es_homero){
        return true;
    }
    else{
        return false;
    }
}

bool es_pergamino(coordenada_t pergamino, int fil, int col){
    bool es_pergamino = pergamino.fil == fil && pergamino.col == col;
    if(es_pergamino){
        return true;
    }
    else{
        return false;
    }
}

bool es_runa(coordenada_t camino[MAX_CAMINO], int fil, int col){
    bool es_runa = (camino[0].fil == fil) && (camino[0].col == col);
    if(es_runa){
        return true;
    }
    return false;
}

bool es_altar(coordenada_t camino[MAX_CAMINO], int tope_camino, int fil, int col){
    bool es_altar = (camino[tope_camino - 1].fil == fil) && (camino[tope_camino - 1].col == col);
    if(es_altar){
        return true;
    }
    return false;
}

bool es_camino(coordenada_t camino[MAX_CAMINO], int tope_camino, int fil, int col){
    int i = 0;
    bool es_posicion_igual = false;
    while((i < tope_camino) && !(es_posicion_igual)){
        if((camino[i].fil == fil) && (camino[i].col == col)){
            es_posicion_igual = true;
        }
        else{
            i++;
        }
    }
    return es_posicion_igual;
}

bool es_totem(objeto_t herramientas[MAX_ELEMENTOS], int tope_herramientas, int fil, int col){
    int i = 0;
    bool es_posicion_igual = false;
    while((i < tope_herramientas) && !(es_posicion_igual)){
        if((herramientas[i].posicion.fil == fil) && (herramientas[i].posicion.col == col) && (herramientas[i].tipo == TOTEM)){
            es_posicion_igual = true;
        }
        else{
            i++;
        }
    }
    return es_posicion_igual;
}

bool es_piedra_castigo(objeto_t obstaculos[MAX_ELEMENTOS], int tope_obstaculos, int fil, int col){
    int i = 0;
    bool es_posicion_igual = false;
    while((i < tope_obstaculos) && !(es_posicion_igual)){
        if((obstaculos[i].posicion.fil == fil) && (obstaculos[i].posicion.col == col) && (obstaculos[i].tipo == PIEDRA_CASTIGO)){
            es_posicion_igual = true;
        }
        else{
            i++;
        }
    }
    return es_posicion_igual;
}

//CAMBIAR NOMBRE DE LA FUNCION ES_VALIDO 
bool es_valido(nivel_t nivel, personaje_t homero, int fil, int col){
    if((es_homero(homero, fil, col)) || (es_pared(nivel.paredes, nivel.tope_paredes, fil, col)) || (es_pergamino(nivel.pergamino, fil, col)) || (es_runa(nivel.camino, fil, col)) || (es_altar(nivel.camino, nivel.tope_camino, fil, col)) || (es_totem(nivel.herramientas, nivel.tope_herramientas, fil, col))){
        return false;
    }
    return true;
}

bool es_vacio(coordenada_t paredes[MAX_PAREDES], int tope_paredes, coordenada_t camino[MAX_CAMINO], int tope_camino, int fil, int col){
    if(!(es_pared(paredes, tope_paredes, fil, col)) && !(es_camino(camino, tope_camino, fil, col))){
        return true;
    }
    return false;
}

void restar_vida(personaje_t *homero){
    homero->vidas_restantes -= 1; 
    printf("¡Oh no! Has perdido una vida. Te quedan %d vidas.\n", homero->vidas_restantes);  
}

void sumar_vida(personaje_t *homero){
    homero->vidas_restantes += 1;
    printf("¡Has ganado una vida! Ahora tienes %d vidas.\n", homero->vidas_restantes);   
}

void inicializar_homero(juego_t *juego){
    juego->homero.posicion.col = juego->niveles[juego->nivel_actual - 1].camino[0].col;
    juego->homero.posicion.fil = juego->niveles[juego->nivel_actual - 1].camino[0].fil;
    juego->homero.recolecto_pergamino = false;
    juego->homero.hechizos_reveladores = CANTIDAD_HECHIZOS_INICIALES;
    juego->homero.vidas_restantes = CANTIDAD_VIDAS_INICIALES;
    juego->homero.antorchas = CANTIDAD_ANTORCHAS_INICIALES;
    juego->homero.antorcha_encendida = false;
}

void generar_posicion_random(coordenada_t *posicion){
    posicion->fil = rand() % MAX_FILAS;
    posicion->col = rand() % MAX_COLUMNAS;
}

void inicializar_totems(juego_t *juego){
    juego->niveles[juego->nivel_actual - 1].tope_herramientas = 0;
    for (int i = 0; i < CANTIDAD_TOTEMS; i++) {
        juego->niveles[juego->nivel_actual - 1].herramientas[juego->niveles[juego->nivel_actual - 1].tope_herramientas].tipo = TOTEM;
        juego->niveles[juego->nivel_actual - 1].herramientas[juego->niveles[juego->nivel_actual - 1].tope_herramientas].posicion.fil = rand() % MAX_FILAS;
        juego->niveles[juego->nivel_actual - 1].herramientas[juego->niveles[juego->nivel_actual - 1].tope_herramientas].posicion.col = rand() % MAX_COLUMNAS;
        if(es_valido(juego->niveles[juego->nivel_actual - 1], juego->homero, juego->niveles[juego->nivel_actual - 1].herramientas[juego->niveles[juego->nivel_actual - 1].tope_herramientas].posicion.fil, juego->niveles[juego->nivel_actual - 1].herramientas[juego->niveles[juego->nivel_actual - 1].tope_herramientas].posicion.col)){
            juego->niveles[juego->nivel_actual - 1].tope_herramientas += 1;
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
        fil_pergamino = rand() % MAX_FILAS;
        col_pergamino = rand() % MAX_COLUMNAS;
    } while((!(es_camino(juego->niveles[juego->nivel_actual - 1].camino, juego->niveles[juego->nivel_actual - 1].tope_camino, fil_pergamino, col_pergamino))) || (es_homero(juego->homero, fil_pergamino, col_pergamino)) || (es_runa(juego->niveles[juego->nivel_actual - 1].camino, fil_pergamino, col_pergamino)) || (es_altar(juego->niveles[juego->nivel_actual - 1].camino, juego->niveles[juego->nivel_actual - 1].tope_camino, fil_pergamino, col_pergamino)));
    juego->niveles[juego->nivel_actual - 1].pergamino.fil = fil_pergamino;
    juego->niveles[juego->nivel_actual - 1].pergamino.col = col_pergamino;
}

void insertar_totem_valido(){
    
}

void inicializar_piedras_castigo(juego_t *juego){
    juego->niveles[juego->nivel_actual - 1].tope_obstaculos = 0;
    for (int i = 0; i < CANTIDAD_PIEDRAS_CASTIGO; i++) {
        juego->niveles[juego->nivel_actual - 1].obstaculos[juego->niveles[juego->nivel_actual - 1].tope_obstaculos].tipo = PIEDRA_CASTIGO;
        juego->niveles[juego->nivel_actual - 1].obstaculos[juego->niveles[juego->nivel_actual - 1].tope_obstaculos].posicion.fil = rand() % MAX_FILAS;
        juego->niveles[juego->nivel_actual - 1].obstaculos[juego->niveles[juego->nivel_actual - 1].tope_obstaculos].posicion.col = rand() % MAX_COLUMNAS;
        if((es_valido(juego->niveles[juego->nivel_actual - 1], juego->homero, juego->niveles[juego->nivel_actual - 1].obstaculos[juego->niveles[juego->nivel_actual - 1].tope_obstaculos].posicion.fil, juego->niveles[juego->nivel_actual - 1].obstaculos[juego->niveles[juego->nivel_actual - 1].tope_obstaculos].posicion.col))){
            juego->niveles[juego->nivel_actual - 1].tope_obstaculos += 1;
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
        generar_posicion_random(&(juego->niveles[juego->nivel_actual - 1].obstaculos[juego->niveles[juego->nivel_actual - 1].tope_obstaculos].posicion));

    } while ((es_camino(juego->niveles[juego->nivel_actual - 1].camino, juego->niveles[juego->nivel_actual - 1].tope_camino, juego->niveles[juego->nivel_actual - 1].obstaculos[juego->niveles[juego->nivel_actual - 1].tope_obstaculos].posicion.fil, juego->niveles[juego->nivel_actual - 1].obstaculos[juego->niveles[juego->nivel_actual - 1].tope_obstaculos].posicion.col)) || !(es_valido(juego->niveles[juego->nivel_actual - 1], juego->homero, juego->niveles[juego->nivel_actual - 1].obstaculos[juego->niveles[juego->nivel_actual - 1].tope_obstaculos].posicion.fil, juego->niveles[juego->nivel_actual - 1].obstaculos[juego->niveles[juego->nivel_actual - 1].tope_obstaculos].posicion.col)));

    juego->niveles[juego->nivel_actual - 1].tope_obstaculos += 1;
}

int posicion_camino_buscado(coordenada_t camino[MAX_CAMINO], int tope_camino, int fil_buscada, int col_buscada){
   int i = 0;
   bool coordenada_encontrada = false;
   int posicion_encontrada = -1;
    while((i < tope_camino) && !(coordenada_encontrada)){
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

void impacto_camino_bola_de_fuego(juego_t *juego, coordenada_t posicion_a_eliminar){

    int indice_camino = posicion_camino_buscado(juego->niveles[juego->nivel_actual - 1].camino, juego->niveles[juego->nivel_actual - 1].tope_camino, posicion_a_eliminar.fil, posicion_a_eliminar.col);
    eliminar_camino(juego->niveles[juego->nivel_actual - 1].camino, &juego->niveles[juego->nivel_actual - 1].tope_camino, indice_camino);
    printf("¡La bola de fuego destruyó parte del camino en (%d, %d)!\n", posicion_a_eliminar.fil, posicion_a_eliminar.col);
}

void eliminar_random(juego_t *juego){
    int fil_a_eliminar = rand() % MAX_FILAS;
    int col_a_eliminar = rand() % MAX_COLUMNAS;
    do
    {
        fil_a_eliminar = rand() % MAX_FILAS;
        col_a_eliminar = rand() % MAX_COLUMNAS;
    } while (es_homero(juego->homero, fil_a_eliminar, col_a_eliminar) || es_runa(juego->niveles[juego->nivel_actual - 1].camino, fil_a_eliminar, col_a_eliminar) || es_altar(juego->niveles[juego->nivel_actual - 1].camino, juego->niveles[juego->nivel_actual - 1].tope_camino, fil_a_eliminar, col_a_eliminar) || es_pared(juego->niveles[juego->nivel_actual - 1].paredes, juego->niveles[juego->nivel_actual - 1].tope_paredes, fil_a_eliminar, col_a_eliminar) || es_pergamino(juego->niveles[juego->nivel_actual - 1].pergamino, fil_a_eliminar, col_a_eliminar));
    if(es_camino(juego->niveles[juego->nivel_actual - 1].camino, juego->niveles[juego->nivel_actual - 1].tope_camino, fil_a_eliminar, col_a_eliminar)){
        int indice_camino = posicion_camino_buscado(juego->niveles[juego->nivel_actual - 1].camino, juego->niveles[juego->nivel_actual - 1].tope_camino, fil_a_eliminar, col_a_eliminar);
        eliminar_camino(juego->niveles[juego->nivel_actual - 1].camino, &juego->niveles[juego->nivel_actual - 1].tope_camino, indice_camino);
        printf("¡La bola de fuego destruyó parte del camino en (%d, %d)!\n", fil_a_eliminar, col_a_eliminar);
    }
    else{
        printf("La bola de fuego cayó en (%d, %d) y no destruyó camino.\n", fil_a_eliminar, col_a_eliminar);
    }
}

void activar_catapulta(juego_t *juego, char movimiento){
    if((es_runa(juego->niveles[juego->nivel_actual - 1].camino, juego->homero.posicion.fil, juego->homero.posicion.col)) || (movimiento == MOVIMIENTO_USO_HECHIZO)){
        printf("¡Has activado la catapulta! .\n");
        eliminar_random(juego);
    }
}

void inicializar_juego(juego_t *juego){
    juego->nivel_actual = 1;
    juego->tope_niveles = MAX_NIVELES;
    juego->camino_visible = true;

    for(int nivel = 1; nivel <= MAX_NIVELES; nivel++){
        juego->nivel_actual = nivel;
        obtener_mapa(juego->niveles[nivel - 1].paredes, &juego->niveles[nivel - 1].tope_paredes, juego->niveles[nivel - 1].camino, &juego->niveles[nivel - 1].tope_camino, nivel);
        inicializar_homero(juego);
        inicializar_pergamino(juego);
        inicializar_totems(juego);
        inicializar_piedras_castigo(juego);
        inicializar_catapulta(juego);
    }
    
    juego->nivel_actual = 1;
    inicializar_homero(juego);
}

void cambiar_nivel(juego_t *juego){

    if((estado_juego(*juego) != 0) || (juego->nivel_actual >= MAX_NIVELES)){
        return;
    }
    juego->homero.recolecto_pergamino = false;
    juego->nivel_actual += 1;
    juego->homero.antorchas = CANTIDAD_ANTORCHAS_INICIALES;
    juego->homero.antorcha_encendida = false;
    juego->homero.posicion.fil = juego->niveles[juego->nivel_actual - 1].camino[0].fil;
    juego->homero.posicion.col = juego->niveles[juego->nivel_actual - 1].camino[0].col;
    juego->camino_visible = true;
}

void camino_visible(juego_t *juego){
    if((es_runa(juego->niveles[juego->nivel_actual - 1].camino, juego->homero.posicion.fil, juego->homero.posicion.col)) || (es_pergamino(juego->niveles[juego->nivel_actual - 1].pergamino, juego->homero.posicion.fil, juego->homero.posicion.col))){
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

void imprimir_mapa_completo(char mapa[MAX_FILAS][MAX_COLUMNAS]){
    for (int i = 0; i < MAX_FILAS; i++) {
        for (int j = 0; j < MAX_COLUMNAS; j++) {
            printf("%c ", mapa[i][j]);
        }
        printf("\n");
    }
}

void imprimir_mapa_antorcha(char mapa[MAX_FILAS][MAX_COLUMNAS], coordenada_t posicion_homero){
    for (int i = 0; i < MAX_FILAS; i++) {
        for (int j = 0; j < MAX_COLUMNAS; j++) {
            int distancia_manhattan = abs(posicion_homero.fil - i) + abs(posicion_homero.col - j);
            if(distancia_manhattan <= DISTANCIA_MANHATTAN_MAXIMA){
                printf("%c ", mapa[i][j]);
            }
            else{
                printf("%c ", ESPACIO_VACIO);
            }
        }
        printf("\n");
    }
}
    
void mostrar_juego(juego_t juego){
    char mapa[MAX_FILAS][MAX_COLUMNAS];

    crear_mapa(mapa, juego);

    camino_visible(&juego);
    
    if(juego.camino_visible){
        printf("\n        ------------Nivel actual: %d ------------\n \n", juego.nivel_actual);
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

    es_valido = ((movimiento == MOVIMIENTO_ARRIBA) || (movimiento == MOVIMIENTO_IZQUIERDA) || (movimiento == MOVIMIENTO_ABAJO) || (movimiento == MOVIMIENTO_DERECHA) || ((movimiento == MOVIMIENTO_USO_HECHIZO) && (juego.homero.hechizos_reveladores > 0)) || ((movimiento == MOVIMIENTO_USO_ANTORCHA) && (juego.homero.antorchas > 0)));
    if(!es_valido){
        printf("Movimiento no valido, por favor ingrese un movimiento valido (W, A, S, D, H, L)\n");
    }
    return es_valido;
}

int posicion_totem_buscado(objeto_t herramientas[MAX_ELEMENTOS], int tope_herramientas, int fil_buscada, int col_buscada){
   int i = 0;
   bool coordenada_encontrada = false;
   int posicion_encontrada = -1;
    while((i < tope_herramientas) && !(coordenada_encontrada)){
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
    while((i < tope_obstaculos) && !(coordenada_encontrada)){
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

void pre_armar_jugada(char movimiento, int *proxima_fil, int *proxima_col){    
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

void mover_homero(juego_t *juego, int proxima_fil, int proxima_col){
    juego->homero.posicion.fil = proxima_fil;
    juego->homero.posicion.col = proxima_col;
}

void realizar_efecto_totem(juego_t *juego, int proxima_fil, int proxima_col){
    int indice_totem = -1;
    sumar_vida(&juego->homero);
    indice_totem = posicion_totem_buscado(juego->niveles[juego->nivel_actual - 1].herramientas, juego->niveles[juego->nivel_actual - 1].tope_herramientas, proxima_fil, proxima_col);
    eliminar_totem(juego->niveles[juego->nivel_actual - 1].herramientas, &juego->niveles[juego->nivel_actual - 1].tope_herramientas, indice_totem);
}

void realizar_efecto_piedra(juego_t *juego, int proxima_fil, int proxima_col){
    int indice_piedra = -1;
    inicializar_pergamino(juego);
    printf("¡Has pisado una piedra de castigo! El pergamino cambio de posición.\n");
    juego->homero.recolecto_pergamino = false;
    indice_piedra = posicion_piedra_buscado(juego->niveles[juego->nivel_actual - 1].obstaculos, juego->niveles[juego->nivel_actual - 1].tope_obstaculos, proxima_fil, proxima_col);
    eliminar_piedra(juego->niveles[juego->nivel_actual - 1].obstaculos, &juego->niveles[juego->nivel_actual - 1].tope_obstaculos, indice_piedra);
}

void verificar_cambio_nivel(juego_t *juego){
    int estado = 0;
    estado = estado_nivel(juego->niveles[juego->nivel_actual - 1], juego->homero);
    if(estado == 1){
        cambiar_nivel(juego);
    }
}

void realizar_accion(juego_t *juego, int proxima_fil, int proxima_col){
    int indice_totem = -1;
    int indice_piedra = -1;
    int estado = 0;
    if(es_pared(juego->niveles[juego->nivel_actual - 1].paredes, juego->niveles[juego->nivel_actual - 1].tope_paredes, proxima_fil, proxima_col)){
        //restar_vida(juego);
        printf("Has chocado con una pared.\n");
    }
    else if(posicion_en_rango(proxima_fil, proxima_col)){
        juego->homero.posicion.fil = proxima_fil;
        juego->homero.posicion.col = proxima_col;
        if(es_vacio(juego->niveles[juego->nivel_actual - 1].paredes, juego->niveles[juego->nivel_actual - 1].tope_paredes, juego->niveles[juego->nivel_actual - 1].camino, juego->niveles[juego->nivel_actual - 1].tope_camino, proxima_fil, proxima_col)){
            restar_vida(&juego->homero);
        }
        if(es_totem(juego->niveles[juego->nivel_actual - 1].herramientas, juego->niveles[juego->nivel_actual - 1].tope_herramientas, proxima_fil, proxima_col)){
            sumar_vida(&juego->homero);
            indice_totem = posicion_totem_buscado(juego->niveles[juego->nivel_actual - 1].herramientas, juego->niveles[juego->nivel_actual - 1].tope_herramientas, proxima_fil, proxima_col);
            eliminar_totem(juego->niveles[juego->nivel_actual - 1].herramientas, &juego->niveles[juego->nivel_actual - 1].tope_herramientas, indice_totem);
        }
        if(es_piedra_castigo(juego->niveles[juego->nivel_actual - 1].obstaculos, juego->niveles[juego->nivel_actual - 1].tope_obstaculos, proxima_fil, proxima_col)){
            inicializar_pergamino(juego);
            printf("¡Has pisado una piedra de castigo! El pergamino cambio de posición.\n");
            juego->homero.recolecto_pergamino = false;
            indice_piedra = posicion_piedra_buscado(juego->niveles[juego->nivel_actual - 1].obstaculos, juego->niveles[juego->nivel_actual - 1].tope_obstaculos, proxima_fil, proxima_col);
            eliminar_piedra(juego->niveles[juego->nivel_actual - 1].obstaculos, &juego->niveles[juego->nivel_actual - 1].tope_obstaculos, indice_piedra);
        }
        if(es_pergamino(juego->niveles[juego->nivel_actual - 1].pergamino, proxima_fil, proxima_col)){
            printf("¡Has recolectado el pergamino! Ahora llevalo hasta el altar!.\n");
            juego->homero.recolecto_pergamino = true;
            juego->camino_visible = true;
        }
        if(juego->homero.recolecto_pergamino){
            juego->niveles[juego->nivel_actual - 1].pergamino.fil = juego->homero.posicion.fil;
            juego->niveles[juego->nivel_actual - 1].pergamino.col = juego->homero.posicion.col;
        }
        if(es_runa(juego->niveles[juego->nivel_actual - 1].camino, proxima_fil, proxima_col)){
            juego->camino_visible = true;
            activar_catapulta(juego, MOVIMIENTO_INVALIDO);
        }
        estado = estado_nivel(juego->niveles[juego->nivel_actual - 1], juego->homero);
        if(estado == 1){
            cambiar_nivel(juego);
        }
    }
    else {
        printf("Movimiento fuera de rango. Por favor, intenta un movimiento diferente.\n");
    }
}


bool es_movimiento_herramienta(personaje_t homero, coordenada_t camino[MAX_CAMINO], int tope_camino, char movimiento){
    bool uso_herramienta = (((movimiento == MOVIMIENTO_USO_HECHIZO) && (homero.hechizos_reveladores > 0)) || ((movimiento == MOVIMIENTO_USO_ANTORCHA) && (homero.antorchas > 0))) && !(es_runa(camino, homero.posicion.fil, homero.posicion.col));
    
    return uso_herramienta;
}

bool es_movimiento_direccion(char movimiento){
    return (movimiento == MOVIMIENTO_ARRIBA || movimiento == MOVIMIENTO_ABAJO || movimiento == MOVIMIENTO_IZQUIERDA || movimiento == MOVIMIENTO_DERECHA);
}

void uso_herramienta(juego_t *juego, char movimiento){
    if((movimiento == MOVIMIENTO_USO_HECHIZO) && (juego->homero.hechizos_reveladores > 0)){
        printf("¡Has usado un hechizo revelador! Ahora tienes %d hechizos.\n", juego->homero.hechizos_reveladores - 1);
        juego->homero.hechizos_reveladores--;
        juego->camino_visible = true;
        activar_catapulta(juego, movimiento);
    }
    else if((movimiento == MOVIMIENTO_USO_ANTORCHA) && (juego->homero.antorchas > 0)){
        printf("¡Has usado una antorcha! Ahora tienes %d antorchas.\n", juego->homero.antorchas - 1);
        juego->homero.antorchas--;
        juego->homero.antorcha_encendida = true;
    }
}

void detectar_tipo_entrada(juego_t *juego, char movimiento, int proxima_fil, int proxima_col){
    if(es_movimiento_herramienta(juego->homero, juego->niveles[juego->nivel_actual - 1].camino, juego->niveles[juego->nivel_actual - 1].tope_camino, movimiento)){
        uso_herramienta(juego, movimiento);
    }
    else if(es_movimiento_direccion(movimiento)){
        juego->homero.antorcha_encendida = false;
        pre_armar_jugada(movimiento, &proxima_fil, &proxima_col);
        realizar_accion(juego, proxima_fil, proxima_col);
    }
    else{
        printf("No podes usar herramientas estando sobre la runa.\n");
    }
    if(es_runa(juego->niveles[juego->nivel_actual - 1].camino, juego->homero.posicion.fil, juego->homero.posicion.col)){
        juego->camino_visible = true;
    }
}

void realizar_jugada(juego_t *juego, char movimiento){

    int proxima_fil = juego->homero.posicion.fil;
    int proxima_col = juego->homero.posicion.col;

    juego->camino_visible = false;
    if(es_movimiento_direccion(movimiento)){
        juego->homero.antorcha_encendida = false;
    }
    
    if(movimiento_valido(*juego, movimiento)){
        if(es_movimiento_herramienta(juego->homero, juego->niveles[juego->nivel_actual - 1].camino, juego->niveles[juego->nivel_actual - 1].tope_camino, movimiento)){
            uso_herramienta(juego, movimiento);
        }
        else if(es_movimiento_direccion(movimiento)){
            pre_armar_jugada(movimiento, &proxima_fil, &proxima_col);
            realizar_accion(juego, proxima_fil, proxima_col);
        }
        else{
            printf("No podes usar herramientas estando sobre la runa.\n");
        }

        if(es_runa(juego->niveles[juego->nivel_actual - 1].camino, juego->homero.posicion.fil, juego->homero.posicion.col)){
            juego->camino_visible = true;
        }
    }
}

int estado_nivel(nivel_t nivel, personaje_t homero){
    int estado = 0;
    if((homero.recolecto_pergamino) && (homero.posicion.fil == nivel.camino[nivel.tope_camino - 1].fil) && (homero.posicion.col == nivel.camino[nivel.tope_camino - 1].col)){
        printf("¡Has llegado al altar con el pergamino! Has completado el nivel.\n");
        estado = 1;
    }
    return estado;
}

int estado_juego(juego_t juego){
    int estado = 0;
    if((juego.nivel_actual == MAX_NIVELES) && (estado_nivel(juego.niveles[juego.nivel_actual - 1], juego.homero) == 1)){
        estado = 1;
    }
    else if(juego.homero.vidas_restantes == 0){
        estado = -1;
    }
    return estado;
}
