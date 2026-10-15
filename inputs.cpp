#include <iostream>
#include <cstring>
#include <cctype>
#include "inputs.h"
#include "comando.h"
using namespace std;


void aMinuscula(char* destino, const char* origen) {
    int i = 0;
    while (origen[i] != '\0') {
        destino[i] = tolower(origen[i]);
        i++;
    }
    destino[i] = '\0';
}

int leerEntero() {
    int n;
    while (!(cin >> n)) {
        if (cin.eof()) return 0;
        cin.clear();
        cin.ignore(1000, '\n');
        cout << "Ingrese un numero valido: ";
    }
    cin.ignore(1000, '\n');
    return n;
}

// Lee una linea completa (con espacios) de hasta tam-1 caracteres.
void leerLinea(char* destino, int tam) {
    cin.getline(destino, tam);
    if (cin.fail() && !cin.eof()) {   // la linea era mas larga que el buffer
        cin.clear();
        cin.ignore(1000, '\n');
    }
}

int pedirHora() {
    int hora;
    do {
        cout << "Ingrese la hora actual (0 a 23): ";
        hora = leerEntero();
        if (hora < 0 || hora > 23) cout << "Hora invalida.\n";
    } while (hora < 0 || hora > 23);
    return hora;
}

