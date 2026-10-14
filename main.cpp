#define _CRT_SECURE_NO_WARNINGS
#include <iostream> //Libreria estandar de entrada y salida, funciones que permiten mostrar mensajes en pantalla y leer datos ingresados por el usuario como COUT y CIN
#include <cstring>  //Libreria estandar de manejo de cadenas, funciones que permiten manipular cadenas de caracteres.

#include "comando.h"  // Modulo de dominio: struct Comando, arreglo y matriz de usos
#include "lista.h"     // Biblioteca de lista enlazada: historial de interacciones
#include "archivos.h"  // Modulo de archivos: binario, acceso aleatorio, texto y corte de control

using namespace std; //Evita tener que escribir std:: antes de cada funcion o clase de la libreria estandar, como cout, cin, string, etc.

// =============================================================
// ENTRADA POR CONSOLA
// =============================================================

// Lee un entero y limpia el resto de la linea. Si el usuario escribe letras, vuelve a pedir.
// Si se cierra la entrada (fin de archivo) devuelve 0, que en todos los menus significa "salir / volver".
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

// =============================================================
// CONVERSACION CON LA IA
// =============================================================

void mostrarOpcionesDeSesion() {
    cout << "\n--------- SESION CON LA IA ---------\n";
    cout << "1. Ver comandos disponibles\n";
    cout << "2. Decirle algo a la IA\n";
    cout << "3. Preguntar que hace un comando\n";
    cout << "0. Terminar la conversacion\n";
    cout << "Opcion: ";
}

// El usuario escribe una frase libre; la IA busca un comando dentro de ella y responde.
// Si lo entiende: suma un uso al comando, +1 en la matriz y guarda la interaccion en el historial.
void decirleAlgoALaIa(Comando comandos[MAX_COMANDOS], int cantComandos,
                      int matrizUsos[CANT_CATEGORIAS][FRANJAS_HORARIAS], int franja,
                      NodoHistorial*& historial, int& contadorID) {
    char frase[100];
    cout << "Usuario dice: ";
    leerLinea(frase, 100);

    int pos = procesarFraseUsuario(comandos, cantComandos, frase);
    mostrarRespuesta(comandos, cantComandos, pos);   // si pos es -1 muestra el error

    if (pos != -1) {
        comandos[pos].usos++;
        matrizUsos[filaDeCategoria(comandos[pos].categoria)][franja]++;
        agregarInteraccionHistorial(historial, contadorID++, frase, comandos[pos].respuesta);
    }
    else {
        agregarInteraccionHistorial(historial, contadorID++, frase, "Comando Desconocido");
    }
}

// El usuario escribe una palabra clave (o parte de ella) y se le muestra que hace ese comando.
void preguntarQueHaceUnComando(Comando comandos[MAX_COMANDOS], int cantComandos) {
    char palabra[30];
    cout << "Palabra clave del comando: ";
    leerLinea(palabra, 30);

    int pos = buscarComando(comandos, cantComandos, palabra);
    mostrarDescripcion(comandos, cantComandos, pos);   // si pos es -1 muestra el error
}

void conversarConLaIa(Comando comandos[MAX_COMANDOS], int cantComandos,
                      int matrizUsos[CANT_CATEGORIAS][FRANJAS_HORARIAS],
                      NodoHistorial*& historial, int& contadorID) {
    if (cantComandos == 0) {
        cout << "\n[!] No hay comandos cargados. Cargue datos de prueba primero (opcion 1).\n";
        return;
    }

    int franja = franjaDeHora(pedirHora());   // la hora se pide una sola vez por conversacion
    cout << "\nIA: Hola! Soy tu asistente virtual. Escribi 'ayuda' para ver lo que puedo hacer.\n";

    int opcion;
    do {
        mostrarOpcionesDeSesion();
        opcion = leerEntero();

        switch (opcion) {
        case 1:
            mostrarComandos(comandos, cantComandos);
            break;
        case 2:
            decirleAlgoALaIa(comandos, cantComandos, matrizUsos, franja, historial, contadorID);
            break;
        case 3:
            preguntarQueHaceUnComando(comandos, cantComandos);
            break;
        case 0:
            cout << "IA: Hasta luego!\n";
            break;
        default:
            cout << "Opcion invalida.\n";
        }
    } while (opcion != 0);
}

// =============================================================
// MENU PRINCIPAL
// =============================================================

void mostrarMenu() {
    cout << "\n=============================================\n";
    cout << "   ASISTENTE CONVERSACIONAL SIMULADO (E1)   \n";
    cout << "=============================================\n";
    cout << "1. Cargar Datos de Prueba\n";
    cout << "2. Mostrar Comandos en Memoria\n";
    cout << "3. Ordenar Arreglo (1: Codigo | 2: Palabra)\n";
    cout << "4. Buscar Comando por Codigo (Busqueda Binaria)\n";
    cout << "5. Conversar con la IA\n";
    cout << "6. Ver Historial Completo (Lista Enlazada)\n";
    cout << "7. Buscar / Eliminar en Historial\n";
    cout << "8. Ver Reporte de Matriz de Usos\n";
    cout << "9. Modificar Respuesta N en Disco (Acceso Aleatorio)\n";
    cout << "10. Exportar Sesion a TXT y Corte de Control\n";
    cout << "0. Salir y Guardar Estado\n";
    cout << "Opcion: ";
}

void elegirOrdenamiento(Comando comandos[], int cantComandos) {
    int criterio;
    do {
        cout << "Elija criterio (1: Codigo, 2: Palabra, 0: Cancelar): ";
        criterio = leerEntero();
    } while (criterio < 0 || criterio > 2);

    if (criterio == 1) ordenarPorCodigo(comandos, cantComandos);
    else if (criterio == 2) ordenarPorPalabra(comandos, cantComandos);
}

// La binaria solo sirve si el arreglo esta ordenado por codigo: se verifica en el momento.
void consultarPorCodigo(Comando comandos[], int cantComandos) {
    if (!estaOrdenadoPorCodigo(comandos, cantComandos)) {
        cout << "El arreglo no esta ordenado por codigo. Ordene por codigo (opcion 3) e intente de nuevo.\n";
        return;
    }
    cout << "Ingrese Codigo a buscar: ";
    int codigo = leerEntero();
    int pos = buscarPorCodigoBinaria(comandos, cantComandos, codigo);
    if (pos != -1) cout << "ENCONTRADO en pos [" << pos << "]: " << comandos[pos].palabraClave << " -> " << comandos[pos].respuesta << "\n";
    else cout << "Codigo no encontrado.\n";
}

void gestionarHistorial(NodoHistorial*& historial) {
    cout << "1: Buscar Palabra | 2: Eliminar Registro del Historial: ";
    int sub = leerEntero();
    if (sub == 1) {
        char palabra[30];
        cout << "Palabra: ";
        leerLinea(palabra, 30);
        buscarInteraccionPorPalabraClave(historial, palabra);
    }
    else if (sub == 2) {
        cout << "ID a eliminar: ";
        int id = leerEntero();
        eliminarInteraccion(historial, id);
    }
}

// Acceso aleatorio: el cambio se hace sobre el archivo. Antes se guarda la memoria para que el
// disco este al dia (si no, al recargar se perderian los usos acumulados en esta sesion).
void modificarRespuestaEnDisco(Comando comandos[], int& cantComandos) {
    if (cantComandos == 0) {
        cout << "No hay comandos para modificar.\n";
        return;
    }
    cout << "Numero de registro N a modificar (0 a " << cantComandos - 1 << "): ";
    int n = leerEntero();
    if (n < 0 || n >= cantComandos) {
        cout << "Registro inexistente.\n";
        return;
    }
    char nuevaResp[100];
    cout << "Nueva Respuesta: ";
    leerLinea(nuevaResp, 100);

    guardarEnArchivoBinario(comandos, cantComandos);
    modificarRespuestaRegistroN(n, nuevaResp);
    leerDesdeArchivoBinario(comandos, cantComandos);
}

// El corte de control necesita comandos.dat ordenado por categoria.
void exportarYListar(Comando comandos[], int cantComandos, NodoHistorial* historial) {
    exportarSesionTXT(historial);
    ordenarPorCategoria(comandos, cantComandos);
    guardarEnArchivoBinario(comandos, cantComandos);
    listadoPorCategoriaConCorte();
}

void procesarOpcion(int opcion, Comando comandos[MAX_COMANDOS], int& cantComandos,
                    int matrizUsos[CANT_CATEGORIAS][FRANJAS_HORARIAS],
                    NodoHistorial*& historial, int& contadorID) {
    switch (opcion) {
    case 1:
        generarDatosDePrueba(comandos, cantComandos);
        break;
    case 2:
        mostrarComandosEnMemoria(comandos, cantComandos);
        break;
    case 3:
        elegirOrdenamiento(comandos, cantComandos);
        break;
    case 4:
        consultarPorCodigo(comandos, cantComandos);
        break;
    case 5:
        conversarConLaIa(comandos, cantComandos, matrizUsos, historial, contadorID);
        break;
    case 6:
        mostrarHistorialCompleto(historial);
        break;
    case 7:
        gestionarHistorial(historial);
        break;
    case 8:
        procesarMatrizUsos(matrizUsos);
        break;
    case 9:
        modificarRespuestaEnDisco(comandos, cantComandos);
        break;
    case 10:
        exportarYListar(comandos, cantComandos, historial);
        break;
    case 0:
        guardarEnArchivoBinario(comandos, cantComandos);
        liberarHistorial(historial);
        cout << "Saliendo del programa...\n";
        break;
    default:
        cout << "Opcion invalida, intente otra vez.\n";
    }
}

int main() {
    Comando comandos[MAX_COMANDOS];
    int cantComandos = 0;

    // Matriz inicializada en 0. Mantiene su valor mientras la app este abierta.
    int matrizUsos[CANT_CATEGORIAS][FRANJAS_HORARIAS] = { { 0 } };

    NodoHistorial* historialSesion = NULL;
    int contadorIDInteraccion = 1;

    leerDesdeArchivoBinario(comandos, cantComandos);

    int opcion;
    do {
        mostrarMenu();
        opcion = leerEntero();
        procesarOpcion(opcion, comandos, cantComandos, matrizUsos, historialSesion, contadorIDInteraccion);
    } while (opcion != 0);

    return 0;
}
