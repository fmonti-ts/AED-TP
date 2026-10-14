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

const int MAX_COMANDOS = 50;       // Tamano fijo del arreglo de comandos
const int CANT_CATEGORIAS = 3;     // Filas de la matriz de usos (Sistema, Consultas, Ocio)
const int FRANJAS_HORARIAS = 4;    // Columnas de la matriz de usos (Manana, Tarde, Noche, Trasnoche)

struct Comando { // Estructura que representa un comando del asistente conversacional
	int codigo; //Codigo unico del comando
	char palabraClave[30]; //Palabra clave que activa el comando
	char categoria[30]; //Categoria a la que pertenece el comando (Sistema, Consultas, Ocio)
	char descripcion[100]; //Explica que hace el comando (se muestra con mostrarDescripcion)
	char respuesta[100]; //Respuesta que el asistente devuelve al usuario cuando se activa el comando
	int usos; //Cantidad de veces que el comando ha sido utilizado
};

// --- Strings auxiliares ---
void aMinuscula(char* destino, const char* origen);

// --- Carga de comandos ---
void cargarComandoAux(Comando& c, int cod, const char* clave, const char* cat, const char* resp, const char* desc, int u);

void generarDatosDePrueba(Comando arreglo[], int& cant);

// --- Recorrido / muestra ---
void mostrarComandosEnMemoria(Comando arreglo[], int cant);   // Vista completa (todos los campos), para el administrador

void mostrarComandos(Comando arreglo[MAX_COMANDOS], int cantComandos);  // Vista para el usuario: solo codigo y palabra clave (exige orden por codigo)

// --- Ordenamiento (dos criterios + categoria para el corte de control) ---
bool estaOrdenadoPorCodigo(Comando arreglo[], int cant);

void ordenarPorCodigo(Comando arreglo[], int cant);

void ordenarPorPalabra(Comando arreglo[], int cant);

void ordenarPorCategoria(Comando arreglo[], int cant);

// --- Busqueda binaria por codigo (requiere arreglo ordenado por codigo) ---
int buscarPorCodigoBinaria(Comando arreglo[], int cant, int codigoBuscado);

// --- Busqueda por palabra clave (insensible a mayusculas, admite texto parcial): devuelve posicion o -1 ---
int buscarComando(Comando arreglo[MAX_COMANDOS], int cantComandos, const char* palabraClave);

// --- Busqueda de la clave DENTRO de una frase, insensible a mayusculas (para procesar frases) ---
int procesarFraseUsuario(Comando arreglo[], int cant, const char* fraseInput);

// --- Mostrar el dato de un comando a partir de su posicion (error si la posicion no es valida) ---
void mostrarRespuesta(Comando arreglo[MAX_COMANDOS], int cantComandos, int pos);

void mostrarDescripcion(Comando arreglo[MAX_COMANDOS], int cantComandos, int pos);

// --- Matriz de usos por categoria / franja horaria ---
int filaDeCategoria(const char* categoria);   // Categoria -> fila de la matriz

int franjaDeHora(int hora);                   // Hora (0 a 23) -> columna de la matriz

void procesarMatrizUsos(int matriz[CANT_CATEGORIAS][FRANJAS_HORARIAS]);

#endif
