# Megatronix
Stefan Trifan y Alfredo Muñoz
Sistemas Operativos · U‑tad · 2025

Portada

MEGATRONIX tenía un bus de memoria de 12 bits y usaba memoria física, con una caché de 8 líneas con correspondencia directa y 16 bytes por línea. Con lo que hemos aprendido en teoría, sabemos que el caché interpretará cada dirección de memoria recibida de la CPU en tres campos: palabra (4 bits), línea (3 bits) y etiqueta (5 bits).

Hay que desarrollar en Csobre Linux un proceso MEMsym. El proceso dispone de un array de 8elementos, del tipo T_CACHE_LINE, cuya definición es:

typedef struct 
{
    unsigned char ETQ;
    unsigned char Data[TAM_LINEA];
} 
T_CACHE_LINE;
El proceso tendrá una variable globaltime que inicializará un valor 0. También creará otra con el nombre numfallos inicializada a 0.

En el arranque, MEMsyminicializa a xFF(hexadecimal) los campos Label ya x23(hexa) todos los campos de datos de la caché. Luego lea el fichero binario CONTENTS_RAM.binen la variable Simul_RAM, que es un array de 4096 unsigned char. A continuación, comienza la lectura del fichero de texto dirs_memoria.txtque contiene una lista de direcciones de memoria en hexadecimal, una por línea. El proceso tiene que tratar adecuadamente los errores si alguno de los ficheros no existe y avisar con el mensaje correspondiente. En ese caso terminará el proceso con return(-1).

Se repetirá el siguiente protocolo:

MEMsymLee una dirección del fichero accesos_memoria.txt.
Obtenga el número de línea y compruebe si la etiqueta de la dirección es igual a Labella línea de la caché.
Si no es así, incrementa el valor de numfallos y escribe una línea con el texto “T: %d, Fallo de CACHE %d, ADDR %04X Label %X linea %02X palabra %02X bloque %02X”, siendo Tel instante. Se incrementa en 20el contador globaltime. Se copia el bloque correspondiente desde la matriz RAM y se imprime un mensaje indicando que se está cargando el bloque Xen la línea Y. Se actualizan tanto el campo Labelcomo los 16bytes de datos de la línea.
Por pantalla se escribe “T: %d, Acierto de CACHE, ADDR %04X Label %X linea %02X palabra %02X DATO %02X”. Cada carácter leído se añade a una variable llamada texto, que es una matriz de 100caracteres como máximo (no hace falta usar memoria dinámica).
El proceso vuelca el contenido de la caché por pantalla con el siguiente formato: Los datos se imprimen de izquierda a derecha de mayor a menor peso. Esto significa que el byte situado más a la izquierda es el byte 15 de la línea y el situado a la derecha el byte 0.
EL proceso hace un sleep()segundo 1.
Al final se imprimirá un mensaje con el número total de accesos, número de fallos y tiempo medio de acceso.

Debajo, otro mensaje con el texto leído carácter a carácter desde el caché.

Antes de salir, el programa volcará los contenidos de los 128 bytes de información ( 8líneas de 16bytes cada una) de la caché en un fichero binario llamado CONTENTS_CACHE.bin. El byte 0de ese fichero es el byte 0de la línea 0de la caché y el byte 128, es el byte 15de la línea 8.

El proceso tendrá que desarrollar OBLIGATORIAMENTE las siguientes funciones:

void LimpiarCACHE(T_CACHE_LINE tbl[NUM_FILAS]);

void VolcarCACHE(T_CACHE_LINE *tbl);

void ParsearDireccion(unsigned int addr, int *ETQ, int *palabra, int *linea, int *bloque);

void TratarFallo(T_CACHE_LINE *tbl, char *MRAM, int ETQ, int linea, int bloque);
