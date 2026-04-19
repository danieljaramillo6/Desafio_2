#include "jugador.h"
#include <cstring>

Jugador::Jugador() {
    nombre = nullptr;
    apellido = nullptr;
    dorsal = 0;

    goles = 0;
    minutosJugados = 0;
    tarjetasAmarillas = 0;
    tarjetasRojas = 0;
    faltas = 0;
}

Jugador::Jugador(const char *nombre, const char *apellido, int dorsal){
    if (nombre != nullptr) {
        this->nombre = new char[strlen(nombre) + 1];
        strcpy(this->nombre,nombre);
    } else {
        nombre = nullptr;
    }


    if (apellido != nullptr) {
        this->apellido = new char[strlen(apellido) + 1];
        strcpy(this->apellido,apellido);
    } else {
        apellido = nullptr;
    }

    this->dorsal = dorsal;

    goles = 0;
    faltas = 0;
    minutosJugados = 0;
    tarjetasAmarillas = 0;
    tarjetasRojas = 0;
}

Jugador::Jugador(const Jugador& otra) {
    if (otra.nombre != nullptr) {
        nombre = new char[strlen(otra.nombre) + 1];
        strcpy(nombre, otra.nombre);
    } else {
        nombre = nullptr;
    }

    if (otra.apellido != nullptr) {
        apellido = new char[strlen(otra.apellido) + 1];
        strcpy(apellido, otra.apellido);
    } else {
        apellido = nullptr;
    }

    dorsal = otra.dorsal;
    goles = otra.goles;
    tarjetasAmarillas = otra.tarjetasAmarillas;
    tarjetasRojas = otra.tarjetasRojas;
    faltas = otra.faltas;
    minutosJugados = otra.minutosJugados;
}
Jugador& Jugador::operator=(const Jugador& otra) {

    if (this != &otra) {

        // liberar memoria actual
        delete[] nombre;
        delete[] apellido;

        // copiar nombre
        if (otra.nombre != nullptr) {
            nombre = new char[strlen(otra.nombre) + 1];
            strcpy(nombre, otra.nombre);
        } else {
            nombre = nullptr;
        }

        // copiar apellido
        if (otra.apellido != nullptr) {
            apellido = new char[strlen(otra.apellido) + 1];
            strcpy(apellido, otra.apellido);
        } else {
            apellido = nullptr;
        }

        dorsal = otra.dorsal;
        goles = otra.goles;
        tarjetasAmarillas = otra.tarjetasAmarillas;
        tarjetasRojas = otra.tarjetasRojas;
        faltas = otra.faltas;
        minutosJugados = otra.minutosJugados;
    }

    return *this;
}

Jugador::~Jugador(){
    delete[] nombre;
    delete[] apellido;
}

Jugador& Jugador::operator++() {
    goles++;
    return *this;
}

Jugador& Jugador::operator++(int) {
    Jugador copia = *this;
    goles++;
    return copia;
}

void Jugador::recibirAmarilla(){
    tarjetasAmarillas++;
    return;
}

void Jugador::recibirRoja(){
    tarjetasRojas++;
    return;
}

void Jugador::cometerFalta(){
    faltas++;
    return;
}

void Jugador::agregarMinutos(int minutos){
    minutosJugados += minutos;
    return;
}

int Jugador::getDorsal()const{
    return dorsal;
}

int Jugador::getMinutos()const{
    return minutosJugados;
}

int Jugador::getGoles()const{
    return goles;
}

const char* Jugador::getNombre()const{
    return nombre;
}

const char* Jugador::getApellido()const{
    return apellido;
}

