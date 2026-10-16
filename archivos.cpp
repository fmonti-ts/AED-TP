#define _CRT_SECURE_NO_WARNINGS 
#include <iostream>
#include <fstream>
#include <cstring>
#include <cctype>
#include <filesystem>   // C++17: para detectar los .dat de la carpeta
#include "archivos.h"

using namespace std;
namespace fs = std::filesystem;

// =============================================================
// Nombres de archivo
// =============================================================
static bool terminaEnDat(const char* s) {
    return fs::path(s).extension() == ".dat";
}
void armarNombreDat(const char* entrada, char* destino, int tamDestino) {
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

int detectarArchivosDat(char nombres[][MAX_NOMBRE_ARCHIVO], int maxNombres) {
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

// =============================================================
// Archivo binario
// =============================================================

void guardarEnArchivoBinario(Comando arreglo[], int cant, const char* nombreArchivo) {
    ofstream archivo(nombreArchivo, ios::binary | ios::trunc);
    if (!archivo) { cout << "Error al crear " << nombreArchivo << "\n"; return; }
    archivo.write((char*)(arreglo), sizeof(Comando) * cant);
    archivo.close();
    cout << "\n[OK] Archivo '" << nombreArchivo << "' generado/actualizado en disco.\n";
}

int leerDesdeArchivoBinario(Comando arreglo[], int& cant, const char* nombreArchivo) {
    ifstream archivo(nombreArchivo, ios::binary);
    if (!archivo) { cout << "No se encontro '" << nombreArchivo << "'. Se creara uno al guardar datos.\n"; return -1; }
    cant = 0;
    // Se lee como maximo MAX_COMANDOS: el arreglo no tiene lugar para mas.
    while (cant < MAX_COMANDOS && archivo.read((char*)(&arreglo[cant]), sizeof(Comando))) {
        cant++;
    }
    bool hayMas = (cant == MAX_COMANDOS && archivo.peek() != EOF);
    archivo.close();
    cout << "\n[OK] " << cant << " comandos cargados desde '" << nombreArchivo << "'.\n";
    if (hayMas) {
        cout << "[!] El archivo tiene mas de " << MAX_COMANDOS << " comandos: solo se cargaron los primeros " << MAX_COMANDOS << ".\n";
    }
    return 0;
}

void modificarRespuestaRegistroN(const char* nombreArchivo, int nRegistro, const char* nuevaRespuesta) {
    fstream archivo(nombreArchivo, ios::in | ios::out | ios::binary);
    if (!archivo) { cout << "Error al abrir " << nombreArchivo << "\n"; return; }

    archivo.seekg(nRegistro * sizeof(Comando));
    Comando reg;
    archivo.read((char*)(&reg), sizeof(Comando));
    strcpy(reg.respuesta, nuevaRespuesta);

    archivo.seekp(nRegistro * sizeof(Comando));
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
    ifstream archivo(nombre, ios::binary);
    if (!archivo) { cant = 0; return NULL; }
    archivo.seekg(0, ios::end);
    streamoff bytes = archivo.tellg();
    archivo.seekg(0, ios::beg);
    cant = (int)(bytes / (streamoff)sizeof(Comando));
    Comando* v = new Comando[cant > 0 ? cant : 1];
    if (cant > 0) archivo.read((char*)v, sizeof(Comando) * cant);
    archivo.close();
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

    // El apareo exige ambos ordenados por la clave de comparacion (palabra clave)
    if (!estaOrdenadoPorClave(a, na)) {
        cout << "[!] '" << archivoA << "' no esta ordenado por palabra clave. Ordenando en memoria...\n";
        ordenarPorClave(a, na);
    }
    if (!estaOrdenadoPorClave(b, nb)) {
        cout << "[!] '" << archivoB << "' no esta ordenado por palabra clave. Ordenando en memoria...\n";
        ordenarPorClave(b, nb);
    }

    // Apareo: se toma siempre el menor de los dos frentes; si la palabra clave es igual a la del
    // ultimo registro del resultado, no se agrega otro: se suman los usos. En empate sale primero A,
    // asi que se conserva el registro del primer archivo.
    Comando* res = new Comando[na + nb + 1];
    int nr = 0, combinados = 0;
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

    // Los codigos tienen que ser unicos (la busqueda binaria los necesita)
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

    // La busqueda binaria necesita el archivo ordenado por codigo
    ordenarPorCodigo(res, nr);

    ofstream salida(archivoSalida, ios::binary | ios::trunc);
    if (!salida) {
        cout << "[!] Error al crear " << archivoSalida << "\n";
        delete[] a; delete[] b; delete[] res;
        return -1;
    }
    salida.write((char*)res, sizeof(Comando) * nr);
    salida.close();

    cout << "\n[OK] '" << archivoSalida << "' creado con " << nr << " comandos ("
         << archivoA << ": " << na << " | " << archivoB << ": " << nb
         << " | repetidos combinados: " << combinados << ").\n";
    if (nr > MAX_COMANDOS) {
        cout << "[!] El resultado supera los " << MAX_COMANDOS << " comandos: al cargarlo solo se leeran los primeros " << MAX_COMANDOS << ".\n";
    }

    delete[] a; delete[] b; delete[] res;
    return nr;
}

// =============================================================
// Corte de control y log de sesion
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
