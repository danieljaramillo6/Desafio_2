#ifndef MANAGER_H
#define MANAGER_H
#include <string.h>
#include "seleccion.h"
Seleccion* CrearSelecciones(const char* archivo, int& cantidad);
void CrearBombos(Seleccion* selecciones, Seleccion* Bombos[4][12]);
bool esValido(Grupo& g, Seleccion* s);
Grupo* CrearGrupos(Seleccion* Bombos[4][12]);
#endif // MANAGER_H
