#define _CRT_SECURE_NO_WARNINGS 
#include <iostream>
#include <fstream>
#include <cstring>
#include "archivos.h"

using namespace std;

void guardarEnArchivoBinario(Comando arreglo[], int cant) {
    ofstream archivo("comandos.dat", ios::binary | ios::trunc);
    if (!archivo) { cout << "Error al crear comandos.dat\n"; return; }
    archivo.write((char*)(arreglo), sizeof(Comando) * cant);
    archivo.close();
    cout << "\n[OK] Archivo 'comandos.dat' generado/actualizado en disco automaticamente.\n";
}

int leerDesdeArchivoBinario(Comando arreglo[], int& cant) {
    ifstream archivo("comandos.dat", ios::binary);
    if (!archivo) { cout << "No se encontro 'comandos.dat'. Se creara uno al guardar datos.\n"; return -1; }
    cant = 0;
    while (archivo.read((char*)(&arreglo[cant]), sizeof(Comando))) {
        cant++;
    }
    archivo.close();
    cout << "\n[OK] " << cant << " comandos cargados desde 'comandos.dat'.\n";
    return 0;
}

void modificarRespuestaRegistroN(int nRegistro, const char* nuevaRespuesta) {
    fstream archivo("comandos.dat", ios::in | ios::out | ios::binary);
    if (!archivo) { cout << "Error al abrir comandos.dat\n"; return; }

    archivo.seekg(nRegistro * sizeof(Comando));
    Comando reg;
    archivo.read((char*)(&reg), sizeof(Comando));
    strcpy(reg.respuesta, nuevaRespuesta);

    archivo.seekp(nRegistro * sizeof(Comando));
    archivo.write((char*)(&reg), sizeof(Comando));

    archivo.close();
    cout << "\n[OK] Registro N=" << nRegistro << " modificado con Acceso Aleatorio (seekg/seekp).\n";
}

void listadoPorCategoria() {
    ifstream archivo("comandos.dat", ios::binary);
    if (!archivo) { cout << "No existe comandos.dat para procesar el corte de control.\n"; return; }

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