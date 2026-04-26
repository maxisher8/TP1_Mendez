#include <time.h> 
#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include "ascension_magios.h"

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
    inicializar_juego(&juego);
    mostrar_juego(juego);
    jugar_juego(juego);
    imprimir_resultado(juego);
    return 0;
}
