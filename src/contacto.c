#include "../head/contacto.h"
#include "../head/utils.h"
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>


// Crea un nuevo contacto.
Contacto *contacto_crear(char *nombre, char *apellido, unsigned edad,
                         char *telefono) {
  Contacto *nuevo = malloc(sizeof(Contacto));
  assert(nuevo != NULL);

  nuevo->nombre = malloc(sizeof(char) * (strlen(nombre) + 1));
  assert(nuevo->nombre != NULL);
  strcpy(nuevo->nombre, nombre);

  nuevo->apellido = malloc(sizeof(char) * (strlen(apellido) + 1));
  assert(nuevo->apellido != NULL);
  strcpy(nuevo->apellido, apellido);

  nuevo->telefono = malloc(sizeof(char) * (strlen(telefono) + 1));
  strcpy(nuevo->telefono, telefono);

  nuevo->edad = edad;

  return nuevo;
}

// Crea una copia física del contacto.
Contacto *contacto_copia(Contacto * contacto) {
  Contacto *copia =
      contacto_crear(contacto->nombre, contacto->apellido, contacto->edad,
                     contacto->telefono);
  return copia;
}

// Función destructora de un contacto.
void contacto_destruir(Contacto * contacto) {
  free(contacto->nombre);
  free(contacto->apellido);
  free(contacto->telefono);
  free(contacto);
}

// Modifica la edad de un contacto.
void contacto_editar_edad(Contacto * contacto, unsigned edadNueva) {
  contacto->edad = edadNueva;
}

// Modifica el telefono de un contacto.
void contacto_editar_tel(Contacto * contacto, char *telNuevo) {
  free(contacto->telefono);
  contacto->telefono = malloc(sizeof(char) * (strlen(telNuevo) + 1));
  strcpy(contacto->telefono, telNuevo);
}

// Compara dos contactos por nombre y apellido.
// Retorna un entero negativo si contacto1 < contacto2, 0 si son iguales y un entero
// positivo si contacto1 > contacto2
int contacto_comparar(Contacto * c1, Contacto * c2) {
  int a = strcmp(c1->nombre, c2->nombre);
  return (a ? a : strcmp(c1->apellido, c2->apellido));
}

// Compara dos contactos por nombre.
int contacto_comparar_nombre(Contacto * c1, Contacto * c2) {
  return strcmp(c1->nombre, c2->nombre);
}

// Compara dos contactos por apellido.
int contacto_comparar_apellido(Contacto * c1, Contacto * c2) {
  return strcmp(c1->apellido, c2->apellido);
}

// Compara dos contactos por edad.
int contacto_comparar_edad(Contacto * c1, Contacto * c2) {
  return c1->edad - c2->edad;
}

// Compara dos contactos por telefono.
int contacto_comparar_telefono(Contacto * c1, Contacto * c2) {
  return strcmp(c1->telefono, c2->telefono);
}

// Compara dos contactos, segun los atributos que tenga el segundo.
int contacto_comparar_full(Contacto * a, Contacto * b) {
  int res = 1;
  if (*(b->nombre))
    res *= !strcmp(b->nombre, a->nombre);
  if (*(b->apellido))
    res *= !strcmp(b->apellido, a->apellido);
  if ((b->edad))
    res *= b->edad == a->edad;
  if (*(b->telefono))
    res *= !strcmp(b->telefono, a->telefono);
  return res;
}

// Compara un contacto con el primer contacto de una lista.
int contacto_comparar_nombre_con_lista(Contacto * c, GList lista) {
  return contacto_comparar_nombre((Contacto *) glist_primero(lista), c);
}

// Compara un contacto con el primer contacto de una lista.
int contacto_comparar_apellido_con_lista(Contacto * c, GList lista) {
  return contacto_comparar_apellido((Contacto *) glist_primero(lista), c);
}

// Compara un contacto con el primer contacto de una lista.
int contacto_comparar_edad_con_lista(Contacto * c, GList lista) {
  return contacto_comparar_edad((Contacto *) glist_primero(lista), c);
}

// Compara un contacto con el primer contacto de una lista.
int contacto_comparar_telefono_con_lista(Contacto * c, GList lista) {
  return contacto_comparar_telefono((Contacto *) glist_primero(lista), c);
}


// Hashear un contacto por su nombre y apellido
unsigned contacto_hashear(Contacto * contacto) {
  // Concatenar nombre y apellido.
  char *nombre =
      malloc(sizeof(char) *
             (strlen(contacto->nombre) + strlen(contacto->apellido) + 2));
  char *apellido = malloc(sizeof(char) * (strlen(contacto->apellido) + 1));
  strcpy(nombre, contacto->nombre);
  strcpy(apellido, contacto->apellido);

  strcat(nombre, " ");
  strcat(nombre, apellido);

  unsigned hash = KRHash(nombre);
  free(nombre);
  free(apellido);
  return hash;
}

// Imprime por pantalla un contacto.
void contacto_imprimir(Contacto * contacto) {
  printf("{%s, %s, %d, %s}\n", contacto->nombre, contacto->apellido,
         contacto->edad, contacto->telefono);
}
