#ifndef TORNEO_H
#define TORNEO_H
#include "grupo.h"
#include "partido.h"
#include "manager.h"

class Torneo
{
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
};


#endif // TORNEO_H
