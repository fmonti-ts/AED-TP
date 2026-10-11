#ifndef COMANDO_H
#define COMANDO_H

// =============================================================
// MODULO DE DOMINIO: Comando y Matriz de Usos
// =============================================================
// Declara el struct principal del sistema (Comando), las
// constantes de la matriz de usos y todas las funciones que
// operan sobre el arreglo de comandos (carga, orden, busqueda,
// matriz y reconocimiento de frases).
// =============================================================

struct Comando { // Estructura que representa un comando del asistente conversacional
	int codigo; //Codigo unico del comando
	char palabraClave[30]; //Palabra clave que activa el comando
	char categoria[30]; //Categoria a la que pertenece el comando (Sistema, Consultas, Ocio)
	char respuesta[100]; //Respuesta que el asistente devuelve al usuario cuando se activa el comando
	int usos; //Cantidad de veces que el comando ha sido utilizado
};

const int CANT_CATEGORIAS = 3; //Cantidad de filas de la matriz de usos (Sistema, Consultas, Ocio)
const int FRANJAS_HORARIAS = 4; // Cantidad de columnas de la matriz de usos (Manana, Tarde, Noche, Trasnoche)

// --- Strings auxiliares ---
void aMinuscula(char* destino, const char* origen);

// --- Mostrar Comandos ---
void mostrarComandos();

// --- Carga de comandos ---
void cargarComandoAux(Comando& c, int cod, const char* clave, const char* cat, const char* resp, int u);

void generarDatosDePrueba(Comando arreglo[], int& cant);

// --- Recorrido / muestra ---
void mostrarComandos(Comando arreglo[], int cant);

// --- Ordenamiento (dos criterios) ---
void ordenarPorCodigo(Comando arreglo[], int cant);

void ordenarPorPalabra(Comando arreglo[], int cant);

// --- Busqueda binaria por codigo (requiere arreglo ordenado por codigo) ---
int buscarPorCodigoBinaria(Comando arreglo[], int cant, int codigoBuscado);

// --- Busqueda por texto parcial, insensible a mayusculas (para procesar frases) ---
int procesarFraseUsuario(Comando arreglo[], int cant, const char* fraseInput);

// --- Matriz de usos por categoria / franja horaria ---
void procesarMatrizUsos(int matriz[CANT_CATEGORIAS][FRANJAS_HORARIAS]);

#endif
