#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include <cstring>
#include <cctype>
#include "comando.h"

using namespace std;

// =============================================================
// Strings auxiliares
// =============================================================

void aMinuscula(char* destino, const char* origen) {
    int i = 0;
    while (origen[i] != '\0') {
        destino[i] = tolower(origen[i]);
        i++;
    }
    destino[i] = '\0';
}

// =============================================================
// Carga de comandos
// =============================================================

void cargarComandoAux(Comando& c, int cod, const char* clave, const char* cat, const char* resp, int u) {
    c.codigo = cod;
    strcpy(c.palabraClave, clave);
    strcpy(c.categoria, cat);
    strcpy(c.respuesta, resp);
    c.usos = u;
}

void generarDatosDePrueba(Comando arreglo[], int& cant) {
    cant = 10;
    cargarComandoAux(arreglo[0], 105, "hora", "Sistema", "La hora actual es 16:00 hs.", 12);
    cargarComandoAux(arreglo[1], 101, "clima", "Consultas", "El clima esta despejado y templado.", 8);
    cargarComandoAux(arreglo[2], 104, "chiste", "Ocio", "Que hace una abeja en el gimnasio? Zumba.", 25);
    cargarComandoAux(arreglo[3], 102, "ayuda", "Sistema", "Comandos disponibles: hora, clima, chiste, apagar.", 5);
    cargarComandoAux(arreglo[4], 103, "nombre", "Consultas", "Soy AsistenteVirtual v1.0.", 15);
    cargarComandoAux(arreglo[5], 103, "nombre", "Consultas", "Soy AsistenteVirtual v1.0.", 15);
    cargarComandoAux(arreglo[6], 103, "nombre", "Consultas", "Soy AsistenteVirtual v1.0.", 15);
    cargarComandoAux(arreglo[7], 103, "nombre", "Consultas", "Soy AsistenteVirtual v1.0.", 15);
    cargarComandoAux(arreglo[8], 103, "nombre", "Consultas", "Soy AsistenteVirtual v1.0.", 15);
    cargarComandoAux(arreglo[9], 103, "nombre", "Consultas", "Soy AsistenteVirtual v1.0.", 15);
    cargarComandoAux(arreglo[10], 103, "nombre", "Consultas", "Soy AsistenteVirtual v1.0.", 15);

    cout << "\n[OK] Datos de prueba cargados en el arreglo.\n";
}

void mostrarMenu(){
        cout << "\n=============================================\n";
        cout << "   ASISTENTE CONVERSACIONAL SIMULADO (E1)   \n";
        cout << "=============================================\n";
        cout << "1. Cargar Datos de Prueba\n";
        cout << "2. Mostrar Comandos en Memoria\n";
        cout << "3. Ordenar Arreglo (1: Codigo | 2: Palabra)\n";
        cout << "4. Buscar Comando por Codigo (Busqueda Binaria)\n";
        cout << "5. Simular Conversacion (Procesar Frase + Lista)\n";
        cout << "6. Ver Historial Completo (Lista Enlazada)\n";
        cout << "7. Buscar / Eliminar en Historial\n";
        cout << "8. Ver Reporte de Matriz de Usos\n";
        cout << "9. Modificar Respuesta N en Disco (Acceso Aleatorio)\n";
        cout << "10. Exportar Sesion a TXT y Corte de Control\n";
        cout << "0. Salir y Guardar Estado\n";
        cout << "Opcion: ";
}

// =============================================================
// Recorrido / muestra
// =============================================================

void mostrarComandos(Comando arreglo[], int cant) {
    cout << "\n=== LISTADO DE COMANDOS EN MEMORIA ===\n";
    for (int i = 0; i < cant; i++) {
        cout << "Cod: " << arreglo[i].codigo
            << " | Clave: " << arreglo[i].palabraClave
            << " | Cat: " << arreglo[i].categoria
            << " | Usos: " << arreglo[i].usos << "\n";
    }
}

// =============================================================
// Ordenamiento (dos criterios)
// =============================================================

void ordenarPorCodigo(Comando arreglo[], int cant) {
    int i = 0;
    bool ordenado = false;
    while(i != cant - 1 && !ordenado) {
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
    int i = 0;
    bool ordenado = false;
    while (i != cant - 1 || not ordenado) {
        ordenado = true;
        int minIdx = i;
        for (int j = i + 1; j < cant; j++) {
            if (strcmp(arreglo[j].palabraClave, arreglo[minIdx].palabraClave) < 0) {
                ordenado = false;
                minIdx = j;
            }
        }
        Comando aux = arreglo[i];
        arreglo[i] = arreglo[minIdx];
        arreglo[minIdx] = aux;
        i++;
    }
    ordenado = true;
    cout << "\n[OK] Arreglo ordenado por PALABRA CLAVE.\n";
}

// =============================================================
// Busqueda binaria por codigo
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

// =============================================================
// Busqueda por texto parcial, insensible a mayusculas
// =============================================================

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
// Matriz de usos
// =============================================================

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
