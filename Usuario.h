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
    string contrasena;
    vector<Cuenta*> cuentas;  // Un usuario puede tener varias cuentas

    // Constructor
    Usuario(string nombre, string direccion, string contrasena) : nombre(nombre), direccion(direccion), contrasena(contrasena) {}

    // Metodo para agregar una cuenta al usuario
    void agregarCuenta(Cuenta* cuenta) {
        cuentas.push_back(cuenta);
    }

    // Metodo para leer monto y fecha desde la entrada estándar
    void leerMontoYFecha(double& monto, string& fecha) {
        cout << "Ingrese monto: ";
        cin >> monto;
        cout << "Ingrese fecha (dd/mm/aaaa): ";
        cin >> fecha;
    }

    // Método para leer el número de cuenta desde la entrada estándar
    int leerNumeroCuenta() {
        int numeroCuenta;
        cout << "Ingrese numero de cuenta: ";
        cin >> numeroCuenta;
        return numeroCuenta;
    }

    // Metodo para abrir una nueva cuenta
    void abrirCuenta() {
        int numeroCuenta = leerNumeroCuenta();
        double saldoInicial;
        Cuenta* nuevaCuenta = new Cuenta(numeroCuenta, 0);
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

    // Metodo para verificar la contraseña
    bool verificarContrasena(const string& contrasenaIngresada) const {
        return contrasena == contrasenaIngresada;
    }

    // Funciones static para manejar usuarios
    static void cargarUsuariosDesdeArchivo(vector<Usuario*>& usuarios) {
        ifstream archivo("usuarios.txt");
        if (!archivo.is_open()) {
            cerr << "Error al abrir el archivo de usuarios.\n";
            return;
        }
        string nombre, direccion, contrasena;
        while (getline(archivo, nombre) && getline(archivo, direccion) && getline(archivo, contrasena)) {
            Usuario* usuario = new Usuario(nombre, direccion, contrasena);
            usuarios.push_back(usuario);
        }
        archivo.close();
    }

    static void guardarUsuariosEnArchivo(const vector<Usuario*>& usuarios) {
        ofstream archivo("usuarios.txt");
        if (!archivo.is_open()) {
            cerr << "Error al guardar el archivo de usuarios.\n";
            return;
        }
        for (const auto& usuario : usuarios) {
            archivo << usuario->nombre << "\n";
            archivo << usuario->direccion << "\n";
        }
        archivo.close();
    }

    static Usuario* buscarUsuarioLineal(const vector<Usuario*>& usuarios, const string& nombre, const string& contrasenaIngresada) {
    for (Usuario* usuario : usuarios) {
        if (usuario->nombre == nombre && usuario->verificarContrasena(contrasenaIngresada)) {
            return usuario;
        }
    }
    return nullptr; // Usuario no encontrado o contraseña incorrecta
}
};

#endif // USUARIO_H
