/*
    *   Autor
        Stefan Trifan

    *   Nombre del Proyecto
        Megatronix

    *   Configuración del Proyecto
        Correspondencia Directa

        Bus de memoria: 12 bits 
            +-------------+----------+------------+
            |  0 0 1 0 0  |  0 1 0   |  1 1 1 0   |
            +-------------+----------+------------+
            | Etiqueta(5) | Linea(3) | Palabra(4) |
            +-------------+----------+------------+

        Caché:  
            - tam_cache  = 128 (B)
            - tam_linea  = 16 (B)
            - num_lineas = 8

        RAM
            - tam_ram     = 4096 (B)
            - tam_bloque  = 16 (B)
            - num_bloques = 256
*/

/* _________________________________________
   Inicio cabecera */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define TAM_LINEA 16
#define NUM_FILAS 8

// Variables Globales

// Linea de caché
typedef struct 
{
    unsigned char ETQ; // Guarda el valor decimal de la etiqueta
    unsigned char Data[TAM_LINEA]; // Guarda las 16 Bytes/palabras
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

    // todo comprobar que accesos_memoria.txt existe o devolver EXIT_FAILURE
    // todo comprobar que CONTENTS_RAM.bin existe o devolver EXIT_FAILURE
    // todo declaramos fld_accesos_memoria 
    // todo declaramos fld_contents_ram

    // todo Declaracion de variables
    T_CACHE_LINE simul_cache[NUM_FILAS];
    char simul_ram[4096];
    char texto[100];
    int globaltime = 0;
    int numfallos = 0;

    // todo limpiar_cache(T_CACHE_LINE tbl[NUM_FILAS]);

    // todo simul_ram = contenido de CONTENTS_RAM.bin
    
    // todo leemos las direcciones de memoria de accesos_memoria.txt, de una en una
    // todo while(fld_accesos_memoria != '\0)
        // todo int addr = 0
        // todo int *ETQ
        // todo int *palabra
        // todo int *linea
        // todo int *bloque

        // todo guardamos la direccion de memoria actual en la variable addr 
        
        // todo parsear_direccion(unsigned int addr, int *ETQ, int *palabra, int *linea, int *bloque)

        // todo comprueba si ETQ actual es igual a simul_cache[linea].ETQ    

            // todo Si no coincide:  
                // todo ++numfallos
                // todo void tratar_fallo(T_CACHE_LINE *tbl, char *MRAM, int ETQ, int linea, int bloque)
                // todo globaltime += 20
            
            // todo si coincide:
                // todo imprimimos “T: %d, Acierto de CACHE, ADDR %04X Label %X linea %02X palabra %02X DATO %02X”. 
                // todo Cada carácter leído se añade a la variable llamada texto

            // todo El proceso vuelca el contenido de la caché por pantalla con el formato indicado
            // todo Los datos se imprimen de izquierda a derecha de mayor a menor peso. Esto significa que el byte situado más a la izquierda es el byte 15 de la línea y el situado a la derecha el byte 0.

    // todo sleep() de 1 segundo.

    // todo imprimimos el número total de accesos, número de fallos y tiempo medio de acceso.
    // todo imprimimos el texto leído carácter a carácter desde la caché.

    // todo VolcarCACHE(T_CACHE_LINE *tbl);

    printf("\n_________________________________________EXIT\n\n");
    return EXIT_SUCCESS;
}

/* _________________________________________
   Inicio definicion de funciones */

// Funciones del programa
/**
 * @brief   inicializamos los valores ETQ y data
 * 
 * @param[out] tbl   Contenedor de las líneas y etiquetas de cache 
 */
void limpiar_cache(T_CACHE_LINE tbl[NUM_FILAS])
{
    // todo inicializamos los campos ETQ a FF o 1111 1111
    // todo inicializamos los campos data a 23 o 0010 0011
}

/**
 * @brief descompone la direccion de memoria en etiqueta, palabra, linea, bloque
 * 
 * @param[in] addr      Dirección de memoria actual   
 * @param[out] ETQ          
 * @param[out] palabra     
 * @param[out] linea       
 * @param[out] bloque    
 */
void parsear_direccion(unsigned int addr, int *ETQ, int *palabra, int *linea, int *bloque)
{
    // todo Obtiene el número de línea de la direccion
}

/**
 * @param[in,out] tbl  
 * @param[in,out] MRAM  Simulador de memoria RAM
 * @param[in] ETQ       
 * @param[in] linea     
 * @param[in] bloque       
 */
void tratar_fallo(T_CACHE_LINE *tbl, char *MRAM, int ETQ, int linea, int bloque)
{
    // todo imprimimos: “T: %d, Fallo de CACHE %d, ADDR %04X, etq %X, linea %02X, palabra %02X, bloque %02X”. 
        // todo   printf            Descripcion         Variable
        // todo T: %d               Instante del fallo  globaltime
        // todo Fallo de CACHE %d   Contador de fallos  ??????????????   
        // todo ADDR %04X           Direccion Actual    ??????????????
        // todo etq %X              Etiqueta            ETQ
        // todo linea %02X          Linea               linea
        // todo palabra %02X        Palabra             ??????????????
        // todo bloque %02X         Bloque              bloque
        
    // todo Se copia el bloque correspondiente desde el array RAM y se imprime un mensaje indicando que se está cargando el bloque X en la línea Y
    // todo Se actualizan tanto el campo Label como los 8 bytes de datos de la línea.
}

/**
 * @param[in,out] tbl  
 */
void volcar_cache(T_CACHE_LINE *tbl)
{
    // todo Volcamos los contenidos de los 128 bytes de información (8 líneas de 16 bytes cada una) de la caché en CONTENTS_CACHE.bin. 
    // todo El byte 0 de ese fichero es el byte 0 de la línea 0 de la caché y el byte 128, es el byte 15 de la línea 15.
}


// Funciones auxiliares
void clearBuffer()
{
    while (getchar() != '\n');
}