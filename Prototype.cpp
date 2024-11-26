#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include <algorithm>
#include <map>

// CLASES
#include "Transaccion.h"
#include "Cuenta.h"
#include "Usuario.h"
#include "Cola.h"
#include "Pila.h"
#include "Grafo.h"
#include "ListaEstatica.h"

// Angel Alberto Campos Pelayo
// Antonio Viña Hernandez

using namespace std;

void mostrarMenu(){
    cout << "\n=== Menu Bancario ===\n";
    cout << "1. Abrir cuenta\n";
    cout << "2. Realizar deposito\n";
    cout << "3. Realizar retiro\n";
    cout << "4. Transferir dinero\n";
    cout << "5. Ver historial de transacciones\n";
    cout << "6. Deshacer ultima operacion\n";
    cout << "7. Ver relaciones entre cuentas\n";
    cout << "8. Salir\n";
    cout << "Seleccione una opcion: ";
}

void mostrarMenuAutenticacion() {
    cout << "\n=== Menu de Autenticacion ===\n";
    cout << "1. Iniciar sesion\n";
    cout << "2. Crear usuario\n";
    cout << "3. Salir\n";
    cout << "Seleccione una opcion: ";
}

void menuAutenticacion(vector<Usuario*>& usuarios, Usuario*& usuarioActivo) {
    int opcion;
    while (true) {
        mostrarMenuAutenticacion();
        cin >> opcion;
        switch (opcion) {
            case 1: { // Iniciar sesión
                string nombre, contrasena;
                cout << "Ingrese su nombre: ";
                cin >> nombre;
                cout << "Ingrese su contrasena: ";
                cin >> contrasena;

                usuarioActivo = Usuario::buscarUsuarioLineal(usuarios, nombre, contrasena);
                if (usuarioActivo != nullptr) {
                    cout << "Sesion iniciada como " << nombre << ".\n";
                    return;
                } else {
                    cout << "Nombre de usuario o contrasena incorrectos.\n";
                }
                break;
            }
            case 2: { // Crear usuario
                string nombre, direccion, contrasena;
                cout << "Ingrese su nombre: ";
                cin >> nombre;
                cout << "Ingrese su direccion: ";
                cin >> direccion;
                cout << "Ingrese su contrasena: ";
                cin >> contrasena;
                Usuario* nuevoUsuario = new Usuario(nombre, direccion, contrasena);
                usuarios.push_back(nuevoUsuario);
                cout << "Usuario creado exitosamente. Ahora puede iniciar sesion.\n";
                break;
            }
            case 3: { // Salir
                cout << "Saliendo del sistema...\n";
                exit(0);
            }
            default:
                cout << "Opcion no valida. Intente nuevamente.\n";
        }
    }
}

void menuPrincipal(vector<Usuario*>& usuarios, Usuario*& usuarioActivo, Pila<Transaccion>& pilaTransacciones, Grafo* grafo) {
    int opcion;
    while (true) {
        mostrarMenu();
        cin >> opcion;
        switch (opcion) {
            case 1: { // Abrir cuenta
                usuarioActivo->abrirCuenta();
                break;
            }
            case 2: { // Realizar deposito
                if (usuarioActivo == nullptr) {
                    cerr << "Error: No hay un usuario activo. Por favor, inicie sesion.\n";
                    break;
                }

                int numeroCuenta;
                cout << "Ingrese numero de cuenta: ";
                cin >> numeroCuenta;

                // Validar que el número de cuenta es válido (positivo)
                if (numeroCuenta <= 0) {
                    cerr << "Error: El numero de cuenta debe ser un valor positivo.\n";
                    break;
                }

                // Buscar la cuenta
                Cuenta* cuenta = Cuenta::buscarCuentaBinaria(usuarioActivo->cuentas, numeroCuenta);

                // Validar si la cuenta existe antes de continuar
                if (cuenta == nullptr) {
                    cerr << "Error: La cuenta ingresada no existe. Verifique el numero de cuenta.\n";
                    break;
                }

                // Si la cuenta existe, solicitar el monto y la fecha
                double monto;
                string fecha;

                cout << "Ingrese monto a depositar: ";
                cin >> monto;

                // Validar que el monto es válido (positivo)
                if (monto <= 0) {
                    cerr << "Error: El monto debe ser mayor que cero.\n";
                    break;
                }

                cout << "Ingrese fecha (dd/mm/aaaa): ";
                cin >> fecha;

                // Realizar el depósito
                cuenta->depositar(monto, fecha);
                cout << "Deposito de $" << monto << " realizado exitosamente en la cuenta " << numeroCuenta << ".\n";
                break;
            }

            case 3: { // Realizar retiro
                int numeroCuenta;
                double monto;
                string fecha;
                cout << "Ingrese numero de cuenta: ";
                cin >> numeroCuenta;
                cout << "Ingrese monto a retirar: ";
                cin >> monto;
                cout << "Ingrese fecha (dd/mm/aaaa): ";
                cin >> fecha;

                Cuenta* cuenta = Cuenta::buscarCuentaBinaria(usuarioActivo->cuentas, numeroCuenta);
                if (cuenta != nullptr) {
                    cuenta->retirar(monto, fecha);
                    cout << "Retiro realizado.\n";
                } else {
                    cout << "Cuenta no encontrada.\n";
                }
                break;
            }
            case 4: { // Transferir dinero
                if (usuarioActivo == nullptr) {
                    cerr << "Error: No hay un usuario activo. Por favor, inicie sesion.\n";
                    break;
                }

                int numeroCuentaOrigen, numeroCuentaDestino;
                double monto;
                string fecha;

                cout << "Ingrese numero de cuenta origen: ";
                cin >> numeroCuentaOrigen;

                // Buscar cuenta origen globalmente
                Cuenta* cuentaOrigen = nullptr;
                for (Usuario* usuario : usuarios) {
                    cuentaOrigen = Cuenta::buscarCuentaLineal(usuario->cuentas, numeroCuentaOrigen);
                    if (cuentaOrigen != nullptr) break;
                }

                cout << "Ingrese numero de cuenta destino: ";
                cin >> numeroCuentaDestino;

                // Buscar cuenta destino globalmente
                Cuenta* cuentaDestino = nullptr;
                for (Usuario* usuario : usuarios) {
                    cuentaDestino = Cuenta::buscarCuentaLineal(usuario->cuentas, numeroCuentaDestino);
                    if (cuentaDestino != nullptr) break;
                }

                if (cuentaDestino == nullptr) {
                    cerr << "Error: La cuenta destino no existe. Verifique el numero de cuenta ingresado.\n";
                    break;
                }

                cout << "Ingrese monto a transferir: ";
                cin >> monto;

                if (monto <= 0) {
                    cerr << "Error: El monto debe ser mayor que cero.\n";
                    break;
                }

                cout << "Ingrese fecha (dd/mm/aaaa): ";
                cin >> fecha;

                // Realizar la transferencia
                if (cuentaOrigen->obtenerSaldo() < monto) {
                    cerr << "Error: Saldo insuficiente en la cuenta origen.\n";
                } else {
                    cuentaOrigen->transferir(monto, cuentaDestino, fecha);

                    // Crear la transacción de transferencia
                    Transaccion transaccion("Transferencia", monto, fecha, numeroCuentaOrigen, numeroCuentaOrigen, numeroCuentaDestino);

                    //apilar la transaccion para deshacer si es necesario
                    pilaTransacciones.apilar(transaccion);

                    //agregar la relacion al grafo
                    grafo->agregarRelacion(cuentaOrigen->numeroCuenta,cuentaDestino->numeroCuenta);
                    cout << "Transferencia realizada exitosamente.\n";
                }
                break;
            }
            case 5: { // Ver historial de transacciones
                for (const auto& cuenta : usuarioActivo->cuentas) {
                    cuenta->mostrarHistorial();
                }
                break;
            }
            case 6: { // Deshacer ultima operacion
                if (!pilaTransacciones.estaVacia()) {
                    Transaccion ultimaTransaccion = pilaTransacciones.desapilar();
                    cout << "Operacion deshecha: " << ultimaTransaccion.tipo << endl;
                } else {
                    cout << "No hay operaciones para deshacer.\n";
                }
                break;
            }
            case 7: { // Ver relaciones entre cuentas
                int numeroCuenta;
                cout << "Ingrese el numero de cuenta para ver sus relaciones: ";
                cin >> numeroCuenta;
                grafo->mostrarRelaciones(numeroCuenta);
                break;
            }
            case 8: { // Cerrar sesion
                cout << "Cerrando sesion...\n";
                return;
            }
            default:
                cout << "Opcion no valida.\n";
        }
    }
}

// Funcion principal
int main() {
    vector<Usuario*> usuarios;
    Pila<Transaccion> pilaTransacciones;
    Grafo* grafo = new Grafo();
    Usuario* usuarioActivo = nullptr;

    while (true) {
        menuAutenticacion(usuarios, usuarioActivo);
        if (usuarioActivo) {
            menuPrincipal(usuarios, usuarioActivo, pilaTransacciones, grafo);
        }
    }

    return 0;
}
