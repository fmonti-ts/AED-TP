#ifndef COLA_H
#define COLA_H

#include <iosfwd>

// =============================================================
// BIBLIOTECA PROPIA DE COLA (FIFO) - lista enlazada
// =============================================================
// Los datos de la cola son strings de hasta 49 caracteres + '\0'
// (los textos mas largos se truncan al encolar).
// La cola se pasa siempre por referencia; no hay variables globales.
// =============================================================

const int LARGO_TEXTO_COLA = 50;

struct NodoCola {
    char dato[LARGO_TEXTO_COLA];
    NodoCola* siguiente;
};

struct Cola {
    NodoCola* frente;   // proximo en salir
    NodoCola* fin;      // ultimo en haber entrado
};

void crearCola(Cola& c);

// Agrega el texto al final de la cola (truncado a 49 caracteres).
void encolar(Cola& c, const char* texto);

// Saca el primero y lo copia en destino (min. LARGO_TEXTO_COLA chars). false si estaba vacia.
bool desencolar(Cola& c, char* destino);

// Copian el primero / ultimo en destino SIN sacarlo. false si esta vacia.
bool traerPrimero(const Cola& c, char* destino);
bool traerUltimo(const Cola& c, char* destino);

// Muestra por pantalla el ultimo elemento de la cola (sin sacarlo).
void mostrarFinal(const Cola& c);

// Recorre la cola de frente a fin escribiendo un elemento por linea en 'salida'
// (sirve para cout o para un ofstream). No modifica la cola.
void recorrerCola(const Cola& c, std::ostream& salida);

void liberarCola(Cola& c);

bool estaVacio(const Cola& c);

#endif
