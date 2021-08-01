#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../head/parser.h"
#include "../head/utils.h"
#include "../head/agenda.h"
#include "../head/contacto.h"

// Compara una cadena y devuelve un numero correspondiente.
static int orden(char *str) {
  if (!strcmp(str, "nombre"))
    return 0;
  if (!strcmp(str, "apellido"))
    return 1;
  if (!strcmp(str, "edad"))
    return 2;
  if (!strcmp(str, "telefono"))
    return 3;
  return 0;
}

// Lee una cadena y le quita el salto de linea final.
static void leer_cadena(char *cadena, int capacidad, FILE * stream) {
  fgets(cadena, capacidad, stream);
  cadena[strcspn(cadena, "\n")] = 0;
}

// Lee un entero.
static void leer_entero(int *n) {
  scanf("%d", n);
  getchar();
}

// Lee una edad, si es invalida vuelve a pedirla.
static int leer_edad() {
  int edad;
  leer_entero(&edad);
  while (edad < 1 || edad > 150) {
    puts("Edad invalida, devuelta...");
    leer_entero(&edad);
  }
  return edad;
}

// Lee una cadena cuyos caracteres estan validados por una funcion dada.
static void leer_dato(char *cad, int (*validar) (int)) {
  leer_cadena(cad, 30, stdin);
  int valido = cadena_homo(cad, validar);
  while (!valido) {
    puts("Cadena invalida, devuelta... ");
    leer_cadena(cad, 30, stdin);
    valido = cadena_homo(cad, validar);
  }
}

// Lee una cadena cuyos caracteres estan validados por una funcion dada.
// La cadena no puede ser vacia.
static void leer_dato_no_vacio(char *cad, int (*validar) (int)) {
  leer_dato(cad,validar);
  while (!strcmp(cad, "")){
    printf("El dato no puede ser vacio.\n>");
    leer_dato(cad,validar);
  }
}

// Imprime el mensaje inicial.
static void mensaje_inicial() {
  puts("Menu de acciones:");
  puts("1. Buscar");
  puts("2. Agregar");
  puts("3. Eliminar");
  puts("4. Editar");
  puts("5. Cargar");
  puts("6. Guardar");
  puts("7. Deshacer");
  puts("8. Rehacer");
  puts("9. AND");
  puts("10. OR");
  puts("11. Guardar ordenado");
  puts("12. Buscar por suma de edades");
  puts("13. Salir");
  puts("");
}

// Inicia el bucle de interaccion con el programa.
void parser(Agenda agenda) {
  int accion = 1, n;
  char nombre[30], apellido[30], tel[30], ruta[100], ord[15], ed[10];
  unsigned edad;

  mensaje_inicial();

  printf("Seleccione una accion:\n>");
  leer_entero(&accion);
  while (accion != 13) {
    switch (accion) {
    case 1:                    // Buscar
      printf("Ingrese nombre:\n>");
      leer_dato_no_vacio(nombre, isalpha_or_space);
      printf("Ingrese apellido:\n>");
      leer_dato_no_vacio(apellido, isalpha_or_space);
      Contacto *c1 = agenda_buscar(agenda, nombre, apellido);
      if (c1)
        contacto_imprimir((Contacto *) c1);
      else
        printf("El contacto no se encuentra.\n");
      break;
    case 2:                    // Agregar
      printf("Ingrese nombre:\n>");
      leer_dato_no_vacio(nombre, isalpha_or_space);
      printf("Ingrese apellido:\n>");
      leer_dato_no_vacio(apellido, isalpha_or_space);
      printf("Ingrese edad:\n>");
      edad = leer_edad();
      printf("Ingrese telefono:\n>");
      leer_dato_no_vacio(tel, isdigit);
      agenda_agregar(agenda, nombre, apellido, edad, tel);
      break;
    case 3:                    // Eliminar
      printf("Ingrese nombre:\n>");
      leer_dato_no_vacio(nombre, isalpha_or_space);
      printf("Ingrese apellido:\n>");
      leer_dato_no_vacio(apellido, isalpha_or_space);
      agenda_eliminar(agenda, nombre, apellido);
      break;
    case 4:                    // Editar
      printf("Ingrese nombre:\n>");
      leer_dato_no_vacio(nombre, isalpha_or_space);
      printf("Ingrese apellido:\n>");
      leer_dato_no_vacio(apellido, isalpha_or_space);
      Contacto *c2 = agenda_buscar(agenda, nombre, apellido);
      if (c2) {
        contacto_imprimir(c2);

        printf("Ingrese edad:\n>");
        edad = leer_edad();
        printf("Ingrese telefono:\n>");
        leer_dato_no_vacio(tel, isdigit);

        agenda_editar(agenda, nombre, apellido, edad, tel);
      } else
        printf("El contacto no se encuentra.\n");
      break;
    case 5:                    // Cargar
      printf("Ingrese ruta de entrada:\n>");
      leer_cadena(ruta, 100, stdin);
      FILE *fpt1 = fopen(ruta, "r+");
      if (fpt1) {
        agenda_cargar(agenda, fpt1);
        fclose(fpt1);
      } else
        printf("Ruta incorrecta.\n");
      break;
    case 6:                    // Guardar
      printf("Ingrese ruta de salida:\n>");
      leer_cadena(ruta, 100, stdin);
      FILE *fpt2 = fopen(ruta, "w+");
      if (fpt2) {
        agenda_guardar(agenda, 0, fpt2);
        fclose(fpt2);
      } else
        printf("Ruta incorrecta.\n");
      break;
    case 7:                    // Deshacer
      agenda_deshacer(agenda);
      break;
    case 8:                    // Rehacer
      agenda_rehacer(agenda);
      break;
    case 9:                    // AND
      printf
          ("(Si desea dejar un atributo vacio, presione 'Enter' cuando corresponda).\n");
      printf("Ingrese nombre:\n>");
      leer_dato(nombre, isalpha_or_space);
      printf("Ingrese apellido:\n>");
      leer_dato(apellido, isalpha_or_space);
      printf("Ingrese edad:\n>");
      leer_dato(ed, isdigit);
      if (!(strcmp(ed, "")))
        edad = 0;
      else
        edad = abs(atoi(ed));
      printf("Ingrese telefono:\n>");
      leer_dato(tel, isdigit);
      if (*nombre || *apellido || edad || *tel)
        agenda_and(agenda, nombre, apellido, edad, tel);
      else
        printf("Algun atributo debe ser no vacio.\n");
      break;
    case 10:                   // OR
      printf
          ("(Si desea dejar un atributo vacio, presione 'Enter' cuando corresponda).\n");
      printf("Ingrese nombre:\n>");
      leer_dato(nombre, isalpha_or_space);
      printf("Ingrese apellido:\n>");
      leer_dato(apellido, isalpha_or_space);
      printf("Ingrese edad:\n>");
      leer_dato(ed, isdigit);
      if (!(strcmp(ed, "")))
        edad = 0;
      else
        edad = abs(atoi(ed));
      printf("Ingrese telefono:\n>");
      leer_dato(tel, isdigit);
      if (*nombre || *apellido || edad || *tel)
        agenda_or(agenda, nombre, apellido, edad, tel);
      else
        printf("Algun atributo debe ser no vacio.\n");
      break;
    case 11:                   // Guardar ordenado
      printf("Ingrese ruta de salida:\n>");
      leer_cadena(ruta, 100, stdin);
      FILE *fpt3 = fopen(ruta, "w+");
      if (fpt3) {
        printf
            ("Ingrese nombre de atributo: (nombre,apellido,edad,telefono)\n>");
        leer_cadena(ord, 15, stdin);
        agenda_guardar(agenda, orden(ord), fpt3);
        fclose(fpt3);
      } else
        printf("Ruta incorrecta.\n");
      break;
    case 12:                   // Buscar por suma de edades
      printf("Ingrese un natural:\n>");
      leer_entero(&n);
      if (n > 0)
        agenda_suma_edades(agenda, n);
      break;
    }

    puts("");
    printf("Seleccione una accion:\n>");
    leer_entero(&accion);
  }
}
