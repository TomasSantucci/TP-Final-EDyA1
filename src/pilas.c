#include <stdlib.h>
#include "../head/pilas.h"
#include "../head/arreglo.h"

// Estructura que representa una pila con un arreglo dinámico.
// Una vez que esta llena, empieza a sobreescribir los primeros datos insertados.
struct _Pila {
  ArregloG arr;
  int ultimo;
};

// Crea una nueva pila vacia.
Pila pila_crear(int tamanio, FuncionCopiadora copia, FuncionDestructora destr) {
  Pila pila = malloc(sizeof(struct _Pila));
  pila->arr = arreglog_crear(tamanio, copia, destr);
  pila->ultimo = -1;
  return pila;
}

// Destruye una pila.
void pila_destruir(Pila pila) {
  arreglog_destruir(pila->arr);
  free(pila);
}

// Chequea si una pila es vacia.
int pila_es_vacia(Pila pila) {
  return (pila->ultimo == -1 ? 1 : arreglog_leer(pila->arr, pila->ultimo) ==
          NULL);
}

// Devuelve el elemento del tope de la pila.
void *pila_tope(Pila pila) {
  return (!pila_es_vacia(pila) ? arreglog_leer(pila->arr, pila->ultimo) : NULL);
}

// Agrega un elemento a la pila.
void pila_apilar(Pila pila, void *dato) {
  pila->ultimo = (pila->ultimo + 1) % arreglog_capacidad(pila->arr);
  arreglog_escribir(pila->arr, pila->ultimo, dato);
}

// Quita un elemento de la pila.
void pila_desapilar(Pila pila) {
  if (!pila_es_vacia(pila)) {
    arreglog_borrar(pila->arr, pila->ultimo);
    pila->ultimo =
        (pila->ultimo + arreglog_capacidad(pila->arr) -
         1) % arreglog_capacidad(pila->arr);
  }
}

// Vacia la pila.
void pila_vaciar(Pila pila) {
  while (!pila_es_vacia(pila)) {
    pila_desapilar(pila);
  }
}
