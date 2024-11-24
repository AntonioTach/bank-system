#ifndef USUARIO_H
#define USUARIO_H

#include <vector>
#include <string>
#include <iostream>
#include "Cuenta.h"
using namespace std;

// Clase para manejar los usuarios
class Usuario {
public:
    string nombre;
    string direccion;
    vector<Cuenta*> cuentas;  // Un usuario puede tener varias cuentas

    // Constructor
    Usuario(string nombre, string direccion) : nombre(nombre), direccion(direccion) {}

    // Método para agregar una cuenta al usuario
    void agregarCuenta(Cuenta* cuenta) {
        cuentas.push_back(cuenta);
    }

    // Método para leer monto y fecha desde la entrada estándar
    void leerMontoYFecha(double& monto, string& fecha) {
        cout << "Ingrese monto: ";
        cin >> monto;
        cout << "Ingrese fecha (dd/mm/aaaa): ";
        cin >> fecha;
    }

    // Método para leer el número de cuenta desde la entrada estándar
    int leerNumeroCuenta() {
        int numeroCuenta;
        cout << "Ingrese número de cuenta: ";
        cin >> numeroCuenta;
        return numeroCuenta;
    }

    // Método para abrir una nueva cuenta
    void abrirCuenta() {
        int numeroCuenta = leerNumeroCuenta();
        double saldoInicial;
        cout << "Ingrese saldo inicial: ";
        cin >> saldoInicial;
        Cuenta* nuevaCuenta = new Cuenta(numeroCuenta, saldoInicial);
        agregarCuenta(nuevaCuenta);
        cout << "Cuenta creada exitosamente.\n";
    }

    // Método para mostrar todas las cuentas del usuario
    void mostrarCuentas() const {
        cout << "Cuentas de " << nombre << ":\n";
        for (const auto& cuenta : cuentas) {
            cout << "Cuenta No. " << cuenta->numeroCuenta << " con saldo $" << cuenta->obtenerSaldo() << endl;
        }
    }
};

#endif // USUARIO_H
