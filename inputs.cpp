#include <iostream>
#include <cstring>
#include <cctype>
#include <chrono>
#include <thread>
#include "inputs.h"
#include "comando.h"
using namespace std;


void aMinuscula(char* destino, const char* origen) {
    int i = 0;
    while (origen[i] != '\0') {
        destino[i] = (char)tolower((unsigned char)origen[i]);
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
// Con permitirVacio = false repite hasta que se escriba algo (o se cierre la entrada).
void leerLinea(char* destino, int tam, bool permitirVacio) {
    do {
        cin.getline(destino, tam);
        if (cin.fail() && !cin.eof()) {   // la linea era mas larga que el buffer
            cin.clear();
            cin.ignore(1000, '\n');
        }
        if (!permitirVacio && destino[0] == '\0' && !cin.eof()) {
            cout << "No puede quedar vacio. Intente de nuevo: ";
        }
    } while (!permitirVacio && destino[0] == '\0' && !cin.eof());
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

void pausar(int milisegundos) {
    this_thread::sleep_for(chrono::milliseconds(milisegundos));
}
