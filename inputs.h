

#ifndef INPUTS_H
#define INPUTS_H

void aMinuscula(char* destino, const char* origen);
int leerEntero();
// Con permitirVacio = false repite hasta que se escriba algo (o se cierre la entrada).
void leerLinea(char* destino, int tam, bool permitirVacio = true);
int pedirHora();
// Unico lugar donde el programa se detiene (pausas de la interfaz)
void pausar(int milisegundos);
#endif