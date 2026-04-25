#include "torneo.h"
#include <iostream>
#include <cstring>
#include <cstdlib>
#include <ctime>
using namespace std;

Torneo::Torneo()
{
    selections      = nullptr;
    n_selections    = 0;
    groups          = nullptr;
    group_matches   = nullptr;   // 72 partidos de grupos (12 grupos x 6)
    classifieds     = nullptr;
    n_classifieds   = 0;
    sixteenth_final = nullptr;   // 32 partidos
    eighth_final    = nullptr;   // 16 partidos
    fourth_final    = nullptr;   // 8 partidos
    semi_final      = nullptr;   // 4 partidos (2 semis + 2 plazas 3er/4o)
}
Torneo::Torneo(const char* archivo) : Torneo()
{
    cargar(archivo);
}

Torneo::Torneo(const Torneo& otro) {

    // ── 1. selections: arreglo de objetos reales ──
    n_selections = otro.n_selections;
    if (otro.selections) {
        selections = new Seleccion[n_selections];
        for (int i = 0; i < n_selections; i++)
            selections[i] = otro.selections[i];
    } else {
        selections = nullptr;
    }

    // ── 2. groups ──
    if (otro.groups) {
        groups = new Grupo[12];
        for (int i = 0; i < 12; i++)
            groups[i] = otro.groups[i];
    } else {
        groups = nullptr;
    }

    // ── 3. group_matches ──
    if (otro.group_matches) {
        group_matches = new Partido[72];
        for (int i = 0; i < 72; i++)
            group_matches[i] = otro.group_matches[i];
    } else {
        group_matches = nullptr;
    }

    // ── 4. classifieds: punteros → redirigir a nuestra selections ──
    // BUG si haces: classifieds[i] = otro.classifieds[i]
    // porque ese puntero apuntaría a otro.selections[j], que
    // se destruye cuando el otro objeto muere.
    // Solución: buscar en qué índice j vive ese objeto en
    // otro.selections[] y apuntar al mismo j en nuestro selections[].
    n_classifieds = otro.n_classifieds;
    if (otro.classifieds && n_classifieds > 0) {
        classifieds = new Seleccion*[32];
        for (int i = 0; i < n_classifieds; i++) {
            classifieds[i] = nullptr;
            for (int j = 0; j < n_selections; j++) {
                if (otro.classifieds[i] == &otro.selections[j]) {
                    classifieds[i] = &selections[j]; // apunta a la nuestra
                    break;
                }
            }
        }
    } else {
        classifieds = nullptr;
    }

    // ── 5. partidos de eliminatorias ──
    if (otro.sixteenth_final) {
        sixteenth_final = new Partido[16];
        for (int i = 0; i < 16; i++)
            sixteenth_final[i] = otro.sixteenth_final[i];
    } else {
        sixteenth_final = nullptr;
    }

    if (otro.eighth_final) {
        eighth_final = new Partido[8];
        for (int i = 0; i < 8; i++)
            eighth_final[i] = otro.eighth_final[i];
    } else {
        eighth_final = nullptr;
    }

    if (otro.fourth_final) {
        fourth_final = new Partido[4];
        for (int i = 0; i < 4; i++)
            fourth_final[i] = otro.fourth_final[i];
    } else {
        fourth_final = nullptr;
    }

    if (otro.semi_final) {
        semi_final = new Partido[2];
        for (int i = 0; i < 2; i++)
            semi_final[i] = otro.semi_final[i];
    } else {
        semi_final = nullptr;
    }

    // final y third_place son objetos directos (no punteros)
    // su operator= se encarga de la copia correctamente
    final       = otro.final;
    third_place = otro.third_place;
}

Torneo& Torneo::operator=(const Torneo& otro) {

    if (this == &otro) return *this;

    // ── 1. liberar memoria propia ──
    delete[] selections;
    delete[] groups;
    delete[] group_matches;
    delete[] classifieds;
    delete[] sixteenth_final;
    delete[] eighth_final;
    delete[] fourth_final;
    delete[] semi_final;

    // ── 2. copiar igual que en el constructor de copia ──
    n_selections = otro.n_selections;
    if (otro.selections) {
        selections = new Seleccion[n_selections];
        for (int i = 0; i < n_selections; i++)
            selections[i] = otro.selections[i];
    } else {
        selections = nullptr;
    }

    if (otro.groups) {
        groups = new Grupo[12];
        for (int i = 0; i < 12; i++)
            groups[i] = otro.groups[i];
    } else {
        groups = nullptr;
    }

    if (otro.group_matches) {
        group_matches = new Partido[72];
        for (int i = 0; i < 72; i++)
            group_matches[i] = otro.group_matches[i];
    } else {
        group_matches = nullptr;
    }

    n_classifieds = otro.n_classifieds;
    if (otro.classifieds && n_classifieds > 0) {
        classifieds = new Seleccion*[32];
        for (int i = 0; i < n_classifieds; i++) {
            classifieds[i] = nullptr;
            for (int j = 0; j < n_selections; j++) {
                if (otro.classifieds[i] == &otro.selections[j]) {
                    classifieds[i] = &selections[j];
                    break;
                }
            }
        }
    } else {
        classifieds = nullptr;
    }

    if (otro.sixteenth_final) {
        sixteenth_final = new Partido[16];
        for (int i = 0; i < 16; i++)
            sixteenth_final[i] = otro.sixteenth_final[i];
    } else { sixteenth_final = nullptr; }

    if (otro.eighth_final) {
        eighth_final = new Partido[8];
        for (int i = 0; i < 8; i++)
            eighth_final[i] = otro.eighth_final[i];
    } else { eighth_final = nullptr; }

    if (otro.fourth_final) {
        fourth_final = new Partido[4];
        for (int i = 0; i < 4; i++)
            fourth_final[i] = otro.fourth_final[i];
    } else { fourth_final = nullptr; }

    if (otro.semi_final) {
        semi_final = new Partido[2];
        for (int i = 0; i < 2; i++)
            semi_final[i] = otro.semi_final[i];
    } else { semi_final = nullptr; }

    final       = otro.final;
    third_place = otro.third_place;

    return *this;
}

Torneo::~Torneo() {
    delete[] selections;
    delete[] groups;
    delete[] group_matches;
    delete[] classifieds;      // solo el arreglo de punteros, no las selecciones
    delete[] sixteenth_final;
    delete[] eighth_final;
    delete[] fourth_final;
    delete[] semi_final;
}

void Torneo::cargar(const char* archivo){

    delete[] selections;
    selections = CrearSelecciones(archivo, n_selections);

    if (!selections || n_selections == 0)
    {
        cout << "error " << archivo << endl;
        return;
    }

    Seleccion* bombos[4][12];
    CrearBombos(selections, bombos);

    delete[] groups;
    groups = CrearGrupos(bombos);

    if (!groups) {
        cout << "Error al crear grupos." << endl;
        return;
    }

    cout << "Torneo cargado: " << n_selections << " selecciones en 12 grupos." << endl;
}

void Torneo::imprimirGrupos() const
{
    if (!groups)
    {
        cout << "Grupos no generados.\n";
        return;
    }
    for (int i = 0; i < 12; i++)
    {
        groups[i].imprimirGrupo();
        cout << "---------------------\n";
    }
}

// Devuelve la fecha como string "DD/MM/AAAA" dado un offset de días desde 20/06/2026
static void fechaGrupo(int offsetDias, char* salida) {
    int dia = 11 + offsetDias;   // inicia 11 junio 2026
    int mes = 6;
    int anio = 2026;

    // Junio tiene 30 días
    if (dia > 30) {
        dia -= 30;
        mes = 7;
    }

    // DD/MM/AAAA
    salida[0] = '0' + dia / 10;
    salida[1] = '0' + dia % 10;
    salida[2] = '/';
    salida[3] = '0' + mes / 10;
    salida[4] = '0' + mes % 10;
    salida[5] = '/';
    salida[6] = '2';
    salida[7] = '0';
    salida[8] = '2';
    salida[9] = '6';
    salida[10] = '\0';
}


void Torneo::simularFaseGrupos() {
    if (!groups) { cout << "Primero carga el torneo.\n"; return; }

    delete[] group_matches;
    group_matches = new Partido[72];

    // Parejas round-robin dentro de un grupo de 4: (0,1),(0,2),(0,3),(1,2),(1,3),(2,3)
    // Las dividimos en 3 jornadas de 2 partidos por grupo:
    //   J1: (0,1) y (2,3)
    //   J2: (0,2) y (1,3)
    //   J3: (0,3) y (1,2)
    int par[3][2][2] = {
        {{0,1},{2,3}},
        {{0,2},{1,3}},
        {{0,3},{1,2}}
    };

    // Distribución de días:
    // Jornada 1: días 0-4   (4 partidos/día × 4 días + ...)
    // Para simplificar: cada grupo ocupa 3 "slots de jornada" separados por ≥3 días.
    // Asignamos grupo g a los días: g%4*1, g%4*1+5, g%4*1+10  (dentro de 19 días).
    // El día real de la jornada j del grupo g:
    //   dia = (g % 4) + j * 5     → días: 0..3, 5..8, 10..13 → máx día 13 ✓ (≤19)

    char fecha[11];
    int idx = 0; // índice global en group_matches[]

    for (int g = 0; g < 12; g++) {
        for (int j = 0; j < 3; j++) {
            int dia = (g % 4) + j * 7;   // garantiza ≥3 días entre jornadas del mismo equipo
            fechaGrupo(dia, fecha);

            for (int p = 0; p < 2; p++) {
                int i1 = par[j][p][0];
                int i2 = par[j][p][1];

                Seleccion* loc = groups[g].getSeleccion(i1);
                Seleccion* vis = groups[g].getSeleccion(i2);

                group_matches[idx] = Partido(loc, vis, fecha, "nombreSede");
                group_matches[idx].simulator(false);
                group_matches[idx].update_statistics();

                cout << "Grupo " << (char)('A' + g)
                     << " [" << fecha << "] ";
                group_matches[idx].imp_result();
                cout << "------------------------\n";

                idx++;
            }
        }
    }

    // Ordenar cada grupo según puntos/DG/GF
    for (int g = 0; g < 12; g++)
        groups[g].ordenarGrupo();

    cout << "\n=== TABLA DE POSICIONES FASE DE GRUPOS ===\n";
    imprimirGrupos();
}

void Torneo::clasificar() {
    if (!groups)
    {
        cout << "Simula la fase de grupos primero.\n"; return;
    }

    delete[] classifieds;
    classifieds = new Seleccion*[32];
    n_classifieds = 0;

    // 12 primeros
    for (int g = 0; g < 12; g++)
    {
        classifieds[n_classifieds++] = groups[g].getSeleccion(0);
    }

    // 12 segundos
    for (int g = 0; g < 12; g++)
    {
        classifieds[n_classifieds++] = groups[g].getSeleccion(1);
    }

    // Recoger los 12 terceros y ordenar por puntos → DG → GF
    Seleccion* terceros[12];

    for (int g = 0; g < 12; g++)
    {
        terceros[g] = groups[g].getSeleccion(2);
    }

    // Burbuja sobre los 12 terceros
    for (int i = 0; i < 11; i++)
    {
        for (int j = 0; j < 11 - i; j++)
        {
            Seleccion* a = terceros[j];
            Seleccion* b = terceros[j + 1];
            bool swap = false;
            if (a->getPuntosGrupo() < b->getPuntosGrupo())
            {

                swap = true;
            }
            else if (a->getPuntosGrupo() == b->getPuntosGrupo())
            {
                if (a->getDGGrupo() < b->getDGGrupo())
                {

                    swap = true;
                }
                else if (a->getDGGrupo() == b->getDGGrupo())
                {
                    if (a->getGFGrupo() < b->getGFGrupo())
                    {
                        swap = true;
                    }
                }
            }
            if (swap)
            {
                Seleccion* tmp = terceros[j];
                terceros[j] = terceros[j+1];
                terceros[j+1] = tmp;
            }
        }
    }

    // Los 8 mejores terceros
    for (int i = 0; i < 8; i++)
    {
        classifieds[n_classifieds++] = terceros[i];
    }

    cout << "\n=== 32 CLASIFICADOS A R16 ===\n";
    cout << "--- Primeros de grupo ---\n";
    for (int i = 0; i < 12; i++)
    {
        cout << "  " << classifieds[i]->getnombre() << "\n";
    }
    cout << "--- Segundos de grupo ---\n";
    for (int i = 12; i < 24; i++)
    {
        cout << "  " << classifieds[i]->getnombre() << "\n";
    }
    cout << "--- Mejores 8 terceros ---\n";
    for (int i = 24; i < 32; i++)
    {
        cout << "  " << classifieds[i]->getnombre() << "\n";
    }
}


// Devuelve el equipo que ganó el partido
// loc y vis son los punteros que se pasaron
// al constructor de ese Partido
static Seleccion* ganador(Partido& p, Seleccion* loc, Seleccion* vis)
{
    if (p.getGol_local() >= p.getGol_visiting())
    {
        return loc;
    }
    else
    {
        return vis;
    }
}

// Devuelve el equipo que perdió el partido
static Seleccion* perdedor(Partido& p, Seleccion* loc, Seleccion* vis)
{
    if (p.getGol_local() >= p.getGol_visiting())
    {
        return vis;
    }
    else
    {
        return loc;
    }
}


static void simularRonda(Partido* partidos, Seleccion** loc, Seleccion** vis,int n,const char* nombreEtapa, Seleccion** ganadores)
{
    cout << "\n=== " << nombreEtapa << " ===\n";

    for (int i = 0; i < n; i++) {
        partidos[i].simulator(true);        // eliminatoria → prorroga si empate
        partidos[i].update_statistics();
        partidos[i].imp_result();
        cout << "------------------------\n";

        // como recibimos loc[] y vis[] sabemos exactamente
        // a quién apunta cada extremo del partido
        ganadores[i] = ganador(partidos[i], loc[i], vis[i]);
    }
}

void Torneo::simular_16() {
    if (n_classifieds < 32) {
        cout << "Clasifica los equipos primero.\n"; return;
    }

    delete[] sixteenth_final;
    sixteenth_final = new Partido[16];

    // arreglos locales para pasar a simularRonda
    Seleccion* loc[16];
    Seleccion* vis[16];

    // primero vs tercero (8 partidos)
    for (int i = 0; i < 8; i++) {
        loc[i] = classifieds[i];
        vis[i] = classifieds[24 + i];
        sixteenth_final[i] = Partido(loc[i], vis[i], "10/07/2026", "nombreSede");
    }
    // primero restante vs segundo peor (4 partidos)
    for (int i = 0; i < 4; i++) {
        loc[8 + i] = classifieds[8 + i];
        vis[8 + i] = classifieds[20 + i];
        sixteenth_final[8 + i] = Partido(loc[8+i], vis[8+i], "10/07/2026", "nombreSede");
    }
    // segundos entre sí (4 partidos)
    for (int i = 0; i < 4; i++) {
        loc[12 + i] = classifieds[12 + 2*i];
        vis[12 + i] = classifieds[12 + 2*i + 1];
        sixteenth_final[12 + i] = Partido(loc[12+i], vis[12+i], "10/07/2026", "nombreSede");
    }

    Seleccion* ganadores[16];
    simularRonda(sixteenth_final, loc, vis, 16, "DIECISEISAVOS DE FINAL (R16)", ganadores);

    // los 16 ganadores son los nuevos clasificados
    n_classifieds = 16;
    for (int i = 0; i < 16; i++)
        classifieds[i] = ganadores[i];
}

void Torneo::simular_8() {
    if (n_classifieds < 16) {
        cout << "Simula R16 primero.\n"; return;
    }

    delete[] eighth_final;
    eighth_final = new Partido[8];

    Seleccion* loc[8];
    Seleccion* vis[8];

    for (int i = 0; i < 8; i++) {
        loc[i] = classifieds[2*i];
        vis[i] = classifieds[2*i + 1];
        eighth_final[i] = Partido(loc[i], vis[i], "10/07/2026", "nombreSede");
    }

    Seleccion* ganadores[8];
    simularRonda(eighth_final, loc, vis, 8, "OCTAVOS DE FINAL (R8)", ganadores);

    n_classifieds = 8;
    for (int i = 0; i < 8; i++)
        classifieds[i] = ganadores[i];
}

void Torneo::simular_4() {
    if (n_classifieds < 8) {
        cout << "Simula R8 primero.\n"; return;
    }

    delete[] fourth_final;
    fourth_final = new Partido[4];

    Seleccion* loc[4];
    Seleccion* vis[4];

    for (int i = 0; i < 4; i++) {
        loc[i] = classifieds[2*i];
        vis[i] = classifieds[2*i + 1];
        fourth_final[i] = Partido(loc[i], vis[i], "10/07/2026", "nombreSede");
    }

    Seleccion* ganadores[4];
    simularRonda(fourth_final, loc, vis, 4, "CUARTOS DE FINAL (QF)", ganadores);

    // guardar perdedores antes de sobreescribir classifieds
    Seleccion* perdedores[4];
    for (int i = 0; i < 4; i++)
        perdedores[i] = perdedor(fourth_final[i], loc[i], vis[i]);

    // 0-3: semifinalistas   4-7: perdedores (para 3er puesto)
    n_classifieds = 8;
    for (int i = 0; i < 4; i++) classifieds[i]     = ganadores[i];
    for (int i = 0; i < 4; i++) classifieds[4 + i] = perdedores[i];
}

void Torneo::simular_semi() {
    if (n_classifieds < 4) {
        cout << "Simula QF primero.\n"; return;
    }

    delete[] semi_final;
    semi_final = new Partido[2];

    Seleccion* loc[2];
    Seleccion* vis[2];

    for (int i = 0; i < 2; i++) {
        loc[i] = classifieds[2*i];
        vis[i] = classifieds[2*i + 1];
        semi_final[i] = Partido(loc[i], vis[i], "10/07/2026", "nombreSede");
    }

    Seleccion* ganadores[2];
    simularRonda(semi_final, loc, vis, 2, "SEMIFINALES", ganadores);

    // perdedores disputan el 3er puesto
    classifieds[0] = ganadores[0];
    classifieds[1] = ganadores[1];
    classifieds[2] = perdedor(semi_final[0], loc[0], vis[0]);
    classifieds[3] = perdedor(semi_final[1], loc[1], vis[1]);
    n_classifieds  = 4;
}

void Torneo::simular_Final() {
    if (n_classifieds < 4) {
        cout << "Simula las semifinales primero.\n"; return;
    }

    cout << "\n=== PARTIDO POR EL TERCER PUESTO ===\n";
    third_place = Partido(classifieds[2], classifieds[3], "10/07/2026", "nombreSede");
    third_place.simulator(true);
    third_place.update_statistics();
    third_place.imp_result();

    cout << "\n=== GRAN FINAL ===\n";
    final = Partido(classifieds[0], classifieds[1], "10/07/2026", "nombreSede");
    final.simulator(true);
    final.update_statistics();
    final.imp_result();
}

void Torneo::imprimirEstadisticas() const {
    if (!classifieds) { cout << "Completa el torneo primero.\n"; return; }

    // ── ranking 1°-4° ──
    Seleccion* loc_f = classifieds[0];
    Seleccion* vis_f = classifieds[1];
    Seleccion* loc_3 = classifieds[2];
    Seleccion* vis_3 = classifieds[3];

    Seleccion* campeon    = (final.getGol_local() >= final.getGol_visiting()) ? loc_f : vis_f;
    Seleccion* subcampeon = (campeon == loc_f) ? vis_f : loc_f;
    Seleccion* tercero    = (third_place.getGol_local() >= third_place.getGol_visiting()) ? loc_3 : vis_3;
    Seleccion* cuarto     = (tercero == loc_3) ? vis_3 : loc_3;

    cout << "\n=== RANKING FINAL ===\n";
    cout << "1. " << campeon->getnombre()    << "\n";
    cout << "2. " << subcampeon->getnombre() << "\n";
    cout << "3. " << tercero->getnombre()    << "\n";
    cout << "4. " << cuarto->getnombre()     << "\n";

    // ── máximo goleador del campeón ──
    cout << "\n=== MAXIMO GOLEADOR DE " << campeon->getnombre() << " ===\n";
    int maxG = -1, maxD = -1;
    for (int i = 0; i < 26; i++) {
        Jugador& j = campeon->getJugador(i);
        if (j.getGoles() > maxG) {
            maxG = j.getGoles();
            maxD = j.getDorsal();
        }
    }
    cout << "Dorsal #" << maxD << " con " << maxG << " goles\n";

    // ── top 3 goleadores del torneo ──
    cout << "\n=== TOP 3 GOLEADORES DEL TORNEO ===\n";
    int tG[3] = {-1, -1, -1};
    int tS[3] = {-1, -1, -1};
    int tJ[3] = {-1, -1, -1};

    for (int s = 0; s < n_selections; s++) {
        for (int j = 0; j < 26; j++) {
            int g = selections[s].getJugador(j).getGoles();
            if (g > tG[2]) {
                tG[2] = g; tS[2] = s; tJ[2] = j;
                // ordenar los 3 de mayor a menor (burbuja de 3)
                for (int k = 2; k > 0 && tG[k] > tG[k-1]; k--) {
                    int tmp;
                    tmp = tG[k]; tG[k] = tG[k-1]; tG[k-1] = tmp;
                    tmp = tS[k]; tS[k] = tS[k-1]; tS[k-1] = tmp;
                    tmp = tJ[k]; tJ[k] = tJ[k-1]; tJ[k-1] = tmp;
                }
            }
        }
    }

    for (int k = 0; k < 3; k++) {
        if (tS[k] >= 0) {
            Jugador& j = selections[tS[k]].getJugador(tJ[k]);
            cout << (k+1) << ". " << selections[tS[k]].getnombre()
                 << " - #" << j.getDorsal()
                 << " (" << tG[k] << " goles)\n";
        }
    }

    // ── equipo con más goles históricos ──
    cout << "\n=== EQUIPO CON MAS GOLES HISTORICOS ===\n";
    Seleccion* masGoles = &selections[0];
    for (int i = 1; i < n_selections; i++)
        if (selections[i].getGolesFavor() > masGoles->getGolesFavor())
            masGoles = &selections[i];
    cout << masGoles->getnombre() << " con " << masGoles->getGolesFavor() << " goles\n";
}
