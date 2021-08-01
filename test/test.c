#include <stdio.h>

void test(Agenda agenda) {
  FILE *fpt1 = fopen("test/contactos.csv", "r+");
  printf("Carga de contactos.\n");
  agenda_cargar(agenda, fpt1);
  agenda_imprimir(agenda);
  puts("");

  puts("Agrego 3 contactos de a uno y edito uno.");
  agenda_agregar(agenda, "Carlos", "Perez", 18, "3364111111");
  agenda_agregar(agenda, "Maria Susana", "Torres", 89, "3364233333");
  agenda_agregar(agenda, "Juan", "Torres", 18, "3364111111");
  agenda_editar(agenda, "Carlos", "Perez", 18, "3364000000");
  agenda_imprimir(agenda);
  puts("");

  printf("Deshacer\n");
  agenda_deshacer(agenda);
  agenda_imprimir(agenda);
  puts("");

  printf("Deshacer\n");
  agenda_deshacer(agenda);
  agenda_imprimir(agenda);
  puts("");

  printf("Rehacer\n");
  agenda_rehacer(agenda);
  agenda_imprimir(agenda);
  puts("");

  printf("Deshacer\n");
  agenda_deshacer(agenda);
  agenda_imprimir(agenda);
  puts("");

  printf("Deshacer\n");
  agenda_deshacer(agenda);
  agenda_imprimir(agenda);
  puts("");

  printf("Rehacer\n");
  agenda_rehacer(agenda);
  agenda_imprimir(agenda);
  puts("");

  printf("Rehacer\n");
  agenda_rehacer(agenda);
  agenda_imprimir(agenda);
  puts("");

  printf("Rehacer\n");
  agenda_rehacer(agenda);
  agenda_imprimir(agenda);
  puts("");

  printf("Busco a Carlos Torres:\n");
  Contacto *c2 = agenda_buscar(agenda, "Carlos", "Torres");
  if (c2)
    contacto_imprimir((Contacto *) c2);
  else
    printf("El contacto no se encuentra.\n");
  puts("");

  printf("Busco a Delfina Perez:\n");
  Contacto *c3 = agenda_buscar(agenda, "Delfina", "Perez");
  if (c3)
    contacto_imprimir((Contacto *) c3);
  else
    printf("El contacto no se encuentra.\n");
  puts("");

  printf("Elimino a Maria Susana Torres y luego la busco.\n");
  agenda_eliminar(agenda, "Maria Susana", "Torres");
  Contacto *c4 = agenda_buscar(agenda, "Maria Susana", "Torres");
  if (c4)
    contacto_imprimir((Contacto *) c4);
  else
    printf("El contacto no se encuentra.\n");
  puts("");

  printf("Deshacer.\n");
  agenda_deshacer(agenda);
  puts("");

  printf("Guardo la agenda en el archivo guardo.csv ordenada por edad.\n");
  agenda_imprimir(agenda);
  FILE *fpt2 = fopen("test/guardo.csv", "w+");
  agenda_guardar(agenda, 2, fpt2);
  fclose(fpt1);
  fclose(fpt2);
  puts("");
  puts("");

  printf("AND\n");
  printf("vacio,Perez,vacio,vacio\n");
  agenda_and(agenda, "", "Perez", 0, "");
  puts("");

  printf("vacio,Perez,18,vacio\n");
  agenda_and(agenda, "", "Perez", 18, "");
  puts("");

  printf("Mateo,vacio,37,vacio\n");
  agenda_and(agenda, "Mateo", "", 37, "");
  puts("");

  printf("vacio,Acosta,18,vacio\n");
  agenda_and(agenda, "", "Acosta", 18, "");
  puts("");

  printf("Maria,Diaz,57,vacio\n");
  agenda_and(agenda, "Maria", "Diaz", 57, "");
  puts("");

  printf("Maria,Diaz,56,vacio\n");
  agenda_and(agenda, "Maria", "Diaz", 56, "");
  puts("");
  puts("");

  printf("OR\n");
  printf("Juan,vacio,37,vacio\n");
  agenda_or(agenda, "Juan", "", 37, "");
  puts("");

  printf("vacio,Perez,vacio,3364999999\n");
  agenda_or(agenda, "", "Perez", 0, "3364999999");
  puts("");

  printf("vacio,vacio,18,vacio\n");
  agenda_or(agenda, "", "", 18, "");
  puts("");

  printf("vacio,vacio,vacio,3364100000\n");
  agenda_or(agenda, "", "", 0, "3364100000");
  puts("");

  printf("Pedro,vacio,18,vacio\n");
  agenda_or(agenda, "Pedro", "", 18, "");
  puts("");

  printf("Pedro,vacio,15,vacio\n");
  agenda_or(agenda, "Pedro", "", 15, "");
  puts("");
  puts("");

  printf("BUSCAR POR SUMA DE EDADES\n");
  printf("SUMA = 36\n");
  agenda_suma_edades(agenda, 36);
  puts("");
  printf("SUMA = 3\n");
  agenda_suma_edades(agenda, 3);
  puts("");
  printf("SUMA = 258\n");
  agenda_suma_edades(agenda, 258);
  puts("");
  printf("SUMA = 221\n");
  agenda_suma_edades(agenda, 221);
  puts("");
  puts("");

  printf("SUMA = 178\n");
  agenda_suma_edades(agenda, 178);
  puts("");
  printf("SUMA = 6\n");
  agenda_suma_edades(agenda, 6);
  puts("");
  printf("SUMA = 9\n");
  agenda_suma_edades(agenda, 9);
  puts("");
  printf("SUMA = 27\n");
  agenda_suma_edades(agenda, 27);
  puts("");
  printf("SUMA = 90\n");
  agenda_suma_edades(agenda, 90);
  puts("");
  printf("SUMA = 146\n");
  agenda_suma_edades(agenda, 146);
  puts("");
}