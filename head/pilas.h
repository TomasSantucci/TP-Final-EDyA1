#ifndef __PILAS_H__
#define __PILAS_H__

typedef void *(*FuncionCopiadora) (void *dato);
typedef void (*FuncionDestructora) (void *dato);

typedef struct _Pila *Pila;

// Crea una nueva pila vacia.
Pila pila_crear(int tamanio, FuncionCopiadora copia, FuncionDestructora destr);

// Destruye una pila.
void pila_destruir(Pila pila);

// Chequea si una pila es vacia.
int pila_es_vacia(Pila pila);

// Devuelve el elemento del tope de la pila.
void *pila_tope(Pila pila);

// Agrega un elemento a la pila.
void pila_apilar(Pila pila, void *dato);

// Quita un elemento de la pila.
void pila_desapilar(Pila pila);

// Vacia la pila.
void pila_vaciar(Pila pila);


#endif                          /* __PILAS_H__ */
