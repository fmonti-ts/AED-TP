#define _CRT_SECURE_NO_WARNINGS
#include <iostream> 
#include <cstring> 

#include "comando.h"  
#include "lista.h"     
#include "archivos.h" 
#include "inputs.h"
#include "cola.h"      
#include "pila.h"      


using namespace std;

// --------------------
//  MANEJO DE HISTORIAL
// --------------------

void esperar(){
    cout << ".";
    pausar(300);
    cout << ".";
    pausar(300);
    cout << ".\n";
    pausar(300);
    cout << "\n\n\n";
}

void generarDatosDePrueba(Comando arreglo[], int& cant) {
    cant = 0;
    cargarComando(arreglo[0], 103, "hora","Sistema","son entre las 16 y las 18 hs.","Informa la hora actual.",12,cant);
    cargarComando(arreglo[1], 100, "clima","Consultas","El clima esta despejado y templado.","Informa el estado del clima.",8,cant);
    cargarComando(arreglo[2], 102, "chiste","Ocio","Que hace una abeja en el gimnasio? Zumba.","Cuenta un chiste.",25,cant);
    cargarComando(arreglo[3], 101, "nombre","Consultas","Soy AsistenteVirtual v1.0.","Dice como se llama el asistente.",15,cant);
    cargarComando(arreglo[4], 104, "fecha","Sistema","La fecha de hoy es 10/10/2026.","Informa la fecha actual.",7,cant);
    cargarComando(arreglo[5], 105, "adios","Sistema","Hasta luego! Fue un gusto charlar.","Se despide del usuario.",0,cant);
    cargarComando(arreglo[6], 106, "musica","Ocio","Te recomiendo escuchar algo de Rock Nacional.","Recomienda musica para escuchar",10,cant);
    cargarComando(arreglo[7], 107, "creador",   "Consultas", "Me desarrollo Facundo, Lucas, Juan Oliver y Stefano .","Cuenta quien desarrollo el asistente.",4,cant);
    cargarComando(arreglo[8], 108,"acertijo","Ocio","Que tiene ciudades pero no casas? Un mapa.","Propone un acertijo para resolver.",6,cant);
    cargarComando(arreglo[9], 109, "hola","Sistema","Hola, soy una IA convrsasional.","Saluda al usuario.",30, cant);

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

void procesarMatrizUsos(int matriz[CANT_CATEGORIAS][FRANJAS_HORARIAS]) {
    cout << "\n=== REPORTE DE LA MATRIZ DE USOS (categoria x franja horaria) ===\n\n";
    totalesMatrizUsos(matriz);
    maximoMatrizUsos(matriz);
}

void menuDeOpciones(){
    cout << "------ elija una opcion ------- \n ";
    cout << "oprima 1 para hablar con la IA \n";
    cout << "oprima 2 para entrar al menu de ajustes \n";
    cout << "oprima 0 para terminar el programa \n" ;
}

// -------------------
//  MANEJO DE ARCHIVOS
// -------------------

void cargarArchivoInicial(Comando comandos[], int& cantComandos, char* archivoActual) {
    pausar(600);
    int n = listarArchivosDat();
    if (n == 0) {
        cout << "[!] No se detectaron archivos .dat, entre al menu de ajustes y cargue datos antes de conversar con la IA \n \n ";
        return;
    }
    char nombre[MAX_NOMBRE_ARCHIVO];
    if (!pedirArchivoExistente("Escriba el nombre del archivo a leer (enter vacio para no leer ninguno): ", nombre)) {
        cout << "No se leyo ningun archivo, entre al menu de ajustes y cargue datos antes de conversar con la IA \n \n ";
        return;
    }
    if (leerDesdeArchivoBinario(comandos, cantComandos, nombre) == 0) {
        strcpy(archivoActual, nombre);
        if (cantComandos == 0) {
            cout << "[!] El archivo esta vacio, entre al menu de ajustes y cargue datos antes de conversar con la IA \n \n ";
        }
    }
}

void exportarYListar(Comando comandos[], int cantComandos, char* archivoActual) {
    if (!asegurarArchivoActual(archivoActual)) {
        cout << "Operacion cancelada.\n";
        return;
    }
    // El corte de control necesita el .dat ordenado por categoria: se ordena una copia
    // para no alterar el orden del arreglo en memoria (la busqueda binaria lo necesita por codigo).
    Comando copia[MAX_COMANDOS];
    memcpy(copia, comandos, sizeof(Comando) * cantComandos);
    ordenarPorCategoria(copia, cantComandos);
    cout << "\n[OK] Comandos ordenados por CATEGORIA (el arreglo en memoria no se modifica).\n";

    if (guardarEnArchivoBinario(copia, cantComandos, archivoActual)) {
        listadoPorCategoria(archivoActual);
    }
}

void modificarRespuestaEnDisco(Comando comandos[], int& cantComandos, char* archivoActual) {
    if (cantComandos == 0) {
        cout << "No hay comandos para modificar.\n";
        return;
    }
    if (!asegurarArchivoActual(archivoActual)) {
        cout << "Operacion cancelada.\n";
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

    if (!guardarEnArchivoBinario(comandos, cantComandos, archivoActual)) return;
    modificarRespuestaRegistroN(archivoActual, n, nuevaResp);
    if (leerDesdeArchivoBinario(comandos, cantComandos, archivoActual) == -1) {
        cout << "[!] No se pudo volver a leer '" << archivoActual << "' despues de modificarlo.\n";
    }
}

void fusionarComandos() {
    int n = listarArchivosDat();
    if (n < 2) {
        cout << "[!] Se necesitan al menos 2 archivos .dat para fusionar (detectados: " << n << ").\n";
        return;
    }
    char archivoA[MAX_NOMBRE_ARCHIVO], archivoB[MAX_NOMBRE_ARCHIVO], salida[MAX_NOMBRE_ARCHIVO];
    if (!pedirArchivoExistente("Primer archivo (enter vacio para cancelar): ", archivoA) ||
        !pedirArchivoExistente("Segundo archivo (enter vacio para cancelar): ", archivoB) ||
        !pedirNombreDat("Nombre del archivo nuevo (enter vacio para cancelar): ", salida)) {
        cout << "Fusion cancelada.\n";
        return;
    }
    aparearArchivos(archivoA, archivoB, salida);
}

void conversarConLaIA(Comando comandos[MAX_COMANDOS], int cantComandos, int matrizUsos[CANT_CATEGORIAS][FRANJAS_HORARIAS], int franja, NodoHistorial*& historial, int& contadorID) {
    char frase[100] = "";
    char fraseMin[100] = "";
    do {
        cout << "Usuario dice: ";
        leerLinea(frase, 100);
        int pos = procesarFraseUsuario(comandos, cantComandos, frase);
        mostrarRespuesta(comandos, cantComandos, pos);
        if (pos != -1) {
            comandos[pos].usos++;
            matrizUsos[filaDeCategoria(comandos[pos].categoria)][franja]++;
            agregarInteraccionHistorial(historial, contadorID++, frase, comandos[pos].respuesta);
        } else {
            agregarInteraccionHistorial(historial, contadorID++, frase, "Comando invalido");
        }
        aMinuscula(fraseMin, frase);
    } while (strcmp(fraseMin, "adios") != 0 && !cin.eof());
}

// -------------------
//  MANEJO DE COMANDDOS
// -------------------


void elegirOrdenamiento(Comando comandos[], int cantComandos) {
    int criterio;
    do {
        cout << "Elija criterio (1: Codigo, 2: Palabra, otro: Cancelar): ";
        criterio = leerEntero();
    } while (criterio < 0 || criterio > 2);

    if (criterio == 1) {
        ordenarPorCodigo(comandos, cantComandos);
        cout << "\n[OK] Arreglo ordenado por CODIGO.\n";
    }
    else if (criterio == 2) {
        ordenarPorPalabra(comandos, cantComandos);
        cout << "\n[OK] Arreglo ordenado por PALABRA CLAVE.\n";
    }
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

void crearComando(Comando comandos[], int& cantComandos) {
    if (cantComandos >= MAX_COMANDOS) {
        cout << "[!] Se alcanzo el limite de " << MAX_COMANDOS << " comandos. No se pueden agregar mas.\n";
        return;
    }

    char clave[30], respuesta[100], descripcion[100];
    bool repetida;
    do {
        cout << "Palabra clave: ";
        leerLinea(clave, 30, false);
        repetida = existePalabraClave(comandos, cantComandos, clave);
        if (repetida) cout << "Esa palabra clave ya existe en otro comando. Escriba otra.\n";
    } while (repetida && !cin.eof());

    int cat;
    do {
        cout << "Categoria (1: Sistema | 2: Consultas | 3: Ocio): ";
        cat = leerEntero();
    } while ((cat < 1 || cat > CANT_CATEGORIAS) && !cin.eof());
    if (cat < 1 || cat > CANT_CATEGORIAS) return;   // entrada cerrada

    cout << "Respuesta: ";
    leerLinea(respuesta, 100, false);
    cout << "Descripcion: ";
    leerLinea(descripcion, 100, false);

    int usos;
    do {
        cout << "Usos iniciales (0 o mas): ";
        usos = leerEntero();
    } while (usos < 0 && !cin.eof());
    if (usos < 0) return;

    int codigo = calcularCodigoNuevo(comandos, cantComandos);
    cargarComando(comandos[cantComandos], codigo, clave, NOMBRES_CATEGORIAS[cat - 1], respuesta, descripcion, usos, cantComandos);

    cout << "comando agregado correctamente";
    if (cantComandos == MAX_COMANDOS) cout << ". Se alcanzo el limite de comandos";
    cout << "\n";
}

// -------------------
//  SIMULACION DE CONVERSACION
// -------------------

void simularUsuario() {
    Cola cola;
    crearCola(cola);

    cout << "Escriba comandos uno por uno. Enter vacio para terminar.\n";
    char linea[100];
    while (true) {
        cout << "Usuario dice: ";
        linea[0] = '\0';
        leerLinea(linea, 100);
        if (linea[0] == '\0') break;
        if ((int)strlen(linea) >= LARGO_TEXTO_COLA) {
            cout << "[!] Se guardara recortado a " << LARGO_TEXTO_COLA - 1 << " caracteres.\n";
        }
        encolar(cola, linea);
    }

    if (estaVacio(cola)) {
        cout << "No se escribio ningun comando: no se genero usuario.txt.\n";
        return;
    }

    cout << "\nComandos encolados:\n";
    recorrerCola(cola, cout);
    mostrarFinal(cola);

    guardarColaEnTXT(cola, "usuario.txt");
    liberarCola(cola);
}

void simularIA(Comando comandos[], int cantComandos) {
    if (cantComandos == 0) {
        cout << "[!] No hay comandos en memoria. Cargue datos primero.\n";
        return;
    }

    Cola mensajes;
    crearCola(mensajes);
    int total = leerMensajesTXT("usuario.txt", mensajes);
    if (total == -1) {
        cout << "No se encontro 'usuario.txt'. Simule primero una conversacion de usuario.\n";
        return;
    }
    if (total == 0) {
        cout << "'usuario.txt' no tiene mensajes: no se genero simulacro.txt.\n";
        return;
    }

    Pila pila;
    crearPila(pila);
    char linea[LARGO_TEXTO_COLA];
    while (desencolar(mensajes, linea)) {
        int pos = buscarComandoExacto(comandos, cantComandos, linea);
        const char* respuesta = (pos != -1) ? comandos[pos].respuesta : "no se como responder eso";

        cout << "Usuario dice: " << linea << "\nIA responde: " << respuesta << "\n";
        apilar(pila, linea);
        apilar(pila, respuesta);
    }

    guardarSimulacroTXT(pila, total, "simulacro.txt");
    liberarPila(pila);
}

void simularConversacion(Comando comandos[], int cantComandos) {
    cout << "1: Simular conversacion de usuario | 2: Simular respuestas de la IA | otro: Cancelar: ";
    int sub = leerEntero();
    if (sub == 1) simularUsuario();
    else if (sub == 2) simularIA(comandos, cantComandos);
}

void salirSesionAdministrador( char* archivoActual, Comando comandos[MAX_COMANDOS], int& cantComandos, NodoHistorial*& historial){
    char nombre[MAX_NOMBRE_ARCHIVO];
            if (archivoActual[0] != '\0') cout << "(Archivo actual: " << archivoActual << ")\n";
            if (pedirNombreDat("Escriba el nombre del archivo O deje vacio para no guardar: ", nombre)) {
                guardarEnArchivoBinario(comandos, cantComandos, nombre);   // si existe, se sobreescribe
                strcpy(archivoActual, nombre);
            }
            else {
                cout << "No se guardaron los comandos.\n";
            }
            liberarHistorial(historial);
            cout << "Saliendo del programa...\n";
            menuDeOpciones();
}

void salirSesionIA(NodoHistorial*& historial){
    cout << "IA: guardando sesion en txt";
    esperar();
    exportarSesionTXT(historial);
    cout << "IA: Hasta luego!\n";
    cout << "\n\n";
    menuDeOpciones();
}
// -------------------
//      MAIN
// -------------------


void sesionDeUsuario(Comando comandos[MAX_COMANDOS], int cantComandos, int matrizUsos[CANT_CATEGORIAS][FRANJAS_HORARIAS], NodoHistorial*& historial, int& contadorID) {
    if (cantComandos == 0) {
        cout << "\n[!] No hay comandos cargados. Cargue datos de prueba primero (menu de ajustes, opcion 1).\n";
        return;
    }

    int franja = franjaDeHora(pedirHora());
    cout << "\nIA: Hola! Soy tu asistente virtual escribe .\n";
    mostrarOpcionesDeSesion();
    
    int opcion;
    do {
        cout << "Opcion: ";
        opcion = leerEntero();
        switch (opcion) {
        case 1:
            mostrarComandosParaUsuario(comandos, cantComandos);
            break;
        case 2:
            conversarConLaIA(comandos, cantComandos, matrizUsos, franja, historial, contadorID);
            cout << "\n\n";
            mostrarOpcionesDeSesion();
            
            break;
        case 3:
            preguntarQueHaceUnComando(comandos, cantComandos);
            break;
        case 4: 
            mostrarOpcionesDeSesion();
            break;
        case 0:
            salirSesionIA(historial);
            break;
        default:
            cout << "Opcion invalida.\n";
        }
    } while (opcion != 0);
}

void sesionDeAdministrador(Comando comandos[MAX_COMANDOS], int& cantComandos, int matrizUsos[CANT_CATEGORIAS][FRANJAS_HORARIAS], NodoHistorial*& historial, char* archivoActual){
    int opcion;
    mostrarMenuAdministracion();
    do{
        cout << "opcion: ";
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
        case 5:
            crearComando(comandos, cantComandos);
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
            modificarRespuestaEnDisco(comandos, cantComandos, archivoActual);
            break;
        case 10:
            exportarYListar(comandos, cantComandos, archivoActual);
            break;
        case 11:
            simularConversacion(comandos, cantComandos);
            break;
        case 12:
            fusionarComandos();
            break;
        case 0: {
            salirSesionAdministrador(archivoActual, comandos, cantComandos, historial);
            break;
        }
        default:
            cout << "Opcion invalida, intente otra vez.\n";
        }
    } while (opcion != 0); 
}


int main() {
    Comando comandos[MAX_COMANDOS];
    int cantComandos = 0;

    int matrizUsos[CANT_CATEGORIAS][FRANJAS_HORARIAS] = { { 0 } };

    NodoHistorial* historialSesion = NULL;
    int contadorIDInteraccion = 1;

    char archivoActual[MAX_NOMBRE_ARCHIVO] = "";   // .dat cargado / guardado en esta ejecucion ("" = ninguno)

    cargarArchivoInicial(comandos, cantComandos, archivoActual);
    pausar(500);
    int opcion;
    menuDeOpciones();
    
    do {
        cout << "opcion: ";
        opcion = leerEntero();
        switch (opcion)
        {
        case 1:
            sesionDeUsuario(comandos, cantComandos, matrizUsos, historialSesion, contadorIDInteraccion);
            break;
        case 2:
            sesionDeAdministrador(comandos, cantComandos, matrizUsos, historialSesion, archivoActual);
            break;
        case 0: 
            cout << "nos vemos! \n";
            break;
        default:    
            cout << "escriba una opcion valida: ";
        }
    } while (opcion != 0);

    return 0;
}
