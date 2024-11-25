#ifndef TRANSACCION_H
#define TRANSACCION_H

#include <string>
#include <iostream>
#include "Pila.h"

using namespace std;

// Clase para manejar transacciones
class Transaccion {
public:
    string tipo;  // Tipo de transacción: "Deposito", "Retiro", "Transferencia"
    double monto;
    string fecha;
    int numeroCuenta;  // Para depósitos y retiros
    int numeroCuentaOrigen;  // Para transferencias
    int numeroCuentaDestino;  // Para transferencias

    // Constructor
    Transaccion(const string& tipo, double monto, const string& fecha, int numeroCuenta, int numeroCuentaOrigen = 0, int numeroCuentaDestino = 0)
        : tipo(tipo), monto(monto), fecha(fecha), numeroCuenta(numeroCuenta), numeroCuentaOrigen(numeroCuentaOrigen), numeroCuentaDestino(numeroCuentaDestino) {}

    // Método para mostrar la transaccion
    void mostrarTransaccion() const {
        cout << tipo << ": $" << monto << " en fecha " << fecha;
        if (tipo == "Transferencia") {
            cout << ", de cuenta " << numeroCuentaOrigen << " a cuenta " << numeroCuentaDestino;
        }
        cout << endl;
    }

    void deshacerUltimaOperacion(Pila<Transaccion>& pila){
    if (!pila.estaVacia()) {
        Transaccion t = pila.desapilar();  // Obtener la �ltima transacci�n
        // Deshacer la operaci�n seg�n el tipo de transacci�n (deposici�n, retiro, etc.)
        cout << "Operaci�n deshecha: " << t.tipo << " de $" << t.monto << endl;
    } else {
        cout << "No hay operaciones recientes para deshacer.\n";
    }
}
};

#endif // TRANSACCION_H
