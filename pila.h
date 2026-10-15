#ifndef PILA_H
#define PILA_H

#include <iosfwd>

// =============================================================
// BIBLIOTECA PROPIA DE PILA (LIFO) - lista enlazada
// =============================================================
// Los datos de la pila son respuestas de tipo char[100]
// (99 caracteres + '\0'; los textos mas largos se truncan).
// La pila se pasa siempre por referencia; no hay variables globales.
// =============================================================

const int LARGO_RESPUESTA_PILA = 100;

struct NodoPila {
    char respuesta[LARGO_RESPUESTA_PILA];
    NodoPila* siguiente;
};

struct Pila {
    NodoPila* tope;
};

void crearPila(Pila& p);

void apilar(Pila& p, const char* respuesta);

// Saca el tope y lo copia en destino (min. LARGO_RESPUESTA_PILA chars). false si estaba vacia.
bool desapilar(Pila& p, char* destino);

bool estaVacia(const Pila& p);

// Copia el tope en destino SIN sacarlo. false si esta vacia.
bool traerPrimero(const Pila& p, char* destino);

// Recorre la pila de tope a base escribiendo un elemento por linea en 'salida'.
// No modifica la pila.
void recorrerPila(const Pila& p, std::ostream& salida);

void liberarPila(Pila& p);

#endif
