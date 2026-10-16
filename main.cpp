#define _CRT_SECURE_NO_WARNINGS
#include <iostream> //Libreria estandar de entrada y salida, funciones que permiten mostrar mensajes en pantalla y leer datos ingresados por el usuario como COUT y CIN
#include <cstring>  //Libreria estandar de manejo de cadenas, funciones que permiten manipular cadenas de caracteres.
#include <fstream>
#include <chrono>   // Necesario para las unidades de tiempo (seconds, milliseconds)
#include <thread> 


#include "comando.h"  // Modulo de dominio: struct Comando, arreglo y matriz de usos
#include "lista.h"     // Biblioteca de lista enlazada: historial de interacciones
#include "archivos.h"  // Modulo de archivos: binario, acceso aleatorio, texto y corte de control
#include "inputs.h"
#include "cola.h"      // Biblioteca de cola: comandos escritos por el usuario simulado
#include "pila.h"      // Biblioteca de pila: respuestas de la IA simulada


using namespace std; //Evita tener que escribir std:: antes de cada funcion o clase de la libreria estandar, como cout, cin, string, etc.

// --------------------
//  MANEJO DE HISTORIAL
// --------------------

void esperar(){
    cout << ".";
    this_thread::sleep_for(std::chrono::milliseconds(300));
    cout << ".";
    this_thread::sleep_for(std::chrono::milliseconds(300));
    cout << ".\n";
    this_thread::sleep_for(std::chrono::milliseconds(300));
    cout << "\n\n\n";
}

void generarDatosDePrueba(Comando arreglo[], int& cant) {
    cant = 0;
    // Codigos unicos (100 a 109) y desordenados a proposito, para poder probar el ordenamiento.
    // Con 10 comandos cargados, el proximo codigo (cant + 100) es 110: queda libre.
    cargarComando(arreglo[0], 103, "hora","Sistema","La hora actual es 16:00 hs.","Informa la hora actual.",12,cant);
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

int agregarComando(Comando arreglo[], int& cant, int cod, const char* clave, const char* cat, const char* resp, const char* desc, int u) {
    if (cant >= MAX_COMANDOS) {
        return -1;
    }
    cargarComando(arreglo[cant], cod, clave, cat, resp, desc, u, cant);
    if (cant == MAX_COMANDOS) {
        return -2;
    }
    return cant;

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

void menuDeOpciones(){
    cout << "------ elija una opcion ------- \n ";
    cout << "oprima 1 para hablar con la IA \n";
    cout << "oprima 2 para entrar al menu de ajustes \n";
    cout << "oprima 0 para terminar el programa \n" ;
};

// -------------------
//  MANEJO DE ARCHIVOS
// -------------------

// Pide un nombre de archivo .dat (agrega ".dat" si falta). Devuelve false si se deja el input vacio.
// Solo escribe en destino cuando devuelve true.
bool pedirNombreDat(const char* mensaje, char* destino) {
    char entrada[MAX_ENTRADA_NOMBRE];
    cout << mensaje;
    leerLinea(entrada, MAX_ENTRADA_NOMBRE);

    char nombre[MAX_NOMBRE_ARCHIVO];
    armarNombreDat(entrada, nombre, MAX_NOMBRE_ARCHIVO);
    if (strcmp(nombre, ".dat") == 0) return false;   // input vacio
    strcpy(destino, nombre);
    return true;
}

// Igual que pedirNombreDat, pero insiste hasta que el archivo exista (o se deje vacio).
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

// Lista los .dat de la carpeta. Devuelve cuantos hay.
int listarArchivosDat() {
    char nombres[MAX_ARCHIVOS_DAT][MAX_NOMBRE_ARCHIVO];
    int n = detectarArchivosDat(nombres, MAX_ARCHIVOS_DAT);
    if (n > 0) {
        cout << "Se detectaron " << n << " archivo(s) .dat:\n";
        for (int i = 0; i < n; i++) cout << "  " << i + 1 << ". " << nombres[i] << "\n";
    }
    return n;
}

// Si todavia no hay un archivo cargado/guardado, pide el nombre. false si se cancela.
bool asegurarArchivoActual(char* archivoActual) {
    if (archivoActual[0] != '\0') return true;
    cout << "No hay un archivo .dat cargado.\n";
    return pedirNombreDat("Nombre del archivo .dat a usar (enter vacio para cancelar): ", archivoActual);
}

void cargarArchivoInicial(Comando comandos[], int& cantComandos, char* archivoActual) {
    this_thread::sleep_for(std::chrono::milliseconds(600));
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
    ordenarPorCategoria(comandos, cantComandos);
    guardarEnArchivoBinario(comandos, cantComandos, archivoActual);
    listadoPorCategoria(archivoActual);
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

    guardarEnArchivoBinario(comandos, cantComandos, archivoActual);
    modificarRespuestaRegistroN(archivoActual, n, nuevaResp);
    int lectura = leerDesdeArchivoBinario(comandos, cantComandos, archivoActual);
    if(lectura == -1){
        "por alguna razon el guardado fallo";
    }
}

void fusionarComandos() {
    int n = listarArchivosDat();
    if (n < 2) {
        cout << "[!] Se necesitan al menos 2 archivos .dat para fusionar (detectados: " << n << ").\n";
        return;
    }
    char archivoA[MAX_NOMBRE_ARCHIVO], archivoB[MAX_NOMBRE_ARCHIVO], salida[MAX_NOMBRE_ARCHIVO];
    if (!pedirArchivoExistente("Primer archivo (enter vacio para cancelar): ", archivoA)) {
        cout << "Fusion cancelada.\n";
        return;
    }
    if (!pedirArchivoExistente("Segundo archivo (enter vacio para cancelar): ", archivoB)) {
        cout << "Fusion cancelada.\n";
        return;
    }
    if (!pedirNombreDat("Nombre del archivo nuevo (enter vacio para cancelar): ", salida)) {
        cout << "Fusion cancelada.\n";
        return;
    }
    aparearArchivos(archivoA, archivoB, salida);
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


void conversarConLaIA(Comando comandos[MAX_COMANDOS], int cantComandos, int matrizUsos[CANT_CATEGORIAS][FRANJAS_HORARIAS], int franja, NodoHistorial*& historial, int& contadorID) {
    char frase[100] = "";      // inicializada: antes se comparaba sin haberle dado valor
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
    } while (strcmp(fraseMin, "adios") != 0 && !cin.eof());   // termina con "adios" (o si se cierra la entrada)
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
        cout << "Elija criterio (1: Codigo, 2: Palabra, otro: Cancelar): ";
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

// Pide un texto que no puede quedar vacio.
void pedirTextoNoVacio(const char* mensaje, char* destino, int tam) {
    do {
        cout << mensaje;
        leerLinea(destino, tam);
        if (destino[0] == '\0') cout << "No puede quedar vacio.\n";
    } while (destino[0] == '\0' && !cin.eof());
}

// Pide 1 a 1 los datos del comando. El codigo se calcula solo (cant + 100) y todo se pasa a agregarComando().
void crearComando(Comando comandos[], int& cantComandos) {
    if (cantComandos >= MAX_COMANDOS) {
        cout << "[!] Se alcanzo el limite de " << MAX_COMANDOS << " comandos. No se pueden agregar mas.\n";
        return;
    }

    char clave[30], respuesta[100], descripcion[100];
    bool repetida;
    do {
        pedirTextoNoVacio("Palabra clave: ", clave, 30);
        repetida = existePalabraClave(comandos, cantComandos, clave);
        if (repetida) cout << "Esa palabra clave ya existe en otro comando. Escriba otra.\n";
    } while (repetida && !cin.eof());

    const char* categorias[CANT_CATEGORIAS] = { "Sistema", "Consultas", "Ocio" };
    int cat;
    do {
        cout << "Categoria (1: Sistema | 2: Consultas | 3: Ocio): ";
        cat = leerEntero();
    } while ((cat < 1 || cat > CANT_CATEGORIAS) && !cin.eof());
    if (cat < 1 || cat > CANT_CATEGORIAS) return;   // entrada cerrada

    pedirTextoNoVacio("Respuesta: ", respuesta, 100);
    pedirTextoNoVacio("Descripcion: ", descripcion, 100);

    int usos;
    do {
        cout << "Usos iniciales (0 o mas): ";
        usos = leerEntero();
    } while (usos < 0 && !cin.eof());
    if (usos < 0) return;

    int codigo = calcularCodigoNuevo(comandos, cantComandos);
    int flag = agregarComando(comandos, cantComandos, codigo, clave, categorias[cat - 1], respuesta, descripcion, usos);
    switch(flag){
        case -1:
        cout << "no se agrego el comando, se exedio el limite de comandos\n";
    break;
        case -2:
        cout << "comando agregado correctamente. Se alcanzo el limite de comandos\n";
    break;
        default:
        cout << "comando agregado correctamente\n";
    break;
        
    }
}

// -------------------
//  SIMULACION DE CONVERSACION
// -------------------

// Busca un comando cuya palabra clave sea IGUAL al texto (sin distinguir mayusculas). -1 si no hay.
int buscarComandoExacto(Comando arreglo[], int cant, const char* texto) {
    char textoMin[100], claveMin[30];
    aMinuscula(textoMin, texto);
    for (int i = 0; i < cant; i++) {
        aMinuscula(claveMin, arreglo[i].palabraClave);
        if (strcmp(textoMin, claveMin) == 0) return i;
    }
    return -1;
}

// Pide comandos uno tras otro (enter vacio para terminar), los encola y al final recorre la cola
// para guardarla en usuario.txt (un comando por linea).
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
        colar(cola, linea);
    }

    if (estaVacio(cola)) {
        cout << "No se escribio ningun comando: no se genero usuario.txt.\n";
        return;
    }

    cout << "\nComandos encolados:\n";
    recorrerCola(cola, cout);
    mostrarFinal(cola);

    ofstream txt("usuario.txt");
    if (!txt) {
        cout << "Error al crear usuario.txt\n";
    }
    else {
        recorrerCola(cola, txt);
        txt.close();
        cout << "\n[OK] Archivo 'usuario.txt' generado.\n";
    }
    liberarCola(cola);
}

// Lee usuario.txt; responde cada mensaje si hay un comando en memoria con ese nombre, y apila las
// respuestas. Al final desapila todo (sin invertir: sale primero lo ultimo) a simulacro.txt con el
// mismo formato que sesion.txt.
// Por cada interaccion se apilan DOS textos: primero el mensaje del usuario y despues la respuesta de la IA,
// asi al desapilar sale la respuesta y justo despues su mensaje.
void simularIA(Comando comandos[], int cantComandos) {
    if (cantComandos == 0) {
        cout << "[!] No hay comandos en memoria. Cargue datos primero.\n";
        return;
    }
    ifstream archivo("usuario.txt");
    if (!archivo) {
        cout << "No se encontro 'usuario.txt'. Simule primero una conversacion de usuario.\n";
        return;
    }

    Pila pila;
    crearPila(pila);
    int total = 0;
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

        int pos = buscarComandoExacto(comandos, cantComandos, linea);
        const char* respuesta = (pos != -1) ? comandos[pos].respuesta : "no se como responder eso";

        cout << "Usuario dice: " << linea << "\nIA responde: " << respuesta << "\n";
        apilar(pila, linea);
        apilar(pila, respuesta);
        total++;
    }
    archivo.close();

    if (total == 0) {
        cout << "'usuario.txt' no tiene mensajes: no se genero simulacro.txt.\n";
        return;
    }

    ofstream txt("simulacro.txt");
    if (!txt) {
        cout << "Error al crear simulacro.txt\n";
        liberarPila(pila);
        return;
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
    liberarPila(pila);
    cout << "\n[OK] Archivo 'simulacro.txt' generado.\n";
}

void simularConversacion(Comando comandos[], int cantComandos) {
    cout << "1: Simular conversacion de usuario | 2: Simular respuestas de la IA | otro: Cancelar: ";
    int sub = leerEntero();
    if (sub == 1) simularUsuario();
    else if (sub == 2) simularIA(comandos, cantComandos);
}

// -------------------
//      MAIN
// -------------------


void sesionDeUsuario(Comando comandos[MAX_COMANDOS], int cantComandos, int matrizUsos[CANT_CATEGORIAS][FRANJAS_HORARIAS], NodoHistorial*& historial, int& contadorID) {
    if (cantComandos == 0) {
        cout << "\n[!] No hay comandos cargados. Cargue datos de prueba primero (opcion 2).\n";
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
            
            break;
        case 3:
            preguntarQueHaceUnComando(comandos, cantComandos);
            break;
        case 4: 
            mostrarOpcionesDeSesion();
            break;
        case 0:
            cout << "IA: guardando sesion en txt";
            esperar();
            exportarSesionTXT(historial);
            cout << "IA: Hasta luego!\n";
            cout << "\n\n";
            menuDeOpciones();
            break;
        default:
            cout << "Opcion invalida.\n";
        }
    } while (opcion != 0);
}

void sesionDeAdministrador(Comando comandos[MAX_COMANDOS], int& cantComandos, int matrizUsos[CANT_CATEGORIAS][FRANJAS_HORARIAS], NodoHistorial*& historial, int& contadorID, char* archivoActual){
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
            break;
        }
        default:
            cout << "Opcion invalida, intente otra vez.\n";
        }
    } while (opcion != 0); 
};


int main() {
    Comando comandos[MAX_COMANDOS];
    int cantComandos = 0;

    int matrizUsos[CANT_CATEGORIAS][FRANJAS_HORARIAS] = { { 0 } };

    NodoHistorial* historialSesion = NULL;
    int contadorIDInteraccion = 1;

    char archivoActual[MAX_NOMBRE_ARCHIVO] = "";   // .dat cargado / guardado en esta ejecucion ("" = ninguno)

    cargarArchivoInicial(comandos, cantComandos, archivoActual);
    std::this_thread::sleep_for(std::chrono::milliseconds(500));
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
            sesionDeAdministrador(comandos, cantComandos, matrizUsos, historialSesion, contadorIDInteraccion, archivoActual);
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
