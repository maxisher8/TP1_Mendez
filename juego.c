#include <time.h> 
#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include "ascension_magios.h"

int main(){
    juego_t juego;
    char movimiento = 'q';
    inicializar_juego(&juego);
    mostrar_juego(juego);
    while(estado_nivel(juego.niveles[juego.nivel_actual], juego.homero) == 0){
        printf("ingrese un movimiento");
        scanf(" %c", &movimiento);
        realizar_jugada(&juego, movimiento);
        system("clear");
        mostrar_juego(juego);
    }
    return 0;
}
