#include "../head/utils.h"
#include "../head/agenda.h"
#include "../head/contacto.h"
#include "../head/parser.h"
#include "../test/test.c"

int main() {
  // Creo una agenda.
  Agenda agenda = agenda_crear((FuncionCopiadora) copia_falsa,
                               (FuncionComparadora) contacto_comparar,
                               (FuncionComparadora)
                               contacto_comparar_nombre_con_lista,
                               (FuncionComparadora)
                               contacto_comparar_apellido_con_lista,
                               (FuncionComparadora)
                               contacto_comparar_edad_con_lista,
                               (FuncionComparadora)
                               contacto_comparar_telefono_con_lista,
                               (FuncionDestructora) contacto_destruir);

  // Inicio el bucle principal del programa.
  parser(agenda);

  // Descomentar para ver los casos de prueba.
  //test(agenda);

  // Destruyo la agenda.
  agenda_destruir(agenda);
  return 0;
}