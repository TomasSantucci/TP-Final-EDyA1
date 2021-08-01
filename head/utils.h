#ifndef __UTILS_H__
#define __UTILS_H__

// Funcion de hash para strings propuesta por Kernighan & Ritchie en "The C
// Programming Language (Second Ed.)".
unsigned KRHash(char *s);

// Recibe un dato, pero no hace nada.
void destr_falso( __attribute__ ((unused))
                 void *dato);

// Recibe un puntero y devuelve el mismo.
void *copia_falsa(void *dato);

// Crea una copia fisica de un entero.
void *copiar_entero(void *a);

// Destruye un puntero de entero.
void destruir_entero(void *n);

// Chequea que todos los caracteres de una cadena verifiquen la funcion dada.
int cadena_homo(char *cadena, int (*validar) (int));

// Chequea que un caracter sea una letra o un espacio.
int isalpha_or_space(int caracter);

#endif /** __UTILS_H__ */
