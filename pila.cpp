#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include <cstring>
#include "pila.h"

using namespace std;

void crearPila(Pila& p) {
    p.tope = NULL;
}

bool estaVacia(const Pila& p) {
    return p.tope == NULL;
}

void apilar(Pila& p, const char* respuesta) {
    NodoPila* nuevo = new NodoPila;
    strncpy(nuevo->respuesta, respuesta, LARGO_RESPUESTA_PILA - 1);
    nuevo->respuesta[LARGO_RESPUESTA_PILA - 1] = '\0';
    nuevo->siguiente = p.tope;
    p.tope = nuevo;
}

bool desapilar(Pila& p, char* destino) {
    if (estaVacia(p)) return false;
    NodoPila* aBorrar = p.tope;
    strcpy(destino, aBorrar->respuesta);
    p.tope = aBorrar->siguiente;
    delete aBorrar;
    return true;
}

bool traerPrimero(const Pila& p, char* destino) {
    if (estaVacia(p)) return false;
    strcpy(destino, p.tope->respuesta);
    return true;
}

void recorrerPila(const Pila& p, ostream& salida) {
    NodoPila* aux = p.tope;
    while (aux != NULL) {
        salida << aux->respuesta << "\n";
        aux = aux->siguiente;
    }
}

void liberarPila(Pila& p) {
    while (p.tope != NULL) {
        NodoPila* aux = p.tope;
        p.tope = p.tope->siguiente;
        delete aux;
    }
}
