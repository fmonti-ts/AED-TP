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

void cargarComandoAux(Comando& c, int cod, const char* clave, const char* cat, const char* resp, const char* desc, int u) {
    c.codigo = cod;
    strcpy(c.palabraClave, clave);
    strcpy(c.categoria, cat);
    strcpy(c.respuesta, resp);
    strcpy(c.descripcion, desc);
    c.usos = u;
}

void generarDatosDePrueba(Comando arreglo[], int& cant) {
    // Codigos unicos y desordenados a proposito, para poder probar el ordenamiento.
    cargarComandoAux(arreglo[0], 105, "hora",      "Sistema",   "La hora actual es 16:00 hs.",                      "Informa la hora actual.",                  12);
    cargarComandoAux(arreglo[1], 101, "clima",     "Consultas", "El clima esta despejado y templado.",              "Informa el estado del clima.",              8);
    cargarComandoAux(arreglo[2], 104, "chiste",    "Ocio",      "Que hace una abeja en el gimnasio? Zumba.",        "Cuenta un chiste.",                        25);
    cargarComandoAux(arreglo[4], 103, "nombre",    "Consultas", "Soy AsistenteVirtual v1.0.",                       "Dice como se llama el asistente.",         15);
    cargarComandoAux(arreglo[5], 106, "fecha",     "Sistema",   "La fecha de hoy es 10/10/2026.",                   "Informa la fecha actual.",                  7);
    cargarComandoAux(arreglo[6], 107, "adios",     "Sistema",   "Hasta luego! Fue un gusto charlar.",               "Se despide del usuario.",                   3);
    cargarComandoAux(arreglo[7], 108, "musica",    "Ocio",      "Te recomiendo escuchar algo de Rock Nacional.",    "Recomienda musica para escuchar.",         10);
    cargarComandoAux(arreglo[8], 109, "creador",   "Consultas", "Me desarrollo Facundo, Lucas, Juan Oliver y Stefano .",   "Cuenta quien desarrollo el asistente.",     4);
    cargarComandoAux(arreglo[9], 110, "acertijo",  "Ocio",      "Que tiene ciudades pero no casas? Un mapa.",       "Propone un acertijo para resolver.",        6);
    cant = 10;

    cout << "\n[OK] Datos de prueba cargados en el arreglo.\n";
}

// =============================================================    
// Recorrido / muestra
// =============================================================

// Vista completa (todos los campos). Sirve para ver el estado interno del arreglo.
void mostrarComandosEnMemoria(Comando arreglo[], int cant) {
    cout << "\n=== LISTADO DE COMANDOS EN MEMORIA ===\n";
    for (int i = 0; i < cant; i++) {
        cout << "[" << i << "] Cod: " << arreglo[i].codigo
            << " | Clave: " << arreglo[i].palabraClave
            << " | Cat: " << arreglo[i].categoria
            << " | Usos: " << arreglo[i].usos << "\n";
    }
}

// Vista para el usuario: que palabras clave puede decirle a la IA.
// Solo funciona si el arreglo esta ordenado por codigo.
void mostrarComandos(Comando arreglo[MAX_COMANDOS], int cantComandos) {
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

bool estaOrdenadoPorCodigo(Comando arreglo[], int cant) {
    for (int i = 0; i < cant - 1; i++) {
        if (arreglo[i].codigo > arreglo[i + 1].codigo) return false;
    }
    return true;
}

// Burbuja con corte anticipado
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

// Seleccion
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

// Seleccion por categoria (y por palabra dentro de cada categoria). La usa el corte de control.
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

// Busca el texto ingresado DENTRO de la palabra clave: "CLIM" encuentra "clima".
// Devuelve la posicion del primer comando que coincide, o -1 si no hay ninguno.
int buscarComando(Comando arreglo[MAX_COMANDOS], int cantComandos, const char* palabraClave) {
    if (palabraClave[0] == '\0') return -1;   // texto vacio: coincidiria con todo

    char buscada[100], claveMin[30];
    aMinuscula(buscada, palabraClave);

    for (int i = 0; i < cantComandos; i++) {
        aMinuscula(claveMin, arreglo[i].palabraClave);
        if (strstr(claveMin, buscada) != NULL) {
            return i;
        }
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
