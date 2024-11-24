#ifndef COLA_H
#define COLA_H

#include <stdexcept>
using namespace std;

// Clase para manejar las colas (transacciones pendientes)
template <typename T>
class Cola {
private:
    // Nodo interno de la cola
    struct Nodo {
        T dato;
        Nodo* siguiente;

        Nodo(const T& dato) : dato(dato), siguiente(NULL) {}
    };

    Nodo* frente;  // Apunta al frente de la cola
    Nodo* final;   // Apunta al final de la cola

public:
    // Constructor
    Cola() : frente(NULL), final(NULL) {}

    // Destructor
    ~Cola() {
        while (frente != NULL) {
            Nodo* temp = frente;
            frente = frente->siguiente;
            delete temp;
        }
    }

    // Encola un elemento al final de la cola
    void encolar(const T& dato) {
        Nodo* nuevo = new Nodo(dato);
        if (final != NULL) {
            final->siguiente = nuevo;
        } else {
            frente = nuevo;
        }
        final = nuevo;
    }

    // Desencola un elemento del frente de la cola
    T desencolar() {
        if (frente != NULL) {
            Nodo* temp = frente;
            T dato = frente->dato;
            frente = frente->siguiente;
            if (frente == NULL) {
                final = NULL;  // La cola está vacía
            }
            delete temp;
            return dato;
        }
        throw underflow_error("Cola vacía");
    }

    // Verifica si la cola está vacía
    bool estaVacia() const {
        return frente == NULL;
    }
};

#endif // COLA_H
