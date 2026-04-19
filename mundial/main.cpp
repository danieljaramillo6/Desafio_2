#include <iostream>
#include "Grupo.h"
using namespace std;

int main() {

    Grupo grupoA('A');

    Seleccion s1("Colombia", "DT1", "CONMEBOL", "FCF", 15, 0, 0, 0, 0, 0);
    Seleccion s2("Brasil", "DT2", "CONMEBOL", "CBF", 3, 0, 0, 0, 0, 0);
    Seleccion s3("Alemania", "DT3", "UEFA", "DFB", 10, 0, 0, 0, 0, 0);
    Seleccion s4("Japon", "DT4", "AFC", "JFA", 20, 0, 0, 0, 0, 0);

    grupoA.agregarSeleccion(s1);
    grupoA.agregarSeleccion(s2);
    grupoA.agregarSeleccion(s3);
    grupoA.agregarSeleccion(s4);

    // 🔥 Usando ++ y --
    for (int i = 0; i < 5; i++) ++grupoA.getSeleccion(0);
    for (int i = 0; i < 2; i++) --grupoA.getSeleccion(0);

    for (int i = 0; i < 6; i++) ++grupoA.getSeleccion(1);
    for (int i = 0; i < 3; i++) --grupoA.getSeleccion(1);

    for (int i = 0; i < 2; i++) ++grupoA.getSeleccion(2);
    for (int i = 0; i < 4; i++) --grupoA.getSeleccion(2);

    for (int i = 0; i < 1; i++) ++grupoA.getSeleccion(3);
    for (int i = 0; i < 5; i++) --grupoA.getSeleccion(3);

    // Resultados
    grupoA.getSeleccion(0).registrarVictoria();
    grupoA.getSeleccion(0).registrarEmpate();

    grupoA.getSeleccion(1).registrarVictoria();
    grupoA.getSeleccion(1).registrarVictoria();

    grupoA.getSeleccion(2).registrarEmpate();
    grupoA.getSeleccion(2).registrarDerrota();

    grupoA.getSeleccion(3).registrarDerrota();
    grupoA.getSeleccion(3).registrarDerrota();

    cout << "\n--- Grupo sin ordenar ---\n";
    grupoA.imprimirGrupo();

    grupoA.ordenarGrupo();

    cout << "\n--- Grupo ordenado ---\n";
    grupoA.imprimirGrupo();

    return 0;
}
