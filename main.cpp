#define _CRT_SECURE_NO_WARNINGS
#include <iostream> //Libreria estandar de entrada y salida, funciones que permiten mostrar mensajes en pantalla y leer datos ingresados por el usuario como COUT y CIN
#include <cstring>  //Libreria estandar de manejo de cadenas, funciones que permiten manipular cadenas de caracteres.

#include "comando.h"  // Modulo de dominio: struct Comando, arreglo y matriz de usos
#include "lista.h"     // Biblioteca de lista enlazada: historial de interacciones
#include "archivos.h"  // Modulo de archivos: binario, acceso aleatorio, texto y corte de control

using namespace std; //Evita tener que escribir std:: antes de cada funcion o clase de la libreria estandar, como cout, cin, string, etc.

// =============================================================
// MENU PRINCIPAL
// =============================================================

int main() {
    Comando comandos[50];
    int cantComandos = 0;
    bool ordenadoPorCodigo, ordenadoPorPalabra  = false;  

    // Matriz inicializada en 0. Mantiene su valor mientras la app este abierta.
    int matrizUsos[CANT_CATEGORIAS][FRANJAS_HORARIAS] = { 0 };

    NodoHistorial* historialSesion = NULL;
    int contadorIDInteraccion = 1;

    leerDesdeArchivoBinario(comandos, cantComandos);

    int opcion = -1;
    mostrarMenu();
    do {

        cin >> opcion;
        cin.ignore();

        switch (opcion) {
        case 1: //generar datos
            generarDatosDePrueba(comandos, cantComandos);
            cout << "seleccione cualquier opcion u 11 para mostrar de vuelta el menu\n";
            break;
        case 2: // mostrar comandos posibles
            mostrarComandos(comandos, cantComandos);
            cout << "seleccione cualquier opcion u 11 para mostrar de vuelta el menu\n";
            break;
        case 3: { // ordenar
            int cr;
            cout << "Elija Criterio (1: Codigo, 2: Palabra): ";
            do {
                cin >> cr;
                if (cr != 1 && cr != 2) cout << "invalido";
            } while (cr != 1 && cr != 2);
                if (cr == 1){
                    ordenarPorCodigo(comandos, cantComandos);
                    ordenadoPorCodigo = true;
                    ordenadoPorPalabra = false;
                } else { 
                    ordenarPorPalabra(comandos, cantComandos);
                    ordenadoPorCodigo = false;
                    ordenadoPorPalabra = true;
                }
            cout << "seleccione cualquier opcion u 11 para mostrar de vuelta el menu\n";
            break;
        }
        case 4: { // bus (binaria por codigo)
            int cod;
            if(ordenadoPorCodigo == true){
                cout << "Ingrese Codigo a buscar: ";
                cin >> cod;
                int pos = buscarPorCodigoBinaria(comandos, cantComandos, cod);
                if (pos != -1) cout << "ENCONTRADO en pos [" << pos << "]: " << comandos[pos].palabraClave << " -> " << comandos[pos].respuesta << "\n";
                else cout << "Codigo no encontrado.\n";
                cout << "seleccione  opcion u 11 para mostrar de vuelta el menu\n";
            } else cout << "array de comandos no ordenado, por favor ordene por codigo e intente de nuevo\n";
            break;
        }
        case 5: { // charlar con el bot
            // 1. Pedir la hora antes de procesar el mensaje
            int hora;
            cout << "\nIngrese la hora actual (0 a 23 hs): ";
            cin >> hora;
            cin.ignore();
            // Mapeo de hora a columna de la matriz
            int franja = 0;
            if (hora >= 6 && hora < 12)       franja = 0; // Manana
            else if (hora >= 12 && hora < 19) franja = 1; // Tarde
            else if (hora >= 19 && hora < 24) franja = 2; // Noche
            else                              franja = 3; // Trasnoche (0 a 5 hs)
            // 2. Pedir la frase
            char frase[100];
            cout << "Usuario dice: ";
            cin.getline(frase, 100);

            int idx = procesarFraseUsuario(comandos, cantComandos, frase);
            if (idx != -1) {
                cout << "IA responde: " << comandos[idx].respuesta << "\n";
                comandos[idx].usos++;

                // Mapeo de categoria a fila de la matriz
                int filaCat = 0; // 0: Sistema por defecto
                if (strcmp(comandos[idx].categoria, "Consultas") == 0) filaCat = 1;
                else if (strcmp(comandos[idx].categoria, "Ocio") == 0) filaCat = 2;

                // Sumamos +1 en la matriz (persistente en la sesion)
                matrizUsos[filaCat][franja] += 1;

                agregarInteraccionHistorial(historialSesion, contadorIDInteraccion++, frase, comandos[idx].respuesta);
            }
            else {
                cout << "IA responde: Lo siento, no entendi el comando.\n";
                agregarInteraccionHistorial(historialSesion, contadorIDInteraccion++, frase, "Comando Desconocido");
            }
            cout << "seleccione cualquier opcion u 11 para mostrar de vuelta el menu\n";
            break;
        }
        case 6:
            mostrarHistorialCompleto(historialSesion);
            break;
        case 7: {
            int sub;
            cout << "1: Buscar Palabra | 2: Eliminar Registro del Historial: ";
            cin >> sub;
            cin.ignore();
            if (sub == 1) {
                char pal[30];
                cout << "Palabra: "; cin.getline(pal, 30);
                buscarInteraccionPorPalabraClave(historialSesion, pal);
            }
            else if (sub == 2) {
                int id;
                cout << "ID a eliminar: "; cin >> id;
                eliminarInteraccion(historialSesion, id);
            }
            cout << "seleccione cualquier opcion u 11 para mostrar de vuelta el menu\n";
            break;
        }
        case 8:
            procesarMatrizUsos(matrizUsos);
            cout << "seleccione cualquier opcion u 11 para mostrar de vuelta el menu\n";
            break;
        case 9: {
            int nReg;
            char nuevaResp[100];
            cout << "Numero de registro N a modificar (0, 1, 2...): ";
            cin >> nReg;
            cin.ignore();
            cout << "Nueva Respuesta: ";
            cin.getline(nuevaResp, 100);
            modificarRespuestaRegistroN(nReg, nuevaResp);
            leerDesdeArchivoBinario(comandos, cantComandos);
            cout << "seleccione cualquier opcion u 11 para mostrar de vuelta el menu\n";
            break;
        }
        case 10:
            exportarSesionTXT(historialSesion);
            ordenarPorPalabra(comandos, cantComandos);
            guardarEnArchivoBinario(comandos, cantComandos);
            listadoPorCategoriaConCorte();
            cout << "seleccione cualquier opcion u 11 para mostrar de vuelta el menu\n";
            break;
        case 11:
            mostrarMenu();
            break;
        case 0:
            guardarEnArchivoBinario(comandos, cantComandos);
            liberarHistorial(historialSesion);
            cout << "Saliendo del programa...\n";
            cout << "seleccione cualquier opcion u 11 para mostrar de vuelta el menu\n";
            break;
        default:
            cout << "Opcion invalida, intente otra vez.\n";
        }
    } while (opcion != 0);

    return 0;
}
