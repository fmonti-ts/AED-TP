#define _CRT_SECURE_NO_WARNINGS
#include <iostream> //Libreria estandar de entrada y salida, funciones que permiten mostrar mensajes en pantalla y leer datos ingresados por el usuario como COUT y CIN
#include <cstring>  //Libreria estandar de manejo de cadenas, funciones que permiten manipular cadenas de caracteres.
#include <iostream>
#include <chrono>   // Necesario para las unidades de tiempo (seconds, milliseconds)
#include <thread> 


#include "comando.h"  // Modulo de dominio: struct Comando, arreglo y matriz de usos
#include "lista.h"     // Biblioteca de lista enlazada: historial de interacciones
#include "archivos.h"  // Modulo de archivos: binario, acceso aleatorio, texto y corte de control
#include "inputs.h"


using namespace std; //Evita tener que escribir std:: antes de cada funcion o clase de la libreria estandar, como cout, cin, string, etc.

// --------------------
//  MANEJO DE HISTORIAL
// --------------------

void generarDatosDePrueba(Comando arreglo[], int& cant) {
    // Codigos unicos y desordenados a proposito, para poder probar el ordenamiento.
    cargarComando(arreglo[0], 105, "hora",      "Sistema",   "La hora actual es 16:00 hs.",                      "Informa la hora actual.",                  12);
    cargarComando(arreglo[1], 101, "clima",     "Consultas", "El clima esta despejado y templado.",              "Informa el estado del clima.",              8);
    cargarComando(arreglo[2], 104, "chiste",    "Ocio",      "Que hace una abeja en el gimnasio? Zumba.",        "Cuenta un chiste.",                        25);
    cargarComando(arreglo[4], 103, "nombre",    "Consultas", "Soy AsistenteVirtual v1.0.",                       "Dice como se llama el asistente.",         15);
    cargarComando(arreglo[5], 106, "fecha",     "Sistema",   "La fecha de hoy es 10/10/2026.",                   "Informa la fecha actual.",                  7);
    cargarComando(arreglo[6], 107, "adios",     "Sistema",   "Hasta luego! Fue un gusto charlar.",               "Se despide del usuario.",                   3);
    cargarComando(arreglo[7], 108, "musica",    "Ocio",      "Te recomiendo escuchar algo de Rock Nacional.",    "Recomienda musica para escuchar.",         10);
    cargarComando(arreglo[8], 109, "creador",   "Consultas", "Me desarrollo Facundo, Lucas, Juan Oliver y Stefano .",   "Cuenta quien desarrollo el asistente.",     4);
    cargarComando(arreglo[9], 110, "acertijo",  "Ocio",      "Que tiene ciudades pero no casas? Un mapa.",       "Propone un acertijo para resolver.",        6);
    cant = 10;

    cout << "\n[OK] Datos de prueba cargados en el arreglo.\n";
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


// -------------------
//  MANEJO DE ARCHIVOS
// -------------------

void exportarYListar(Comando comandos[], int cantComandos, NodoHistorial* historial) {
    exportarSesionTXT(historial);
    ordenarPorCategoria(comandos, cantComandos);
    guardarEnArchivoBinario(comandos, cantComandos);
    listadoPorCategoria();
}

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
    int lectura = leerDesdeArchivoBinario(comandos, cantComandos);
    if(lectura == -1){
        "por alguna razon el guardado fallo";
    }
}

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


void decirleAlgoALaIa(Comando comandos[MAX_COMANDOS], int cantComandos, int matrizUsos[CANT_CATEGORIAS][FRANJAS_HORARIAS], int franja, NodoHistorial*& historial, int& contadorID) {
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
        agregarInteraccionHistorial(historial, contadorID++, frase, "Comando invalido");
    }
}



// -------------------
//  MANEJO DE COMANDDOS
// -------------------


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

void elegirOrdenamiento(Comando comandos[], int cantComandos) {
    int criterio;
    do {
        cout << "Elija criterio (1: Codigo, 2: Palabra, 0: Cancelar): ";
        criterio = leerEntero();
    } while (criterio < 0 || criterio > 2);

    if (criterio == 1) ordenarPorCodigo(comandos, cantComandos);
    else if (criterio == 2) ordenarPorPalabra(comandos, cantComandos);
}

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


void preguntarQueHaceUnComando(Comando comandos[], int cantComandos) {
    char palabra[30];
    cout << "Palabra clave del comando: ";
    leerLinea(palabra, 30);
    aMinuscula(palabra, palabra);

    int pos = buscarComando(comandos, cantComandos, palabra);
    mostrarDescripcion(comandos, cantComandos, pos);   // si pos es -1 muestra el error
}

// -------------------
//  CONVERSACION DE IA
// -------------------


void conversarConLaIa(Comando comandos[MAX_COMANDOS], int cantComandos, int matrizUsos[CANT_CATEGORIAS][FRANJAS_HORARIAS], NodoHistorial*& historial, int& contadorID) {
    if (cantComandos == 0) {
        cout << "\n[!] No hay comandos cargados. Cargue datos de prueba primero (opcion 2).\n";
        cout << "ordenando... \n";
        ordenarPorCodigo(comandos, cantComandos);
        std::this_thread::sleep_for(std::chrono::milliseconds(500));

        cout << "listo! \n ";

    }

    int franja = franjaDeHora(pedirHora());
    cout << "\nIA: Hola! Soy tu asistente virtual escribe .\n";
    mostrarOpcionesDeSesion();
    
    int opcion;
    do {
        opcion = leerEntero();

        switch (opcion) {
        case 1:
            mostrarComandosParaUsuario(comandos, cantComandos);
            break;
        case 2:
            decirleAlgoALaIa(comandos, cantComandos, matrizUsos, franja, historial, contadorID);
            break;
        case 3:
            preguntarQueHaceUnComando(comandos, cantComandos);
            break;
        case 4: 
            mostrarOpcionesDeSesion();
            break;
        case 0:
            cout << "IA: Hasta luego!\n";
            menuDeOpciones();
            break;
        default:
            cout << "Opcion invalida.\n";
            cout << "escriba de vuelta: ";
        }
    } while (opcion != 0);
}

void administrarSesion(Comando comandos[MAX_COMANDOS], int& cantComandos, int matrizUsos[CANT_CATEGORIAS][FRANJAS_HORARIAS], NodoHistorial*& historial, int& contadorID){
    int opcion;
    mostrarMenuAdministracion();
    do{
        opcion = leerEntero();
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
            menuDeOpciones();
            break;
        default:
            cout << "Opcion invalida, intente otra vez.\n";
        }
    } while (opcion != 0); 
};

void menuDeOpciones(){
    cout << "------ elija una opcion ------- \n ";
    cout << "oprima 1 para hablar con la IA \n";
    cout << "oprima 2 para entrar al menu de ajustes \n";
    cout << "oprima 0 para terminar el programa \n" ;
    cout << "opcion: ";
    };


int main() {
    Comando comandos[MAX_COMANDOS];
    int cantComandos = 0;

    int matrizUsos[CANT_CATEGORIAS][FRANJAS_HORARIAS] = { { 0 } };

    NodoHistorial* historialSesion = NULL;
    int contadorIDInteraccion = 1;

    int lectura = leerDesdeArchivoBinario(comandos, cantComandos);
    this_thread::sleep_for(std::chrono::seconds(1));
    if (lectura == -1 || cantComandos == 0){cout << "no se detectaron comandos, entre al menu de ajustes y cargue datos antes de conversar con la IA \n \n ";}
    std::this_thread::sleep_for(std::chrono::seconds(1));
    int opcion;
        menuDeOpciones();
    
    do {
        opcion = leerEntero();
        switch (opcion)
        {
        case 1:
            conversarConLaIa(comandos, cantComandos, matrizUsos, historialSesion, contadorIDInteraccion);
            break;
        case 2:
            administrarSesion(comandos, cantComandos, matrizUsos, historialSesion, contadorIDInteraccion);
            break;
        default:
            cout << "escriba una opcion valida: ";
        }
    } while (opcion != 0);

    return 0;
}
