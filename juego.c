#include <time.h> 
#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include "ascension_magios.h"

int main(){
    juego_t juego;
    inicializar_juego(&juego);
    mostrar_juego(juego);
    return 0;
}