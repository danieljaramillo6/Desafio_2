#include <fstream>
#include <sstream>
#include <iostream>
#include "Seleccion.h"
#include <cstdlib>
#include <ctime>
#include "grupo.h"
#include <cstring>

using namespace std;


Seleccion* CrearSelecciones(const char* archivo, int& cantidad) {

    std::ifstream file(archivo);

    if (!file.is_open()) {
        std::cout <<"Error";
        cantidad=0;
        return nullptr;
    }

    Seleccion* selecciones = new Seleccion[48];
    cantidad = 0;

    std::string linea;

    getline(file, linea);
    getline(file, linea);

    while (getline(file, linea)) {

        std::stringstream ss(linea);

        std::string campo;

        // Variables temporales
        int ranking, gf, gc, pg, pe, pp;
        std::string pais, dt, federacion, confederacion;

        // Leer campos separados por coma
        getline(ss, campo, ';');
        ranking = stoi(campo);
        getline(ss, pais, ';');
        getline(ss, dt, ';');
        getline(ss, federacion, ';');
        getline(ss, confederacion, ';');
        getline(ss, campo, ';');
        gf = stoi(campo);
        getline(ss, campo, ';');
        gc = stoi(campo);
        getline(ss, campo, ';');
        pg = stoi(campo);
        getline(ss, campo, ';');
        pe = stoi(campo);
        getline(ss, campo, ';');
        pp = stoi(campo);

        // Crear Seleccion (con char*)
        selecciones[cantidad] = Seleccion( pais.c_str(), dt.c_str(),confederacion.c_str(),federacion.c_str(),ranking,gf,gc,pp,pg, pe);

        cantidad++;
    }

    file.close();
    return selecciones;
}
void CrearBombos(Seleccion* selecciones, Seleccion* Bombos[4][12]) {

    // Bombo 0: USA + sel[0..10] = 12 equipos
    Bombos[0][0] = &selecciones[15];
    int c = 1;
    for (int i = 0; i < 12; i++) {
        if (i == 15) continue;
        if (c < 12) {
            Bombos[0][c++] = &selecciones[i];
        }
    }

    // Bombos 1-3: empieza en 11 para incluir sel[11] que el loop anterior dejó fuera
    c = 0;
    int b = 1;
    for (int i = 11; i < 48; i++) {  // ✅ antes era i = 12
        if (i == 15) continue;
        Bombos[b][c++] = &selecciones[i];
        if (c == 12) { b++; c = 0; }
    }
    // sel[11..47] - sel[15] = 36 equipos exactos ✓
}

bool esValido(Grupo& g, Seleccion* s) {

    int uefa = 0;

    for (int i = 0; i < g.getCantidad(); i++) {

        Seleccion* actual = g.getSeleccion(i);

        if (strcmp(actual->getConfederacion(), s->getConfederacion()) == 0) {

            if (strcmp(s->getConfederacion(), "UEFA") == 0) {
                uefa++;
                if (uefa >= 2) return false;
            } else {
                return false;
            }
        }
    }

    return true;
}

Grupo* CrearGrupos(Seleccion* Bombos[4][12]) {

    while (true) {

        srand(time(0));

        Grupo* grupos = new Grupo[12];
        bool usado[4][12] = {false};
        bool fallo = false;

        for (int i = 0; i < 12; i++) {
            grupos[i] = Grupo('A' + i);
        }

        for (int g = 0; g < 12; g++) {
            for (int b = 0; b < 4; b++) {

                int intentos = 0;
                bool asignado = false;

                while (intentos < 1000) {  // más intentos por bombo
                    int idx = rand() % 12;

                    if (usado[b][idx]) {
                        intentos++;
                        continue;
                    }

                    Seleccion* s = Bombos[b][idx];

                    if (esValido(grupos[g], s)) {
                        grupos[g].agregarSeleccion(s);
                        usado[b][idx] = true;
                        asignado = true;
                        break;
                    }

                    intentos++;
                }

                if (!asignado) {
                    fallo = true;
                    break;
                }
            }

            if (fallo) break;
        }

        if (!fallo) {
            return grupos;  // grupos válidos, salir
        }

        // si falló, liberar y reintentar
        delete[] grupos;
    }
}
