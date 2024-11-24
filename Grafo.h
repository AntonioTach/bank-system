#ifndef GRAFO_H
#define GRAFO_H

#include <map>
#include <vector>
#include <iostream>
using namespace std;

// Clase Grafo (esquemática para representar la relación entre cuentas o usuarios)
class Grafo {
public:
    map<int, vector<int>> adjList;  // Representación de un grafo como lista de adyacencia

    // Método para agregar una relación entre dos nodos (ej., deudas o transferencias)
    void agregarRelacion(int cuentaOrigen, int cuentaDestino) {
        adjList[cuentaOrigen].push_back(cuentaDestino);
    }

    // Método para mostrar todas las relaciones de un nodo
    void mostrarRelaciones(int cuenta) const {
        if (adjList.find(cuenta) != adjList.end()) {
            cout << "Relaciones para la cuenta " << cuenta << ":\n";
            for (int destino : adjList.at(cuenta)) {
                cout << "- Cuenta " << destino << endl;
            }
        } else {
            cout << "No hay relaciones para la cuenta " << cuenta << ".\n";
        }
    }

    // Método para mostrar todo el grafo
    void mostrarGrafoCompleto() const {
        cout << "Relaciones en el grafo:\n";
        for (const auto& par : adjList) {
            cout << "Cuenta " << par.first << ":\n";
            for (int destino : par.second) {
                cout << "  -> Cuenta " << destino << endl;
            }
        }
    }
};

#endif // GRAFO_H
