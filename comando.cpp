#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include <cstring>
#include <cctype>
#include "comando.h"                                

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
    cout << "6. Ver Historial Completo (Lista Enlazada)\n";
    cout << "7. Buscar / Eliminar en Historial\n";
    cout << "8. Ver Reporte de Matriz de Usos\n";
    cout << "9. Modificar Respuesta N en Disco (Acceso Aleatorio)\n";
    cout << "10. Exportar Sesion a TXT y Corte de Control\n";
    cout << "0. Salir y Guardar Estado\n";
    cout << "Opcion: ";
}

void mostrarOpcionesDeSesion() {
    cout << "\n--------- SESION CON LA IA ---------\n";
    cout << "1. Ver comandos disponibles\n";
    cout << "2. Decirle algo a la IA\n";
    cout << "3. Preguntar que hace un comando\n";
    cout << "4. Mostrar opciones devuelta\n";
    cout << "0. Terminar la conversacion\n";
    cout << "Opcion: ";
}



// =============================================================
// Carga de comandos
// =============================================================

void cargarComando(Comando& c, int cod, const char* clave, const char* cat, const char* resp, const char* desc, int u) {
    c.codigo = cod;
    strcpy(c.palabraClave, clave);
    strcpy(c.categoria, cat);
    strcpy(c.respuesta, resp);
    strcpy(c.descripcion, desc);
    c.usos = u;
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
        cout << "\n[!] Los comandos no estan ordenados por codigo. Ordene por codigo e intente de nuevo.\n";
        return;
    }
    cout << "\n=== COMANDOS DISPONIBLES ===\n";
    for (int i = 0; i < cantComandos; i++) {
        cout << arreglo[i].codigo << " - " << arreglo[i].palabraClave << "\n";
    }
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
    cout << "\n[OK] Arreglo ordenado por CODIGO.\n";
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
    cout << "\n[OK] Arreglo ordenado por PALABRA CLAVE.\n";
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
    cout << "\n[OK] Arreglo ordenado por CATEGORIA.\n";
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

int buscarPorPalabra(Comando arreglo[MAX_COMANDOS], int cantComandos, const char* palabraClave) {
    if (palabraClave[0] == '\0') return -1;   // texto vacio: coincidiria con todo

    char buscada[100], claveMin[30];

    for (int i = 0; i < cantComandos; i++) {
        if (strstr(claveMin, buscada) != NULL) {
            return i;
        }
    }
    return -1;
}


// =============================================================
// Mostrar respuesta / descripcion de un comando
// =============================================================

// Busca si alguna palabra clave aparece DENTRO de la frase: "decime la HORA" encuentra "hora".
void mostrarRespuesta(Comando arreglo[MAX_COMANDOS], int cantComandos, int pos) {
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

void procesarMatrizUsos(int matriz[CANT_CATEGORIAS][FRANJAS_HORARIAS]) {
    cout << "\n=== REPORTES DE LA MATRIZ DE USOS ===\n";
    for (int i = 0; i < CANT_CATEGORIAS; i++) {
        int totalFila = 0;
        for (int j = 0; j < FRANJAS_HORARIAS; j++) totalFila += matriz[i][j];
        cout << "Total usos Categoria " << i << ": " << totalFila << "\n";
    }

    for (int j = 0; j < FRANJAS_HORARIAS; j++) {
        int totalCol = 0;
        for (int i = 0; i < CANT_CATEGORIAS; i++) totalCol += matriz[i][j];
        cout << "Total usos Franja Horaria " << j << ": " << totalCol << "\n";
    }

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
    cout << "PICO DE USO MAXIMO: " << maxVal << " (Categoria: " << maxFila << ", Franja: " << maxCol << ")\n";
}
