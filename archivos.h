#ifndef ARCHIVOS_H
#define ARCHIVOS_H

#include "comando.h"
#include "lista.h"

// =============================================================
// MODULO DE ARCHIVOS
// =============================================================
// Maneja la persistencia del sistema: los archivos binarios .dat
// de comandos (con cualquier nombre, acceso aleatorio y apareo),
// el archivo de texto sesion.txt y el listado con corte de control
// por categoria.
// =============================================================

const int MAX_NOMBRE_ARCHIVO = 100;   // tamano de un nombre de archivo (incluye '\0')
const int MAX_ENTRADA_NOMBRE = 90;    // tamano del buffer donde se tipea el nombre (deja lugar para ".dat")
const int MAX_ARCHIVOS_DAT = 50;      // maximo de archivos .dat que se listan

// --- Nombres de archivo ---
// Arma un nombre valido: recorta espacios y agrega ".dat" si no lo tiene ("pruebas" -> "pruebas.dat")
void armarNombreDat(const char* entrada, char* destino, int tamDestino);

bool existeArchivo(const char* nombre);

// Detecta los .dat de la carpeta actual (ordenados alfabeticamente). Devuelve cuantos encontro (hasta maxNombres).
int detectarArchivosDat(char nombres[][MAX_NOMBRE_ARCHIVO], int maxNombres);

// --- Archivo binario (nombre elegido) ---
void guardarEnArchivoBinario(Comando arreglo[], int cant, const char* nombreArchivo);

// Lee hasta MAX_COMANDOS registros. Devuelve 0 si pudo abrir el archivo, -1 si no.
int leerDesdeArchivoBinario(Comando arreglo[], int& cant, const char* nombreArchivo);

// --- Acceso aleatorio (seekg/seekp) sobre el .dat elegido ---
void modificarRespuestaRegistroN(const char* nombreArchivo, int nRegistro, const char* nuevaRespuesta);

// --- Apareo de dos .dat ---
// Ambos se ordenan por palabra clave (en memoria) si no lo estaban; el resultado queda ordenado por palabra clave.
// Un mismo comando (misma palabra clave) queda una sola vez con los usos sumados.
// Devuelve la cantidad de comandos del archivo nuevo, o -1 si hubo un error.
int aparearArchivos(const char* archivoA, const char* archivoB, const char* archivoSalida);

// --- Archivo de texto: log de sesion ---
void exportarSesionTXT(NodoHistorial* cabeza);

// --- Corte de control por categoria (requiere el .dat ordenado por categoria) ---
void listadoPorCategoria(const char* nombreArchivo);

#endif
