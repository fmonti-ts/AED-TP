

--------------------------
    archivos
--------------------------

void guardarEnArchivoBinario
    Recibe 
        Arreglo con todos los comandos que la IA conoce (en memoria)
        cant de comandos que la  IA conoce (en memoria)
    Retorna 
        nada
    Hace
        crea o sobreescibe el archivo .dat guardando todos los comandos quue la ia puede ejecutar + su ejecucion

void leerDesdeArchivoBinario
    Recibe 
        Arreglo con todos los comandos que la IA conoce (en memoria)
        puntero a la cant de comandos que la  IA conoce (en memoria)
    Retorna
        nada
    Hace
        Busca el archivo .dat para abrir y lo intenta abrir

modificarRespuestaRegistroN
    Recibe 
        que registro se quiere cambiar en el arhcivo
        el nuevo dato de ese registro
    Retorna
        nada
    Hace 
        busca el registro con el puntero seekg y escribe sobre el con el seekp
listadoPorCategoria
    Recibe
        nada
    Retorna
        nada
    Hace
        un listado de cada comando que puede usar la IA en el arcivo haciendo corte de control
exportarSesionTXT
    Recibe
        el primer nodo del historial
    Retorna
        nada
    Hace
        recorre el historial de principio a fin y mientras recorre lo va guardando

--------------------------
    comando
--------------------------

mostrarMenuAdministracion()
    Recibe
        una posicion de array de comandos y todos los datos que va a guardar en esa posicion
    Retorna
        nada
    Hace
        guarda los datos en esa poscicion del array para usar como comandos

cargarComando()
    Recibe
        nada
    Retorna
        nada
    Hace
        muestra las opciones del menu administracion

mostrarComandosEnMemoria()
    Recibe
        arreglo con todos los comandos que la IA conoce (memoria)
        la cantidad de comandos que la IA conoce (memoria)
    Retorna
        nada
    Hace
        Recorre el arreglo de los comanddos en memoria y los muestra por consola con todos sus atributos

mostrarComandosEnMemoria()
    Recibe
        arreglo con todos los comandos que la IA conoce (memoria)
        la cantidad de comandos que la IA conoce (memoria)
    Retorna
        nada
    Hace
        Recorre el arreglo de los comanddos en memoria y los muestra por consola con todos sus atributos
    

--------------------------
    inputs
--------------------------

--------------------------
    lista
--------------------------

