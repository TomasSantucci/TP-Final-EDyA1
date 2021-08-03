#include "../head/tablahash.h"
#include "../head/glist.h"
#include "../head/utils.h"
#include <assert.h>
#include <stdlib.h>
#include <stdio.h>

#define LIM_FACTOR_CARGA 0.7



//Crea una nueva tabla hash vacia, con la capacidad dada.
TablaHash tablahash_crear(unsigned capacidad, FuncionCopiadora copia,
                          FuncionComparadora comp, FuncionDestructora destr,
                          FuncionHash hash) {

  // Pedimos memoria para la estructura principal y las casillas.
  TablaHash tabla = malloc(sizeof(struct _TablaHash));
  assert(tabla != NULL);
  tabla->elems = malloc(sizeof(CasillaHash) * capacidad);
  assert(tabla->elems != NULL);
  tabla->numElems = 0;
  tabla->capacidad = capacidad;
  tabla->copia = copia;
  tabla->comp = comp;
  tabla->destr = destr;
  tabla->hash = hash;

  // Inicializamos las casillas con datos nulos.
  for (unsigned idx = 0; idx < capacidad; ++idx) {
    tabla->elems[idx].lista = glist_crear();
  }

  return tabla;
}


// Retorna el numero de elementos de la tabla.
int tablahash_nelems(TablaHash tabla) {
  return tabla->numElems;
}


// Retorna la capacidad de la tabla.
int tablahash_capacidad(TablaHash tabla) {
  return tabla->capacidad;
}


// Destruye la tabla.
void tablahash_destruir(TablaHash tabla) {

  // Destruir cada uno de los datos.
  for (unsigned idx = 0; idx < tabla->capacidad; ++idx) {
    GList lista = tabla->elems[idx].lista;
    glist_destruir(lista, tabla->destr);
  }

  // Liberar el arreglo de casillas y la tabla.
  free(tabla->elems);
  free(tabla);
  return;
}


// Inserta un dato en la tabla si no se encontraba.
void tablahash_insertar(TablaHash tabla, void *dato) {

  // Calculamos la posicion del dato dado, de acuerdo a la funcion hash.
  unsigned idx = tabla->hash(dato) % tabla->capacidad;

  GList lista = tabla->elems[idx].lista;
  void *existe = glist_encontrar(lista, dato, tabla->comp);

  if (!existe) {
    // Agrego el dato
    tabla->numElems++;
    tabla->elems[idx].lista = glist_agregar_inicio(lista, dato, tabla->copia);
    if (tabla->numElems / (double) tabla->capacidad > LIM_FACTOR_CARGA) {
      tablahash_redimensionar(tabla);
    }
  }

  return;
}


// Retorna el dato de la tabla que coincida con el dato dado, o NULL si el dato
// buscado no se encuentra en la tabla.
void *tablahash_buscar(TablaHash tabla, void *dato) {

  // Calculamos la posicion del dato dado, de acuerdo a la funcion hash.
  unsigned idx = tabla->hash(dato) % tabla->capacidad;

  void *d = glist_encontrar(tabla->elems[idx].lista, dato, tabla->comp);
  return d;
}


// Elimina el dato de la tabla que coincida con el dato dado.
void tablahash_eliminar(TablaHash tabla, void *dato) {

  // Calculamos la posicion del dato dado, de acuerdo a la funcion hash.
  unsigned idx = tabla->hash(dato) % tabla->capacidad;

  if (glist_eliminar
      (&(tabla->elems[idx].lista), dato, tabla->comp, tabla->destr)) {
    tabla->numElems--;
  }
}

// Agrega un dato dado a una tabla hash dada.
static void agregar_dato(void *dato, TablaHash tabla) {
  if (dato) {
    tablahash_insertar(tabla, dato);
  }
}

// Aumenta la capacidad de la tabla hash al doble.
void tablahash_redimensionar(TablaHash tabla) {
  // Me guardo los datos de la tabla.
  CasillaHash *casillas = tabla->elems;

  tabla->numElems = 0;
  tabla->capacidad *= 2;
  // Pido memoria para la nueva capacidad.
  tabla->elems = malloc(sizeof(CasillaHash) * tabla->capacidad);
  for (unsigned idx = 0; idx < tabla->capacidad; ++idx) {
    tabla->elems[idx].lista = glist_crear();
  }

  // LLeno la nueva memoria con los datos anteriores y destruyo las casillas anteriores.
  for (int i = 0; i < (int) tabla->capacidad / 2; i++) {
    glist_recorrer(casillas[i].lista, (FuncionVisitanteExtra) agregar_dato,
                   tabla);
    glist_destruir(casillas[i].lista, destr_falso);
  }

  free(casillas);
}
