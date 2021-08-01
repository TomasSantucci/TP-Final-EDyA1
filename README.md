# Agenda de contactos

## MODULOS DEL PROGRAMA:
Estructuras de datos (definiciones de funciones)
Utils (funciones varias utiles para diferentes archivos)
Agenda (funciones para manejar una agenda)
Parser (Crea el bucle interactivo del programa)
Test (casos de prueba de todas las funciones principales)

## ESTRUCTURAS DE DATOS USADAS:
Arreglo dinamico: para crear una pila de tamaño fijo que se vaya pisando.

### Pila: para crear el algoritmo deshacer/rehacer que necesita una estructura LIFO.

### Lista enlazada simple: para ir guardando datos que no necesitan ningun requisito. Se usa para guardar datos repetidos en los arboles AVL.

### Arbol AVL: para buscar contactos por atributos rapidamente y tambien tenerlos ordenados.

### Tabla hash: Se usa como estructura principal. Para buscar rapidamente en la agenda y guardar la copia original de todos los contactos.

## ALGORITMOS:
Deshacer/Rehacer: Guardo en una pila, la funcion que realice y su inversa. Luego saco de la pila la funcion y los datos que cambie.

Guardar ordenado: Como tengo los contactos ordenados por cada atributo en arboles AVL, simplemente los recorro de manera inorder.

Buscar por suma de edades: Uso programación dinámica (bottom-up). Veo que suma puedo generar con el primer contacto, luego si agrego el segundo, y siguiendo asi hasta lograr la suma o llegar al ultimo contacto.


## DIFICULTADES ENCONTRADAS:
La dificultad mas grande fue encontrar una forma de representar la agenda de manera que todas las acciones se realicen en un tiempo aceptable. Particularmente por AND y OR.
Lo resolvi creando una estructura principal (tabla hash) con arboles avl auxiliares que mantienen los contactos ordenados y facil de buscar por atributos


## COMPILACION E INVOCACION DEL PROGRAMA:
Ejecutar make para compilarlo.
Ejecutar bin/proyecto para invocarlo.
Ejecutar make test para usar valgrind.


## BIBLIOGRAFIA CONSULTADA:
Slides y plantillas de estructuras de la catedra.

## OBSERVACION:
Para ejecutar los casos de prueba, descomentar la funcion test en el archivo main.c