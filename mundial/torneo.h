#ifndef TORNEO_H
#define TORNEO_H
#include "grupo.h"
#include "partido.h"
#include "manager.h"

class Torneo {
private:
    Seleccion* selections;
    int n_selections;

    Grupo* groups;
    Partido* group_matches;

    Seleccion** classifieds;
    int n_classifieds;

    Partido* sixteenth_final;
    Partido* eighth_final;
    Partido* fourth_final;
    Partido* semi_final;
    Partido final;
    Partido third_place;

    Seleccion* r16_teams[16];
    Seleccion* r8_teams[8];
    Seleccion* r4_teams[4];

    int ultimo_dia[48];

    // métricas
    mutable long long iter;

    // helpers
    int    buscarIndice(Seleccion* s) const;
    void   asignarFechasJornada(Partido* partidos, int n,
                              int dia_inicio, int dia_fin);
    size_t calcularMemoriaTotal() const;
    void   reportar(const char* func) const;

public:
    Torneo();
    Torneo(const char* archivo);
    Torneo(const Torneo& otro);
    ~Torneo();
    Torneo& operator=(const Torneo& otro);

    void cargar(const char* archivo);
    void imprimirGrupos() const;
    void simularFaseGrupos();
    void clasificar();
    void simular_16();
    void simular_8();
    void simular_4();
    void simular_semi();
    void simular_Final();
    void imprimirEstadisticas() const;
    void guardarCSV(const char* archivo) const;
};

#endif
