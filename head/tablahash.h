#ifndef __TABLAHASH_H__
#define __TABLAHASH_H__
#include "glist.h"

// Retorna una copia fisica del dato
typedef void *(*FuncionCopiadora) (void *dato);
// Retorna un entero negativo si dato1 < dato2, 0 si son iguales y un entero
// positivo si dato1 > dato2
typedef int (*FuncionComparadora) (void *dato1, void *dato2);
// Libera la memoria alocada para el dato.
typedef void (*FuncionDestructora) (void *dato);
// Retorna un entero sin signo para el dato
typedef unsigned (*FuncionHash) (void *dato);

// Casillas en la que almacenaremos los datos de la tabla hash.
typedef struct _CasillaHash {
  GList lista;
} CasillaHash;

// Estructura principal que representa la tabla hash.
struct _TablaHash {
  CasillaHash *elems;
  unsigned numElems;
  unsigned capacidad;
  FuncionCopiadora copia;
  FuncionComparadora comp;
  FuncionDestructora destr;
  FuncionHash hash;
};

typedef struct _TablaHash *TablaHash;

// Crea una nueva tabla hash vacia, con la capacidad dada.
TablaHash tablahash_crear(unsigned capacidad, FuncionCopiadora copia,
                          FuncionComparadora comp, FuncionDestructora destr,
                          FuncionHash hash);

// Retorna el numero de elementos de la tabla.
int tablahash_nelems(TablaHash tabla);

// Retorna la capacidad de la tabla.
int tablahash_capacidad(TablaHash tabla);

// Destruye la tabla.
void tablahash_destruir(TablaHash tabla);

// Inserta un dato en la tabla si no se encontraba.
void tablahash_insertar(TablaHash tabla, void *dato);

// Retorna el dato de la tabla que coincida con el dato dado, o NULL si el dato
// buscado no se encuentra en la tabla.
void *tablahash_buscar(TablaHash tabla, void *dato);

// Elimina el dato de la tabla que coincida con el dato dado.
void tablahash_eliminar(TablaHash tabla, void *dato);

// Aumenta la capacidad de la tabla al doble.
void tablahash_redimensionar(TablaHash tabla);

#endif                          /* __TABLAHASH_H__ */
