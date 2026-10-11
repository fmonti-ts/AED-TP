#ifndef LISTA_H
#define LISTA_H

// =============================================================
// BIBLIOTECA PROPIA DE LISTA ENLAZADA (HISTORIAL DE INTERACCIONES)
// =============================================================
// Modulo independiente y reutilizable. Contiene el struct Nodo
// y todas las operaciones sobre la lista enlazada que representa
// el historial completo de interacciones de la sesion.
// =============================================================

struct NodoHistorial {
    int idInteraccion;
    char mensajeUsuario[100];
    char respuestaAsistente[100];
    NodoHistorial* siguiente;
};

// --- Creacion de nodo ---
NodoHistorial* crearNodo(int id, const char* msg, const char* resp);

// --- Alta ---
void agregarInteraccionHistorial(NodoHistorial*& cabeza, int id, const char* msg, const char* resp);

// --- Recorrido y muestra ---
void mostrarHistorialCompleto(NodoHistorial* cabeza);

// --- Busqueda por palabra clave ---
void buscarInteraccionPorPalabraClave(NodoHistorial* cabeza, const char* palabra);

// --- Baja ---
void eliminarInteraccion(NodoHistorial*& cabeza, int id);

// --- Liberacion de memoria ---
void liberarHistorial(NodoHistorial*& cabeza);

#endif
