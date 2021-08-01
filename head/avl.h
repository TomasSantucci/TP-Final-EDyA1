#ifndef __AVL_H__
#define __AVL_H__

// Retorna una copia fisica del dato
typedef void *(*FuncionCopiadora) (void *dato);
// Retorna un entero negativo si dato1 < dato2, 0 si son iguales y un entero
// positivo si dato1 > dato2
typedef int (*FuncionComparadora) (void *, void *);
// Libera la memoria alocada para el dato.
typedef void (*FuncionDestructora) (void *dato);
typedef void (*FuncionVisitanteExtra) (void *dato, void *extra);

typedef enum {
  AVL_RECORRIDO_IN,  /** Inorden */
  AVL_RECORRIDO_PRE, /** Preorden */
  AVL_RECORRIDO_POST /** Postorden */
} AVLRecorrido;

typedef struct _AVL *AVL;

// Crea un arbol AVL y lo retorna.
AVL avl_crear(FuncionComparadora, FuncionDestructora, FuncionCopiadora);

// Destruye un arbol AVL
void avl_destruir(AVL);

// Retorna la lista donde se encuentra el dato a buscar.
GList avl_buscar(AVL, void *);

// Inserta un dato en el arbol, manteniendo la propiedad del arbol de busqueda binaria.
void avl_insertar(AVL, void *);

// Recorrido DSF del arbol
void avl_recorrer(AVL, AVLRecorrido, FuncionVisitanteExtra, void *extra);

// Elimina un dato del arbol, manteniendo la propiedad del arbol de busqueda binaria.
void avl_eliminar(AVL arbol, void *dato, FuncionComparadora comp);

#endif                          //__AVL_H__
