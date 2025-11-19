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

#define RED "\033[1;31m"
#define GREEN "\033[1;32m"
#define YELLOW "\033[1;33m"
#define RESET "\033[0m"

#define FILE_ACCESOS_MEMORIA "./assets/accesos_memoria.txt"
#define FILE_CONTENTS_RAM "./assets/CONTENTS_RAM.bin"

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
int comprobar_lectura_ficheros(FILE *fd_accesos_memoria, FILE *fd_contents_ram);
void imprimir_cache(T_CACHE_LINE simul_cache[NUM_FILAS]);
void clearBuffer();

/* _________________________________________
   Inicio main() */

int main(int argc, char *argv[])
{
    printf("\n_________________________________________START\n\n");

    // Leemos los archivos
    FILE *fd_accesos_memoria = fopen(FILE_ACCESOS_MEMORIA, "r");
    FILE *fd_contents_ram    = fopen(FILE_CONTENTS_RAM, "rb");

    // Comprobamos que los archivos se han leído con éxito
    // Si la funcion devuelve 0, los archivos no se han leido bien
    if(!comprobar_lectura_ficheros(fd_accesos_memoria, fd_contents_ram))
        return EXIT_FAILURE;

    // Declaracion de variables
    T_CACHE_LINE simul_cache[NUM_FILAS] = {0};
    char simul_ram[4096];
    char texto[100];
    unsigned int addr = 0;
    int globaltime   = 0,
        num_fallos   = 0,
        num_aciertos = 0,
        t_access     = 0;

    // Inicializamos los valores de la cache con valores por defecto
    limpiar_cache(simul_cache);    

    // Volcamos el contenido de de CONTENTS_RAM.bin en simul_ram (Nuestra RAM virtual) 
    fread(simul_ram ,1 , 4096, fd_contents_ram);

    // Leemos direcciones de memoria de una en una, hasta que se acaben
    // y vas vamos guardando en addr
    // Para cada direccion, la parsea y la resuelve mediante Correspondencia Directa
    while(fscanf(fd_accesos_memoria, "%x", &addr) == 1)
    {
        
        int etq     = 0,
            palabra = 0,
            linea   = 0,
            bloque  = 0;
            

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
    }



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
 * @brief   inicializamos los valores de etq y data
 * 
 * @param[out] simul_cache   Contenedor de las líneas y etiquetas de cache 
 */
void limpiar_cache(T_CACHE_LINE simul_cache[NUM_FILAS])
{
    for(int i = 0; i < NUM_FILAS; i++)
    {
        // inicializamos los campos etq a FF
        simul_cache[i].etq = 0xFF;

        // inicializamos los campos data a 23 o 0010 0011
        for(int j = 0; j < TAM_LINEA; j++)
        {
            simul_cache[i].data[j] = 0x23;
        }
    }
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

int comprobar_lectura_ficheros(FILE *fd_accesos_memoria, FILE *fd_contents_ram)
{
    int archivos_leidos = 0;
    if(fd_accesos_memoria == NULL)
    {
        printf(RED
            "ERROR: No ha sido posible abrir el fichero \"accesos_memoria.txt\"\n"YELLOW
            "Asegurate de que esta bien nombrado y esta dentro de la carpeta /assets"RED
            "\n\n_________________________________________FAIL\n\n"RESET);
        archivos_leidos = 0;
    }
    else if(fd_contents_ram == NULL)
    {
        printf(RED
            "ERROR: No ha sido posible abrir el fichero \"CONTENTS_RAM.bin\"\n"YELLOW
            "Asegurate de que esta bien nombrado y esta dentro de la carpeta /assets"RED
            "\n\n_________________________________________FAIL\n\n"RESET);
        archivos_leidos = 0;
    }
    else
    {
        printf(GREEN"\"accesos_memoria.txt\" y \"CONTENTS_RAM.bin\" se han abieto correctamente.\n\n"RESET);
        archivos_leidos = 1;
    }
    return archivos_leidos;
}

void imprimir_cache(T_CACHE_LINE simul_cache[NUM_FILAS])
{
    for(int i = 0; i < NUM_FILAS; i++)
    {
        // inicializamos los campos etq a FF
        printf("simul_cache[%d].etq = %02X\n", i, simul_cache[i].etq);

        // inicializamos los campos data a 23 o 0010 0011
        for(int j = 0; j < TAM_LINEA; j++)
        {
            printf("simul_cache[%d].data[%.2d] = %02X\n", i, j, simul_cache[i].data[j]);
        }
        printf("\n");
    }
}

void clearBuffer()
{
    while (getchar() != '\n');
}

