#ifndef __CONTACTO_H__
#define __CONTACTO_H__

#include <stdio.h>
#include "glist.h"

// Definicion de la estructura Contacto.
typedef struct {
  char *nombre;
  char *apellido;
  unsigned edad;
  char *telefono;
} Contacto;


// Crea un nuevo contacto.
Contacto *contacto_crear(char *nombre, char *apellido, unsigned edad,
                         char *telefono);

// Crea una copia física del contacto.
Contacto *contacto_copia(Contacto * contacto);

// Función destructora de un contacto.
void contacto_destruir(Contacto * contacto);

// Modifica la edad de un contacto.
void contacto_editar_edad(Contacto * contacto, unsigned edadNueva);

// Modifica el telefono de un contacto.
void contacto_editar_tel(Contacto * contacto, char *telNuevo);

// Compara dos contactos por nombre y apellido.
// Retorna un entero negativo si contacto1 < contacto2, 0 si son iguales y un entero
// positivo si contacto1 > contacto2
int contacto_comparar(Contacto * c1, Contacto * c2);

// Compara dos contactos por nombre.
int contacto_comparar_nombre(Contacto * c1, Contacto * c2);

// Compara dos contactos por apellido.
int contacto_comparar_apellido(Contacto * c1, Contacto * c2);

// Compara dos contactos por edad.
int contacto_comparar_edad(Contacto * c1, Contacto * c2);

// Compara dos contactos por telefono.
int contacto_comparar_telefono(Contacto * c1, Contacto * c2);

// Compara dos contactos, segun los atributos que tenga el segundo.
int contacto_comparar_full(Contacto * a, Contacto * b);

// Compara un contacto con el primer contacto de una lista.
int contacto_comparar_nombre_con_lista(Contacto * c, GList lista);

// Compara un contacto con el primer contacto de una lista.
int contacto_comparar_apellido_con_lista(Contacto * c, GList lista);

// Compara un contacto con el primer contacto de una lista.
int contacto_comparar_edad_con_lista(Contacto * c, GList lista);

// Compara un contacto con el primer contacto de una lista.
int contacto_comparar_telefono_con_lista(Contacto * c, GList lista);

// Hashear un contacto por su nombre y apellido
unsigned contacto_hashear(Contacto * contacto);

// Imprime por pantalla un contacto.
void contacto_imprimir(Contacto * contacto);

void contacto_imprimir_extra(Contacto *contacto, void *extra);

#endif                          /* __CONTACTO_H__ */
