#ifndef __AGENDA_H__
#define __AGENDA_H__
#include <stdio.h>

typedef void *(*FuncionCopiadora) (void *dato);
typedef int (*FuncionComparadora) (void *, void *);
typedef void (*FuncionDestructora) (void *dato);
typedef unsigned (*FuncionHash) (void *dato);

typedef struct _Agenda *Agenda;

typedef void *(*FuncionAccion) (Agenda, char *, char *, unsigned, char *);

typedef struct _Accion *Accion;

// Crea una agenda. Recibe una funcion que copie, otra que destruye los datos.
// Recibe funciones que comparen contactos por cada atributo por separado.
Agenda agenda_crear(FuncionCopiadora copia, FuncionComparadora comp1,
                    FuncionComparadora comp2, FuncionComparadora comp3,
                    FuncionComparadora comp4, FuncionComparadora comp5,
                    FuncionDestructora destr);

// Destruye la agenda
void agenda_destruir(Agenda agenda);

// Busca un contacto
void *agenda_buscar(Agenda ag, void *nombre, void *apellido);

// Agrega un contacto a la agenda y guarda la accion para deshacerla
void agenda_agregar(Agenda ag, char *nombre, char *apellido, unsigned edad,
                    char *telefono);

// Elimina un contacto de la agenda y guarda la accion para deshacerla
void agenda_eliminar(Agenda ag, char *nombre, char *apellido);

// Edita un contacto de la agenda y guarda la accion para deshacerla
void agenda_editar(Agenda ag, char *nombre, char *apellido, unsigned edadNueva,
                   char *telNuevo);

// Agrega a la agenda todos los contactos escritos en un archivo
void agenda_cargar(Agenda agenda, FILE * fpt);

// Escribe todos los contactos de la agenda en un archivo dado,
// ordenados por algun atributo.
void agenda_guardar(Agenda agenda, int orden, FILE * fpt);

// Deshace la ultima accion realizada en la agenda
void agenda_deshacer(Agenda agenda);

// Rehace la ultima accion deshecha en la agenda
void agenda_rehacer(Agenda agenda);

// AND
void agenda_and(Agenda agenda, char *nombre, char *apellido, int edad,
                char *tel);

// OR
void agenda_or(Agenda agenda, char *nombre, char *apellido, int edad,
               char *tel);

// Dado un entero n, si hay un subconjunto de contactos en la agenda
// cuyas edades sumen n, los imprime.
void agenda_suma_edades(Agenda agenda, int n);

// Imprime la agenda
void agenda_imprimir(Agenda agenda);

#endif                          /* __AGENDA_H__ */
