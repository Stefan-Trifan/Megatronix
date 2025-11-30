/*
 MEMsym - simulador de caché simple
 Compila: gcc -std=c11 -Wall -Wextra -o MEMsym main.c
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
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
    // Aseguramos que la dirección está en 12 bits
    addr &= 4095; // 0x0FFF en decimal

    // palabra: últimos 4 bits (bits 0-3)
    *palabra = addr & 15; // 15 = 0b1111 (4 bits encendidos)

    // línea: siguientes 3 bits (bits 4-6)
    *linea = (addr >> 4) & 7; // 7 = 0b111 (3 bits encendidos)

    // etiqueta: siguientes 5 bits (bits 7-11)
    *ETQ = (addr >> 7) & 31; // 31 = 0b11111 (5 bits encendidos)

    // bloque: dirección base del bloque (múltiplo de 16)
    *bloque = addr & (~15); // ~15 = todos menos los 4 bits menos significativos
}
