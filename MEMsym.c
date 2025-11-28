/*
    *   Autor
        Stefan Trifan

    *   Nombre del Proyecto
        Megatronix

    *   Especificaciones del Sistema
        Correspondencia Directa

        Bus de memoria: 12 bits 

        Cache:  
            - Tamaño           = 128 (B)
            - Tamaño linea     = 16 (B)
            - Numero de lineas = 8

        RAM
            - Tamaño            = 4096 (B)
            - Tamaño bloque     = 16 (B)
            - Numero de bloques = 256
*/

/* _________________________________________
   Inicio cabecera */

#include <stdio.h>
#include <stdlib.h>  
#include <unistd.h> 

#define TAM_LINEA 16  // Bytes por linea de CACHE
#define NUM_FILAS 8   // Numero de filas en la CACHE
#define TAM_RAM 4096  // Tamaño de RAM en Bytes
#define T_MISS 20     // Penalizacion por fallo en la CACHE
#define T_HIT 1       // Tiempo de acierto en la CACHE
#define TAM_TEXTO 100 // Numero de caracteres que se pueden leer desde la CACHE

#define RED "\033[1;31m"
#define GREEN "\033[1;32m"
#define YELLOW "\033[1;33m"
#define RESET "\033[0m"

#define FILE_ACCESOS_MEMORIA "./accesos_memoria.txt"
#define FILE_CONTENTS_RAM "./CONTENTS_RAM.bin"

// Estructura para linea de cache
typedef struct 
{
    unsigned char etq;             // Guarda el valor de la etiqueta
    unsigned char data[TAM_LINEA]; // Guarda [TAM_LINEA] Bytes (palabras)
} 
t_cache_line;

// Funciones del programa
void limpiar_cache(
    t_cache_line tbl[NUM_FILAS]
);
void parsear_direccion(
    unsigned int addr, 
    int *etq, 
    int *palabra, 
    int *linea, 
    int *bloque
);
void tratar_fallo(
    t_cache_line *tbl, 
    char *simul_ram, 
    int etq, 
    int linea, 
    int bloque
);
void volcar_cache(
    t_cache_line *tbl
);
    
// Funciones auxiliares
int comprobar_lectura_ficheros(
    FILE *fd_accesos_memoria, 
    FILE *fd_contents_ram
);
void imprimir_contenido_cache(
    t_cache_line tbl[NUM_FILAS]
);
void imprimir_stats(
    int globaltime, 
    int num_aciertos, 
    int num_fallos
);

/* _________________________________________
   Inicio main() */

int main(int argc, char *argv[])
{
    printf("\n_________________________________________START\n\n");

    // Abrimos los archivos
    FILE *fd_accesos_memoria = fopen(FILE_ACCESOS_MEMORIA, "r");
    FILE *fd_contents_ram    = fopen(FILE_CONTENTS_RAM, "rb");

    // Comprobamos que los archivos se han leido con exito
    // Si la funcion devuelve 0, los archivos no se han leido bien
    if(!comprobar_lectura_ficheros(fd_accesos_memoria, fd_contents_ram))
        return EXIT_FAILURE;

    // Declaracion de variables
    t_cache_line simul_cache[NUM_FILAS] = {0};
    char simul_ram[TAM_RAM] = {0},
         texto[TAM_TEXTO]   = {0};
    int globaltime        = 0, // Tiempo total de acceso a la CACHE
        num_fallos        = 0,
        num_aciertos      = 0, 
        caracteres_leidos = 0; // Numero de caracteres leidos desde la cache.
    unsigned int addr = 0;

    // Inicializamos los valores de la cache con valores por defecto
    limpiar_cache(simul_cache);    

    // Volcamos el contenido de de CONTENTS_RAM.bin en simul_ram (Nuestra RAM virtual) 
    fread(simul_ram, 1, TAM_RAM, fd_contents_ram);

    // Leemos direcciones de memoria de una en una, hasta que se acaben
    // y vas vamos guardando en addr
    // Para cada direccion, la parsea y la resuelve mediante Correspondencia Directa
    while(fscanf(fd_accesos_memoria, "%x", &addr) == 1)
    {
        int etq     = 0,
            linea   = 0,
            palabra = 0,
            bloque  = 0;
        
        printf("-> 0x%03X\n", addr);

        // Descomponemos la direccion en etiqueta, palabra, linea y bloque
        parsear_direccion(addr, &etq, &palabra, &linea, &bloque);

        // Comprobamos si el bloque esta mapeado en cache o no 
        // Comparamos la etiqueta de la direccion de memoria con la etiqueta 
        // de la linea correspondeinte  
        if(simul_cache[linea].etq != etq)
        {  
             // Si no esta mapeado, lo traemos de la RAM
            num_fallos++;
            printf(YELLOW
                "T: %d, Fallo de CACHE: %d, ADDR: 0x%03X, Etq: %02X, Linea: %01X, "
                "Palabra: %01X, Bloque: %02X\n"RESET,
                globaltime, num_fallos, addr, etq, linea, palabra, bloque);
            tratar_fallo(simul_cache, simul_ram, etq, linea, bloque);
            // Imprimimos por pantalla en hexadecimal el contenido de la cache
            imprimir_contenido_cache(simul_cache);
            globaltime += 20;
        }   
        else
        {
            // Si ya estaba mapeado desde antes, imprimimos el acierto
            globaltime++;
            num_aciertos++;
            printf(GREEN
                "T: %d, Acierto de CACHE:  ADDR 0x%03X, Etq: %02X, Linea: %01X, "
                "Palabra: %01X, Bloque: %02X\n"RESET, 
                globaltime, addr, etq, linea, palabra, bloque);
            // Imprimimos por pantalla en hexadecimal el contenido de la cache
            imprimir_contenido_cache(simul_cache);
        }
        
        // Cada caracter leido se añade a la variable llamada texto
        texto[caracteres_leidos] = simul_cache[linea].data[palabra];
        texto[++caracteres_leidos] = '\0';

        // sleep de 1 segundo.
        sleep(1);
        printf("\n");
        
    }

    // Imprimimos numero de aciertos, numero de fallos y tiempo de acceso
    imprimir_stats(globaltime, num_aciertos, num_fallos);

    // Imprimimos el texto leido caracter a caracter desde cache
    printf("--- Texto leido ---\n\n");
    printf("\"%s\"", texto);
    
    // Volcamos el contenido de la cache en CONTENTS_CACHE.bin
    volcar_cache(simul_cache);

    // Cerramos los file descriptor
    fclose(fd_accesos_memoria);
    fclose(fd_contents_ram);

    printf("\n_________________________________________EXIT_SUCCES\n\n");
    return EXIT_SUCCESS;
}

/* _________________________________________
   Inicio definicion de funciones */

// * Funciones del programa
/**
 * @brief Inicializa los valores de etq y data
 * 
 * @param[out] tbl Corresponde a simul_cache en main()
 *                 Contenedor de las lineas y etiquetas de cache.
 */
void limpiar_cache(t_cache_line tbl[NUM_FILAS])
{
    for(int i = 0; i < NUM_FILAS; i++)
    {
        // inicializamos los campos etq a FF
        tbl[i].etq = 0xFF;

        // inicializamos los campos data a 23 (#)
        for(int j = 0; j < TAM_LINEA; j++)
        {
            tbl[i].data[j] = 0x23;
        }
    }
}

/**
 * @brief descompone la direccion de memoria en etiqueta, palabra, linea, bloque
 * 
 * @param[in] addr Direccion de memoria actual   
 * @param[out] etq          
 * @param[out] palabra     
 * @param[out] linea       
 * @param[out] bloque    
 */
void parsear_direccion(
    unsigned int addr, 
    int *etq, 
    int *palabra, 
    int *linea, 
    int *bloque)
{
    /*  
    Ejemplo
    +-------------+----------+------------+
    |  0 0 1 0 0  |  0 1 0   |  1 1 1 0   |
    +-------------+----------+------------+
    | Etiqueta(5) | Linea(3) | Palabra(4) |
    +-------------+----------+------------+ */

    // Desplazamos 7 bits de la direccion de memoria a la derecha. Obtenemos la etiqueta
    *etq = addr >> 7;

    // Desplazamos 4 bits a la derecha para la Linea. 
    // Aplicamos mascara y nos quedamos con los 3 bits de la derecha
    *linea = (addr >> 4) & 7;

    // Aplicamos mascara y nos quedamos con los 4 bits de la derecha
    *palabra = addr & 15;

    // Desplazamos 4 bits a la derecha.    
    *bloque = addr >> 4;
}

/**
 * @brief Carga en la linea correspondiente el bloque desde la RAM
 *        Actualiza la etiqueta en la CACHE
 * 
 * @param[out] tbl Simulador cache. Contiene las lineas  
 * @param[in] simul_ram    Simulador RAM. Contiene los bloques                  
 * @param[in] etq       
 * @param[in] linea        
 * @param[in] bloque       
 */
void tratar_fallo(
    t_cache_line *tbl, 
    char *simul_ram, 
    int etq, 
    int linea, 
    int bloque)
{
    printf("Cargando el bloque %X en la linea %02X\n", bloque, linea);

    // Se traen los 16 bytes de datos del bloque a la linea
    for(int i = 0; i < TAM_LINEA; i++)
    {
        tbl[linea].data[i] = simul_ram[TAM_LINEA * bloque + i];
    } 

    // Se actualiza el campo etiqueta de la cache tbl[linea].etq = etq
    tbl[linea].etq = etq;    
    
    printf("Bloque cargado en la cache!\n"); 
}

/**
 * @brief Vuelca el contenido de la cache en un nuevo archivo binario
 * 
 * @param[in] tbl     
 */
void volcar_cache(t_cache_line *tbl)
{
    // Creamos un archivo binario llamado CONTENTS_CACHE.bin
    FILE *fd_contents_cache = fopen("CONTENTS_CACHE.bin", "wb");

    // Volcamos los contenidos de los 128 bytes de informacion 
    // (8 lineas de 16 bytes cada una) 
    // de la cache  en en CONTENTS_CACHE.bin. 
    // El byte 0 de ese fichero es el byte 0 de la linea 0 de la cache 
    // El byte 128, es el byte 15 de la linea 8.
    for(int i = 0; i < NUM_FILAS; i++)
    {
        fwrite(tbl[i].data, 1, TAM_LINEA, fd_contents_cache);
    } 

    fclose(fd_contents_cache);
}

// * Funciones auxiliares
/**
 * @return 1: Si Ha leido bien los archivos
 *         0: No ha podido encontrar los archivos
 */
int comprobar_lectura_ficheros(FILE *fd_accesos_memoria, FILE *fd_contents_ram)
{
    // Comprueba que existen los ficheros "accesos_memoria.txt" y "CONTENTS_RAM.bin"
    int archivos_leidos = 0;
    if(fd_accesos_memoria == NULL)
    {
        printf(RED
            "ERROR: No ha sido posible abrir el fichero \"accesos_memoria.txt\"\n"YELLOW
            "Asegurate de que esta bien nombrado y dentro de la raiz del proyecto"RED
            "\n\n_________________________________________FAIL\n\n"RESET);
        archivos_leidos = 0;
    }
    else if(fd_contents_ram == NULL)
    {
        printf(RED
            "ERROR: No ha sido posible abrir el fichero \"CONTENTS_RAM.bin\"\n"YELLOW
            "Asegurate de que esta bien nombrado y dentro de la raiz del proyecto"RED
            "\n\n_________________________________________FAIL\n\n"RESET);
        archivos_leidos = 0;
    }
    else
    {
        printf(GREEN
            "\"accesos_memoria.txt\" y \"CONTENTS_RAM.bin\" "
            "se han abieto correctamente.\n\n\n"RESET);
        archivos_leidos = 1;
    }
    return archivos_leidos;
}

void imprimir_contenido_cache(t_cache_line tbl[NUM_FILAS])
{
    // Imprime en hexadecimal los datos almacenados en CACHE
    printf("--- Contenido volcado de la CACHE: ---\n\n");
    for(int i = 0; i < NUM_FILAS; i++)
    {
        printf("[ %02X   Datos: ", tbl[i].etq);
        // Los datos se imprimen de izquierda a derecha de mayor a menor peso. 
        // El byte situado más a la izquierda es el byte 15 de la linea 
        // y el situado a la derecha el byte 0.
        for(int j = TAM_LINEA - 1; j >= 0; j--)
        {
            printf("%02X ", tbl[i].data[j]);
        }
        printf("]\n\n");
    }
}

void imprimir_stats(int globaltime, int num_aciertos, int num_fallos)
{
    // Imprime numero de aciertos, numero de fallos y tiempo de acceso
    float t_access = 0; // Tiempo medio de acceso a la CACHE

    t_access = (float)globaltime / ((float)num_aciertos + (float)num_fallos);
    printf(
        "\n\n--- Stats CACHE ---\n\n"
        "Accesos totales = %d\n" 
        "Fallos          = %d\n"
        "Tiempo medio    = %.3f\n\n", num_aciertos + num_fallos, num_fallos, t_access);
}