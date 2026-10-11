#ifndef ARCHIVOS_H
#define ARCHIVOS_H

#include "comando.h"
#include "lista.h"

// =============================================================
// MODULO DE ARCHIVOS
// =============================================================
// Maneja la persistencia del sistema: el archivo binario
// comandos.dat (fuente de verdad, con acceso aleatorio), el
// archivo de texto sesion.txt y el listado con corte de control
// por categoria.
// =============================================================

// --- Archivo binario ---
void guardarEnArchivoBinario(Comando arreglo[], int cant);

void leerDesdeArchivoBinario(Comando arreglo[], int& cant);

// --- Acceso aleatorio (seekg/seekp) sobre comandos.dat ---
void modificarRespuestaRegistroN(int nRegistro, const char* nuevaRespuesta);

// --- Archivo de texto: log de sesion ---
void exportarSesionTXT(NodoHistorial* cabeza);

// --- Corte de control por categoria (requiere comandos.dat ordenado por categoria) ---
void listadoPorCategoriaConCorte();

#endif
