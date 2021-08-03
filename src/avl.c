#include <assert.h>
#include <stdlib.h>
#include "../head/glist.h"
#include "../head/avl.h"


// Estructura de arbol AVL donde los datos son listas para guardar repetidos.
typedef struct _AVL_Nodo {
  GList dato;
  struct _AVL_Nodo *izq, *der;
  int altura;
} AVL_Nodo;

// Destruye un nodo.
static void destruir_nodo(AVL_Nodo * arbol, FuncionDestructora destruir) {
  glist_destruir(arbol->dato, destruir);
  free(arbol);
}

// Destruye el arbol y sus datos
static void avl_nodo_destruir(AVL_Nodo * raiz, FuncionDestructora destr) {
  if (raiz != NULL) {
    avl_nodo_destruir(raiz->izq, destr);
    avl_nodo_destruir(raiz->der, destr);
    destruir_nodo(raiz, destr);
  }
}

// Retorna la lista donde se encuentra el dato a buscar.
static GList avl_nodo_buscar(AVL_Nodo * raiz, void *dato,
                             FuncionComparadora comp) {
  if (raiz == NULL)
    return NULL;
  else if (comp(dato, raiz->dato) == 0) // raiz->dato == dato
    return raiz->dato;
  else if (comp(dato, raiz->dato) < 0) {        // dato < raiz->dato
    return avl_nodo_buscar(raiz->izq, dato, comp);
  } else {                      // raiz->dato < dato
    return avl_nodo_buscar(raiz->der, dato, comp);
  }
}

// Retorna la altura de un nodo.
static int avl_nodo_altura(AVL_Nodo * raiz) {
  return (raiz == NULL ? -1 : raiz->altura);
}

// Retorna el maximo entre las alturas de los dos hijos.
static unsigned int avl_nodo_max_altura_hijos(AVL_Nodo * raiz) {
  assert(raiz != NULL);
  int alturaIzq = avl_nodo_altura(raiz->izq);
  int alturaDer = avl_nodo_altura(raiz->der);
  return (alturaIzq < alturaDer ? alturaDer : alturaIzq);
}

// Retorna el factor de balanceo del nodo.
static int avl_nodo_factor_balance(AVL_Nodo * raiz) {
  assert(raiz != NULL);
  int factor = avl_nodo_altura(raiz->der) - avl_nodo_altura(raiz->izq);
  assert(-2 <= factor || factor <= 2);
  return factor;
}

// Realiza una rotacion simple a izquierda.
static AVL_Nodo *avl_nodo_rotacion_simple_izq(AVL_Nodo * raiz) {
  AVL_Nodo *hijoDer = raiz->der;
  assert(hijoDer != NULL);
  // actualizar punteros
  raiz->der = hijoDer->izq;
  hijoDer->izq = raiz;
  // actualizar alturas
  raiz->altura = 1 + avl_nodo_max_altura_hijos(raiz);
  hijoDer->altura = 1 + avl_nodo_max_altura_hijos(hijoDer);
  return hijoDer;
}

// Realiza una rotacion simple a derecha.
static AVL_Nodo *avl_nodo_rotacion_simple_der(AVL_Nodo * raiz) {
  AVL_Nodo *hijoIzq = raiz->izq;
  assert(hijoIzq != NULL);
  // actualizar punteros
  raiz->izq = hijoIzq->der;
  hijoIzq->der = raiz;
  // actualizar alturas
  raiz->altura = 1 + avl_nodo_max_altura_hijos(raiz);
  hijoIzq->altura = 1 + avl_nodo_max_altura_hijos(hijoIzq);
  return hijoIzq;
}

// Crea un nodo con un dato y lo devuelve.
static AVL_Nodo *avl_nodo_crear(void *dato, FuncionCopiadora copia) {
  AVL_Nodo *nuevoNodo = malloc(sizeof(AVL_Nodo));
  assert(nuevoNodo != NULL);

  GList lista = glist_crear();
  lista = glist_agregar_inicio(lista, dato, copia);
  nuevoNodo->dato = lista;
  nuevoNodo->izq = nuevoNodo->der = NULL;
  nuevoNodo->altura = 0;
  return nuevoNodo;
}


// Inserta un dato en el arbol, manteniendo la propiedad del arbol de busqueda binaria.
static AVL_Nodo *avl_nodo_insertar(AVL_Nodo * raiz, void *dato,
                                   FuncionCopiadora copia,
                                   FuncionComparadora comp) {
  if (raiz == NULL) {           // insertar el dato en un nuevo nodo
    return avl_nodo_crear(dato, copia);
  } else if (comp(dato, raiz->dato) < 0) {      // dato < raiz->dato
    raiz->izq = avl_nodo_insertar(raiz->izq, dato, copia, comp);
    // chequear balance
    if (avl_nodo_factor_balance(raiz) == -2) {
      // casos 1 o 2
      if (avl_nodo_factor_balance(raiz->izq) == 1)      // caso 2
        raiz->izq = avl_nodo_rotacion_simple_izq(raiz->izq);
      raiz = avl_nodo_rotacion_simple_der(raiz);        // caso 1
    }
    raiz->altura = 1 + avl_nodo_max_altura_hijos(raiz);
    return raiz;
  } else if (comp(dato, raiz->dato) > 0) {      // raiz->dato < dato
    raiz->der = avl_nodo_insertar(raiz->der, dato, copia, comp);
    // chequear balance
    if (avl_nodo_factor_balance(raiz) == 2) {
      // casos 3 y 4
      if (avl_nodo_factor_balance(raiz->der) == -1)     // caso 3
        raiz->der = avl_nodo_rotacion_simple_der(raiz->der);
      raiz = avl_nodo_rotacion_simple_izq(raiz);        // caso 4
    }
    raiz->altura = 1 + avl_nodo_max_altura_hijos(raiz);
    return raiz;
  } else {
    raiz->dato = glist_agregar_inicio(raiz->dato, dato, copia);
    return raiz;
  }
}

// Recorrido DSF del arbol
static void avl_nodo_recorrer(AVL_Nodo * raiz, AVLRecorrido orden,
                              FuncionVisitanteExtra visita, void *extra) {
  if (raiz != NULL) {
    if (orden == AVL_RECORRIDO_PRE)
      glist_recorrer(raiz->dato, visita, extra);
    avl_nodo_recorrer(raiz->izq, orden, visita, extra);
    if (orden == AVL_RECORRIDO_IN)
      glist_recorrer(raiz->dato, visita, extra);
    avl_nodo_recorrer(raiz->der, orden, visita, extra);
    if (orden == AVL_RECORRIDO_POST)
      glist_recorrer(raiz->dato, visita, extra);
  }
}

// Devuelve el padre del minimo nodo a derecha del nodo dado;
static AVL_Nodo *padre_minimo_nodo(AVL_Nodo * arbol) {
  AVL_Nodo *padre = arbol->der;
  AVL_Nodo *hijo = padre->izq;
  if (hijo == NULL) {
    return arbol;
  } else {
    while (hijo->izq != NULL) {
      padre = hijo;
      hijo = hijo->izq;
    }
    return padre;
  }
}

// Elimina un dato del arbol, manteniendo la propiedad del arbol de busqueda binaria.
static AVL_Nodo *avl_nodo_eliminar(AVL_Nodo * arbol, void *dato,
                                   FuncionComparadora comp1,
                                   FuncionComparadora comp2,
                                   FuncionDestructora destruir) {
  if (arbol == NULL) {
    return arbol;
  }

  if (comp1(dato, arbol->dato) < 0)     // dato < arbol->dato
    arbol->izq = avl_nodo_eliminar(arbol->izq, dato, comp1, comp2, destruir);

  else if (comp1(dato, arbol->dato) > 0)        // dato > arbol->dato
    arbol->der = avl_nodo_eliminar(arbol->der, dato, comp1, comp2, destruir);

  else {                        // encontre el nodo a eliminar

    if (glist_unitaria(arbol->dato) && !comp2(glist_primero(arbol->dato), dato)) {
      if (arbol->izq == NULL) {
        AVL_Nodo *aux = arbol;
        arbol = aux->der;
        destruir_nodo(aux, destruir);
      } else if (arbol->der == NULL) {
        AVL_Nodo *aux = arbol;
        arbol = aux->izq;
        destruir_nodo(aux, destruir);
      } else {
        // El nodo tiene dos hijos
        AVL_Nodo *padre = padre_minimo_nodo(arbol);

        AVL_Nodo *aux = arbol;
        if (padre != arbol) {
          arbol = padre->izq;
          padre->izq = arbol->der;
          padre->altura = 1 + avl_nodo_max_altura_hijos(padre);

          arbol->der = aux->der;
        } else {
          arbol = padre->der;
        }

        arbol->izq = aux->izq;

        destruir_nodo(aux, destruir);
      }
    } else {
      glist_eliminar(&(arbol->dato), dato, comp2, destruir);
      return arbol;
    }
  }

  if (arbol == NULL)
    return arbol;

  // Balanceo del arbol
  arbol->altura = 1 + avl_nodo_max_altura_hijos(arbol);

  int balance = avl_nodo_factor_balance(arbol);

  if (balance == -2) {
    // casos 1 o 2
    if (avl_nodo_factor_balance(arbol->izq) == 1)       // caso 2
      arbol->izq = avl_nodo_rotacion_simple_izq(arbol->izq);
    arbol = avl_nodo_rotacion_simple_der(arbol);        // caso 1
  }

  if (balance == 2) {
    // casos 3 y 4
    if (avl_nodo_factor_balance(arbol->der) == -1)      // caso 3
      arbol->der = avl_nodo_rotacion_simple_der(arbol->der);
    arbol = avl_nodo_rotacion_simple_izq(arbol);        // caso 4
  }

  return arbol;
}

// Estructura de arbol AVL y sus funciones asociadas
struct _AVL {
  AVL_Nodo *raiz;
  FuncionComparadora comp;
  FuncionDestructora destr;
  FuncionCopiadora copia;
};

// Crea un arbol AVL y lo retorna.
AVL avl_crear(FuncionComparadora comp, FuncionDestructora destr,
              FuncionCopiadora copia) {
  AVL arbol = malloc(sizeof(struct _AVL));
  assert(arbol != NULL);
  arbol->raiz = NULL;
  arbol->comp = comp;
  arbol->destr = destr;
  arbol->copia = copia;
  return arbol;
}

// Destruye un arbol AVL
void avl_destruir(AVL arbol) {
  avl_nodo_destruir(arbol->raiz, arbol->destr);
  free(arbol);
}

// Retorna la lista donde se encuentra el dato a buscar.
GList avl_buscar(AVL arbol, void *dato) {
  return avl_nodo_buscar(arbol->raiz, dato, arbol->comp);
}

// Inserta un dato en el arbol, manteniendo la propiedad del arbol de busqueda binaria.
void avl_insertar(AVL arbol, void *dato) {
  arbol->raiz = avl_nodo_insertar(arbol->raiz, dato, arbol->copia, arbol->comp);
}

// Recorrido DSF del arbol
void avl_recorrer(AVL arbol, AVLRecorrido orden,
                  FuncionVisitanteExtra visita, void *extra) {
  avl_nodo_recorrer(arbol->raiz, orden, visita, extra);
}

// Elimina un dato del arbol, manteniendo la propiedad del arbol de busqueda binaria.
void avl_eliminar(AVL arbol, void *dato, FuncionComparadora comp) {
  arbol->raiz =
      avl_nodo_eliminar(arbol->raiz, dato, arbol->comp, comp, arbol->destr);
}
