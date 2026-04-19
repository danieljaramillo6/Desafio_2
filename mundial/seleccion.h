#ifndef SELECCION_H
#define SELECCION_H

#include "Jugador.h"

class Seleccion {
private:
    char* nombre;
    char* dt;
    char* confederacion;
    char* federacion;

    int rankingFIFA;

    int golesFavor;
    int golesContra;

    int partidosGanados;
    int partidosEmpatados;
    int partidosPerdidos;
    int puntosGrupo;
    int gfGrupo;
    int gcGrupo;
    int pgGrupo;
    int peGrupo;
    int ppGrupo;

    Jugador* jugadores; // arreglo dinámico (26)

public:
    Seleccion();
    Seleccion(const char* nombre, const char* dt, const char* confederacion,const char* federacion, int ranking, int golesFavor,int golesContra, int pp, int pg, int pe );
    Seleccion(const Seleccion& otra);

    ~Seleccion();

    void registrarVictoria();
    void registrarEmpate();
    void registrarDerrota();
    void repartirgoles();
    //Sumar goles a favor
    Seleccion& operator++();
    Seleccion& operator++(int);
    //Sumar goles en contra
    Seleccion& operator--();
    Seleccion& operator--(int);


    int getDifGoles() const;
    int getRanking() const;
    int getGolesFavor() const;
    int getGolesContra() const;
    Jugador& getJugador(int indice);
    int getPuntosGrupo() const;
    int getGFGrupo() const;
    int getGCGrupo() const;
    int getDGGrupo() const;
    int getPGGrupo() const;
    int getPEGrupo() const;
    int getPPGrupo() const;
    char* getnombre() const;
};

#endif
