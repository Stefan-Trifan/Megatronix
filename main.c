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
#define T_MISS 20
#define T_HIT 1

// Variables Globales

// Linea de caché
typedef struct 
{
    unsigned char etq; // Guarda el valor decimal de la etiqueta
    unsigned char data[TAM_LINEA]; // Guarda las 16 Bytes/palabras
} 
T_CACHE_LINE;

// Funciones del programa
void limpiar_cache(T_CACHE_LINE simul_cache[NUM_FILAS]);
void parsear_direccion(unsigned int addr, int *etq, int *palabra, int *linea, int *bloque);
void tratar_fallo(T_CACHE_LINE *simul_cache, char *m_ram, 
    int num_fallos, int addr, int etq, int linea, int palabra, int bloque);
void volcar_cache(T_CACHE_LINE *simul_cache);
    
// Funciones auxiliares
void clearBuffer();

/* _________________________________________
   Inicio main() */

int main(int argc, char *argv[])
{
    printf("\n_________________________________________START\n\n");

    // todo comprobar que accesos_memoria.txt existe o devolver EXIT_FAILURE
    // todo comprobar que CONTENTS_RAM.bin existe o devolver EXIT_FAILURE
    // todo declaramos fd_accesos_memoria 
    // todo declaramos fd_contents_ram

    // todo Declaracion de variables
    T_CACHE_LINE simul_cache[NUM_FILAS];
    char simul_ram[4096];
    char texto[100];
    int globaltime   = 0,
        num_fallos   = 0,
        num_aciertos = 0,
        t_access     = 0;

    // todo limpiar_cache(T_CACHE_LINE simul_cache[NUM_FILAS]);

    // todo simul_ram = contenido de CONTENTS_RAM.bin
    
    // todo leemos las direcciones de memoria de accesos_memoria.txt, de una en una
    // todo while(fd_accesos_memoria != '\0)
        unsigned int addr = 0;
        int etq     = 0,
            palabra = 0,
            linea   = 0,
            bloque  = 0;

        // todo guardamos la direccion de memoria actual en la variable addr 
        
        // todo parsear_direccion(unsigned int addr, int *etq, int *palabra, int *linea, int *bloque)

        // todo comprueba si etq actual es igual a simul_cache[linea].etq    

            // todo Si no coincide:  
                // todo ++num_fallos
                // todo void tratar_fallo(*simul_cache, *m_ram, num_fallos, addr, etq, linea, palabra, int bloque)
                // todo      
                // todo globaltime += 20
            
            // todo si coincide:
                // todo globaltime += T_HIT
                // todo ++num_aciertos
                // todo imprimimos:
                // todo “T: %d, Acierto de CACHE, ADDR %04X, etq %X, linea %02X, palabra %02X, DATO %02X”. 
                // todo Cada carácter leído se añade a la variable llamada texto

    // todo Volcamos el contenido de la cache
    // todo Los datos se imprimen de izquierda a derecha de mayor a menor peso. 

    // todo sleep() de 1 segundo.

    // todo t_access = globaltime / (num_aciertos + num_fallos)

    // todo imprimimos num_aciertos, num_fallos, t_access

    // todo imprimimos el texto leído carácter a carácter desde simul_cache[].data

    // todo VolcarCACHE(T_CACHE_LINE *simul_cache);

    printf("\n_________________________________________EXIT\n\n");
    return EXIT_SUCCESS;
}

/* _________________________________________
   Inicio definicion de funciones */

// Funciones del programa
/**
 * @brief   inicializamos los valores etq y data
 * 
 * @param[out] simul_cache   Contenedor de las líneas y etiquetas de cache 
 */
void limpiar_cache(T_CACHE_LINE simul_cache[NUM_FILAS])
{
    // todo inicializamos los campos etq a FF o 1111 1111
    // todo inicializamos los campos data a 23 o 0010 0011
}

/**
 * @brief descompone la direccion de memoria en etiqueta, palabra, linea, bloque
 * 
 * @param[in] addr      Dirección de memoria actual   
 * @param[out] etq          
 * @param[out] palabra     
 * @param[out] linea       
 * @param[out] bloque    
 */
void parsear_direccion(unsigned int addr, int *etq, int *palabra, int *linea, int *bloque)
{
    // todo Obtiene la etiqueta, palabra, linea y bloque de la direccion
}

/**
 * @brief 
 * 
 * @param[in,out] simul_cache   Simulador cache. Contiene las lineas  
 * @param[in,out] m_ram         Simulador RAM. Contiene los bloques        
 * @param[in] num_fallos 
 * @param[in] addr              
 * @param[in] etq       
 * @param[in] linea     
 * @param[in] palabra     
 * @param[in] bloque       
 */
void tratar_fallo(T_CACHE_LINE *simul_cache, char *m_ram, 
    int num_fallos, int addr, int etq, int linea, int palabra, int bloque)
{
    // todo imprimimos: 
        // todo “T: %d, Fallo de CACHE %d, ADDR %04X, etq %X, linea %02X, palabra %02X, bloque %02X”. 

        // todo T: %d               Instante del fallo  globaltime
        // todo Fallo de CACHE %d   Contador de fallos  num_fallos   
        // todo ADDR %04X           Direccion Actual    ADDR
        // todo etq %X              Etiqueta            etq
        // todo linea %02X          Linea               linea
        // todo palabra %02X        Palabra             palabra
        // todo bloque %02X         Bloque              bloque

    // todo Se copia el bloque desde m_ram a simul_cache[linea].data

    // todo se imprime un mensaje indicando que se está cargando el bloque X en la línea Y

    // todo Se actualiza el campo etiqueta de la caché simul_cache[linea].etq = etq

    // todo Se traen los 16 bytes de datos del bloque a la línea
        // todo for(int i = 0; i < TAM_LINEA; i++)
        // todo {
        // todo     simul_cache[linea].data[i] = m_ram[i]
        // todo } 
}

/**
 * @param[in,out] simul_cache     
 */
void volcar_cache(T_CACHE_LINE *simul_cache)
{
    // todo Creamos un archivo binario llamado CONTENTS_CACHE.bin

    // todo Volcamos los contenidos de los 128 bytes de información de la caché en CONTENTS_CACHE.bin
        // todo El byte 0 del fichero es el byte 0 de la línea 0 de la caché 
        // todo El byte 128, es el byte 15 de la línea 15.
}


// Funciones auxiliares
void clearBuffer()
{
    while (getchar() != '\n');
}