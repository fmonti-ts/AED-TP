# README DEMO

## ¿Qué hace el sistema?

En conjunto, se trata de una aplicación estructurada para la resolución de un problema algorítmico mediante la manipulación eficiente de memoria dinámica y archivos en C++.

Nuestro asistente conversacional simulado, con respuestas predeterminadas, muestra opciones sobre qué puede hacer el usuario y, al recibir la respuesta del humano, despliega nuevos menús o funciones para que el usuario logre lo que quiere hacer.

Esto nos permite, además de chatear con la IA, entrar al menú de administración, donde podemos:

- Agregar nuevos comandos para la IA
- Alterar el historial
- Simular conversaciones
- Usar otras funciones de administración de sesión

---

## ¿Cómo se compila?

Los comandos de la IA se guardan en un archivo. El usuario elige si hablar con la IA o administrar datos.

| Modo | Datos que utiliza |
|------|-------------------|
| **Hablar con la IA** | `comandos`, `cantComandos`, `matrizUsos`, `historialSesion`, contador del id del historial |
| **Menú de administración** | `comandos`, `cantComandos`, `matrizUsos`, `historialSesion`, `archivoActual` |

---

## ¿Qué hace cada módulo?

### Archivos
- Creación y lectura de archivos binarios `.dat`
- Lectura por secuencia y acceso aleatorio
- Apareo y corte de control
- Guardado de sesión

### Pila
- Sistema de respuestas de simulación de IA
- Funciones básicas de estructura LIFO

### Cola
- Sistema de simulación de comandos de usuario
- Funciones básicas de estructura FIFO

---

## Pregunta de diseño

### Matriz

| Estructura | Rol en el sistema |
|------------|-------------------|
| **Pila** | Manera natural en que la IA responde las preguntas del usuario. Lee los comandos como una cola y los va "apilando" en el `.txt`. |
| **Cola** | Serie de comandos de usuario que se guardan uno atrás del otro, para resolverse en orden de llegada. |
| **Lista** | Historial editable de conversación con la IA, guardado en memoria. Cada nodo apunta al siguiente. |
| **Array** | Lista de **todos** los comandos que entiende la IA, con sus respuestas. |
| **Matriz** | Sistema que ordena qué tipo de comando se usa más en cada franja horaria. |


## 1. Ordenamientos

| Función | Archivo | Mejor | Promedio | Peor | Observaciones |
|---|---|---|---|---|---|
| `ordenarPorCodigo` | comando.cpp | O(n) | O(n²) | O(n²) | Burbuja con bandera. El mejor caso es arreglo ya ordenado (una sola pasada). |
| `ordenarPorPalabra` | comando.cpp | O(n²) | O(n²) | O(n²) | Selección: siempre hace n(n-1)/2 comparaciones y como máximo n-1 intercambios. |
| `ordenarPorCategoria` | comando.cpp | O(n²) | O(n²) | O(n²) | Selección con desempate por palabra clave. |
| `estaOrdenadoPorCodigo` | comando.cpp | O(1) | O(1)* | O(n) | Corta en la primera inversión. El peor caso es arreglo ordenado. (*) Con datos al azar; con datos casi ordenados tiende a O(n). |
| `ordenarPorClave` | archivos.cpp | O(n) | O(n²) | O(n²) | Inserción estable. El mejor caso es arreglo ya ordenado. |
| `estaOrdenadoPorClave` | archivos.cpp | O(1) | O(1)* | O(n) | Igual que `estaOrdenadoPorCodigo`. |
| `detectarArchivos` | archivos.cpp | O(d + k) | O(d + k²) | O(d + k²) | Recorre la carpeta (O(d)) y ordena los nombres `.dat` por inserción (O(k²)). |

---

## 2. Matriz de usos

No hay ordenamiento de matriz en el código, solo recorridos y clasificación.

| Función | Archivo | Mejor | Promedio | Peor | Observaciones |
|---|---|---|---|---|---|
| `filaDeCategoria` | comando.cpp | O(1) | O(1) | O(1) | Hasta 2 `strcmp`. |
| `franjaDeHora` | comando.cpp | O(1) | O(1) | O(1) | Solo comparaciones de enteros. |
| `totalesMatrizUsos` | comando.cpp | Θ(F·C) | Θ(F·C) | Θ(F·C) | Recorre toda la matriz una vez. |
| `maximoMatrizUsos` | comando.cpp | Θ(F·C) | Θ(F·C) | Θ(F·C) | Recorre toda la matriz una vez. |

> `F` y `C` son constantes, así que respecto de `n` estas funciones son O(1).

---

## 3. Apareo de archivos

| Función | Archivo | Mejor | Promedio | Peor | Observaciones |
|---|---|---|---|---|---|
| `leerRegistros` | archivos.cpp | Θ(n) | Θ(n) | Θ(n) | Una lectura por registro. |
| `leerArchivoCompleto` | archivos.cpp | Θ(n) | Θ(n) | Θ(n) | Reserva memoria dinámica y llama a `leerRegistros`. |
| `ordenarPorCodigo` (llamada desde apareo) | comando.cpp | O(nr) | O(nr²) | O(nr²) | El resultado viene ordenado por clave, no por código. |
| **`aparearArchivos`** (total) | archivos.cpp | O(na + nb) | O((na + nb)²) | O((na + nb)²) | Suma de: lectura O(na+nb), `ordenarSiHaceFalta` (O(n) a O(n²)), mezcla O(na+nb), códigos únicos O(nr²), orden por código O(nr²) y escritura O(nr). |

> El cuello de botella del apareo no es la mezcla (lineal) sino `asegurarCodigosUnicos` y `ordenarPorCodigo`. Memoria extra: O(na + nb).

---

## 4. Búsquedas

| Función | Archivo | Mejor | Promedio | Peor | Observaciones |
|---|---|---|---|---|---|
| `buscarComando` | comando.cpp | O(1) | O(n) | O(n) | Búsqueda lineal con `strstr`. El mejor caso es que coincida el primero. |
| `buscarComandoExacto` | comando.cpp | O(1) | O(n) | O(n) | Búsqueda lineal con `strcmp`. |
| `existePalabraClave` | comando.cpp | O(1) | O(n) | O(n) | Delega en `buscarComandoExacto`. |
| `procesarFraseUsuario` | comando.cpp | O(1) | O(n) | O(n) | Busca cada clave dentro de la frase. |
| `buscarPorCodigoBinaria` | comando.cpp | O(1) | O(log n) | O(log n) | Requiere el arreglo ordenado por código. El mejor caso es el elemento central. |
| `calcularCodigoNuevo` | comando.cpp | O(n) | O(n) | O(n²) | `while` externo con `for` interno O(n). El peor caso es que haya hasta n colisiones consecutivas. |

---

## 5. Lista enlazada (historial)

| Función | Archivo | Mejor | Promedio | Peor | Observaciones |
|---|---|---|---|---|---|
| `crearNodo` | lista.cpp | O(1) | O(1) | O(1) | |
| `agregarInteraccionHistorial` | lista.cpp | O(1) | O(h) | O(h) | Inserta al final recorriendo toda la lista. El mejor caso es lista vacía. |
| `mostrarHistorialCompleto` | lista.cpp | O(1) | Θ(h) | Θ(h) | El mejor caso es lista vacía. |
| `buscarInteraccionPorPalabraClave` | lista.cpp | Θ(h) | Θ(h) | Θ(h) | No corta al encontrar: muestra todas las coincidencias. |
| `eliminarInteraccion` | lista.cpp | O(1) | O(h) | O(h) | El mejor caso es lista vacía o ID en la cabeza. |
| `liberarHistorial` | lista.cpp | Θ(h) | Θ(h) | Θ(h) | |
| `exportarSesionTXT` | archivos.cpp | Θ(h) | Θ(h) | Θ(h) | Recorre la lista y escribe una línea por nodo. |

> Cargar `h` interacciones con `agregarInteraccionHistorial` cuesta O(h²) en total. Con un puntero a la cola bajaría a O(h).

---

## 6. Corte de control y acceso a archivo binario

| Función | Archivo | Mejor | Promedio | Peor | Observaciones |
|---|---|---|---|---|---|
| `listadoPorCategoria` | archivos.cpp | O(1) | Θ(n) | Θ(n) | Un solo pase secuencial. El mejor caso es archivo vacío. Requiere el archivo ordenado por categoría. |
| `modificarRespuestaRegistroN` | archivos.cpp | O(1) | O(1) | O(1) | Acceso aleatorio: `seekg`/`seekp` y un solo registro leído y escrito. |
| `guardarEnArchivoBinario` | archivos.cpp | Θ(n) | Θ(n) | Θ(n) | |
| `leerDesdeArchivoBinario` | archivos.cpp | Θ(n) | Θ(n) | Θ(n) | Lee como máximo `MAX_COMANDOS`. |

---

## 7. Pila (pila.cpp)

| Función | Mejor | Promedio | Peor | Observaciones |
|---|---|---|---|---|
| `crearPila` | O(1) | O(1) | O(1) | |
| `estaVacia` | O(1) | O(1) | O(1) | |
| `apilar` | O(1) | O(1) | O(1) | Inserta en el tope. El `strncpy` es de tamaño constante. |
| `desapilar` | O(1) | O(1) | O(1) | |
| `traerPrimero` | O(1) | O(1) | O(1) | Consulta el tope sin quitarlo. |
| `recorrerPila` | O(1) | Θ(p) | Θ(p) | El mejor caso es pila vacía. |
| `liberarPila` | O(1) | Θ(p) | Θ(p) | El mejor caso es pila vacía. |
---

## 8. Cola (cola.cpp)

| Función | Mejor | Promedio | Peor | Observaciones |
|---|---|---|---|---|
| `crearCola` | O(1) | O(1) | O(1) | |
| `estaVacio` | O(1) | O(1) | O(1) | |
| `encolar` | O(1) | O(1) | O(1) | Gracias al puntero `fin` no recorre la cola. |
| `desencolar` | O(1) | O(1) | O(1) | |
| `traerPrimero` | O(1) | O(1) | O(1) | |
| `traerUltimo` | O(1) | O(1) | O(1) | Usa el puntero `fin`. |
| `mostrarFinal` | O(1) | O(1) | O(1) | |
| `recorrerCola` | O(1) | Θ(c) | Θ(c) | El mejor caso es cola vacía. |
| `liberarCola` | O(1) | Θ(c) | Θ(c) | El mejor caso es cola vacía. |
---

## Resumen

| Operación | Complejidad |
|---|---|
| Operaciones de pila y cola | O(1) |
| Búsqueda lineal / binaria | O(n) / O(log n) |
| Ordenar por código (burbuja) | O(n) a O(n²) |
| Ordenar por palabra o categoría (selección) | O(n²) siempre |
| Ordenar por clave (inserción) | O(n) a O(n²) |
| Mezcla del apareo | Θ(na + nb) |
| Apareo completo | O((na + nb)²) por la verificación de códigos únicos |
| Corte de control | Θ(n) |
| Acceso aleatorio a un registro | O(1) |