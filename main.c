/*
    *   Autor
        Stefan Trifan

    *   Nombre del Proyecto
        Megatronix
*/

/* _________________________________________
   Inicio cabecera */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define TAM_LINEA 0
#define NUM_FILAS 0

// Variables Globales
typedef struct 
{
    unsigned char ETQ;
    unsigned char Data[TAM_LINEA];
} 
T_CACHE_LINE;

// Funciones del programa
void limpiar_cache(T_CACHE_LINE tbl[NUM_FILAS]);
void volcar_cache(T_CACHE_LINE *tbl);
void parsear_direccion(unsigned int addr, int *ETQ, int *palabra, int *linea, int *bloque);
void tratar_fallo(T_CACHE_LINE *tbl, char *MRAM, int ETQ, int linea, int bloque);

// Funciones auxiliares
void clearBuffer();

/* _________________________________________
   Inicio main() */

int main(int argc, char *argv[])
{
    printf("\n_________________________________________START\n\n");

    // Declaracion de variables


    printf("\n_________________________________________EXIT\n\n");
    return EXIT_SUCCESS;
}

/* _________________________________________
   Inicio definicion de funciones */

// Funciones del programa
/**
 * @param[in] tbl   texto
 */
void limpiar_cache(T_CACHE_LINE tbl[NUM_FILAS])
{

}

/**
 * @param[out] tbl  texto
 */
void volcar_cache(T_CACHE_LINE *tbl)
{

}

/**
 * @param[in] addr      texto   
 * @param[out] ETQ      texto    
 * @param[out] palabra  texto   
 * @param[out] linea    texto   
 * @param[out] bloque   texto 
 */
void parsear_direccion(unsigned int addr, int *ETQ, int *palabra, int *linea, int *bloque)
{

}

/**
 * @param[out] tbl      texto 
 * @param[out] MRAM     texto
 * @param[in] ETQ       texto
 * @param[in] linea     texto
 * @param[in] bloque    texto   
 */
void tratar_fallo(T_CACHE_LINE *tbl, char *MRAM, int ETQ, int linea, int bloque)
{

}


// Funciones auxiliares
void clearBuffer()
{
    while (getchar() != '\n');
}