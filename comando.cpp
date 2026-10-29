#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include <cstring>
#include <cctype>
#include "comando.h"
#include "inputs.h"
#include <iomanip>   // setw / left / right para mostrar la matriz como tabla

using namespace std;

// =============================================================
// Carga de comandos
// =============================================================

void mostrarMenuAdministracion() {
    cout << "\n=============================================\n";
    cout << "       MENU DE ADMINISTRACION DE SESION  \n";
    cout << "=============================================\n";
    cout << "1. Cargar Datos de Prueba\n";
    cout << "2. Mostrar Comandos en Memoria\n";
    cout << "3. Ordenar Arreglo (1: Codigo | 2: Palabra)\n";
    cout << "4. Buscar Comando por Codigo (Busqueda Binaria)\n";
    cout << "5. Crear Nuevo Comando\n";
    cout << "6. Ver Historial Completo (Lista Enlazada)\n";
    cout << "7. Buscar / Eliminar en Historial\n";
    cout << "8. Ver Reporte de Matriz de Usos\n";
    cout << "9. Modificar Respuesta N en Disco (Acceso Aleatorio)\n";
    cout << "10. Guardar Comandos y Corte de Control\n";
    cout << "11. Simular Conversacion (Usuario / IA)\n";
    cout << "12. Fusionar Archivos .dat (Apareo)\n";
    cout << "0. Salir y Guardar Estado\n";
}

void mostrarOpcionesDeSesion() {
    cout << "\n--------- SESION CON LA IA ---------\n";
    cout << "1. Ver comandos disponibles\n";
    cout << "2. Decirle algo a la IA\n";
    cout << "3. Preguntar que hace un comando\n";
    cout << "4. Mostrar opciones devuelta\n";
    cout << "0. Terminar la conversacion\n";
}


// =============================================================
// Carga de comandos
// =============================================================



//simp  
void cargarComando(Comando& c, int cod, const char* clave, const char* cat, const char* resp, const char* desc, int u, int& cant) {
    c.codigo = cod;
    strcpy(c.palabraClave, clave);
    strcpy(c.categoria, cat);
    strcpy(c.respuesta, resp);
    strcpy(c.descripcion, desc);
    c.usos = u;
    cant++;
}

bool existePalabraClave(Comando arreglo[], int cant, const char* clave) {
    return buscarComandoExacto(arreglo, cant, clave) != -1;
}

int calcularCodigoNuevo(Comando arreglo[], int cant) {
    int codigo = cant + 100;
    bool usado = true;
    while (usado) {
        usado = false;
        for (int i = 0; i < cant && !usado; i++) {
            if (arreglo[i].codigo == codigo) usado = true;
        }
        if (usado) codigo++;
    }
    return codigo;
}

// =============================================================    
// Recorrido / muestra
// =============================================================

void mostrarComandosEnMemoria(Comando arreglo[], int cant) {
    cout << "\n=== LISTADO DE COMANDOS EN MEMORIA ===\n";
    for (int i = 0; i < cant; i++) {
        cout << "[" << i << "] Cod: " << arreglo[i].codigo
            << " | Clave: " << arreglo[i].palabraClave
            << " | Cat: " << arreglo[i].categoria
            << " | Usos: " << arreglo[i].usos << "\n";
    }
}

void mostrarComandosParaUsuario(Comando arreglo[], int cantComandos) {
    if (cantComandos == 0) {
        cout << "\n[!] No hay comandos cargados.\n";
        return;
    }
    if (!estaOrdenadoPorCodigo(arreglo, cantComandos)) {
        cout << "\n[!] Los comandos no estan ordenados por codigo. ordenando...\n";
        pausar(500);
        ordenarPorCodigo(arreglo, cantComandos);
        cout << "listo! \n ";
    }
    cout << "\n=== COMANDOS DISPONIBLES ===\n";
    for (int i = 0; i < cantComandos; i++) {
        cout << arreglo[i].codigo << " - " << arreglo[i].palabraClave << "\n";
    }
    cout << "\n converse con la IA y elija alguno de estos comandos\n";
}


// =============================================================
// Ordenamiento (dos criterios)
// =============================================================


void ordenarPorCodigo(Comando arreglo[], int cant) {
    int i = 0;
    bool ordenado = false;
    while (i < cant - 1 && !ordenado) {
        ordenado = true;
        for (int j = 0; j < cant - i - 1; j++) {
            if (arreglo[j].codigo > arreglo[j + 1].codigo) {
                ordenado = false;
                Comando aux = arreglo[j];
                arreglo[j] = arreglo[j + 1];
                arreglo[j + 1] = aux;
            }
        }
        i++;
    }
}

void ordenarPorPalabra(Comando arreglo[], int cant) {
    for (int i = 0; i < cant - 1; i++) {
        int minIdx = i;
        for (int j = i + 1; j < cant; j++) {
            if (strcmp(arreglo[j].palabraClave, arreglo[minIdx].palabraClave) < 0) {
                minIdx = j;
            }
        }
        if (minIdx != i) {
            Comando aux = arreglo[i];
            arreglo[i] = arreglo[minIdx];
            arreglo[minIdx] = aux;
        }
    }
}

void ordenarPorCategoria(Comando arreglo[], int cant) {
    for (int i = 0; i < cant - 1; i++) {
        int minIdx = i;
        for (int j = i + 1; j < cant; j++) {
            int cmp = strcmp(arreglo[j].categoria, arreglo[minIdx].categoria);
            if (cmp < 0 ||
               (cmp == 0 && strcmp(arreglo[j].palabraClave, arreglo[minIdx].palabraClave) < 0)) {
                minIdx = j;
            }
        }
        if (minIdx != i) {
            Comando aux = arreglo[i];
            arreglo[i] = arreglo[minIdx];
            arreglo[minIdx] = aux;
        }
    }
}

bool estaOrdenadoPorCodigo(Comando arreglo[], int cant) {
    for (int i = 0; i < cant - 1; i++) {
        if (arreglo[i].codigo > arreglo[i + 1].codigo) return false;
    }
    return true;
}

// =============================================================
// Busquedas
// =============================================================

int buscarComando(Comando arreglo[], int cant, const char* palabraClave) {
    if (palabraClave[0] == '\0') return -2;  
    char buscada[100], claveMin[30];
    aMinuscula(buscada, palabraClave);
    for (int i = 0; i < cant; i++) {
        aMinuscula(claveMin, arreglo[i].palabraClave);
        if (strstr(claveMin, buscada) != NULL) {
            return i;
        }
    }
    return -1;
}

int buscarComandoExacto(Comando arreglo[], int cant, const char* palabraClave) {
    if (palabraClave[0] == '\0') return -2;  
    char textoMin[100], claveMin[30];
    aMinuscula(textoMin, palabraClave);
    for (int i = 0; i < cant; i++) {
        aMinuscula(claveMin, arreglo[i].palabraClave);
        if (strcmp(textoMin, claveMin) == 0) {
            return i;
        };
    }
    return -1;
}

int buscarPorCodigoBinaria(Comando arreglo[], int cant, int codigoBuscado) {
    int inicio = 0, fin = cant - 1;
    while (inicio <= fin) {
        int medio = inicio + (fin - inicio) / 2;
        if (arreglo[medio].codigo == codigoBuscado) return medio;
        if (arreglo[medio].codigo < codigoBuscado) inicio = medio + 1;
        else fin = medio - 1;
    }
    return -1;
}

// Busca si alguna palabra clave aparece DENTRO de la frase: "decime la HORA" encuentra "hora".
int procesarFraseUsuario(Comando arreglo[], int cant, const char* fraseInput) {
    char fraseMin[100], claveMin[30];
    aMinuscula(fraseMin, fraseInput);

    for (int i = 0; i < cant; i++) {
        aMinuscula(claveMin, arreglo[i].palabraClave);
        if (strstr(fraseMin, claveMin) != NULL) {
            return i;
        }
    }
    return -1;
}


// =============================================================
// Mostrar respuesta / descripcion de un comando
// =============================================================

void mostrarRespuesta(Comando arreglo[], int cantComandos, int pos) {
    if (pos < 0 || pos >= cantComandos) {
        cout << "[ERROR] Lo siento, no entendi el comando: no hay respuesta para mostrar.\n";
        return;
    }
    cout << "IA responde: " << arreglo[pos].respuesta << "\n";
}

void mostrarDescripcion(Comando arreglo[MAX_COMANDOS], int cantComandos, int pos) {
    if (pos < 0 || pos >= cantComandos) {
        cout << "[ERROR] Comando inexistente: no hay descripcion para mostrar.\n";
        return;
    }
    cout << "Comando '" << arreglo[pos].palabraClave << "': " << arreglo[pos].descripcion << "\n";
}

// =============================================================
// Matriz de usos
// =============================================================

int filaDeCategoria(const char* categoria) {
    if (strcmp(categoria, "Consultas") == 0) return 1;
    if (strcmp(categoria, "Ocio") == 0) return 2;
    return 0;   // Sistema (y cualquier categoria desconocida)
}

int franjaDeHora(int hora) {
    if (hora >= 6 && hora < 12)  return 0;   // Manana
    if (hora >= 12 && hora < 19) return 1;   // Tarde
    if (hora >= 19 && hora < 24) return 2;   // Noche
    return 3;                                // Trasnoche (0 a 5 hs)
}

// Anchos de columna de la tabla
int ANCHO_ETIQUETA = 12;
int ANCHO_CELDA = 11;
int ANCHO_TOTAL = 7;

void separadorTabla() {
    int ancho = ANCHO_ETIQUETA + ANCHO_CELDA * FRANJAS_HORARIAS + 2 + ANCHO_TOTAL;
    cout << setfill('-') << setw(ancho) << "" << setfill(' ') << "\n";
}

// Muestra la matriz como tabla (categorias en filas, franjas en columnas) con totales por fila y por columna
void totalesMatrizUsos(int matriz[CANT_CATEGORIAS][FRANJAS_HORARIAS]) {
    int totalColumna[FRANJAS_HORARIAS] = { 0 };
    int totalGeneral = 0;

    cout << setw(ANCHO_ETIQUETA) << "";
    for (int j = 0; j < FRANJAS_HORARIAS; j++) cout << setw(ANCHO_CELDA) << NOMBRES_FRANJAS[j];
    cout << " |" << setw(ANCHO_TOTAL) << "Total" << "\n";
    separadorTabla();

    for (int i = 0; i < CANT_CATEGORIAS; i++) {
        int totalFila = 0;
        cout << left << setw(ANCHO_ETIQUETA) << NOMBRES_CATEGORIAS[i] << right;
        for (int j = 0; j < FRANJAS_HORARIAS; j++) {
            cout << setw(ANCHO_CELDA) << matriz[i][j];
            totalFila += matriz[i][j];
            totalColumna[j] += matriz[i][j];
        }
        cout << " |" << setw(ANCHO_TOTAL) << totalFila << "\n";
        totalGeneral += totalFila;
    }

    separadorTabla();
    cout << left << setw(ANCHO_ETIQUETA) << "Total" << right;
    for (int j = 0; j < FRANJAS_HORARIAS; j++) cout << setw(ANCHO_CELDA) << totalColumna[j];
    cout << " |" << setw(ANCHO_TOTAL) << totalGeneral << "\n";
}

void maximoMatrizUsos(int matriz[CANT_CATEGORIAS][FRANJAS_HORARIAS]) {
    int maxVal = matriz[0][0], maxFila = 0, maxCol = 0;
    for (int i = 0; i < CANT_CATEGORIAS; i++) {
        for (int j = 0; j < FRANJAS_HORARIAS; j++) {
            if (matriz[i][j] > maxVal) {
                maxVal = matriz[i][j];
                maxFila = i;
                maxCol = j;
            }
        }
    }
    cout << "\nPICO DE USO MAXIMO: " << maxVal << " usos -> " << NOMBRES_CATEGORIAS[maxFila]
         << " / " << NOMBRES_FRANJAS[maxCol] << " (fila " << maxFila << ", columna " << maxCol << ")\n";
}
