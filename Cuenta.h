#ifndef CUENTA_H
#define CUENTA_H

#include <vector>
#include <string>
#include <iostream>
#include <fstream>
#include <algorithm> // Para sort
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

    // Metodo para depositar dinero en la cuenta
    void depositar(double monto, const string& fecha) {
        if (monto <= 0) {
            cerr << "Error: El monto debe ser mayor que cero.\n";
            return;
        }
        saldo += monto;
        historialTransacciones.push_back(Transaccion("Deposito", monto, fecha, numeroCuenta));
        cout << "Deposito de $" << monto << " realizado exitosamente en la cuenta " << numeroCuenta << ".\n";
    }

    // Metodo para retirar dinero de la cuenta
    void retirar(double monto, const string& fecha) {
        if (monto <= 0) {
            cerr << "Error: El monto debe ser mayor que cero.\n";
            return;
        }
        if (monto <= saldo) {
            saldo -= monto;
            historialTransacciones.push_back(Transaccion("Retiro", monto, fecha, numeroCuenta));
            cout << "Retiro de $" << monto << " realizado exitosamente en la cuenta " << numeroCuenta << ".\n";
        } else {
            cerr << "Error: Saldo insuficiente para realizar el retiro.\n";
        }
    }

    // Metodo para mostrar el historial de transacciones
    void mostrarHistorial() const {
        if (historialTransacciones.empty()) {
            cout << "No hay transacciones registradas en la cuenta " << numeroCuenta << ".\n";
            return;
        }
        cout << "Historial de Transacciones de la cuenta " << numeroCuenta << ":\n";
        for (const auto& t : historialTransacciones) {
            t.mostrarTransaccion();
        }
    }

    // Metodo para obtener el saldo actual
    double obtenerSaldo() const {
        return saldo;
    }

    // Metodo para transferir dinero entre cuentas
    void transferir(double monto, Cuenta* cuentaDestino, const string& fecha) {
        if (cuentaDestino == nullptr) {
            cerr << "Error: La cuenta destino no existe.\n";
            return;
        }
        if (monto <= 0) {
            cerr << "Error: El monto debe ser mayor que cero.\n";
            return;
        }
        if (monto <= saldo) {
            saldo -= monto;
            cuentaDestino->depositar(monto, fecha);
            historialTransacciones.push_back(Transaccion("Transferencia", monto, fecha, numeroCuenta, numeroCuenta, cuentaDestino->numeroCuenta));
            cout << "Transferencia de $" << monto << " realizada exitosamente de la cuenta "
                 << numeroCuenta << " a la cuenta " << cuentaDestino->numeroCuenta << ".\n";
        } else {
            cerr << "Error: Saldo insuficiente para realizar la transferencia.\n";
        }
    }

    // Metodo estatico para buscar una cuenta linealmente
    static Cuenta* buscarCuentaLineal(const vector<Cuenta*>& cuentas, int numeroCuenta) {
        for (Cuenta* cuenta : cuentas) {
            if (cuenta->numeroCuenta == numeroCuenta) {
                return cuenta;
            }
        }
        //cerr << "Error: Cuenta con numero " << numeroCuenta << " no encontrada.\n";
        return nullptr;
    }

    // Metodo estatico para buscar una cuenta usando busqueda binaria
    static Cuenta* buscarCuentaBinaria(vector<Cuenta*>& cuentas, int numeroCuenta) {
        ordenarPorNumeroCuenta(cuentas);
        size_t inicio = 0, fin = cuentas.size() - 1;
        while (inicio <= fin) {
            size_t medio = inicio + (fin - inicio) / 2;
            if (cuentas[medio]->numeroCuenta == numeroCuenta) {
                return cuentas[medio];
            }
            if (cuentas[medio]->numeroCuenta < numeroCuenta) {
                inicio = medio + 1;
            } else {
                fin = medio - 1;
            }
        }
        cerr << "Error: Cuenta con numero " << numeroCuenta << " no encontrada.\n";
        return nullptr;
    }

    // Metodo estatico para ordenar cuentas por numero de cuenta
    static void ordenarPorNumeroCuenta(vector<Cuenta*>& cuentas) {
        sort(cuentas.begin(), cuentas.end(), [](Cuenta* a, Cuenta* b) {
            return a->numeroCuenta < b->numeroCuenta;
        });
    }

    // Metodo para buscar transacciones por tipo
    void buscarTransaccionesPorTipo(const string& tipo) const {
        bool encontrado = false;
        for (const auto& transaccion : historialTransacciones) {
            if (transaccion.tipo == tipo) {
                transaccion.mostrarTransaccion();
                encontrado = true;
            }
        }
        if (!encontrado) {
            cout << "No se encontraron transacciones de tipo " << tipo << " en la cuenta " << numeroCuenta << ".\n";
        }
    }

    // Metodo para buscar transacciones por fecha
    void buscarTransaccionesPorFecha(const string& fecha) const {
        bool encontrado = false;
        for (const auto& transaccion : historialTransacciones) {
            if (transaccion.fecha == fecha) {
                transaccion.mostrarTransaccion();
                encontrado = true;
            }
        }
        if (!encontrado) {
            cout << "No se encontraron transacciones en la fecha " << fecha << " en la cuenta " << numeroCuenta << ".\n";
        }
    }

    // Metodo para guardar transacciones en archivo
    static void guardarTransaccionesEnArchivo(const vector<Cuenta*>& cuentas) {
        ofstream archivo("transacciones.txt");
        if (!archivo) {
            cerr << "Error: No se pudo abrir el archivo de transacciones.\n";
            return;
        }

        for (const auto& cuenta : cuentas) {
            for (const auto& transaccion : cuenta->historialTransacciones) {
                archivo << "Cuenta: " << cuenta->numeroCuenta << ", Tipo: " << transaccion.tipo
                        << ", Monto: $" << transaccion.monto << ", Fecha: " << transaccion.fecha << endl;
            }
        }
        cout << "Transacciones guardadas exitosamente en 'transacciones.txt'.\n";
        archivo.close();
    }
};

#endif // CUENTA_H
