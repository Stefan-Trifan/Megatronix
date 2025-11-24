/*
    *   Autor
        Stefan Trifan

    *   Nombre del Proyecto
        Megatronix

    *   Configuración del Proyecto
        Correspondencia Directa

        Bus de memoria: 12 bits 

        Caché:  
            - tam_cache  = 128 (B)
            - tam_linea  = 16 (B)
            - num_lineas = 8

        RAM
            - tasimul_ram = 4096 (B)
            - tam_bloque  = 16 (B)
            - num_bloques = 256
*/

/* _________________________________________
   Inicio cabecera */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

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
void tratar_fallo(T_CACHE_LINE *simul_cache, char *simul_ram, int etq, int linea, int bloque);
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
    // todo descomentar
    // if(!comprobar_lectura_ficheros(fd_accesos_memoria, fd_contents_ram))
    //     return EXIT_FAILURE;

    // Declaracion de variables
    T_CACHE_LINE simul_cache[NUM_FILAS] = {0};
    char simul_ram[4096];
    char texto[100];
    int caracteres_leidos = 0;
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
            linea   = 0,
            palabra = 0,
            bloque  = 0;

        parsear_direccion(addr, &etq, &palabra, &linea, &bloque);
        
        // todo borrar
        printf("--------------- DEBUG ---------------\n");
        printf("addr    = %03X (Hex)\n", addr);
        printf("etq     = %d\n", etq);
        printf("linea   = %d\n", linea);
        printf("palabra = %d\n", palabra);
        printf("bloque  = %d\n", bloque);
        printf("-------------------------------------\n\n");

        // Comprobamos si el bloque está mapeado en cache o no 
        // Comparamos la etiqueta de la linea correspondeinte con la etiqueta de la direccion de memoria
        // Si no esta mapeado, lo traemos de la RAM
        if(simul_cache[linea].etq != etq)
        {  

            ++num_fallos;

            printf(YELLOW"T: %d, Fallo de Cache: %d, ADDR: 0x%03X, Etq: %X, Linea: %02X, Palabra: %02X, Bloque: %02X\n"RESET,
                globaltime, num_fallos, addr, etq, linea, palabra, bloque);
            
            tratar_fallo(simul_cache, simul_ram, etq, linea, bloque);

            globaltime += 20;
        }   

        // Leemos linea de la cache
        globaltime++;
        num_aciertos++;
        printf("T: %d, Acierto de CACHE, ADDR %04X Label %X, linea %02X, palabra %02X, DATO %02X\n\n\n", 
            globaltime, addr, etq, linea, palabra, bloque);

        // Cada carácter leído se añade a la variable llamada texto
        for(int i = 0; i < TAM_LINEA ; i++)
        {
            texto[caracteres_leidos] = simul_cache[linea].data[i];
            caracteres_leidos++;
        }
        caracteres_leidos++;
        texto[caracteres_leidos] = '\0';
    }

    //  Volcamos el contenido de la cache
    //  Los datos se imprimen de izquierda a derecha de mayor a menor peso. 
    for(int i = 0; i < NUM_FILAS; i++)
    {
        printf("%X   Datos: ", simul_cache[i].etq);
        for(int j = TAM_LINEA - 1; j >= 0; j--)
        {
            printf("%X ", simul_cache[i].data[j]);
        }
        printf("\n\n");
    }
    
    // sleep() de 1 segundo.
    // todo descomentar
    // todo sleep(1);

    // Imprimimos num_aciertos, num_fallos, t_access
    t_access = globaltime / (num_aciertos + num_fallos);
    printf(
        "Numero total de accesos = %d\n" 
        "Numero total de fallos  = %d\n"
        "Tiempo medio de acceso  = %d", num_aciertos, num_fallos, t_access);

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
    // +-------------+----------+------------+
    // |  0 0 1 0 0  |  0 1 0   |  1 1 1 0   |
    // +-------------+----------+------------+
    // | Etiqueta(5) | Linea(3) | Palabra(4) |
    // +-------------+----------+------------+

    // Desplazamos 7 bits de la direccion de memoria a la derecha. Obtenemos la etiqueta
    *etq = addr >> 7;

    // Desplazamos 4 bits a la derecha para la Linea. 
    // Aplicamos mascara y nos quedamos con los 3 bits de la derecha
    *linea = (addr >> 4) & 7;

    // Aplicamos mascara y nos quedamos con los 5 bits de la derecha
    *palabra = addr & 15;

    // Desplazamos 4 bits a la derecha.    
    *bloque = addr >> 4;
}

/**
 * @brief 
 * 
 * @param[out] simul_cache   Simulador cache. Contiene las lineas  
 * @param[in] simul_ram     Simulador RAM. Contiene los bloques        
 * @param[in] num_fallos 
 * @param[in] addr              
 * @param[in] etq       
 * @param[in] linea     
 * @param[in] palabra     
 * @param[in] bloque       
 */
void tratar_fallo(T_CACHE_LINE *simul_cache, char *simul_ram, int etq, int linea, int bloque)
{
    printf("Cargando el bloque %X en la linea %02X\n", bloque, linea);

    // Se traen los 16 bytes de datos del bloque a la línea
    for(int i = 0; i < TAM_LINEA; i++)
    {
        simul_cache[linea].data[i] = simul_ram[bloque];
        bloque++;
    } 

    printf("DEBUG %c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c\n", 
         simul_cache[linea].data[0]
        ,simul_cache[linea].data[1]
        ,simul_cache[linea].data[2]
        ,simul_cache[linea].data[3]
        ,simul_cache[linea].data[4]
        ,simul_cache[linea].data[5]
        ,simul_cache[linea].data[6]
        ,simul_cache[linea].data[7]
        ,simul_cache[linea].data[8]
        ,simul_cache[linea].data[9]
        ,simul_cache[linea].data[10]
        ,simul_cache[linea].data[11]
        ,simul_cache[linea].data[12]
        ,simul_cache[linea].data[13]
        ,simul_cache[linea].data[14]
        ,simul_cache[linea].data[15]
    );

    // Se actualiza el campo etiqueta de la caché simul_cache[linea].etq = etq
    simul_cache[linea].etq = etq;    
    
    printf(GREEN"Bloque mapeado en la cache!\n"RESET);
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

