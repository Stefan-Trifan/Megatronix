/*
 MEMsym - simulador de caché simple
 Compila: gcc -std=c11 -Wall -Wextra -o MEMsym main.c
*/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#define TAM_LINEA 16
#define NUM_FILAS 8
#define MEM_SIZE 4096

typedef struct {
	unsigned char ETQ;
	unsigned char Data[TAM_LINEA];
} T_CACHE_LINE;

/* variables globales requeridas */
int globaltime = 0;
int numfallos = 0;

/* prototipos obligatorios */
void LimpiarCACHE(T_CACHE_LINE tbl[NUM_FILAS]);
void VolcarCACHE(T_CACHE_LINE *tbl);
void ParsearDireccion(unsigned int addr, int *ETQ, int *palabra, int *linea, int *bloque);
void TratarFallo(T_CACHE_LINE *tbl, unsigned char *MRAM, int ETQ, int linea, int bloque);

void LimpiarCACHE(T_CACHE_LINE tbl[NUM_FILAS]) {
	for (int i = 0; i < NUM_FILAS; ++i) {
		tbl[i].ETQ = 0xFF;
		for (int j = 0; j < TAM_LINEA; ++j) tbl[i].Data[j] = 0x23;
	}
}

void ParsearDireccion(unsigned int addr, int *ETQ, int *palabra, int *linea, int *bloque)
{
    // Aseguramos que la dirección esta en 12 bits
    addr &= 4095; // 0x0FFF en decimal

    // palabra: últimos 4 bits (bits 0-3)
    *palabra = addr & 15; // 15 = 0b1111 (4 bits encendidos)

    // línea: siguientes 3 bits (bits 4-6)
    *linea = (addr >> 4) & 7; // 7 = 0b111 (3 bits encendidos)

    // etiqueta: siguientes 5 bits (bits 7-11)
    *ETQ = (addr >> 7) & 31; // 31 = 0b11111 (5 bits encendidos)

    // bloque: dirección base del bloque (multiplo de 16)
    *bloque = addr & (~15); // ~15 = todos menos los 4 bits menos significativos
}

void TratarFallo(T_CACHE_LINE *tbl, unsigned char *MRAM, int ETQ, int linea, int bloque) {

    printf("Cargando bloque %X en línea %X\n", bloque, linea);

    for (int i = 0; i < TAM_LINEA; ++i)
        tbl[linea].Data[i] = MRAM[bloque + i];
    
    tbl[linea].ETQ = ETQ;

    printf("Bloque cargado.\n");
}

void VolcarCACHE(T_CACHE_LINE *tbl) {

    FILE *f = fopen("CONTENTS_CACHE.bin", "wb");
    if (!f) {
        printf("No se pudo crear CONTENTS_CACHE.bin\n");
        return;
    }

    for (int i = 0; i < NUM_FILAS; ++i) {
        fwrite(tbl[i].Data, 1, TAM_LINEA, f);
    }
    fclose(f);
}

void imprimirContenidoCache(T_CACHE_LINE tbl[NUM_FILAS]) {

    for (int i = 0; i < NUM_FILAS; ++i) {
        printf("Linea %X ETQ %X: ", i, tbl[i].ETQ);
        
        for (int j = TAM_LINEA - 1; j >= 0; --j)
            printf("%02X ", tbl[i].Data[j]);
        printf("\n");
    }
}
