#ifndef CUENTA_H
#define CUENTA_H

#include <vector>
#include <string>
#include <iostream>
#include "Transaccion.h"
using namespace std;

// Clase para manejar las cuentas bancarias
class Cuenta {
public:
    int numeroCuenta;
    double saldo;
    vector<Transaccion> historialTransacciones;

    // Constructor
    Cuenta(int numero, double saldoInicial) : numeroCuenta(numero), saldo(saldoInicial) {}

    // Método para depositar dinero en la cuenta
    void depositar(double monto, const string& fecha) {
        if (monto <= 0) {
            cout << "El monto debe ser mayor que cero.\n";
            return;
        }
        saldo += monto;
        historialTransacciones.push_back(Transaccion("Deposito", monto, fecha, numeroCuenta));
    }

    // Método para retirar dinero de la cuenta
    void retirar(double monto, const string& fecha) {
        if (monto <= 0) {
            cout << "El monto debe ser mayor que cero.\n";
            return;
        }
        if (monto <= saldo) {
            saldo -= monto;
            historialTransacciones.push_back(Transaccion("Retiro", monto, fecha, numeroCuenta));
        } else {
            cout << "Saldo insuficiente para el retiro.\n";
        }
    }

    // Método para mostrar el historial de transacciones
    void mostrarHistorial() const {
        cout << "Historial de Transacciones de la cuenta " << numeroCuenta << ":\n";

        for (const auto& t : historialTransacciones) {
            t.mostrarTransaccion();
        }
    }

    // Método para obtener el saldo actual
    double obtenerSaldo() const {
        return saldo;
    }

    // Método para transferir dinero entre cuentas
    void transferir(double monto, Cuenta* cuentaDestino, const string& fecha) {
        if (monto <= 0) {
            cout << "El monto debe ser mayor que cero.\n";
            return;
        }
        if (monto <= saldo) {
            saldo -= monto;
            cuentaDestino->depositar(monto, fecha);
            historialTransacciones.push_back(Transaccion("Transferencia", monto, fecha, numeroCuenta, numeroCuenta, cuentaDestino->numeroCuenta));
            cout << "Transferencia exitosa de $" << monto << " a la cuenta " << cuentaDestino->numeroCuenta << ".\n";
        } else {
            cout << "Saldo insuficiente para la transferencia.\n";
        }
    }

    // Método estático para buscar una cuenta en una lista
    static Cuenta* buscarCuentaLineal(const vector<Cuenta*>& cuentas, int numeroCuenta) {
        for (Cuenta* cuenta : cuentas) {
            if (cuenta->numeroCuenta == numeroCuenta) {
                return cuenta;
            }
        }
        return NULL;  // Cuenta no encontrada
    }
};

#endif // CUENTA_H
