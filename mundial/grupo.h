#ifndef GRUPO_H
#define GRUPO_H
#include <cstddef>
#include "Seleccion.h"

class Grupo {
private:
    char letra;
    Seleccion** selecciones;  // arreglo dinámico de punteros
    int n_selecciones;

public:
    // Constructores
    Grupo();
    Grupo(char letra);
    Grupo(const Grupo& otro);

    // Destructor
    ~Grupo();

    // Operador de asignación
    Grupo& operator=(const Grupo& otro);

    // Métodos
    void agregarSeleccion(Seleccion* sel);
    void imprimirGrupo(long long& iter) const;
    void ordenarGrupo(long long& iter);
    size_t calcularMemoria() const;

    // Getters
    char getLetra() const;
    Seleccion* getSeleccion(int indice);
    int getCantidad();
};

#endif
