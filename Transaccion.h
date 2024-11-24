#ifndef TRANSACCION_H
#define TRANSACCION_H

#include <string>
#include <iostream>
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

    // Método para mostrar la transacción
    void mostrarTransaccion() const {
        cout << tipo << ": $" << monto << " en fecha " << fecha;
        if (tipo == "Transferencia") {
            cout << ", de cuenta " << numeroCuentaOrigen << " a cuenta " << numeroCuentaDestino;
        }
        cout << endl;
    }
};

#endif // TRANSACCION_H
