#ifndef PARTIDO_H
#define PARTIDO_H
#include "Seleccion.h"
class Partido
{
private:
    Seleccion* local;
    Seleccion* visiting;

    int gol_local, gol_visiting;
    float xg_l, xg_v;
    float posetion_l, posetion_v;
    int once_l[11];
    int once_v[11];

    bool pos_prorroga;
    int min_game;

    char* sede;
    char* date;
    char* hour;
    char* referees[3];

    float calcularXG(Seleccion* team_a, Seleccion* team_b);
    void once_convocado(Seleccion* team, int once[11]);
    void events_players(Seleccion* team, int once[11], int goals);


public:
    Partido();
    Partido(Seleccion* l,Seleccion* v, const char* date, const char* sede);
    Partido(const Partido& otro);
    ~Partido();

    void simulator(bool eliminatoria = false);
    void update_statistics();
    void imp_result() const;

    int getGol_local() const;
    int getGol_visiting() const;
    bool fue_prorroga() const;



};

#endif
