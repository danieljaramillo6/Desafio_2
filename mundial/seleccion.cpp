#include "Seleccion.h"
#include <cstring>

Seleccion::Seleccion() {
    nombre=nullptr;
    dt=nullptr;
    confederacion=nullptr;
    federacion=nullptr;
    rankingFIFA=0;
    golesFavor=0;
    golesContra=0;
    partidosEmpatados=0;
    partidosGanados=0;
    partidosPerdidos=0;
    puntosGrupo=0;
    gfGrupo=0;
    gcGrupo=0;
    pgGrupo=0;
    peGrupo=0;
    ppGrupo=0;
    jugadores=nullptr;

}

Seleccion::Seleccion(const char* nombre, const char* dt, const char* confederacion,const char* federacion, int ranking, int golesFavor,int golesContra, int pp, int pg, int pe ){
    if (nombre != nullptr) {
        this->nombre = new char[strlen(nombre) + 1];
        strcpy(this->nombre,nombre);
    } else {
        this->nombre = nullptr;
    }

    if (dt != nullptr) {
        this->dt = new char[strlen(dt) + 1];
        strcpy(this->dt,dt);
    } else {
        this->dt = nullptr;
    }

    if (confederacion != nullptr) {
        this->confederacion = new char[strlen(confederacion) + 1];
        strcpy(this->confederacion,confederacion);
    } else {
        this->confederacion = nullptr;
    }

    if (federacion != nullptr) {
        this->federacion = new char[strlen(federacion) + 1];
        strcpy(this->federacion,federacion);
    } else {
        this->federacion = nullptr;
    }

    this->rankingFIFA=ranking;
    this->golesFavor=golesFavor;
    this->golesContra=golesContra;
    this->partidosEmpatados=pe;
    this->partidosGanados=pg;
    this->partidosPerdidos=pp;
    puntosGrupo=0;
    gfGrupo=0;
    gcGrupo=0;
    pgGrupo=0;
    peGrupo=0;
    ppGrupo=0;

    jugadores = new Jugador[26];
    for (int i=0;i<26;i++){
        char nombreTemp[6 + 2 + 1];
        char apellidoTemp[8 + 2 + 1];
        char num[3]; // para el número (máx 2 dígitos + '\0')

        // convertir número a char manualmente
        int n = i + 1;

        if (n < 10) {
            num[0] = '0' + n;
            num[1] = '\0';
        } else {
            num[0] = '0' + (n / 10);
            num[1] = '0' + (n % 10);
            num[2] = '\0';
        }

        // construir "nombreX"
        strcpy(nombreTemp, "nombre");
        strcat(nombreTemp, num);

        // construir "apellidoX"
        strcpy(apellidoTemp, "apellido");
        strcat(apellidoTemp, num);

        // crear jugador
        jugadores[i] = Jugador(nombreTemp, apellidoTemp, n);
    }
}

Seleccion::Seleccion(const Seleccion& otra) {
    if (otra.nombre != nullptr) {
        this->nombre = new char[strlen(otra.nombre) + 1];
        strcpy(this->nombre, otra.nombre);
    } else {
        this->nombre = nullptr;
    }

    if (otra.dt != nullptr) {
        this->dt = new char[strlen(otra.dt) + 1];
        strcpy(this->dt, otra.dt);
    } else {
        this->dt = nullptr;
    }

    if (otra.confederacion != nullptr) {
        this->confederacion = new char[strlen(otra.confederacion) + 1];
        strcpy(this->confederacion, otra.confederacion);
    } else {
        this->confederacion = nullptr;
    }

    if (otra.federacion != nullptr) {
        this->federacion = new char[strlen(otra.federacion) + 1];
        strcpy(this->federacion, otra.federacion);
    } else {
        this->federacion = nullptr;
    }

    this->rankingFIFA = otra.rankingFIFA;
    this->golesFavor = otra.golesFavor;
    this->golesContra = otra.golesContra;
    this->partidosEmpatados = otra.partidosEmpatados;
    this->partidosGanados = otra.partidosGanados;
    this->partidosPerdidos = otra.partidosPerdidos;
    this->puntosGrupo=otra.puntosGrupo;
    this->gfGrupo=otra.gfGrupo;
    this->gcGrupo=otra.gcGrupo;
    this->pgGrupo=otra.pgGrupo;
    this->peGrupo=otra.peGrupo;
    this->ppGrupo=otra.ppGrupo;

    jugadores = new Jugador[26];

    for (int i = 0; i < 26; i++) {
        jugadores[i] = otra.jugadores[i];
    }
}

Seleccion::~Seleccion() {
    delete[] nombre;
    delete[] dt;
    delete[] confederacion;
    delete[] federacion;

    delete[] jugadores;
}

Seleccion& Seleccion::operator=(const Seleccion& otra) {
    if (this != &otra) {

        delete[] nombre;
        delete[] dt;
        delete[] confederacion;
        delete[] federacion;
        delete[] jugadores;

        if (otra.nombre) {
            nombre = new char[strlen(otra.nombre) + 1];
            strcpy(nombre, otra.nombre);
        } else nombre = nullptr;

        if (otra.dt) {
            dt = new char[strlen(otra.dt) + 1];
            strcpy(dt, otra.dt);
        } else dt = nullptr;

        if (otra.confederacion) {
            confederacion = new char[strlen(otra.confederacion) + 1];
            strcpy(confederacion, otra.confederacion);
        } else confederacion = nullptr;

        if (otra.federacion) {
            federacion = new char[strlen(otra.federacion) + 1];
            strcpy(federacion, otra.federacion);
        } else federacion = nullptr;

        rankingFIFA = otra.rankingFIFA;
        golesFavor = otra.golesFavor;
        golesContra = otra.golesContra;
        partidosGanados = otra.partidosGanados;
        partidosEmpatados = otra.partidosEmpatados;
        partidosPerdidos = otra.partidosPerdidos;

        gfGrupo = otra.gfGrupo;
        gcGrupo = otra.gcGrupo;
        pgGrupo = otra.pgGrupo;
        peGrupo = otra.peGrupo;
        ppGrupo = otra.ppGrupo;

        jugadores = new Jugador[26];
        for (int i = 0; i < 26; i++) {
            jugadores[i] = otra.jugadores[i];
        }
    }
    return *this;
}

Seleccion& Seleccion::operator++() {
    golesFavor++;
    gfGrupo++;
    return *this;
}

Seleccion& Seleccion::operator++(int) {
    Seleccion copia = *this;
    golesFavor++;
    gfGrupo++;
    return copia;
}

Seleccion& Seleccion::operator--() {
    golesContra++;
    gcGrupo++;
    return *this;
}

Seleccion& Seleccion::operator--(int) {
    Seleccion copia = *this;
    golesContra++;
    gcGrupo++;
    return copia;
}

void Seleccion::registrarVictoria() {
    partidosGanados++;
    pgGrupo++;
    puntosGrupo+=3;
    return;
}

void Seleccion::registrarEmpate() {
    partidosEmpatados++;
    peGrupo++;
    puntosGrupo++;
    return;
}

void Seleccion::registrarDerrota() {
    ppGrupo++;
    partidosPerdidos++;
    return;
}

int Seleccion::getRanking() const {
    return rankingFIFA;
}

int Seleccion::getGolesFavor() const {
    return golesFavor;
}

int Seleccion::getGolesContra() const {
    return golesContra;
}

int Seleccion::getDifGoles()const{
    return golesFavor-golesContra;
}

Jugador& Seleccion::getJugador(int indice) {
    return jugadores[indice];
}

void Seleccion::repartirgoles() {
    int golesjugador = golesFavor / 26;
    int sobra = golesFavor % 26;
    for (int i = 0; i < 26; i++) {
        for (int j = 0; j < golesjugador; j++) {
            jugadores[i]++;
        }
        if (i < sobra) {
            jugadores[i]++;
        }
    }
}

int Seleccion::getDGGrupo()const{
    return gfGrupo-gcGrupo;
}

int Seleccion::getGCGrupo()const{
    return gcGrupo;
}

int Seleccion::getGFGrupo()const{
    return gfGrupo;
}

int Seleccion::getPuntosGrupo()const{
    return puntosGrupo;
}

int Seleccion::getPGGrupo()const{
    return pgGrupo;
}

int Seleccion::getPEGrupo()const{
    return peGrupo;
}

int Seleccion::getPPGrupo()const{
    return ppGrupo;
}
char* Seleccion::getnombre()const{
    return nombre;
}

char* Seleccion::getConfederacion() const{
    return confederacion;
}
int Seleccion::getPartidosGanados() const  { return partidosGanados; }
int Seleccion::getPartidosEmpatados() const { return partidosEmpatados; }
int Seleccion::getPartidosPerdidos() const  { return partidosPerdidos; }

const char* Seleccion::getDt()         const { return dt; }
const char* Seleccion::getFederacion() const { return federacion; }

size_t Seleccion::calcularMemoria() const {
    size_t mem = sizeof(Seleccion);
    if (nombre)        mem += strlen(nombre)        + 1;
    if (dt)            mem += strlen(dt)            + 1;
    if (confederacion) mem += strlen(confederacion) + 1;
    if (federacion)    mem += strlen(federacion)    + 1;
    if (jugadores)
        for (int i = 0; i < 26; i++)
            mem += jugadores[i].calcularMemoria();
    return mem;
}
