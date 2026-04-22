#include <time.h> 
#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include "ascension_magios.h"

int main(){
    srand ((unsigned)time(NULL)); 
    juego_t juego;
    char movimiento = 'q';
    inicializar_juego(&juego);
    mostrar_juego(juego);
    while(estado_nivel(juego.niveles[juego.nivel_actual], juego.homero) == 0){
        if(estado_juego(juego) == -1){
            printf("¡Has perdido! No te quedan vidas.\n");
            return 0;
        }
        printf("ingrese un movimiento");
        scanf(" %c", &movimiento);
        realizar_jugada(&juego, movimiento);
        mostrar_juego(juego);
    }
    return 0;
}
