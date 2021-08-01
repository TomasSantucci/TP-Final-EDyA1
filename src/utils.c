#include "../head/utils.h"
#include <ctype.h>
#include <stdlib.h>
#include <string.h>

// Funcion de hash para strings propuesta por Kernighan & Ritchie en "The C
// Programming Language (Second Ed.)".
unsigned KRHash(char *s) {
  unsigned hashval;
  for (hashval = 0; *s != '\0'; ++s) {
    hashval = *s + 31 * hashval;
  }
  return hashval;
}

// Recibe un dato, pero no hace nada.
void destr_falso( __attribute__ ((unused))
                 void *dato) {
  return;
}

// Recibe un puntero y devuelve el mismo.
void *copia_falsa(void *dato) {
  return dato;
}

// Crea una copia fisica de un entero.
void *copiar_entero(void *a) {
  int *n = malloc(sizeof(int));
  *n = *((int *) a);
  return n;
}

// Destruye un puntero de entero.
void destruir_entero(void *n) {
  free(n);
}

// Chequea que todos los caracteres de una cadena verifiquen la funcion dada.
int cadena_homo(char *cadena, int (*validar) (int)) {
  int largo = strlen(cadena);
  int valida = 1;
  for (int i = 0; i < largo && valida; ++i) {
    valida = validar(cadena[i]);
  }
  return valida;
}

// Chequea que un caracter sea una letra o un espacio.
int isalpha_or_space(int caracter) {
  return isalpha(caracter) || caracter == ' ';
}