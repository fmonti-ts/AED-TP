#ifndef ARCHIVOS_H
#define ARCHIVOS_H

#include "comando.h"
#include "lista.h"
#include "cola.h"
#include "pila.h"

// =============================================================
// MODULO DE ARCHIVOS
// =============================================================
// Maneja la persistencia del sistema: los archivos binarios .dat
// de comandos (con cualquier nombre, acceso aleatorio y apareo),
// los archivos de texto (sesion, usuario y simulacro) y el
// listado con corte de control por categoria. Tambien pide por
// teclado los nombres de archivo.
// =============================================================

const int MAX_NOMBRE_ARCHIVO = 100;   // tamano de un nombre de archivo (incluye '\0')
const int MAX_ENTRADA_NOMBRE = 90;    // tamano del buffer donde se tipea el nombre (deja lugar para ".dat")
const int MAX_ARCHIVOS_DAT = 50;      // maximo de archivos .dat que se listan

// --- Nombres de archivo ---
// Arma un nombre valido: recorta espacios y agrega ".dat" si no lo tiene ("pruebas" -> "pruebas.dat")
void armarNombre(const char* entrada, char* destino, int tamDestino);

bool existeArchivo(const char* nombre);

// Detecta los .dat de la carpeta actual (ordenados alfabeticamente). Devuelve cuantos encontro (hasta maxNombres).
int detectarArchivos(char nombres[][MAX_NOMBRE_ARCHIVO], int maxNombres);

void listarArchivos(char nombres[][MAX_NOMBRE_ARCHIVO], int n);

// Detecta y muestra los .dat de la carpeta actual. Devuelve cuantos hay.
int listarArchivosDat();

// --- Pedir nombres de archivo por teclado (enter vacio = cancelar, devuelven false) ---
bool pedirNombreDat(const char* mensaje, char* destino);

// Repite hasta que el nombre tipeado corresponda a un archivo que existe.
bool pedirArchivoExistente(const char* mensaje, char* destino);

// Si todavia no hay archivo actual, lo pide. Devuelve false si se cancelo.
bool asegurarArchivoActual(char* archivoActual);

// --- Archivo binario (nombre elegido) ---
// Devuelve false si no pudo crear el archivo.
bool guardarEnArchivoBinario(Comando arreglo[], int cant, const char* nombreArchivo);

// Lee hasta MAX_COMANDOS registros. Devuelve 0 si pudo abrir el archivo, -1 si no.
int leerDesdeArchivoBinario(Comando arreglo[], int& cant, const char* nombreArchivo);

// --- Acceso aleatorio (seekg/seekp) sobre el .dat elegido ---
void modificarRespuestaRegistroN(const char* nombreArchivo, int nRegistro, const char* nuevaRespuesta);

// --- Apareo de dos .dat ---
// Ambos se ordenan por palabra clave (en memoria) si no lo estaban.
// Un mismo comando (misma palabra clave) queda una sola vez con los usos sumados.
// El resultado queda ordenado por CODIGO (lo necesita la busqueda binaria).
// Devuelve la cantidad de comandos del archivo nuevo, o -1 si hubo un error.
int aparearArchivos(const char* archivoA, const char* archivoB, const char* archivoSalida);

// --- Archivos de texto ---
// Log de sesion (sesion.txt)
void exportarSesionTXT(NodoHistorial* cabeza);

// Escribe la cola (de frente a fin) en un .txt, un mensaje por linea. Devuelve false si no pudo crearlo.
bool guardarColaEnTXT(const Cola& cola, const char* nombreArchivo);

// Lee un .txt y encola cada linea no vacia. Devuelve cuantas encolo, o -1 si no pudo abrirlo.
int leerMensajesTXT(const char* nombreArchivo, Cola& cola);

// Vacia la pila escribiendo cada par (usuario, IA) en un .txt. 'total' es el ID del ultimo par.
// Si no puede crear el archivo devuelve false y la pila queda intacta.
bool guardarSimulacroTXT(Pila& pila, int total, const char* nombreArchivo);

// --- Corte de control por categoria (requiere el .dat ordenado por categoria) ---
void listadoPorCategoria(const char* nombreArchivo);

#endif
