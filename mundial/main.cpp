#include <iostream>
#include "Grupo.h"
#include "seleccion.h"
#include "manager.h"
#include "torneo.h"

using namespace std;

int main(){
    int act;
    Torneo cup= Torneo("selecciones_clasificadas_mundial.csv");
    cup.simularFaseGrupos();
    cup.clasificar();
    cout<<"quiere continuar con los 16avos? 1.si ";
    cin>>act;
    if (act==1){
        cup.simular_16();
        cout<<"quiere continuar con los 8vos? 1.si ";
        cin>>act;
            if(act==1){
                cup.simular_8();
                cout<<"quiere continuar con los 4tos? 1.si";
                cin>>act;
                if(act==1){
                    cup.simular_4();
                    cout<<"quiere continuar con las semis? 1.si";
                    cin>>act;
                    if(act==1){
                        cup.simular_semi();
                        cout<<"quiere continuar con la final y tercer puesto 1.si";

                        cin>>act;
                        if (act==1){
                            cup.simular_Final();
                            cup.imprimirEstadisticas();
                            while(true){
                            cout<<"\nque desea hacer 1.imprimir estadisticas 2.imprimir fase de grupos 3. salir";
                            cin>>act;
                            switch (act){
                            case 1:
                                  cup.imprimirEstadisticas();
                                break;
                            case 2:
                                cup.imprimirGrupos();
                                break;
                            default:
                                cup.guardarCSV("selecciones_clasificadas_mundial.csv");
                                return 0;
                            }
                            }
                        }
                    }
                }
            }

    }
     cout<<"Terminando simulacion"<<endl;
}
