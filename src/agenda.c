#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "../head/utils.h"
#include "../head/arreglo.h"
#include "../head/glist.h"
#include "../head/agenda.h"
#include "../head/contacto.h"
#include "../head/avl.h"
#include "../head/tablahash.h"
#include "../head/pilas.h"

#define CAPACIDAD_INICIAL 100
#define N 3

// Definicion de la estructura agenda

struct _Agenda {
  TablaHash th;
  AVL nombre;
  AVL apellido;
  AVL edad;
  AVL telefono;
  Pila deshacer;
  Pila rehacer;
};

// Definicion de la estructura accion

struct _Accion {
  FuncionAccion accion;
  FuncionAccion inversa;
  Contacto *contacto;
};

// Funciones para manejar la estructura accion

static void *copiar_accion(void *acc) {
  Accion a = (Accion) acc;
  Accion copia = malloc(sizeof(struct _Accion));
  copia->accion = a->accion;
  copia->inversa = a->inversa;
  copia->contacto = a->contacto;
  return copia;
}

static void destr_accion(void *acc) {
  Accion a = (Accion) acc;
  contacto_destruir(a->contacto);
  free(a);
}

static struct _Accion accion_crear(FuncionAccion accion, FuncionAccion inversa,
                                   Contacto * c) {
  struct _Accion acc;
  acc.accion = accion;
  acc.inversa = inversa;
  acc.contacto = c;
  return acc;
}


// Crea una agenda. Recibe una funcion que copie, otra que destruye los datos.
// Recibe funciones que comparen contactos por cada atributo por separado.
Agenda agenda_crear(FuncionCopiadora copia, FuncionComparadora comp1,
                    FuncionComparadora comp2, FuncionComparadora comp3,
                    FuncionComparadora comp4, FuncionComparadora comp5,
                    FuncionDestructora destr) {
  Agenda ag = malloc(sizeof(struct _Agenda));
  ag->th =
      tablahash_crear(CAPACIDAD_INICIAL, copia, comp1, destr,
                      (FuncionHash) contacto_hashear);
  ag->nombre = avl_crear(comp2, destr_falso, copia_falsa);
  ag->apellido = avl_crear(comp3, destr_falso, copia_falsa);
  ag->edad = avl_crear(comp4, destr_falso, copia_falsa);
  ag->telefono = avl_crear(comp5, destr_falso, copia_falsa);
  ag->deshacer = pila_crear(N, copiar_accion, destr_accion);
  ag->rehacer = pila_crear(N, copiar_accion, destr_accion);

  return ag;
}

// Destruye la agenda
void agenda_destruir(Agenda agenda) {
  tablahash_destruir(agenda->th);
  avl_destruir(agenda->nombre);
  avl_destruir(agenda->apellido);
  avl_destruir(agenda->edad);
  avl_destruir(agenda->telefono);
  pila_destruir(agenda->deshacer);
  pila_destruir(agenda->rehacer);
  free(agenda);
}

// Busca un contacto
void *agenda_buscar(Agenda ag, void *nombre, void *apellido) {
  Contacto *contacto_aux = contacto_crear(nombre, apellido, 1, "");
  void *contacto = tablahash_buscar(ag->th, contacto_aux);
  contacto_destruir(contacto_aux);
  return contacto;
}

// Agrega un contacto a la tablahash y a los arboles
static void *agenda_agregar_aux(Agenda ag, char *nombre, char *apellido,
                                unsigned edad, char *telefono) {
  Contacto *contacto = contacto_crear(nombre, apellido, edad, telefono);
  tablahash_insertar(ag->th, contacto);
  avl_insertar(ag->nombre, contacto);
  avl_insertar(ag->apellido, contacto);
  avl_insertar(ag->edad, contacto);
  avl_insertar(ag->telefono, contacto);
  return NULL;
}

// Elimina un contacto de la tablahash y los arboles
static void *agenda_eliminar_aux(Agenda ag, char *nombre, char *apellido,
                                 unsigned edad, char *tel) {
  Contacto *contacto_aux = contacto_crear(nombre, apellido, edad, tel);
  avl_eliminar(ag->nombre, contacto_aux, (FuncionComparadora) ag->th->comp);
  avl_eliminar(ag->apellido, contacto_aux, (FuncionComparadora) ag->th->comp);
  avl_eliminar(ag->edad, contacto_aux, (FuncionComparadora) ag->th->comp);
  avl_eliminar(ag->telefono, contacto_aux, (FuncionComparadora) ag->th->comp);
  tablahash_eliminar(ag->th, contacto_aux);
  contacto_destruir(contacto_aux);
  return NULL;
}

// Edita un contacto de la tablahash y los arboles.
// Devuelve una copia del contacto sin editar.
static void *agenda_editar_aux(Agenda ag, char *nombre, char *apellido,
                               unsigned edadNueva, char *telNuevo) {
  Contacto *contacto = agenda_buscar(ag, nombre, apellido);
  if (contacto) {
    contacto = (Contacto *) contacto;
    Contacto *anterior = contacto_copia(contacto);
    if (contacto->edad != edadNueva) {
      avl_eliminar(ag->edad, contacto, (FuncionComparadora) ag->th->comp);
      contacto_editar_edad(contacto, edadNueva);
      avl_insertar(ag->edad, contacto);
    }
    if (strcmp(contacto->telefono, telNuevo)) {
      avl_eliminar(ag->telefono, contacto, (FuncionComparadora) ag->th->comp);
      contacto_editar_tel(contacto, telNuevo);
      avl_insertar(ag->telefono, contacto);
    }
    return anterior;
  } else
    return NULL;
}

// Agrega un contacto a la agenda y guarda la accion para deshacerla
void agenda_agregar(Agenda ag, char *nombre, char *apellido, unsigned edad,
                    char *telefono) {
  if (!agenda_buscar(ag, nombre, apellido)) {
    // Si el contacto no estaba en la agenda.
    agenda_agregar_aux(ag, nombre, apellido, edad, telefono);

    // Guardo la accion para deshacerla.
    struct _Accion acc = accion_crear(agenda_eliminar_aux, agenda_agregar_aux,
                                      contacto_crear(nombre, apellido, edad,
                                                     telefono));
    pila_apilar(ag->deshacer, &acc);
    pila_vaciar(ag->rehacer);
  } else
    puts("Contacto ya existente, no se permite sobreescribir.");
}

// Elimina un contacto de la agenda y guarda la accion para deshacerla
void agenda_eliminar(Agenda ag, char *nombre, char *apellido) {
  Contacto *c = agenda_buscar(ag, nombre, apellido);
  if (c) {
    // Si el contacto a eliminar existia.
    c = contacto_copia((Contacto *) c);
    agenda_eliminar_aux(ag, c->nombre, c->apellido, c->edad, c->telefono);

    // Guardo la accion para deshacerla.
    struct _Accion acc =
        accion_crear(agenda_agregar_aux, agenda_eliminar_aux, c);
    pila_apilar(ag->deshacer, &acc);
    pila_vaciar(ag->rehacer);
  } else
    puts("El contacto no existe.");
}

// Edita un contacto de la agenda y guarda la accion para deshacerla
void agenda_editar(Agenda ag, char *nombre, char *apellido, unsigned edadNueva,
                   char *telNuevo) {
  Contacto *anterior =
      agenda_editar_aux(ag, nombre, apellido, edadNueva, telNuevo);
  // Edito el contacto y guardo el contacto sin editar.
  if (anterior) {

    // Guardo la accion para deshacerla.
    struct _Accion acc =
        accion_crear(agenda_editar_aux, agenda_editar_aux, anterior);
    pila_apilar(ag->deshacer, &acc);
    pila_vaciar(ag->rehacer);
  } else
    puts("El contacto no existe.");
}

// Agrega a la agenda todos los contactos escritos en un archivo
void agenda_cargar(Agenda agenda, FILE * fpt) {
  char buff[100];
  // Leo la primer linea.
  fgets(buff, 100, fpt);
  buff[strcspn(buff, "\n\r")] = 0;
  // Itero sobre las lineas del archivo
  while (fgets(buff, 100, fpt)) {
    buff[strcspn(buff, "\n\r")] = 0;
    // Verifico que sean de la forma: nombre,apellido,edad,telefono
    char *nombre = strtok(buff, ",");
    if (nombre && cadena_homo(nombre, isalpha_or_space)) {
      char *apellido = strtok(NULL, ",");
      if (apellido && cadena_homo(apellido, isalpha_or_space)) {
        char *ed = strtok(NULL, ",");
        if (ed) {
          unsigned edad = atoi(ed);
          if (edad > 0 && edad < 150) {
            char *tel = strtok(NULL, ",");
            if (tel && cadena_homo(tel, isdigit))
              // Agrego el contacto a la agenda si no existia.
              if (!agenda_buscar(agenda, nombre, apellido))
                agenda_agregar_aux(agenda, nombre, apellido, edad, tel);
          }
        }
      }
    }
  }
}

// Escribe un contacto dado en un archivo dado.
static void guardar_contacto(void *contacto, void *archivo) {
  fprintf((FILE *) archivo, "%s,%s,%d,%s\n", ((Contacto *) contacto)->nombre,
          ((Contacto *) contacto)->apellido, ((Contacto *) contacto)->edad,
          ((Contacto *) contacto)->telefono);
}

// Escribe todos los contactos de la agenda en un archivo dado,
// ordenados por algun atributo.
void agenda_guardar(Agenda agenda, int orden, FILE * fpt) {
  fprintf(fpt, "nombre,apellido,edad,telefono\n");
  switch (orden) {
  case 0:                      // Ordenado por nombre
    avl_recorrer(agenda->nombre, AVL_RECORRIDO_IN,
                 (FuncionVisitanteExtra) guardar_contacto, fpt);
    break;
  case 1:                      // Ordenado por apellido
    avl_recorrer(agenda->apellido, AVL_RECORRIDO_IN,
                 (FuncionVisitanteExtra) guardar_contacto, fpt);
    break;
  case 2:                      // Ordenado por edad
    avl_recorrer(agenda->edad, AVL_RECORRIDO_IN,
                 (FuncionVisitanteExtra) guardar_contacto, fpt);
    break;
  case 3:                      // Ordenado por telefono
    avl_recorrer(agenda->telefono, AVL_RECORRIDO_IN,
                 (FuncionVisitanteExtra) guardar_contacto, fpt);
    break;
  }
}

// Deshace la ultima accion realizada en la agenda
void agenda_deshacer(Agenda agenda) {
  Accion accion = pila_tope(agenda->deshacer);
  if (accion != NULL) {
    Contacto *c = accion->contacto;

    Contacto *anterior = agenda_buscar(agenda, c->nombre, c->apellido);
    if (anterior != NULL) {
      anterior = contacto_copia((Contacto *) anterior);
    } else {
      anterior = contacto_copia(c);
    }

    struct _Accion accion2 =
        accion_crear(accion->inversa, accion->accion, anterior);

    // Realizo la accion que deshace la ultima modificacion.
    void *res = accion->accion(agenda, c->nombre, c->apellido, c->edad, c->telefono);
    if (res) contacto_destruir((Contacto *) res);

    pila_apilar(agenda->rehacer, &accion2);
  }
  pila_desapilar(agenda->deshacer);
}


// Rehace la ultima accion deshecha en la agenda
void agenda_rehacer(Agenda agenda) {
  Accion accion = pila_tope(agenda->rehacer);
  if (accion != NULL) {
    Contacto *c = accion->contacto;

    Contacto *anterior = agenda_buscar(agenda, c->nombre, c->apellido);
    if (anterior != NULL) {
      anterior = contacto_copia((Contacto *) anterior);
    } else {
      anterior = contacto_copia(c);
    }

    struct _Accion accion2 =
        accion_crear(accion->inversa, accion->accion, anterior);

    // Realizo la accion que rehacer lo ultimo deshecho.
    void *res = accion->accion(agenda, c->nombre, c->apellido, c->edad, c->telefono);
    if (res) contacto_destruir((Contacto *) res);

    pila_apilar(agenda->deshacer, &accion2);
  }
  pila_desapilar(agenda->rehacer);
}

// AND
void agenda_and(Agenda agenda, char *nombre, char *apellido, int edad,
                char *tel) {
  GList lista = glist_crear();  // Aca se guardan los contactos que vayan verificando alguna condicion
  if (*nombre && *apellido) {   // Si se dio un nombre y un apellido, puedo buscar en la tabla hash
    Contacto *c = agenda_buscar(agenda, nombre, apellido);
    int puede = 1;
    if (c) {
      if (edad)
        if ((int) c->edad != edad) puede = 0;

      if (*tel)
        if (strcmp(c->telefono, tel)) puede = 0;

      if (puede) {
        lista =
            glist_agregar_inicio(lista, c, (FuncionCopiadora) copia_falsa);
      }
    }
  } else {
    Contacto *c = contacto_crear(nombre, apellido, edad, tel);
    if (*nombre) {              // Si se dio un nombre busco con eso
      lista = avl_buscar(agenda->nombre, c);
    } else if (*apellido) {     // Si no busco por apellido
      lista = avl_buscar(agenda->apellido, c);
    } else if (edad) {          // Si no busco por edad
      lista = avl_buscar(agenda->edad, c);
    } else if (*tel) {          // Si no busco por telefono
      lista = avl_buscar(agenda->telefono, c);
    }
    // Si se obtuvo algo, me quedo con los que cumplen todas las condiciones
    if (!glist_vacia(lista)) {
      lista =
          glist_filtrar(lista, (Predicado) contacto_comparar_full,
                        (FuncionCopiadora) copia_falsa,
                        (FuncionDestructora) destr_falso, c, 0);
    }
    contacto_destruir(c);
  }
  // Imprimo y destruyo la lista obtenida
  glist_recorrer(lista, (FuncionVisitanteExtra) contacto_imprimir_extra, NULL);
  glist_destruir(lista, (FuncionDestructora) destr_falso);
}


// OR
void agenda_or(Agenda agenda, char *nombre, char *apellido, int edad, char *tel) {
  GList lista = glist_crear();  // Aca se guardan los contactos que vayan verificando las condiciones
  Contacto *c = contacto_crear(nombre, apellido, edad, tel);
  if (*nombre) {                // Si se dio un nombre, imprimo los que lo tengan
    lista = avl_buscar(agenda->nombre, c);
    glist_recorrer(lista, (FuncionVisitanteExtra) contacto_imprimir_extra, NULL);
  }
  if (*apellido) {              // Si se dio un apellido, imprimo los que lo tengan sin repetir lo anterior
    lista = avl_buscar(agenda->apellido, c);
    lista =
        glist_filtrar(lista, (Predicado) contacto_comparar_nombre,
                      (FuncionCopiadora) copia_falsa,
                      (FuncionDestructora) destr_falso, c, 0);
    glist_recorrer(lista, (FuncionVisitanteExtra) contacto_imprimir_extra, NULL);
    glist_destruir(lista, (FuncionDestructora) destr_falso);
  }
  if (edad) {                   // Si se dio una edad, imprimo los que la tengan sin repetir lo anterior
    lista = avl_buscar(agenda->edad, c);
    lista =
        glist_filtrar(lista, (Predicado) contacto_comparar_nombre,
                      (FuncionCopiadora) copia_falsa,
                      (FuncionDestructora) destr_falso, c, 0);
    lista =
        glist_filtrar(lista, (Predicado) contacto_comparar_apellido,
                      (FuncionCopiadora) copia_falsa,
                      (FuncionDestructora) destr_falso, c, 1);
    glist_recorrer(lista, (FuncionVisitanteExtra) contacto_imprimir_extra, NULL);
    glist_destruir(lista, (FuncionDestructora) destr_falso);
  }
  if (*tel) {                   // Si se dio un telefono, imprimo los que lo tengan sin repetir lo anterior
    lista = avl_buscar(agenda->telefono, c);
    lista =
        glist_filtrar(lista, (Predicado) contacto_comparar_nombre,
                      (FuncionCopiadora) copia_falsa,
                      (FuncionDestructora) destr_falso, c, 0);
    lista =
        glist_filtrar(lista, (Predicado) contacto_comparar_apellido,
                      (FuncionCopiadora) copia_falsa,
                      (FuncionDestructora) destr_falso, c, 1);
    lista =
        glist_filtrar(lista, (Predicado) contacto_comparar_edad,
                      (FuncionCopiadora) copia_falsa,
                      (FuncionDestructora) destr_falso, c, 1);
    glist_recorrer(lista, (FuncionVisitanteExtra) contacto_imprimir_extra, NULL);
    glist_destruir(lista, (FuncionDestructora) destr_falso);
  }
  contacto_destruir(c);
}

// Recibe un arreglo con: un entero n, un entero pos y un arreglo de contactos.
// Si el contacto recibido tiene edad no mayor a n, lo agrega al arreglo.
static void sum_aux(void *dato, ArregloG arr1) {
  ArregloG arr = (ArregloG) arreglog_leer(arr1, 0);
  int n = *((int *) arreglog_leer(arr1, 1));
  int *pos = (int *) arreglog_leer(arr1, 2);
  if ((int) ((Contacto *) dato)->edad <= n) {
    arreglog_escribir(arr, *pos, dato);
    *pos += 1;
  }
}

// Imprime el conjunto encontrado
static void imprimir_subset(ArregloG tabla, int i, int j, ArregloG arr) {
  if (i && j) {
    // Si el valor donde arranca es 1
    if (*(int *) arreglog_leer(arreglog_leer(tabla, i), j)) {
      // Si en la fila superior tambien hay 1, aplico la funcion para esa fila
      if (*((int *) arreglog_leer(arreglog_leer(tabla, i - 1), j)) == 1) {
        imprimir_subset(tabla, i - 1, j, arr);
      } else {
        // Si no, imprimo el contacto y aplico la funcion a la fila de arriba,
        // columna j - edad.
        Contacto *c = (Contacto *) arreglog_leer(arr, i - 1);
        contacto_imprimir(c);
        imprimir_subset(tabla, i - 1, j - c->edad, arr);
      }
    }
  }
}

// Usamos programacion dinamica para resolverlo
static void evaluar_subset(ArregloG arr, int n, int suma) {
  // Creamos una tabla de n+1 x suma+1

  ArregloG tabla = arreglog_crear(n + 1, copia_falsa,
                                  (FuncionDestructora) arreglog_destruir);

  int uno = 1, cero = 0, ultimo = 0;

  // Creamos la fila 0, con todos 0 salvo en la primera columna
  ArregloG fila = arreglog_crear(suma + 1, copiar_entero, destruir_entero);
  arreglog_escribir(fila, 0, &uno);
  arreglog_escribir(tabla, 0, fila);
  for (int i = 1; i <= suma; ++i) {
    arreglog_escribir(arreglog_leer(tabla, 0), i, &cero);
  }

  // Mientras no hallemos un resultado, seguimos agregando filas
  for (int i = 1; i <= n && !ultimo; ++i) {
    ArregloG fila = arreglog_crear(suma + 1, copiar_entero, destruir_entero);
    arreglog_escribir(fila, 0, &uno);
    arreglog_escribir(tabla, i, fila);
    // Rellenamos las columnas de la nueva fila
    for (int j = 1; j <= suma; ++j) {
      // Si el numero de columna es menor a la edad, hereda el valor de la fila superior
      if (j < (int) ((Contacto *) arreglog_leer(arr, i - 1))->edad)
        arreglog_escribir(arreglog_leer(tabla, i), j,
                          arreglog_leer(arreglog_leer(tabla, i - 1), j));
      // Si no, hereda el valor de arriba si es 1.
      // O el valor de la fila superior, columna n veces a la izquierda (n=edad del contacto correspondiente a la fila) 
      if (j >= (int) ((Contacto *) arreglog_leer(arr, i - 1))->edad) {
        int *sup = (int *) arreglog_leer(arreglog_leer(tabla, i - 1), j);
        int *ant = (int *) arreglog_leer(arreglog_leer(tabla, i - 1),
                                         j - ((Contacto *)
                                              arreglog_leer(arr, i - 1))->edad);
        int valor = *sup || *ant;
        arreglog_escribir(arreglog_leer(tabla, i), j, &valor);
      }
    }
    // Si el ultimo valor de la fila es 1, encontre un conjunto de contactos.
    if (*((int *) arreglog_leer(arreglog_leer(tabla, i), suma)))
      ultimo = i;
  }

  // Imprimo el conjunto encontrado, si existe.
  imprimir_subset(tabla, ultimo, suma, arr);

  arreglog_destruir(tabla);
}

// Dado un entero n, si hay un subconjunto de contactos en la agenda
// cuyas edades sumen n, los imprime.
void agenda_suma_edades(Agenda agenda, int n) {
  int pos = 0;
  // Este arreglo va a contener todos los contactos de edad no mayor a n.
  ArregloG arr = arreglog_crear(agenda->th->numElems, copia_falsa, destr_falso);

  ArregloG arr1 = arreglog_crear(3, copia_falsa, destr_falso);
  arreglog_escribir(arr1, 0, arr);
  arreglog_escribir(arr1, 1, &n);
  arreglog_escribir(arr1, 2, &pos);

  // Llenamos el arreglo.
  avl_recorrer(agenda->edad, AVL_RECORRIDO_IN, (FuncionVisitanteExtra) sum_aux,
               arr1);
  arreglog_destruir(arr1);

  // Evaluamos si hay algun subconjunto de contactos cuyas edades sumen n
  evaluar_subset(arr, pos, n);
  arreglog_destruir(arr);
}


// Imprime la agenda
void agenda_imprimir(Agenda agenda) {
  avl_recorrer(agenda->nombre, AVL_RECORRIDO_IN,
               (FuncionVisitanteExtra) contacto_imprimir_extra, NULL);
}
