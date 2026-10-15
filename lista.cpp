#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include <cstring>
#include "lista.h"

using namespace std;

NodoHistorial* crearNodo(int id, const char* msg, const char* resp) {
    NodoHistorial* nuevo = new NodoHistorial;
    nuevo->idInteraccion = id;
    strcpy(nuevo->mensajeUsuario, msg);
    strcpy(nuevo->respuestaAsistente, resp);
    nuevo->siguiente = NULL;
    return nuevo;
}

void agregarInteraccionHistorial(NodoHistorial*& cabeza, int id, const char* msg, const char* resp) {
    NodoHistorial* nuevo = crearNodo(id, msg, resp);
    if (cabeza == NULL) {
        cabeza = nuevo;
    }
    else {
        NodoHistorial* aux = cabeza;
        while (aux->siguiente != NULL) {
            aux = aux->siguiente;
        }
        aux->siguiente = nuevo;
    }
}

void mostrarHistorialCompleto(NodoHistorial* cabeza) {
    if (cabeza == NULL) {
        cout << "\n[HISTORIAL VACIO]\n";
        return;
    }
    cout << "\n=== HISTORIAL DE LA SESION (LISTA ENLAZADA) ===\n";
    NodoHistorial* aux = cabeza;
    while (aux != NULL) {
        cout << "ID: " << aux->idInteraccion
            << " | Usuario: \"" << aux->mensajeUsuario << "\""
            << " -> IA: \"" << aux->respuestaAsistente << "\"\n";
        aux = aux->siguiente;
    }
}

void buscarInteraccionPorPalabraClave(NodoHistorial* cabeza, const char* palabra) {
    cout << "\n--- Buscando '" << palabra << "' en el Historial ---\n";
    NodoHistorial* aux = cabeza;
    bool encontrado = false;
    while (aux != NULL) {
        if (strstr(aux->mensajeUsuario, palabra) != NULL) {
            cout << "Encontrado -> ID: " << aux->idInteraccion
                << " | Mensaje: " << aux->mensajeUsuario << "\n";
            encontrado = true;
        }
        aux = aux->siguiente;
    }
    if (!encontrado) cout << "No se encontraron coincidencias en el historial.\n";
}

void eliminarInteraccion(NodoHistorial*& cabeza, int id) {
    if (cabeza == NULL) return;

    if (cabeza->idInteraccion == id) {
        NodoHistorial* aBorrar = cabeza;
        cabeza = cabeza->siguiente;
        delete aBorrar;
        cout << "Interaccion " << id << " eliminada del historial.\n";
        return;
    }

    NodoHistorial* aux = cabeza;
    while (aux->siguiente != NULL && aux->siguiente->idInteraccion != id) {
        aux = aux->siguiente;
    }

    if (aux->siguiente != NULL) {
        NodoHistorial* aBorrar = aux->siguiente;
        aux->siguiente = aux->siguiente->siguiente;
        delete aBorrar;
        cout << "Interaccion " << id << " eliminada del historial.\n";
    }
    else {
        cout << "ID no encontrado en el historial.\n";
    }
}

void liberarHistorial(NodoHistorial*& cabeza) {
    while (cabeza != NULL) {
        NodoHistorial* aux = cabeza;
        cabeza = cabeza->siguiente;
        delete aux;
    }
    cout << "\nMemoria dinamica del historial liberada correctamente.\n";
}

