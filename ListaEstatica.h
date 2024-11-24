#ifndef LISTAESTATICA_H
#define LISTAESTATICA_H

#include <iostream>
#include <stdexcept>
using namespace std;

// Clase Lista estática para manejar cuentas de usuarios
template <typename T>
class ListaEstatica {
private:
    T* elementos;  // Arreglo de elementos
    int capacidad; // Capacidad máxima de la lista
    int tamano;    // Cantidad actual de elementos en la lista

public:
    // Constructor
    ListaEstatica(int capacidad) : capacidad(capacidad), tamano(0) {
        elementos = new T[capacidad];
    }

    // Destructor
    ~ListaEstatica() {
        delete[] elementos;
    }

    // Método para agregar un elemento a la lista
    void agregar(const T& elemento) {
        if (tamano < capacidad) {
            elementos[tamano++] = elemento;
        } else {
            cout << "Lista estática llena.\n";
        }
    }

    // Método para obtener un elemento por índice
    T obtener(int indice) const {
        if (indice >= 0 && indice < tamano) {
            return elementos[indice];
        }
        throw out_of_range("Índice fuera de rango");
    }

    // Método para obtener el tamaño actual de la lista
    int getTamano() const {
        return tamano;
    }

    // Método para imprimir los elementos de la lista
    void imprimirLista() const {
        cout << "Elementos en la lista:\n";
        for (int i = 0; i < tamano; i++) {
            cout << "- Elemento " << i + 1 << ": " << elementos[i] << endl;
        }
    }
};

#endif // LISTAESTATICA_H
