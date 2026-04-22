#include "partido.h"
#include <cmath>
#include <cstring>
#include <cstdlib>
#include <iostream>
using namespace std;

// constructor por defecto
// inicializa todos los atributos en valores neutros
// los punteros en nullptr y los arreglos en -1

Partido::Partido()
{
    local = nullptr;
    visiting = nullptr;
    gol_local = 0;
    gol_visiting = 0;
    xg_l = 0;
    xg_v = 0;
    posetion_l = 50.0f;
    posetion_v = 50.0f;
    pos_prorroga = false;
    min_game = 90;
    sede = nullptr;
    date = nullptr;
    hour = nullptr;
    for (int i = 0; i < 3; i++)
    {
        referees[i] = nullptr;
    }
    for (int i = 0; i < 11; i++)
    {
        once_l[i] = -1;
        once_v[i] = -1;

        //-1 es que aun no han sido convocados :-)
    }
}

// constructor con parametros
// recibe los dos equipos, la fecha y la sede
// la hora y los arbitros tambien los recibe
Partido::Partido(Seleccion* l, Seleccion* v, const char* _date, const char* _sede) {
    local = l;
    visiting = v;
    gol_local = 0;
    gol_visiting = 0;
    xg_l = 0;
    xg_v = 0;
    posetion_l = 50.0f;
    posetion_v = 50.0f;
    pos_prorroga = false;
    min_game = 90;

    for (int i = 0; i < 11; i++)
    {
        once_l[i] = -1;
        once_v[i] = -1;
    }

    // copia la fecha en memoria dinamica propia
    // no se puede hacer date = _date porque _date es solo lectura
    if (_date != nullptr)
    {
        date = new char[strlen(_date) + 1];
        strcpy(date, _date);
    }
    else
    {
        date = nullptr;
    }

    //copia la sede en la memoria dinamica propia

    if (_sede != nullptr)
    {
        sede = new char[strlen(_sede) + 1];
        strcpy(sede, _sede);
    }
    else
    {
        sede = nullptr;
    }

    hour = new char[6];
    strcpy(hour, "00:00");

    referees[0] = new char[12];
    strcpy(referees[0], "codArbitro1");
    referees[1] = new char[12];
    strcpy(referees[1], "codArbitro2");
    referees[2] = new char[12];
    strcpy(referees[2], "codArbitro3");

    //arbitro y horas son fijos

}

Partido::Partido(const Partido& otro)
{
    local = otro.local;
    visiting = otro.visiting;
    gol_local = otro.gol_local;
    gol_visiting = otro.gol_visiting;
    xg_l = otro.xg_l;
    xg_v = otro.xg_v;
    posetion_l = otro.posetion_l;
    posetion_v = otro.posetion_v;
    pos_prorroga = otro.pos_prorroga;
    min_game = otro.min_game;

    for (int i = 0; i < 11; i++)
    {
        once_l[i] = otro.once_l[i];
        once_v[i] = otro.once_v[i];
    }

    if (otro.date != nullptr) {
        date = new char[strlen(otro.date) + 1];
        strcpy(date, otro.date);

    }
    else
    {
        date = nullptr;
    }

    if (otro.sede != nullptr)
    {
        sede = new char[strlen(otro.sede) + 1];
        strcpy(sede, otro.sede);
    }
    else
    {
        sede = nullptr;
    }

    if (otro.hour != nullptr)
    {
        hour = new char[strlen(otro.hour) + 1];
        strcpy(hour, otro.hour);
    }
    else
    {
        hour = nullptr;
    }

    for (int i = 0; i < 3; i++)
    {
        if (otro.referees[i] != nullptr)
        {
            referees[i] = new char[strlen(otro.referees[i]) + 1];
            strcpy(referees[i], otro.referees[i]);
        }
        else
        {
            referees[i] = nullptr;
        }
    }
}

//destructor se encarga de liberar memoria que se reservo
Partido::~Partido()
{
    delete[] date;

    delete[] sede;

    delete[] hour;

    for (int i = 0; i < 3; i++)
    {
        delete[] referees[i];
    }
}

// calcula los goles esperados del local contra el visitante
// GF y GC son los promedios historicos de goles a favor y en contra
float Partido::calcularXG(Seleccion* team_a, Seleccion* team_b)
{

    // total de partidos jugados por cada equipo en el torneo
    int pj_a = team_a->getPGGrupo() + team_a->getPEGrupo() + team_a->getPPGrupo();
    int pj_b = team_b->getPGGrupo() + team_b->getPEGrupo() + team_b->getPPGrupo();

    float GF = 0;
    float GC = 0;

    // si ya jugo partidos usa el promedio, si no usa el total historico
    // esto evita dividir por cero al inicio del torneo
    if (pj_a > 0)
    {
        GF = (float)team_a->getGolesFavor() / pj_a;
    }
    else
    {
        GF = (float)team_a->getGolesFavor();
    }

    if (pj_b > 0) {
        GC = (float)team_b->getGolesContra() / pj_b;
    }
    else
    {
        GC = (float)team_b->getGolesContra();
    }

    // evita que GF o GC sean cero para no romper el programa
    if (GF <= 0) GF = 0.1f;
    if (GC <= 0) GC = 0.1f;


    float mu = 1.35f;
    float alpha = 0.6f;
    float beta = 0.4f;

    // ecuacion de xg
    return mu * pow(GF / mu, alpha) * pow(GC / mu, beta);
}

void Partido::once_convocado(Seleccion* team, int once[11])
{
    int indices[26];
    for (int i = 0; i < 26; i++)
    {
        indices[i] = i;
    }

    for (int i = 0; i < 11; i++)
    {
        int j = i + rand() % (26 - i);
        int tmp = indices[i];
        indices[i] = indices[j];
        indices[j] = tmp;
        once[i] = indices[i];
    }
}

void Partido::events_players(Seleccion* team, int once[11], int goals)
{
    int goals_given = 0;

    for (int i = 0; i < 11; i++)
    {
        Jugador& j = team->getJugador(once[i]);

        j.agregarMinutos(min_game);

        if (goals_given < goals)
        {
            if ((rand() % 100) < 4)
            {
                ++j;
                goals_given++;
            }
        }

        if ((rand() % 10000) < 600)
        {
            j.recibirAmarilla();
            if ((rand() % 10000) < 115)
            {
                j.recibirAmarilla();
                j.recibirRoja();
            }
        }

        if ((rand() % 10000) < 1300)
        {
            j.cometerFalta();
            if ((rand() % 10000) < 275)
            {
                j.cometerFalta();
                if ((rand() % 10000) < 70)
                {
                    j.cometerFalta();
                }
            }
        }
    }

    while (goals_given < goals)
    {
        ++team->getJugador(once[goals_given % 11]);
        goals_given++;
    }
}

void Partido::simulator(bool eliminatoria)
{
    once_convocado(local, once_l);
    once_convocado(visiting, once_v);

    // la parte de arriba se encarga de elegir los 11 de cada equipo

    xg_l = calcularXG(local, visiting);
    xg_v = calcularXG(visiting, local);

    // calcula los goles esperados de cada seleccion

    gol_local = (int)xg_l;
    if ((rand() % 100) < (xg_l - gol_local) * 100)
    {
        gol_local++;
    }

    gol_visiting = (int)xg_v;
    if ((rand() % 100) < (xg_v - gol_visiting) * 100)
    {
        gol_visiting++;
    }

    //utiliza xg de local y visitante para calcular los goles de cada equipo

    float inv_l = 1.0f / local->getRanking();
    float inv_v = 1.0f / visiting->getRanking();
    float total = inv_l + inv_v;
    posetion_l = (inv_l / total) * 100.0f;
    posetion_v = (inv_v / total) * 100.0f;

    //calcula la posesion siempre el que tenga mejor posicion en el ranking tiene mayor posesion

    if (eliminatoria && gol_local == gol_visiting)
    {
        pos_prorroga = true;
        min_game = 120;
        int rL = local->getRanking();
        int rV = visiting->getRanking();
        if ((rand() % (rL + rV)) < rV)
        {
            gol_local++;
        }
        else
        {
            gol_visiting++;
        }
    }

    // verifica si va a haber prorroga si estan en eliminatorias

    events_players(local, once_l, gol_local);
    events_players(visiting, once_v, gol_visiting);

    //simula posibles eventos para los jugadoes
}


void Partido::update_statistics()
{
    for (int i = 0; i < gol_local; i++)
    {
        local->operator++();
    }
    for (int i = 0; i < gol_visiting; i++)
    {
        visiting->operator++();
    }
    for (int i = 0; i < gol_visiting; i++)
    {
        local->operator--();
    }
    for (int i = 0; i < gol_local; i++)
    {
        visiting->operator--();
    }

    if (gol_local > gol_visiting)
    {
        local->registrarVictoria();
        visiting->registrarDerrota();
    }
    else if (gol_visiting > gol_local)
    {
        visiting->registrarVictoria();
        local->registrarDerrota();
    }
    else
    {
        local->registrarEmpate();
        visiting->registrarEmpate();
    }
}


void Partido::imp_result() const
{
    cout << "Fecha: " << (date ? date : "?") << endl;
    cout << "Hora: " << (hour ? hour : "?") << endl;
    cout << "Sede: " << (sede ? sede : "?") << endl;
    cout << "Arbitros: " << (referees[0] ? referees[0] : "?");
    cout << ", " << (referees[1] ? referees[1] : "?");
    cout << ", " << (referees[2] ? referees[2] : "?") << endl;

    cout << local->getnombre() << " " << gol_local;
    cout << " - ";
    cout << gol_visiting << " " << visiting->getnombre();

    if (pos_prorroga) {
        cout << " (Prorroga)";
    }
    cout << endl;

    cout << "Posesion " << local->getnombre() << ": " << (int)posetion_l << "%" << endl;
    cout << "Posesion " << visiting->getnombre() << ": " << (int)posetion_v << "%" << endl;

    cout << "Goleadores " << local->getnombre() << ": ";
    for (int i = 0; i < 11; i++) {
        Jugador& j = local->getJugador(once_l[i]);
        if (j.getGoles() > 0) {
            cout << "#" << j.getDorsal() << " ";
        }
    }
    cout << endl;

    cout << "Goleadores " << visiting->getnombre() << ": ";
    for (int i = 0; i < 11; i++) {
        Jugador& j = visiting->getJugador(once_v[i]);
        if (j.getGoles() > 0) {
            cout << "#" << j.getDorsal() << " ";
        }
    }
    cout << endl;
}

int Partido::getGol_local() const {
    return gol_local;
}

int Partido::getGol_visiting() const {
    return gol_visiting;
}

bool Partido::fue_prorroga() const {
    return pos_prorroga;
}
