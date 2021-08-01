#ifndef __ARREGLO_H__
#define __ARREGLO_H__

typedef void *(*FuncionCopiadora) (void *dato);
typedef void (*FuncionDestructora) (void *dato);
typedef void (*FuncionVisitante) (void *dato);

typedef struct _ArregloG *ArregloG;

// Crea un nuevo arreglo vacio, con la capacidad dada.
ArregloG arreglog_crear(int, FuncionCopiadora, FuncionDestructora);

// Destruye un arreglo.
void arreglog_destruir(ArregloG);

// Devuelve el dato de la posicion dada.
void *arreglog_leer(ArregloG, int);

// Escribe un dato en la posicion dada. Si ya habia un dato,
// lo sobreescribe.
void arreglog_escribir(ArregloG, int, void *);

// Devuelve la capacidad de un arreglo dado.
int arreglog_capacidad(ArregloG);

// Borra el elemento de una posicion dada.
void arreglog_borrar(ArregloG, unsigned);

// Recorre un arreglo aplicando una funcion dada a sus elementos.
void arreglog_recorrer(ArregloG arreglo, FuncionVisitante visit);

#endif                          /* __ARREGLO_H__ */
