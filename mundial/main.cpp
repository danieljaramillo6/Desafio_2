#include <iostream>
#include "Grupo.h"
#include "seleccion.h"
#include "manager.h"

using namespace std;

bool validarGrupos(Grupo* grupos) {
    for (int g = 0; g < 12; g++) {
        int conteoUEFA = 0;
        for (int i = 0; i < 4; i++) {
            Seleccion* a = grupos[g].getSeleccion(i);
            for (int j = i + 1; j < 4; j++) {
                Seleccion* b = grupos[g].getSeleccion(j);
                if (strcmp(a->getConfederacion(), b->getConfederacion()) == 0) {
                    if (strcmp(a->getConfederacion(), "UEFA") == 0) {
                        conteoUEFA++;
                        if (conteoUEFA > 1) {
                            cout << "Error: mas de 2 UEFA en grupo "
                                 << grupos[g].getLetra() << endl;
                            return false;
                        }
                    } else {
                        cout << "Error: confederacion repetida en grupo "
                             << grupos[g].getLetra() << endl;
                        return false;
                    }
                }
            }
        }
    }
    return true;
}

int main() {
    ios::sync_with_stdio(true);
    cout.setf(ios::unitbuf); // flush automático en cada cout

    int n = 0;

    cout << "[1] Iniciando carga de selecciones...\n";
    Seleccion* selecciones = CrearSelecciones("selecciones_clasificadas_mundial.csv", n);
    cout << "[2] Selecciones cargadas: " << n << "\n";

    if (!selecciones || n == 0) {
        cout << "Error al cargar selecciones\n";
        return 1;
    }

    // Imprimir las primeras 3 selecciones para verificar que leyó bien el CSV
    cout << "[3] Verificando primeras selecciones:\n";
    for (int i = 0; i < 3 && i < n; i++) {
        cout << "  [" << i << "] " << selecciones[i].getnombre()
        << " | " << selecciones[i].getConfederacion()
        << " | ranking: " << selecciones[i].getRanking() << "\n";
    }
    cout << "  [15] " << selecciones[15].getnombre()
         << " | " << selecciones[15].getConfederacion()
         << " | ranking: " << selecciones[15].getRanking() << "\n";

    cout << "[4] Creando bombos...\n";
    Seleccion* Bombos[4][12];
    CrearBombos(selecciones, Bombos);
    cout << "[5] Bombos creados\n";

    // Verificar que los 4 bombos están completos
    cout << "[6] Verificando bombos:\n";
    for (int b = 0; b < 4; b++) {
        cout << "  Bombo " << b+1 << ": ";
        for (int i = 0; i < 12; i++) {
            if (Bombos[b][i] == nullptr) {
                cout << "\n  ERROR: Bombos[" << b << "][" << i << "] es nullptr!\n";
            } else {
                cout << Bombos[b][i]->getnombre() << " ";
            }
        }
        cout << "\n";
    }

    cout << "[7] Creando grupos...\n";
    Grupo* grupos = CrearGrupos(Bombos);
    cout << "[8] Resultado CrearGrupos: " << (grupos ? "OK" : "NULLPTR") << "\n";

    if (!grupos) {
        cout << "Error: no se pudieron crear los grupos\n";
        delete[] selecciones;
        return 1;
    }

    cout << "[9] Imprimiendo grupos...\n";
    for (int i = 0; i < 12; i++) {
        cout << "  Imprimiendo grupo " << i << "...\n";
        grupos[i].imprimirGrupo();
        cout << "---------------------\n";
    }

    cout << "[10] Validando grupos...\n";
    if (validarGrupos(grupos)) {
        cout << "VALIDACION EXITOSA: grupos correctos\n";
    } else {
        cout << "VALIDACION FALLIDA\n";
    }

    cout << "[11] Liberando memoria...\n";
    delete[] grupos;
    delete[] selecciones;
    cout << "[12] Fin del programa\n";

    return 0;
}
