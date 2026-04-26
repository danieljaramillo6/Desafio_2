#include "torneo.h"
#include <fstream>
#include <iostream>
#include <cstring>
using namespace std;

// ── helpers internos ─────────────────────────────────────────────────────────

static void diaAFecha(int dia, char* out) {
    int d = 11 + dia;
    int m = 6;
    if (d > 30) { d -= 30; m = 7; }
    out[0] = '0' + (d / 10); out[1] = '0' + (d % 10);
    out[2] = '/';
    out[3] = '0';             out[4] = '0' + m;
    out[5] = '/';
    out[6]='2'; out[7]='0'; out[8]='2'; out[9]='6';
    out[10] = '\0';
}

int Torneo::buscarIndice(Seleccion* s) const {
    for (int i = 0; i < n_selections; i++) {
        iter++;
        if (selections[i].getRanking() == s->getRanking()) return i;
    }
    return -1;
}

void Torneo::asignarFechasJornada(Partido* partidos, int n,
                                  int dia_inicio, int dia_fin) {
    int partidos_por_dia[17] = {0};
    for (int p = 0; p < n; p++) {
        iter++;
        Seleccion* l = partidos[p].getLocal();
        Seleccion* v = partidos[p].getVisiting();
        int idx_l = buscarIndice(l);
        int idx_v = buscarIndice(v);
        bool asignado = false;
        for (int dia = dia_inicio; dia <= dia_fin && !asignado; dia++) {
            iter++;
            int slot = dia - dia_inicio;
            if (partidos_por_dia[slot] >= 4) continue;
            bool local_ok = (ultimo_dia[idx_l] == -1 ||
                             dia - ultimo_dia[idx_l] >= 3);
            bool visit_ok = (ultimo_dia[idx_v] == -1 ||
                             dia - ultimo_dia[idx_v] >= 3);
            if (local_ok && visit_ok) {
                char fecha_str[11];
                diaAFecha(dia, fecha_str);
                partidos[p] = Partido(l, v, fecha_str, "TBD");
                partidos_por_dia[slot]++;
                ultimo_dia[idx_l] = dia;
                ultimo_dia[idx_v] = dia;
                asignado = true;
            }
        }
        if (!asignado)
            cout << "Advertencia: no se pudo asignar fecha a "
                 << l->getnombre() << " vs " << v->getnombre() << "\n";
    }
}

// ── cálculo de memoria ────────────────────────────────────────────────────────

size_t Torneo::calcularMemoriaTotal() const {

    size_t mem = sizeof(Torneo); // objeto Torneo base (punteros, arrays fijos, iter)

    // selections[]
    if (selections) {
        mem += n_selections * sizeof(Seleccion);
        for (int i = 0; i < n_selections; i++) {
            iter++;
            // solo la parte dinámica (sizeof ya contada arriba)
            const Seleccion& s = selections[i];
            if (s.getnombre())        mem += strlen(s.getnombre())        + 1;
            if (s.getDt())            mem += strlen(s.getDt())            + 1;
            if (s.getConfederacion()) mem += strlen(s.getConfederacion()) + 1;
            if (s.getFederacion())    mem += strlen(s.getFederacion())    + 1;
            // jugadores dinámicos
            for (int j = 0; j < 26; j++) {
                iter++;
                const Jugador& jug = const_cast<Seleccion&>(s).getJugador(j);
                mem += sizeof(Jugador);
                if (jug.getNombre())   mem += strlen(jug.getNombre())   + 1;
                if (jug.getApellido()) mem += strlen(jug.getApellido()) + 1;
            }
        }
    }

    // groups[]
    if (groups) {
        mem += 12 * sizeof(Grupo);
        for (int i = 0; i < 12; i++) {
            iter++;
            mem += 4 * sizeof(Seleccion*); // arreglo interno de punteros
        }
    }

    // group_matches[]
    if (group_matches) {
        mem += 72 * sizeof(Partido);
        for (int i = 0; i < 72; i++) {
            iter++;
            mem += group_matches[i].calcularMemoria() - sizeof(Partido);
        }
    }

    // classifieds[]
    if (classifieds)
        mem += 32 * sizeof(Seleccion*);

    // rondas eliminatorias
    auto contarPartidos = [&](Partido* arr, int n) {
        if (!arr) return;
        mem += n * sizeof(Partido);
        for (int i = 0; i < n; i++) {
            iter++;
            mem += arr[i].calcularMemoria() - sizeof(Partido);
        }
    };
    contarPartidos(sixteenth_final, 16);
    contarPartidos(eighth_final,     8);
    contarPartidos(fourth_final,     4);
    contarPartidos(semi_final,       2);

    // final y third_place (por valor en Torneo, sizeof ya los incluye)
    mem += final.calcularMemoria()       - sizeof(Partido);
    mem += third_place.calcularMemoria() - sizeof(Partido);

    return mem;
}

// ── reporte de métricas ───────────────────────────────────────────────────────

void Torneo::reportar(const char* func) const {
    size_t mem = calcularMemoriaTotal();
    cout << "\n--------------------------------------------------\n";
    cout << "  METRICAS: " << func << "\n";
    cout << "  Iteraciones: " << iter << "\n";
    cout << "  Memoria total: " << mem << " bytes ("
         << mem / 1024 << " KB)\n";
    cout << "--------------------------------------------------\n\n";
}

// ── Constructores / Destructor ────────────────────────────────────────────────

Torneo::Torneo() {
    selections = nullptr; n_selections = 0;
    groups = nullptr; group_matches = nullptr;
    classifieds = nullptr; n_classifieds = 0;
    sixteenth_final = nullptr; eighth_final = nullptr;
    fourth_final = nullptr; semi_final = nullptr;
    iter = 0;
    for (int i = 0; i < 48; i++) ultimo_dia[i] = -1;
    for (int i = 0; i < 16; i++) r16_teams[i]  = nullptr;
    for (int i = 0; i < 8;  i++) r8_teams[i]   = nullptr;
    for (int i = 0; i < 4;  i++) r4_teams[i]   = nullptr;
}

Torneo::Torneo(const char* archivo) {
    selections = nullptr; n_selections = 0;
    groups = nullptr; group_matches = nullptr;
    classifieds = nullptr; n_classifieds = 0;
    sixteenth_final = nullptr; eighth_final = nullptr;
    fourth_final = nullptr; semi_final = nullptr;
    iter = 0;
    for (int i = 0; i < 48; i++) ultimo_dia[i] = -1;
    for (int i = 0; i < 16; i++) r16_teams[i]  = nullptr;
    for (int i = 0; i < 8;  i++) r8_teams[i]   = nullptr;
    for (int i = 0; i < 4;  i++) r4_teams[i]   = nullptr;
    cargar(archivo);
}

Torneo::~Torneo() {
    delete[] selections;
    delete[] groups;
    delete[] group_matches;
    delete[] classifieds;
    delete[] sixteenth_final;
    delete[] eighth_final;
    delete[] fourth_final;
    delete[] semi_final;
}

// ── cargar() ─────────────────────────────────────────────────────────────────

void Torneo::cargar(const char* archivo) {
    iter = 0;

    delete[] selections;    delete[] groups;
    delete[] group_matches; delete[] classifieds;
    delete[] sixteenth_final; delete[] eighth_final;
    delete[] fourth_final;  delete[] semi_final;
    selections = nullptr; groups = nullptr; group_matches = nullptr;
    classifieds = nullptr; sixteenth_final = nullptr;
    eighth_final = nullptr; fourth_final = nullptr; semi_final = nullptr;
    n_selections = 0; n_classifieds = 0;
    for (int i = 0; i < 48; i++) ultimo_dia[i] = -1;

    // componente externo: CrearSelecciones
    cout << "  [Componente externo] CrearSelecciones invocado\n";
    selections = CrearSelecciones(archivo, n_selections);
    if (!selections || n_selections == 0) {
        cout << "Error: no se pudieron cargar las selecciones\n"; return;
    }
    iter += n_selections; // cuenta las líneas leídas del CSV

    // componente externo: CrearBombos
    cout << "  [Componente externo] CrearBombos invocado\n";
    Seleccion* Bombos[4][12];
    CrearBombos(selections, Bombos);
    iter += 48; // un recorrido por las 48 selecciones

    // componente externo: CrearGrupos (while interno con reintentos)
    cout << "  [Componente externo] CrearGrupos invocado\n";
    groups = CrearGrupos(Bombos);
    iter += 12 * 4 * 1000; // worst case: 12 grupos x 4 bombos x 1000 intentos max
    if (!groups) { cout << "Error: grupos\n"; return; }

    group_matches   = new Partido[72];
    classifieds     = new Seleccion*[32];
    for (int i = 0; i < 32; i++) { iter++; classifieds[i] = nullptr; }
    sixteenth_final = new Partido[16];
    eighth_final    = new Partido[8];
    fourth_final    = new Partido[4];
    semi_final      = new Partido[2];

    cout << "Torneo cargado: " << n_selections << " selecciones en 12 grupos.\n";
    reportar("cargar()");
}

// ── simularFaseGrupos() ───────────────────────────────────────────────────────

void Torneo::simularFaseGrupos() {
    iter = 0;
    if (!group_matches || !groups) {
        cout << "Error: Torneo no inicializado.\n"; return;
    }

    int local_f[3][2]    = { {0,2}, {0,1}, {0,3} };
    int visiting_f[3][2] = { {1,3}, {2,3}, {1,2} };
    int inicio[3] = {0,  5, 11};
    int fin[3]    = {4, 10, 16};

    for (int fecha = 0; fecha < 3; fecha++) {
        iter++;
        cout << "\n===== FECHA " << fecha + 1 << " =====\n\n";
        int base = fecha * 24;

        // crear partidos
        for (int g = 0; g < 12; g++) {
            iter++;
            for (int p = 0; p < 2; p++) {
                iter++;
                int idx = base + g * 2 + p;
                Seleccion* l = groups[g].getSeleccion(local_f[fecha][p]);
                Seleccion* v = groups[g].getSeleccion(visiting_f[fecha][p]);
                group_matches[idx] = Partido(l, v, "TBD", "TBD");
            }
        }

        // asignar fechas (iter se acumula dentro de asignarFechasJornada)
        asignarFechasJornada(&group_matches[base], 24, inicio[fecha], fin[fecha]);

        // simular
        for (int i = 0; i < 24; i++) {
            iter++;
            group_matches[base + i].simulator();
            group_matches[base + i].update_statistics();
            group_matches[base + i].imp_result();
            cout << "---\n";
        }

        // ordenar e imprimir tabla
        cout << "\n===== TABLA TRAS FECHA " << fecha + 1 << " =====\n\n";
        for (int g = 0; g < 12; g++) {
            iter++;
            groups[g].ordenarGrupo(iter);
            groups[g].imprimirGrupo(iter);
            cout << "\n";
        }

        if (fecha < 2) {
            cout << "Presiona ENTER para simular la fecha " << fecha + 2 << "...\n";
            cin.ignore(); cin.get();
        }
    }

    cout << "\n===== FASE DE GRUPOS COMPLETADA =====\n";
    reportar("simularFaseGrupos()");
}

// ── imprimirGrupos() ──────────────────────────────────────────────────────────

void Torneo::imprimirGrupos() const {
    iter = 0;
    if (!groups) { cout << "No hay grupos.\n"; return; }
    for (int g = 0; g < 12; g++) {
        iter++;
        groups[g].imprimirGrupo(iter);
        cout << "\n";
    }
    reportar("imprimirGrupos()");
}

// ── clasificar() ──────────────────────────────────────────────────────────────

void Torneo::clasificar() {
    iter = 0;
    if (!groups) { cout << "Simula la fase de grupos primero.\n"; return; }

    delete[] classifieds;
    classifieds   = new Seleccion*[32];
    n_classifieds = 0;

    for (int g = 0; g < 12; g++) { iter++; classifieds[n_classifieds++] = groups[g].getSeleccion(0); }
    for (int g = 0; g < 12; g++) { iter++; classifieds[n_classifieds++] = groups[g].getSeleccion(1); }

    Seleccion* terceros[12];
    for (int g = 0; g < 12; g++) { iter++; terceros[g] = groups[g].getSeleccion(2); }

    // ordenar terceros
    for (int i = 0; i < 11; i++) {
        for (int j = 0; j < 11 - i; j++) {
            iter++;
            Seleccion* a = terceros[j];
            Seleccion* b = terceros[j + 1];
            bool cambiar = false;
            if (a->getPuntosGrupo() < b->getPuntosGrupo()) cambiar = true;
            else if (a->getPuntosGrupo() == b->getPuntosGrupo()) {
                if (a->getDGGrupo() < b->getDGGrupo()) cambiar = true;
                else if (a->getDGGrupo() == b->getDGGrupo())
                    if (a->getGFGrupo() < b->getGFGrupo()) cambiar = true;
            }
            if (cambiar) { Seleccion* tmp = terceros[j]; terceros[j] = terceros[j+1]; terceros[j+1] = tmp; }
        }
    }

    for (int i = 0; i < 8; i++) { iter++; classifieds[n_classifieds++] = terceros[i]; }

    cout << "Clasificados: " << n_classifieds << " equipos.\n";
    reportar("clasificar()");
}

// ── simular_16() ──────────────────────────────────────────────────────────────

void Torneo::simular_16() {
    iter = 0;
    if (n_classifieds < 32) { cout << "Clasifica primero.\n"; return; }

    delete[] sixteenth_final;
    sixteenth_final = new Partido[16];

    for (int i = 0; i < 8;  i++) { iter++; sixteenth_final[i]    = Partido(classifieds[i], classifieds[24+i], "TBD","TBD"); }
    for (int i = 0; i < 4;  i++) { iter++; sixteenth_final[8+i]  = Partido(classifieds[12+2*i], classifieds[12+2*i+1], "TBD","TBD"); }
    for (int i = 0; i < 4;  i++) { iter++; sixteenth_final[12+i] = Partido(classifieds[8+i], classifieds[20+i], "TBD","TBD"); }

    asignarFechasJornada(sixteenth_final, 16, 17, 22);

    cout << "\n===== DIECISEISAVOS DE FINAL =====\n\n";
    for (int i = 0; i < 16; i++) {
        iter++;
        sixteenth_final[i].simulator(true);
        sixteenth_final[i].update_statistics();
        sixteenth_final[i].imp_result();
        cout << "---\n";
    }

    for (int i = 0; i < 16; i++) { iter++; r16_teams[i] = classifieds[i]; }

    delete[] eighth_final;
    eighth_final = new Partido[8];
    for (int i = 0; i < 8; i++) {
        iter++;
        int gl = sixteenth_final[i*2].getGol_local();
        int gv = sixteenth_final[i*2].getGol_visiting();
        Seleccion* g1 = (gl>gv) ? sixteenth_final[i*2].getLocal() : sixteenth_final[i*2].getVisiting();
        gl = sixteenth_final[i*2+1].getGol_local();
        gv = sixteenth_final[i*2+1].getGol_visiting();
        Seleccion* g2 = (gl>gv) ? sixteenth_final[i*2+1].getLocal() : sixteenth_final[i*2+1].getVisiting();
        eighth_final[i] = Partido(g1, g2, "TBD","TBD");
    }

    n_classifieds = 16;
    for (int i = 0; i < 16; i++) {
        iter++;
        int gl = sixteenth_final[i].getGol_local();
        int gv = sixteenth_final[i].getGol_visiting();
        classifieds[i] = (gl>gv) ? sixteenth_final[i].getLocal() : sixteenth_final[i].getVisiting();
    }
    reportar("simular_16()");
}

// ── simular_8() ───────────────────────────────────────────────────────────────

void Torneo::simular_8() {
    iter = 0;
    if (n_classifieds < 16) { cout << "Simula los dieciseisavos primero.\n"; return; }

    asignarFechasJornada(eighth_final, 8, 23, 27);

    cout << "\n===== CUARTOS DE FINAL =====\n\n";
    for (int i = 0; i < 8; i++) {
        iter++;
        eighth_final[i].simulator(true);
        eighth_final[i].update_statistics();
        eighth_final[i].imp_result();
        cout << "---\n";
    }

    for (int i = 0; i < 8; i++) { iter++; r8_teams[i] = classifieds[i]; }

    delete[] fourth_final;
    fourth_final = new Partido[4];
    for (int i = 0; i < 4; i++) {
        iter++;
        int gl = eighth_final[i*2].getGol_local();
        int gv = eighth_final[i*2].getGol_visiting();
        Seleccion* g1 = (gl>gv) ? eighth_final[i*2].getLocal() : eighth_final[i*2].getVisiting();
        gl = eighth_final[i*2+1].getGol_local();
        gv = eighth_final[i*2+1].getGol_visiting();
        Seleccion* g2 = (gl>gv) ? eighth_final[i*2+1].getLocal() : eighth_final[i*2+1].getVisiting();
        fourth_final[i] = Partido(g1, g2, "TBD","TBD");
    }

    n_classifieds = 8;
    for (int i = 0; i < 8; i++) {
        iter++;
        int gl = eighth_final[i].getGol_local();
        int gv = eighth_final[i].getGol_visiting();
        classifieds[i] = (gl>gv) ? eighth_final[i].getLocal() : eighth_final[i].getVisiting();
    }
    reportar("simular_8()");
}

// ── simular_4() ───────────────────────────────────────────────────────────────

void Torneo::simular_4() {
    iter = 0;
    if (n_classifieds < 8) { cout << "Simula los cuartos primero.\n"; return; }

    asignarFechasJornada(fourth_final, 4, 28, 31);

    cout << "\n===== SEMIFINALES =====\n\n";
    for (int i = 0; i < 4; i++) {
        iter++;
        fourth_final[i].simulator(true);
        fourth_final[i].update_statistics();
        fourth_final[i].imp_result();
        cout << "---\n";
    }

    for (int i = 0; i < 4; i++) { iter++; r4_teams[i] = classifieds[i]; }

    delete[] semi_final;
    semi_final = new Partido[2];
    for (int i = 0; i < 2; i++) {
        iter++;
        int gl = fourth_final[i*2].getGol_local();
        int gv = fourth_final[i*2].getGol_visiting();
        Seleccion* g1 = (gl>gv) ? fourth_final[i*2].getLocal() : fourth_final[i*2].getVisiting();
        gl = fourth_final[i*2+1].getGol_local();
        gv = fourth_final[i*2+1].getGol_visiting();
        Seleccion* g2 = (gl>gv) ? fourth_final[i*2+1].getLocal() : fourth_final[i*2+1].getVisiting();
        semi_final[i] = Partido(g1, g2, "TBD","TBD");
    }

    n_classifieds = 4;
    for (int i = 0; i < 4; i++) {
        iter++;
        int gl = fourth_final[i].getGol_local();
        int gv = fourth_final[i].getGol_visiting();
        classifieds[i] = (gl>gv) ? fourth_final[i].getLocal() : fourth_final[i].getVisiting();
    }
    reportar("simular_4()");
}

// ── simular_semi() ────────────────────────────────────────────────────────────

void Torneo::simular_semi() {
    iter = 0;
    if (n_classifieds < 4) { cout << "Simula los cuartos primero.\n"; return; }

    asignarFechasJornada(semi_final, 2, 33, 34);

    cout << "\n===== SEMIFINALES =====\n\n";
    for (int i = 0; i < 2; i++) {
        iter++;
        semi_final[i].simulator(true);
        semi_final[i].update_statistics();
        semi_final[i].imp_result();
        cout << "---\n";
    }

    int gl0 = semi_final[0].getGol_local(), gv0 = semi_final[0].getGol_visiting();
    Seleccion* gan0 = (gl0>gv0) ? semi_final[0].getLocal()    : semi_final[0].getVisiting();
    Seleccion* per0 = (gl0>gv0) ? semi_final[0].getVisiting() : semi_final[0].getLocal();
    int gl1 = semi_final[1].getGol_local(), gv1 = semi_final[1].getGol_visiting();
    Seleccion* gan1 = (gl1>gv1) ? semi_final[1].getLocal()    : semi_final[1].getVisiting();
    Seleccion* per1 = (gl1>gv1) ? semi_final[1].getVisiting() : semi_final[1].getLocal();

    third_place = Partido(per0, per1, "18/07/2026", "TBD");
    final       = Partido(gan0, gan1, "19/07/2026", "TBD");

    n_classifieds = 2;
    classifieds[0] = gan0;
    classifieds[1] = gan1;
    reportar("simular_semi()");
}

// ── simular_Final() ───────────────────────────────────────────────────────────

void Torneo::simular_Final() {
    iter = 0;
    if (final.getLocal() == nullptr) {
        cout << "Simula las semifinales primero.\n"; return;
    }

    cout << "\n===== TERCER Y CUARTO PUESTO =====\n\n";
    iter++;
    third_place.simulator(true);
    third_place.update_statistics();
    third_place.imp_result();

    cout << "\n===== GRAN FINAL =====\n\n";
    iter++;
    final.simulator(true);
    final.update_statistics();
    final.imp_result();

    int gl = final.getGol_local(), gv = final.getGol_visiting();
    Seleccion* campeon = (gl>gv) ? final.getLocal() : final.getVisiting();
    cout << "\nCAMPEON DEL MUNDO: " << campeon->getnombre() << "\n";
    reportar("simular_Final()");
}

// ── imprimirEstadisticas() ────────────────────────────────────────────────────

void Torneo::imprimirEstadisticas() const {
    iter = 0;
    if (final.getLocal() == nullptr) {
        cout << "Completa el torneo primero.\n"; return;
    }

    // top 4
    int gl_f = final.getGol_local(), gv_f = final.getGol_visiting();
    Seleccion* campeon    = (gl_f>gv_f) ? final.getLocal()    : final.getVisiting();
    Seleccion* subcampeon = (gl_f>gv_f) ? final.getVisiting() : final.getLocal();
    int gl_t = third_place.getGol_local(), gv_t = third_place.getGol_visiting();
    Seleccion* tercero = (gl_t>gv_t) ? third_place.getLocal()    : third_place.getVisiting();
    Seleccion* cuarto  = (gl_t>gv_t) ? third_place.getVisiting() : third_place.getLocal();

    cout << "\n=== RANKING FINAL ===\n";
    cout << "1. " << campeon->getnombre()    << "\n";
    cout << "2. " << subcampeon->getnombre() << "\n";
    cout << "3. " << tercero->getnombre()    << "\n";
    cout << "4. " << cuarto->getnombre()     << "\n";

    // goleador del campeón
    cout << "\n=== MAXIMO GOLEADOR DE " << campeon->getnombre() << " ===\n";
    int maxG = -1, maxIdx = -1;
    for (int i = 0; i < 26; i++) {
        iter++;
        Jugador& j = campeon->getJugador(i);
        if (j.getGoles() > maxG) { maxG = j.getGoles(); maxIdx = i; }
    }
    if (maxIdx >= 0) {
        Jugador& g = campeon->getJugador(maxIdx);
        cout << "#" << g.getDorsal() << " " << g.getNombre()
             << " " << g.getApellido() << " - " << maxG << " goles\n";
    }

    // top 3 goleadores
    cout << "\n=== TOP 3 GOLEADORES DEL TORNEO ===\n";
    int tG[3]={-1,-1,-1}, tS[3]={-1,-1,-1}, tJ[3]={-1,-1,-1};
    for (int s = 0; s < n_selections; s++) {
        iter++;
        for (int j = 0; j < 26; j++) {
            iter++;
            int g = selections[s].getJugador(j).getGoles();
            if (g > tG[2]) {
                tG[2]=g; tS[2]=s; tJ[2]=j;
                for (int k=2; k>0 && tG[k]>tG[k-1]; k--) {
                    iter++;
                    int tmp;
                    tmp=tG[k]; tG[k]=tG[k-1]; tG[k-1]=tmp;
                    tmp=tS[k]; tS[k]=tS[k-1]; tS[k-1]=tmp;
                    tmp=tJ[k]; tJ[k]=tJ[k-1]; tJ[k-1]=tmp;
                }
            }
        }
    }
    for (int k = 0; k < 3; k++) {
        iter++;
        if (tS[k] >= 0) {
            Jugador& j = selections[tS[k]].getJugador(tJ[k]);
            cout << (k+1) << ". " << selections[tS[k]].getnombre()
                 << " - #" << j.getDorsal()
                 << " " << j.getNombre() << " " << j.getApellido()
                 << " (" << tG[k] << " goles)\n";
        }
    }

    // equipo con más goles históricos
    cout << "\n=== EQUIPO CON MAS GOLES HISTORICOS ===\n";
    Seleccion* masGoles = &selections[0];
    for (int i = 1; i < n_selections; i++) {
        iter++;
        if (selections[i].getGolesFavor() > masGoles->getGolesFavor())
            masGoles = &selections[i];
    }
    cout << masGoles->getnombre() << " - " << masGoles->getGolesFavor() << " goles\n";

    // confederación dominante por etapa
    cout << "\n=== CONFEDERACION DOMINANTE POR ETAPA ===\n";
    const char* confs[]  = {"UEFA","CONMEBOL","Concacaf","CAF","AFC","OFC"};
    const char* etapas[] = {"R16","R8","R4"};
    Seleccion** rondas[3] = {
        const_cast<Seleccion**>(r16_teams),
        const_cast<Seleccion**>(r8_teams),
        const_cast<Seleccion**>(r4_teams)
    };
    int tamanios[3] = {16, 8, 4};
    for (int r = 0; r < 3; r++) {
        iter++;
        int conteos[6] = {0};
        for (int i = 0; i < tamanios[r]; i++) {
            iter++;
            if (!rondas[r][i]) continue;
            for (int c = 0; c < 6; c++) {
                iter++;
                if (strcmp(rondas[r][i]->getConfederacion(), confs[c]) == 0) {
                    conteos[c]++; break;
                }
            }
        }
        int maxC=0, maxIdx=0;
        for (int c = 1; c < 6; c++) {
            iter++;
            if (conteos[c] > maxC) { maxC=conteos[c]; maxIdx=c; }
        }
        cout << etapas[r] << ": " << confs[maxIdx]
             << " (" << maxC << " equipos)\n";
    }

    reportar("imprimirEstadisticas()");
}

void Torneo::guardarCSV(const char* archivo) const {

    if (!selections || n_selections == 0) {
        cout << "No hay datos para guardar.\n";
        return;
    }

    ofstream file(archivo);

    if (!file.is_open()) {
        cout << "Error: no se pudo abrir el archivo " << archivo << "\n";
        return;
    }

    // encabezados — misma estructura que el CSV original
    file << "ranking;pais;dt;federacion;confederacion;gf;gc;pg;pe;pp\n";
    file << "---\n"; // segunda línea que salta manager.cpp

    for (int i = 0; i < n_selections; i++) {
        const Seleccion& s = selections[i];

        file << s.getRanking()          << ";"
             << s.getnombre()           << ";"
             << s.getDt()               << ";"
             << s.getFederacion()       << ";"
             << s.getConfederacion()    << ";"
             << s.getGolesFavor()       << ";"
             << s.getGolesContra()      << ";"
             << s.getPartidosGanados()  << ";"
             << s.getPartidosEmpatados()<< ";"
             << s.getPartidosPerdidos() << "\n";
    }

    file.close();
    cout << "Datos guardados en " << archivo << "\n";
}
