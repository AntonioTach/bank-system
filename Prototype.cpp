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

// Funciones auxiliares para manejar usuarios
void cargarUsuariosDesdeArchivo(vector<Usuario*>& usuarios){
    ifstream archivo("usuarios.txt");
    string nombre, direccion;

    while (getline(archivo, nombre) && getline(archivo, direccion)) {
        Usuario* usuario = new Usuario(nombre, direccion);
        usuarios.push_back(usuario);
    }
    archivo.close();
}
void guardarUsuariosEnArchivo(const vector<Usuario*>& usuarios){
    ofstream archivo("usuarios.txt");

    for (const auto& usuario : usuarios) {
        archivo << usuario->nombre << "\n";
        archivo << usuario->direccion << "\n";
    }
    archivo.close();
}
Usuario* buscarUsuarioLineal(const vector<Usuario*>& usuarios, const string& nombre){
    for (Usuario* usuario : usuarios) {
        if (usuario->nombre == nombre) {
            return usuario;
        }
    }
    return nullptr;  // Usuario no encontrado
}

// Funciones auxiliares para manejar cuentas
void ordenarCuentasPorNumeroCuenta(vector<Cuenta*>& cuentas){
    sort(cuentas.begin(), cuentas.end(), [](Cuenta* a, Cuenta* b) { //algoritmo de ordenaci�n
        return a->numeroCuenta < b->numeroCuenta;  // Ordena en orden ascendente
    });
}
void ordenarCuentasBurbuja(vector<Cuenta*>& cuentas){
    bool intercambio;
    for (int i = 0; i < cuentas.size() - 1; i++) {
        intercambio = false;
        for (int j = 0; j < cuentas.size() - 1 - i; j++) {
            if (cuentas[j]->numeroCuenta > cuentas[j + 1]->numeroCuenta) {
                // Intercambiar las cuentas
                swap(cuentas[j], cuentas[j + 1]);
                intercambio = true;
            }
        }
        if (!intercambio) break;  // Si no hubo intercambio, la lista ya est� ordenada
    }
}
void ordenarCuentasSeleccion(vector<Cuenta*>& cuentas){
    for (int i = 0; i < cuentas.size() - 1; i++) {
        int minIndex = i;
        for (int j = i + 1; j < cuentas.size(); j++) {
            if (cuentas[j]->numeroCuenta < cuentas[minIndex]->numeroCuenta) {
                minIndex = j;
            }
        }
        // Intercambiar la cuenta m�nima con la primera no ordenada
        swap(cuentas[i], cuentas[minIndex]);
    }
}
Cuenta* buscarCuentaBinaria(const vector<Cuenta*>& cuentas, int numeroCuenta){
    for (Cuenta* cuenta : cuentas) {
        if (cuenta->numeroCuenta == numeroCuenta) {
            return cuenta;
        }
    }
    return nullptr;  // Cuenta no encontrada
}
Cuenta* buscarCuentaLineal(const vector<Cuenta*>& cuentas, int numeroCuenta){
    int inicio = 0, fin = cuentas.size() - 1;
    while (inicio <= fin) {
        int medio = inicio + (fin - inicio) / 2;
        if (cuentas[medio]->numeroCuenta == numeroCuenta) {
            return cuentas[medio];  // Cuenta encontrada
        }
        if (cuentas[medio]->numeroCuenta < numeroCuenta) {
            inicio = medio + 1;  // Buscar en la mitad derecha
        } else {
            fin = medio - 1;  // Buscar en la mitad izquierda
        }
    }
    return nullptr;  // Cuenta no encontrada
}

// Funciones auxiliares para transacciones
void guardarTransaccionesEnArchivo(const vector<Cuenta*>& cuentas){
    ofstream archivo("transacciones.txt");

    for (const auto& cuenta : cuentas) {
        for (const auto& transaccion : cuenta->historialTransacciones) {
            archivo << "Cuenta: " << cuenta->numeroCuenta << ", Tipo: " << transaccion.tipo
                    << ", Monto: $" << transaccion.monto << ", Fecha: " << transaccion.fecha << endl;
        }
    }
    archivo.close();
}
void buscarTransaccionesPorTipo(const Cuenta& cuenta, const string& tipo){
    for (const auto& transaccion : cuenta.historialTransacciones) {
        if (transaccion.tipo == tipo) {
            cout << "Tipo: " << transaccion.tipo << ", Monto: $" << transaccion.monto << ", Fecha: " << transaccion.fecha << endl;
        }
    }
}
void buscarTransaccionesPorFecha(const Cuenta& cuenta, const string& fecha){
    bool encontrado = false;
    for (const auto& transaccion : cuenta.historialTransacciones) {
        if (transaccion.fecha == fecha) {
            cout << "Fecha: " << transaccion.fecha << ", Tipo: " << transaccion.tipo << ", Monto: $" << transaccion.monto << endl;
            encontrado = true;
        }
    }
    if (!encontrado) {
        cout << "No se encontraron transacciones en la fecha " << fecha << ".\n";
    }
}

// Funciones auxiliares generales
void deshacerUltimaOperacion(Pila<Transaccion>& pila){
    if (!pila.estaVacia()) {
        Transaccion t = pila.desapilar();  // Obtener la �ltima transacci�n
        // Deshacer la operaci�n seg�n el tipo de transacci�n (deposici�n, retiro, etc.)
        cout << "Operaci�n deshecha: " << t.tipo << " de $" << t.monto << endl;
    } else {
        cout << "No hay operaciones recientes para deshacer.\n";
    }
}


void mostrarMenu(){
    cout << "\n=== Menu Bancario ===\n";
    cout << "1. Abrir cuenta\n";
    cout << "2. Realizar dep�sito\n";
    cout << "3. Realizar retiro\n";
    cout << "4. Transferir dinero\n";
    cout << "5. Ver historial de transacciones\n";
    cout << "6. Deshacer �ltima operaci�n\n";
    cout << "7. Ver relaciones entre cuentas\n";
    cout << "8. Salir\n";
    cout << "Seleccione una opci�n: ";
}

// Funcion principal
int main() {
    // menu();
    // TEST MODULOS
    // Crear un usuario
    Usuario usuario("Juan Perez", "Calle Falsa 123");

    // Crear una cuenta y agregarla al usuario
    Cuenta* cuenta = new Cuenta(1001, 500.0); // Número de cuenta: 1001, Saldo inicial: 500
    usuario.agregarCuenta(cuenta);

    // Realizar un depósito en la cuenta
    cuenta->depositar(200.0, "23/11/2024");

    // Mostrar las cuentas del usuario y su saldo actualizado
    usuario.mostrarCuentas();

    delete cuenta;

    return 0;
}

int menu() { // MENU DE OPCIONES
    vector<Usuario*> usuarios;
    Pila<Transaccion> pilaTransacciones;
    Grafo grafo;

    Usuario* usuario = new Usuario("Usuario", "Ejemplo default");
    Cuenta* cuenta1 = new Cuenta(1001, 500.0);
    Cuenta* cuenta2 = new Cuenta(1002, 300.0);
    usuario->agregarCuenta(cuenta1);
    usuario->agregarCuenta(cuenta2);

    int opcion;
    while (true) {
        mostrarMenu();
        cin >> opcion;
        switch (opcion) {
            case 1: {
                // Abrir cuenta
                int numeroCuenta;
                double saldoInicial;
                cout << "Ingrese n�mero de cuenta: ";
                cin >> numeroCuenta;
                cout << "Ingrese saldo inicial: ";
                cin >> saldoInicial;
                Cuenta* nuevaCuenta = new Cuenta(numeroCuenta, saldoInicial);
                usuario->agregarCuenta(nuevaCuenta);
                cout << "Cuenta creada exitosamente.\n";
                break;
            }
            case 2: {
                // Realizar dep�sito
                int numeroCuenta;
                double monto;
                string fecha;
                cout << "Ingrese n�mero de cuenta: ";
                cin >> numeroCuenta;
                cout << "Ingrese monto a depositar: ";
                cin >> monto;
                cout << "Ingrese fecha (dd/mm/aaaa): ";
                cin >> fecha;
                Cuenta* cuenta = buscarCuentaBinaria(usuario->cuentas, numeroCuenta);
                if (cuenta != nullptr) {
                    cuenta->depositar(monto, fecha);
                    cout << "Dep�sito realizado.\n";
                } else {
                    cout << "Cuenta no encontrada.\n";
                }
                break;
            }
            case 3: {
                // Realizar retiro
                int numeroCuenta;
                double monto;
                string fecha;
                cout << "Ingrese n�mero de cuenta: ";
                cin >> numeroCuenta;
                cout << "Ingrese monto a retirar: ";
                cin >> monto;
                cout << "Ingrese fecha (dd/mm/aaaa): ";
                cin >> fecha;
                Cuenta* cuenta = buscarCuentaBinaria(usuario->cuentas, numeroCuenta);
                if (cuenta != nullptr) {
                    cuenta->retirar(monto, fecha);
                    cout << "Retiro realizado.\n";
                } else {
                    cout << "Cuenta no encontrada.\n";
                }
                break;
            }
            case 4: {
                // Transferir dinero
                int numeroCuentaOrigen, numeroCuentaDestino;
                double monto;
                string fecha;
                cout << "Ingrese n�mero de cuenta origen: ";
                cin >> numeroCuentaOrigen;
                cout << "Ingrese n�mero de cuenta destino: ";
                cin >> numeroCuentaDestino;
                cout << "Ingrese monto a transferir: ";
                cin >> monto;
                cout << "Ingrese fecha (dd/mm/aaaa): ";
                cin >> fecha;

                Cuenta* cuentaOrigen = buscarCuentaBinaria(usuario->cuentas, numeroCuentaOrigen);
                Cuenta* cuentaDestino = buscarCuentaBinaria(usuario->cuentas, numeroCuentaDestino);
                if (cuentaOrigen != nullptr && cuentaDestino != nullptr) {
                    cuentaOrigen->transferir(monto, cuentaDestino, fecha);
                } else {
                    cout << "Una o ambas cuentas no existen.\n";
                }
                break;
            }
            case 5: {
                // Ver historial de transacciones
                for (const auto& cuenta : usuario->cuentas) {
                    cuenta->mostrarHistorial();
                }
                break;
            }
            case 6: {
            // Deshacer la �ltima operaci�n (�ltima transacci�n)
            if (!pilaTransacciones.estaVacia()) {
                Transaccion ultimaTransaccion = pilaTransacciones.desapilar(); // Desapilamos la �ltima transacci�n

                // L�gica para deshacer la operaci�n
                if (ultimaTransaccion.tipo == "Deposito") {
                    Cuenta* cuenta = buscarCuentaLineal(usuario->cuentas, ultimaTransaccion.numeroCuenta);
                    if (cuenta) {
                        cuenta->saldo -= ultimaTransaccion.monto;  // Deshacer el dep�sito
                        cout << "Dep�sito de $" << ultimaTransaccion.monto << " deshecho en cuenta " << ultimaTransaccion.numeroCuenta << ".\n";
                    }
                } else if (ultimaTransaccion.tipo == "Retiro") {
                    Cuenta* cuenta = buscarCuentaLineal(usuario->cuentas, ultimaTransaccion.numeroCuenta);
                    if (cuenta) {
                        cuenta->saldo += ultimaTransaccion.monto;  // Deshacer el retiro
                        cout << "Retiro de $" << ultimaTransaccion.monto << " deshecho en cuenta " << ultimaTransaccion.numeroCuenta << ".\n";
                    }
                } else if (ultimaTransaccion.tipo == "Transferencia") {
                    // Deshacer una transferencia
                    Cuenta* cuentaOrigen = buscarCuentaLineal(usuario->cuentas, ultimaTransaccion.numeroCuentaOrigen);
                    Cuenta* cuentaDestino = buscarCuentaLineal(usuario->cuentas, ultimaTransaccion.numeroCuentaDestino);
                    if (cuentaOrigen && cuentaDestino) {
                        cuentaOrigen->saldo += ultimaTransaccion.monto;  // Revertir la transferencia
                        cuentaDestino->saldo -= ultimaTransaccion.monto;
                        cout << "Transferencia de $" << ultimaTransaccion.monto
                            << " deshecha entre cuentas " << ultimaTransaccion.numeroCuentaOrigen
                            << " y " << ultimaTransaccion.numeroCuentaDestino << ".\n";
                    }
                }
            } else {
                cout << "No hay operaciones recientes para deshacer.\n";
            }
            break;
            }

            case 7: {
                // Ver relaciones entre cuentas
                int numeroCuenta;
                cout << "Ingrese el n�mero de cuenta para ver sus relaciones: ";
                cin >> numeroCuenta;

                // Mostrar las relaciones usando el grafo
                grafo.mostrarRelaciones(numeroCuenta);
                break;
            }

            case 8:
                // Salir
                cout << "Saliendo del sistema...\n";
                return 0;
            default:
                cout << "Opci�n no v�lida.\n";
        }
    }
}

