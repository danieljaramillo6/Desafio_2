#include "Grupo.h"
#include <iostream>
using namespace std;

// Constructores
Grupo::Grupo() {
    letra = '\0';
    selecciones = new Seleccion[4];
    n_selecciones=0;
}

Grupo::Grupo(char letra) {
    this->letra=letra;
    selecciones = new Seleccion[4];
    n_selecciones=0;
}

Grupo::Grupo(const Grupo& otro) {
    letra = otro.letra;

    selecciones = new Seleccion[4];

    for (int i = 0; i < 4; i++) {
        selecciones[i] = otro.selecciones[i];
    }
    n_selecciones=otro.n_selecciones;
}

// Destructor
Grupo::~Grupo() {
    delete[] selecciones;
}

// Operador de asignación
Grupo& Grupo::operator=(const Grupo& otro) {

    if (this != &otro) {

        // liberar memoria actual
        delete[] selecciones;

        // copiar datos simples
        letra = otro.letra;

        // reservar nueva memoria
        selecciones = new Seleccion[4];

        // copiar selecciones
        for (int i = 0; i < 4; i++) {
            selecciones[i] = otro.selecciones[i];
        }
    }

    return *this;
}

// Métodos
void Grupo::agregarSeleccion(const Seleccion& sel) {
    if (n_selecciones < 4) {
        selecciones[n_selecciones] = sel;
        n_selecciones++;
    }

}

void Grupo::imprimirGrupo() const { 
    cout << "Grupo " << letra << endl;
    cout << "Equipo\tPJ\tPG\tPE\tPP\tGF\tGC\tDG\tPts" << endl;
    for (int i = 0; i < 4; i++) {
        const Seleccion& s=selecciones[i];
        cout<<s.getnombre()<<"\t"<<(s.getPEGrupo()+s.getPGGrupo()+s.getPPGrupo())<<"\t"<<s.getPGGrupo()<<"\t"<<s.getPEGrupo()<<"\t"<<s.getPPGrupo()<<"\t"<<s.getGFGrupo()<<"\t"<<s.getGCGrupo()<<"\t"<<s.getDGGrupo()<<"\t"<<s.getPuntosGrupo()<<endl;
    }
}

void Grupo::ordenarGrupo() {

        for (int i = 0; i < 3; i++) {
            for (int j = 0; j < 3 - i; j++) {

                Seleccion& a = selecciones[j];
                Seleccion& b = selecciones[j + 1];

                bool cambiar = false;

                if (a.getPuntosGrupo() < b.getPuntosGrupo()) {
                    cambiar = true;
                }
                else if (a.getPuntosGrupo() == b.getPuntosGrupo()) {

                    if (a.getDGGrupo() < b.getDGGrupo()) {
                        cambiar = true;
                    }
                    else if (a.getDGGrupo() == b.getDGGrupo()) {

                        if (a.getGFGrupo() < b.getGFGrupo()) {
                            cambiar = true;
                        }
                    }
                }

                if (cambiar) {
                    Seleccion temp = selecciones[j];
                    selecciones[j] = selecciones[j + 1];
                    selecciones[j + 1] = temp;
                }
            }
        }
    }

// Getters
char Grupo::getLetra() const {
    return letra;
}

Seleccion& Grupo::getSeleccion(int indice) {
    return  selecciones[indice];
}

