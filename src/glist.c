#include "../head/glist.h"
#include <stdlib.h>

// Crea una lista vacia
GList glist_crear() {
  return NULL;
}

// Destruye una lista
void glist_destruir(GList lista, FuncionDestructora destr) {
  GNodo *nodoAEliminar;
  while (lista) {
    nodoAEliminar = lista;
    lista = lista->sig;
    destr(nodoAEliminar->dato);
    free(nodoAEliminar);
  }
}

// Chequea si la lista es vacia
int glist_vacia(GList lista) {
  return lista == NULL;
}

// Chequea si la lista tiene solo un elemento
int glist_unitaria(GList lista) {
  return (!glist_vacia(lista) && lista->sig == NULL);
}

// Agrega un elemento al inicio de la lista
GList glist_agregar_inicio(GList lista, void *dato, void *(*copiar) (void *)) {
  GNodo *nuevoNodo = malloc(sizeof(GNodo));
  nuevoNodo->dato = copiar(dato);
  nuevoNodo->sig = lista;
  return nuevoNodo;
}

// Dado un dato, si esta lo elimina.
// Si lo consigue devuelve 1, si no 0.
int glist_eliminar(GList * lista, void *dato, FuncionComparadora comp,
                   FuncionDestructora destr) {
  int eliminado = 0;
  if (!glist_vacia(*lista)) {
    GNodo *nodo = *lista;
    GNodo *nodoAEliminar = NULL;
    if (!comp((*lista)->dato, dato)) {
      nodoAEliminar = *lista;
      *lista = nodo->sig;
    } else {
      while (nodo->sig && comp(nodo->sig->dato, dato)) {
        nodo = nodo->sig;
      }
      if (nodo->sig) {
        nodoAEliminar = nodo->sig;
        nodo->sig = nodo->sig->sig;
      }
    }
    if (nodoAEliminar) {
      eliminado = 1;
      destr(nodoAEliminar->dato);
      free(nodoAEliminar);
    }
  }
  return eliminado;
}

// Recorre la lista, aplicando una funcion a los elementos
void glist_recorrer(GList lista, FuncionVisitanteExtra visit, void *extra) {
  for (GNodo * nodo = lista; nodo; nodo = nodo->sig)
    visit(nodo->dato, extra);
}

// Devuelve el primer elemento de la lista
void *glist_primero(GList lista) {
  return (glist_vacia(lista) ? NULL : lista->dato);
}

// Busca un elemento y si lo encuentra lo retorna
void *glist_encontrar(GList lista, void *dato, int (*comparar) (void *, void *)) {
  GNodo *nodoEncontrado = NULL;
  for (GNodo * nodo = lista; nodo && !nodoEncontrado; nodo = nodo->sig) {
    if (!comparar(nodo->dato, dato))
      nodoEncontrado = nodo;
  }
  return (nodoEncontrado ? nodoEncontrado->dato : NULL);
}

// Dada una lista, se queda con los elementos que verifican el predicado.
// Si se le pasa 0, no destruye la lista original.
// Si se le pasa 1, destruye la lista original.
GList glist_filtrar(GList lista, Predicado p, Copia c, FuncionDestructora destr,
                    void *dato, int liberar) {
  GList filtrada = NULL;
  for (GNodo * nodo = lista; nodo; nodo = nodo->sig) {
    if (p(nodo->dato, dato)) {
      filtrada = glist_agregar_inicio(filtrada, nodo->dato, c);
    }
  }
  if (liberar) {
    glist_destruir(lista, destr);
  }
  return filtrada;
}
