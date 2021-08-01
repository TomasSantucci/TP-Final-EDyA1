#include<stdlib.h>
#include<assert.h>
#include "../head/arreglo.h"

// Estructura principal que representa un arreglo.
typedef struct {
  void **direccion;
  int capacidad;
} Arreglo;


// Crea un nuevo arreglo vacio, con la capacidad dada.
static Arreglo *arreglo_crear(int capacidad) {
  Arreglo *arr = malloc(sizeof(Arreglo));
  arr->capacidad = capacidad;
  arr->direccion = malloc(sizeof(void *) * capacidad);
  // Inicializamos los datos nulos.
  for (int i = 0; i < capacidad; i++) {
    arr->direccion[i] = NULL;
  }
  return arr;
}

// Destruye un arreglo.
static void arreglo_destruir(Arreglo * arreglo, FuncionDestructora destr) {
  for (int i = 0; i < arreglo->capacidad; i++) {
    if (arreglo->direccion[i] != NULL) {
      destr(arreglo->direccion[i]);
    }
  }
  free(arreglo->direccion);
  free(arreglo);
}

// Devuelve la capacidad de un arreglo dado.
static int arreglo_capacidad(Arreglo * arreglo) {
  return arreglo->capacidad;
}

// Devuelve el dato de la posicion dada.
static void *arreglo_leer(Arreglo * arreglo, int pos) {
  assert(pos >= 0 && pos < arreglo_capacidad(arreglo));
  return arreglo->direccion[pos];
}

// Escribe un dato en la posicion dada. Si ya habia un dato,
// lo sobreescribe.
static void arreglo_escribir(Arreglo * arreglo, int pos, void *dato,
                             FuncionCopiadora copia, FuncionDestructora destr) {
  assert(pos >= 0 && pos < arreglo_capacidad(arreglo));
  if (arreglo->direccion[pos] != NULL)
    destr(arreglo->direccion[pos]);
  arreglo->direccion[pos] = copia(dato);
}

// Recorre un arreglo aplicando una funcion dada a sus elementos.
static void arreglo_recorrer(Arreglo * arreglo, FuncionVisitante visit) {
  for (int i = 0; i < arreglo->capacidad; i++) {
    visit(arreglo->direccion[i]);
  }
}

// Borra el elemento de una posicion dada.
static void arreglo_borrar(Arreglo * arreglo, unsigned pos,
                           FuncionDestructora destr) {
  assert((int) pos >= 0 && (int) pos < arreglo_capacidad(arreglo));
  if (arreglo->direccion[pos] != NULL) {
    destr(arreglo->direccion[pos]);
    arreglo->direccion[pos] = NULL;
  }
}

// Estructura que representa un arreglo y sus funciones asociadas.
struct _ArregloG {
  Arreglo *arr;
  FuncionCopiadora copia;
  FuncionDestructora destr;
};

// Crea un arreglo.
ArregloG arreglog_crear(int capacidad, FuncionCopiadora copia,
                        FuncionDestructora destr) {
  ArregloG arrg = malloc(sizeof(struct _ArregloG));
  arrg->arr = arreglo_crear(capacidad);
  arrg->copia = copia;
  arrg->destr = destr;
  return arrg;
}

// Destruye un arreglo.
void arreglog_destruir(ArregloG arrg) {
  arreglo_destruir(arrg->arr, arrg->destr);
  free(arrg);
}

// Devuelve el dato que se encuentra en la posicion dada.
void *arreglog_leer(ArregloG arrg, int pos) {
  return arreglo_leer(arrg->arr, pos);
}

// Escribe un dato en la posicion dada.
void arreglog_escribir(ArregloG arrg, int pos, void *dato) {
  arreglo_escribir(arrg->arr, pos, dato, arrg->copia, arrg->destr);
}

// Devuelve la capacidad del arreglo.
int arreglog_capacidad(ArregloG arrg) {
  return arreglo_capacidad(arrg->arr);
}

// Borra el elemento de una posicion dada.
void arreglog_borrar(ArregloG arrg, unsigned pos) {
  arreglo_borrar(arrg->arr, pos, arrg->destr);
}

// Recorre un arreglo aplicando una funcion dada a sus elementos
void arreglog_recorrer(ArregloG arreglo, FuncionVisitante visit) {
  arreglo_recorrer(arreglo->arr, visit);
}
