#include <time.h> 
#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include "ascension_magios.h"

//funciones a implementar

void pedir_movimiento(char *movimiento){
    printf("Ingrese un movimiento (W/A/S/D para moverse, H para usar hechizo, L para usar antorcha): ");
    scanf(" %c", movimiento);
}

void jugar_juego(juego_t juego){
    char movimiento = 'q';
    while(estado_nivel(juego.niveles[juego.nivel_actual - 1], juego.homero) == 0 && estado_juego(juego) == 0){
        pedir_movimiento(&movimiento);
        system("clear");
        realizar_jugada(&juego, movimiento);
        mostrar_juego(juego);
    }
}

void imprimir_resultado(juego_t juego){
    int estado = estado_juego(juego);
    if(estado == -1){
        printf("¡Has perdido! No te quedan vidas.\n");
    }
    else if(estado == 1){
        printf("¡Felicidades! Has ganado el juego.\n");
    }
}

int main(){
    srand ((unsigned)time(NULL)); 
    juego_t juego;
    char movimiento = 'q';
    inicializar_juego(&juego);
    mostrar_juego(juego);
    while(estado_nivel(juego.niveles[juego.nivel_actual - 1], juego.homero) == 0 && estado_juego(juego) == 0){
        printf("ingrese un movimiento");
        scanf(" %c", &movimiento);
        realizar_jugada(&juego, movimiento);
        mostrar_juego(juego);
    }
    if(estado_juego(juego) == -1){
        printf("¡Has perdido! No te quedan vidas.\n");
        return 0;
    }
    else if(estado_juego(juego) == 1){
        printf("¡Felicidades! Has ganado el juego.\n");
        return 0;
    }
    return 0;
}
