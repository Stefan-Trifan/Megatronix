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
