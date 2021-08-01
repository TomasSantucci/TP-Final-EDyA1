#ifndef __GLIST_H__
#define __GLIST_H__

typedef void (*FuncionVisitanteExtra) (void *dato, void *extra);

typedef int (*Predicado) (void *a, void *b);
typedef void *(*Copia) (void *dato);
typedef void (*FuncionDestructora) (void *dato);
typedef int (*FuncionComparadora) (void *a, void *b);

// Estructura que representa una lista.
typedef struct _GNodo {
  void *dato;
  struct _GNodo *sig;
} GNodo;

typedef GNodo *GList;

// Crea una lista vacia
GList glist_crear();

// Destruye una lista
void glist_destruir(GList lista, FuncionDestructora destr);

// Chequea si la lista es vacia
int glist_vacia(GList lista);

// Chequea si la lista tiene solo un elemento
int glist_unitaria(GList lista);

// Agrega un elemento al inicio de la lista
GList glist_agregar_inicio(GList lista, void *dato, void *(*copiar) (void *));

// Dado un dato, si esta lo elimina.
// Si lo consigue devuelve 1, si no 0.
int glist_eliminar(GList * lista, void *dato, FuncionComparadora comp,
                   FuncionDestructora destr);

// Recorre la lista, aplicando una funcion a los elementos
void glist_recorrer(GList lista, FuncionVisitanteExtra visit, void *extra);

// Devuelve el primer elemento de la lista
void *glist_primero(GList lista);

// Busca un elemento y si lo encuentra lo retorna
void *glist_encontrar(GList lista, void *dato,
                      int (*comparar) (void *, void *));

// Dada una lista, se queda con los elementos que verifican el predicado.
// Si se le pasa 0, no destruye la lista original.
// Si se le pasa 1, destruye la lista original.
GList glist_filtrar(GList lista, Predicado p, Copia c, FuncionDestructora destr,
                    void *dato, int liberar);

#endif                          /* __GLIST_H__ */
