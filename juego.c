#include <time.h> 
#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include "ascension_magios.h"

//funciones a implementar

//Pre: Se debe recibir una variable char valida por referencia
//Pos: Guarda en movimiento la accion ingresada por el usuario
void pedir_movimiento(char *movimiento){
    printf("Ingrese un movimiento (W/A/S/D para moverse, H para usar hechizo, L para usar antorcha): ");
    scanf(" %c", movimiento);
}

//Pre: El juego debe estar inicializado
//Pos: Ejecuta el loop principal hasta que el juego termine
void jugar_juego(juego_t juego){
    char movimiento = 'q';
    while(estado_nivel(juego.niveles[juego.nivel_actual - 1], juego.homero) == 0 && estado_juego(juego) == 0){
        pedir_movimiento(&movimiento);
        system("clear");
        realizar_jugada(&juego, movimiento);
        mostrar_juego(juego);
    }
}

//Pre: El juego debe estar inicializado
//Pos: Muestra por pantalla si el jugador gano o perdio
void imprimir_resultado(juego_t juego){
    int estado = estado_juego(juego);
    if(estado == -1){
        printf("¡Has perdido! No te quedan vidas.\n");
    }
    else if(estado == 1){
        printf("¡Felicidades! Has ganado el juego.\n");
    }
}

//Pre: -
//Pos: Inicializa el juego, lo ejecuta y muestra el resultado final
int main(){
    srand ((unsigned)time(NULL)); 
    juego_t juego;
    inicializar_juego(&juego);
    mostrar_juego(juego);
    jugar_juego(juego);
    imprimir_resultado(juego);
    return 0;
}
