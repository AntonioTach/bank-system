#ifndef PILA_H
#define PILA_H

#include <vector>
#include <stdexcept>
#include "Transaccion.h"
using namespace std;

// Clase para manejar las pilas (operaciones deshacer)
template <typename T>
class Pila {
private:
    vector<T> elementos;  // Almacena los elementos en la pila

public:
    // Método para apilar un elemento
    void apilar(const T& elemento) {
        elementos.push_back(elemento);
    }

    // Método para desapilar un elemento
    T desapilar() {
        if (!estaVacia()) {
            T elemento = elementos.back();
            elementos.pop_back();
            return elemento;
        }
        throw runtime_error("Pila vacía");
    }

    // Verifica si la pila está vacía
    bool estaVacia() const {
        return elementos.empty();
    }

    // Obtiene el último elemento sin desapilarlo
    T verUltima() const {
        if (!estaVacia()) {
            return elementos.back();
        }
        throw runtime_error("Pila vacía");
    }
};

#endif // PILA_H
