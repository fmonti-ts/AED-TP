#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include <cstring>
#include "cola.h"

using namespace std;

void crearCola(Cola& c) {
    c.frente = NULL;
    c.fin = NULL;
}

bool estaVacio(const Cola& c) {
    return c.frente == NULL;
}

void encolar(Cola& c, const char* texto) {
    NodoCola* nuevo = new NodoCola;
    strncpy(nuevo->dato, texto, LARGO_TEXTO_COLA - 1);
    nuevo->dato[LARGO_TEXTO_COLA - 1] = '\0';
    nuevo->siguiente = NULL;

    if (c.fin == NULL) {      // cola vacia
        c.frente = nuevo;
    }
    else {
        c.fin->siguiente = nuevo;
    }
    c.fin = nuevo;
}

bool desencolar(Cola& c, char* destino) {
    if (estaVacio(c)) return false;
    NodoCola* aBorrar = c.frente;
    strcpy(destino, aBorrar->dato);
    c.frente = aBorrar->siguiente;
    if (c.frente == NULL) c.fin = NULL;   // quedo vacia
    delete aBorrar;
    return true;
}

bool traerPrimero(const Cola& c, char* destino) {
    if (estaVacio(c)) return false;
    strcpy(destino, c.frente->dato);
    return true;
}

bool traerUltimo(const Cola& c, char* destino) {
    if (estaVacio(c)) return false;
    strcpy(destino, c.fin->dato);
    return true;
}

void mostrarFinal(const Cola& c) {
    if (estaVacio(c)) {
        cout << "[COLA VACIA] No hay elemento final.\n";
        return;
    }
    cout << "Final de la cola: " << c.fin->dato << "\n";
}

void recorrerCola(const Cola& c, ostream& salida) {
    NodoCola* aux = c.frente;
    while (aux != NULL) {
        salida << aux->dato << "\n";
        aux = aux->siguiente;
    }
}

void liberarCola(Cola& c) {
    while (c.frente != NULL) {
        NodoCola* aux = c.frente;
        c.frente = c.frente->siguiente;
        delete aux;
    }
    c.fin = NULL;
}
