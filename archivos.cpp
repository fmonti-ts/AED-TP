#define _CRT_SECURE_NO_WARNINGS 
#include <iostream>
#include <fstream>
#include <cstring>
#include <cctype>
#include <filesystem>   // C++17: para detectar los .dat de la carpeta
#include "archivos.h"
#include "inputs.h"

using namespace std;
namespace fs = std::filesystem;

// =============================================================
// Nombres de archivo
// =============================================================
static bool terminaEnDat(const char* s) {
    return fs::path(s).extension() == ".dat";
}
void armarNombre(const char* entrada, char* destino, int tamDestino) {
    while (*entrada == ' ' || *entrada == '\t') entrada++;
    int len = (int)strlen(entrada);
    while (len > 0 && (entrada[len - 1] == ' ' || entrada[len - 1] == '\t' || entrada[len - 1] == '\r')) len--;
    if (len > tamDestino - 5) len = tamDestino - 5;   // lugar para ".dat" y '\0'
    strncpy(destino, entrada, len);
    destino[len] = '\0';
    if (!terminaEnDat(destino)) strcat(destino, ".dat");
}

bool existeArchivo(const char* nombre) {
    ifstream archivo(nombre, ios::binary);
    return !archivo.fail();
}

int detectarArchivos(char nombres[][MAX_NOMBRE_ARCHIVO], int maxNombres) {
    int cant = 0;
    error_code ec;
    fs::directory_iterator it(".", ec), fin;
    while (!ec && it != fin && cant < maxNombres) {
        error_code ec2;
        if (it->is_regular_file(ec2) && !ec2) {
            string nombre = it->path().filename().string();
            if (nombre.size() < (size_t)MAX_NOMBRE_ARCHIVO && terminaEnDat(nombre.c_str())) {
                strcpy(nombres[cant], nombre.c_str());
                cant++;
            }
        }
        it.increment(ec);
    }
    // orden alfabetico (insercion)
    for (int i = 1; i < cant; i++) {
        char aux[MAX_NOMBRE_ARCHIVO];
        strcpy(aux, nombres[i]);
        int j = i - 1;
        while (j >= 0 && strcmp(nombres[j], aux) > 0) {
            strcpy(nombres[j + 1], nombres[j]);
            j--;
        }
        strcpy(nombres[j + 1], aux);
    }
    return cant;
}

void listarArchivos(char nombres[][MAX_NOMBRE_ARCHIVO], int n) {
    if (n > 0) {
        cout << "Se detectaron " << n << " archivo(s) .dat:\n";
        for (int i = 0; i < n; i++) cout << "  " << i + 1 << ". " << nombres[i] << "\n";
    }
}

int listarArchivosDat() {
    char nombres[MAX_ARCHIVOS_DAT][MAX_NOMBRE_ARCHIVO];
    int n = detectarArchivos(nombres, MAX_ARCHIVOS_DAT);
    listarArchivos(nombres, n);
    return n;
}

// =============================================================
// Pedir nombres de archivo por teclado
// =============================================================

bool pedirNombreDat(const char* mensaje, char* destino) {
    char entrada[MAX_ENTRADA_NOMBRE];
    cout << mensaje;
    leerLinea(entrada, MAX_ENTRADA_NOMBRE);

    char nombre[MAX_NOMBRE_ARCHIVO];
    armarNombre(entrada, nombre, MAX_NOMBRE_ARCHIVO);
    if (strcmp(nombre, ".dat") == 0) return false;   // enter vacio
    strcpy(destino, nombre);
    return true;
}

bool pedirArchivoExistente(const char* mensaje, char* destino) {
    char nombre[MAX_NOMBRE_ARCHIVO];
    while (pedirNombreDat(mensaje, nombre)) {
        if (existeArchivo(nombre)) {
            strcpy(destino, nombre);
            return true;
        }
        cout << "No existe el archivo '" << nombre << "'. Intente de nuevo.\n";
    }
    return false;
}

bool asegurarArchivoActual(char* archivoActual) {
    if (archivoActual[0] != '\0') return true;
    cout << "No hay un archivo .dat cargado.\n";
    return pedirNombreDat("Nombre del archivo .dat a usar (enter vacio para cancelar): ", archivoActual);
}

// =============================================================
// Archivo binario
// =============================================================

// Cantidad de registros del .dat segun su tamano. -1 si no se pudo abrir.
static int contarRegistros(const char* nombre) {
    ifstream archivo(nombre, ios::binary);
    if (!archivo) return -1;
    archivo.seekg(0, ios::end);
    int cant = (int)(archivo.tellg() / (streamoff)sizeof(Comando));
    archivo.close();
    return cant;
}

// Lee hasta 'max' registros del .dat en destino. Devuelve cuantos leyo (-1 si no se pudo abrir).
static int leerRegistros(const char* nombre, Comando* destino, int max) {
    ifstream archivo(nombre, ios::binary);
    if (!archivo) return -1;
    int cant = 0;
    while (cant < max && archivo.read((char*)(&destino[cant]), sizeof(Comando))) {
        cant++;
    }
    archivo.close();
    return cant;
}

// Pisa el .dat con los 'cant' registros de v. false si no se pudo escribir.
static bool escribirRegistros(const char* nombre, const Comando* v, int cant) {
    ofstream archivo(nombre, ios::binary | ios::trunc);
    if (!archivo) return false;
    archivo.write((const char*)v, sizeof(Comando) * cant);
    bool ok = !archivo.fail();
    archivo.close();
    return ok;
}

bool guardarEnArchivoBinario(Comando arreglo[], int cant, const char* nombreArchivo) {
    if (!escribirRegistros(nombreArchivo, arreglo, cant)) {
        cout << "Error al crear " << nombreArchivo << "\n";
        return false;
    }
    cout << "\n[OK] Archivo '" << nombreArchivo << "' generado/actualizado en disco.\n";
    return true;
}

int leerDesdeArchivoBinario(Comando arreglo[], int& cant, const char* nombreArchivo) {
    int enArchivo = contarRegistros(nombreArchivo);
    if (enArchivo == -1) {
        cout << "No se encontro '" << nombreArchivo << "'. Se creara uno al guardar datos.\n";
        return -1;
    }
    // Se lee como maximo MAX_COMANDOS: el arreglo no tiene lugar para mas.
    cant = leerRegistros(nombreArchivo, arreglo, MAX_COMANDOS);
    cout << "\n[OK] " << cant << " comandos cargados desde '" << nombreArchivo << "'.\n";
    if (enArchivo > MAX_COMANDOS) {
        cout << "[!] El archivo tiene mas de " << MAX_COMANDOS << " comandos: solo se cargaron los primeros " << MAX_COMANDOS << ".\n";
    }
    return 0;
}

void modificarRespuestaRegistroN(const char* nombreArchivo, int nRegistro, const char* nuevaRespuesta) {
    fstream archivo(nombreArchivo, ios::in | ios::out | ios::binary);
    if (!archivo) { cout << "Error al abrir " << nombreArchivo << "\n"; return; }

    streamoff posicion = (streamoff)(nRegistro * sizeof(Comando));
    archivo.seekg(posicion);
    Comando reg;
    if (!archivo.read((char*)(&reg), sizeof(Comando))) {
        cout << "[!] El registro N=" << nRegistro << " no existe en '" << nombreArchivo << "'.\n";
        archivo.close();
        return;
    }
    strcpy(reg.respuesta, nuevaRespuesta);

    archivo.seekp(posicion);
    archivo.write((char*)(&reg), sizeof(Comando));

    archivo.close();
    cout << "\n[OK] Registro N=" << nRegistro << " modificado con Acceso Aleatorio (seekg/seekp).\n";
}

// =============================================================
// Apareo de archivos
// =============================================================

// Compara dos textos sin distinguir mayusculas (<0, 0, >0 como strcmp)
static int compararSinMayusculas(const char* a, const char* b) {
    int i = 0;
    while (a[i] != '\0' && b[i] != '\0') {
        int ca = tolower((unsigned char)a[i]);
        int cb = tolower((unsigned char)b[i]);
        if (ca != cb) return ca - cb;
        i++;
    }
    return tolower((unsigned char)a[i]) - tolower((unsigned char)b[i]);
}

// Lee TODO el .dat en un arreglo dinamico (el que llama hace delete[]). NULL si no se pudo abrir.
static Comando* leerArchivoCompleto(const char* nombre, int& cant) {
    cant = contarRegistros(nombre);
    if (cant == -1) { cant = 0; return NULL; }
    Comando* v = new Comando[cant > 0 ? cant : 1];
    leerRegistros(nombre, v, cant);
    return v;
}

static bool estaOrdenadoPorClave(Comando v[], int cant) {
    for (int i = 0; i < cant - 1; i++) {
        if (compararSinMayusculas(v[i].palabraClave, v[i + 1].palabraClave) > 0) return false;
    }
    return true;
}

// Insercion: estable y simple
static void ordenarPorClave(Comando v[], int cant) {
    for (int i = 1; i < cant; i++) {
        Comando aux = v[i];
        int j = i - 1;
        while (j >= 0 && compararSinMayusculas(v[j].palabraClave, aux.palabraClave) > 0) {
            v[j + 1] = v[j];
            j--;
        }
        v[j + 1] = aux;
    }
}

// El apareo exige ambos ordenados por la clave de comparacion (palabra clave)
static void ordenarSiHaceFalta(Comando v[], int cant, const char* nombre) {
    if (estaOrdenadoPorClave(v, cant)) return;
    cout << "[!] '" << nombre << "' no esta ordenado por palabra clave. Ordenando en memoria...\n";
    ordenarPorClave(v, cant);
}

// Apareo: se toma siempre el menor de los dos frentes; si la palabra clave es igual a la del
// ultimo registro del resultado, no se agrega otro: se suman los usos. En empate sale primero A,
// asi que se conserva el registro del primer archivo. Devuelve el resultado (el que llama hace delete[]).
static Comando* mezclarPorClave(const Comando a[], int na, const Comando b[], int nb, int& nr, int& combinados) {
    Comando* res = new Comando[na + nb + 1];
    nr = 0;
    combinados = 0;
    int i = 0, j = 0;
    while (i < na || j < nb) {
        const Comando* sig;
        if (j >= nb || (i < na && compararSinMayusculas(a[i].palabraClave, b[j].palabraClave) <= 0)) sig = &a[i++];
        else sig = &b[j++];

        if (nr > 0 && compararSinMayusculas(res[nr - 1].palabraClave, sig->palabraClave) == 0) {
            res[nr - 1].usos += sig->usos;
            combinados++;
        }
        else {
            res[nr++] = *sig;
        }
    }
    return res;
}

// Los codigos tienen que ser unicos (la busqueda binaria los necesita)
static void asegurarCodigosUnicos(Comando res[], int nr) {
    int maxCodigo = 0;
    for (int k = 0; k < nr; k++) if (res[k].codigo > maxCodigo) maxCodigo = res[k].codigo;
    for (int k = 1; k < nr; k++) {
        for (int m = 0; m < k; m++) {
            if (res[m].codigo == res[k].codigo) {
                maxCodigo++;
                cout << "[!] Codigo " << res[k].codigo << " repetido en '" << res[k].palabraClave
                     << "': se le asigno el codigo " << maxCodigo << ".\n";
                res[k].codigo = maxCodigo;
                break;
            }
        }
    }
}

int aparearArchivos(const char* archivoA, const char* archivoB, const char* archivoSalida) {
    if (compararSinMayusculas(archivoA, archivoB) == 0) {
        cout << "[!] Debe elegir dos archivos distintos.\n";
        return -1;
    }
    if (compararSinMayusculas(archivoSalida, archivoA) == 0 || compararSinMayusculas(archivoSalida, archivoB) == 0) {
        cout << "[!] El archivo nuevo no puede llamarse igual que uno de los dos originales.\n";
        return -1;
    }

    int na = 0, nb = 0;
    Comando* a = leerArchivoCompleto(archivoA, na);
    if (a == NULL) { cout << "[!] No se pudo abrir '" << archivoA << "'.\n"; return -1; }
    Comando* b = leerArchivoCompleto(archivoB, nb);
    if (b == NULL) { cout << "[!] No se pudo abrir '" << archivoB << "'.\n"; delete[] a; return -1; }

    ordenarSiHaceFalta(a, na, archivoA);
    ordenarSiHaceFalta(b, nb, archivoB);

    int nr = 0, combinados = 0;
    Comando* res = mezclarPorClave(a, na, b, nb, nr, combinados);
    asegurarCodigosUnicos(res, nr);
    ordenarPorCodigo(res, nr);   // la busqueda binaria necesita el archivo ordenado por codigo

    bool ok = escribirRegistros(archivoSalida, res, nr);
    delete[] a; delete[] b; delete[] res;
    if (!ok) {
        cout << "[!] Error al crear " << archivoSalida << "\n";
        return -1;
    }

    cout << "\n[OK] '" << archivoSalida << "' creado con " << nr << " comandos ("
         << archivoA << ": " << na << " | " << archivoB << ": " << nb
         << " | repetidos combinados: " << combinados << ").\n";
    if (nr > MAX_COMANDOS) {
        cout << "[!] El resultado supera los " << MAX_COMANDOS << " comandos: al cargarlo solo se leeran los primeros " << MAX_COMANDOS << ".\n";
    }
    return nr;
}

// =============================================================
// Corte de control
// =============================================================

void listadoPorCategoria(const char* nombreArchivo) {
    ifstream archivo(nombreArchivo, ios::binary);
    if (!archivo) { cout << "No existe " << nombreArchivo << " para procesar el corte de control.\n"; return; }

    Comando reg;
    if (!archivo.read((char*)(&reg), sizeof(Comando))) {
        archivo.close();
        return;
    }

    cout << "\n=== REPORTE CON CORTE DE CONTROL (POR CATEGORIA) ===\n";
    int totalGeneralUsos = 0;

    while (!archivo.eof()) {
        char categoriaActual[30];
        strcpy(categoriaActual, reg.categoria);
        int subtotalCategoriaUsos = 0;

        cout << "\n>> CATEGORIA: " << categoriaActual << "\n";

        while (!archivo.eof() && strcmp(reg.categoria, categoriaActual) == 0) {
            cout << "   - Comando: " << reg.palabraClave << " | Usos: " << reg.usos << "\n";
            subtotalCategoriaUsos += reg.usos;
            archivo.read((char*)(&reg), sizeof(Comando));
        }

        cout << "   TOTAL " << categoriaActual << ": " << subtotalCategoriaUsos << " usos.\n";
        totalGeneralUsos += subtotalCategoriaUsos;
    }

    cout << "\n----------------------------------------------------\n";
    cout << "TOTAL GENERAL DE USOS: " << totalGeneralUsos << "\n";
    archivo.close();
}

// =============================================================
// Archivos de texto
// =============================================================

void exportarSesionTXT(NodoHistorial* cabeza) {
    ofstream txt("sesion.txt");
    if (!txt) { cout << "Error al crear sesion.txt\n"; return; }

    txt << "=== LOG DE SESION DE IA CONVERSACIONAL ===\n";
    NodoHistorial* aux = cabeza;
    while (aux != NULL) {
        txt << "[ID: " << aux->idInteraccion << "] User: " << aux->mensajeUsuario
            << " | IA: " << aux->respuestaAsistente << "\n";
        aux = aux->siguiente;
    }
    txt.close();
    cout << "\n[OK] Archivo 'sesion.txt' generado automaticamente.\n";
}

bool guardarColaEnTXT(const Cola& cola, const char* nombreArchivo) {
    ofstream txt(nombreArchivo);
    if (!txt) {
        cout << "Error al crear " << nombreArchivo << "\n";
        return false;
    }
    recorrerCola(cola, txt);
    txt.close();
    cout << "\n[OK] Archivo '" << nombreArchivo << "' generado.\n";
    return true;
}

int leerMensajesTXT(const char* nombreArchivo, Cola& cola) {
    ifstream archivo(nombreArchivo);
    if (!archivo) return -1;

    int cant = 0;
    char linea[100];
    while (true) {
        archivo.getline(linea, 100);
        if (archivo.fail() && !archivo.eof()) {   // linea demasiado larga: se recorta
            archivo.clear();
            archivo.ignore(1000, '\n');
        }
        else if (archivo.fail()) break;           // fin del archivo

        int len = (int)strlen(linea);
        if (len > 0 && linea[len - 1] == '\r') linea[len - 1] = '\0';
        if (linea[0] == '\0') continue;

        encolar(cola, linea);
        cant++;
    }
    archivo.close();
    return cant;
}

bool guardarSimulacroTXT(Pila& pila, int total, const char* nombreArchivo) {
    ofstream txt(nombreArchivo);
    if (!txt) {
        cout << "Error al crear " << nombreArchivo << "\n";
        return false;
    }
    txt << "=== SIMULACRO DE CONVERSACION USUARIO - IA ===\n";
    char resp[LARGO_RESPUESTA_PILA], msg[LARGO_RESPUESTA_PILA];
    int id = total;
    while (!estaVacia(pila)) {
        desapilar(pila, resp);
        if (!desapilar(pila, msg)) msg[0] = '\0';
        txt << "[ID: " << id << "] User: " << msg << " | IA: " << resp << "\n";
        id--;
    }
    txt.close();
    cout << "\n[OK] Archivo '" << nombreArchivo << "' generado.\n";
    return true;
}
