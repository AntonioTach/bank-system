#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include <algorithm>
#include <map>

// Angel Alberto Campos Pelayo
// Antonio Viña Hernandez

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

    Transaccion(const string& tipo, double monto, const string& fecha, int numeroCuenta, int numeroCuentaOrigen = 0, int numeroCuentaDestino = 0)
        : tipo(tipo), monto(monto), fecha(fecha), numeroCuenta(numeroCuenta), numeroCuentaOrigen(numeroCuentaOrigen), numeroCuentaDestino(numeroCuentaDestino) {}

    void mostrarTransaccion() const {
        cout << tipo << ": $" << monto << " en fecha " << fecha << endl;
    }
};

// Clase para manejar las cuentas bancarias
class Cuenta {
public:
    int numeroCuenta;
    double saldo;
    vector<Transaccion> historialTransacciones;

    Cuenta(int numero, double saldoInicial) : numeroCuenta(numero), saldo(saldoInicial) {}

    void depositar(double monto, const string& fecha) {
    if (monto <= 0) {
        cout << "El monto debe ser mayor que cero.\n";
        return;
    }
    saldo += monto;
    historialTransacciones.push_back(Transaccion("Deposito", monto, fecha, numeroCuenta));
}


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


    void mostrarHistorial() const {
    cout << "Historial de Transacciones de la cuenta " << numeroCuenta << ":\n";
    for (const auto& t : historialTransacciones) {
        t.mostrarTransaccion();
    }
}


    double obtenerSaldo() const {
        return saldo;
    }

    void transferir(double monto, Cuenta* cuentaDestino, const string& fecha) {
    if (monto <= 0) {
        cout << "El monto debe ser mayor que cero.\n";
        return;
    }
    if (monto <= saldo) {
        saldo -= monto;
        cuentaDestino->depositar(monto, fecha);  // Depósito en la cuenta destino
        historialTransacciones.push_back(Transaccion("Transferencia", monto, fecha, numeroCuenta, numeroCuenta, cuentaDestino->numeroCuenta));
        cout << "Transferencia exitosa de $" << monto << " a la cuenta " << cuentaDestino->numeroCuenta << ".\n";
    } else {
        cout << "Saldo insuficiente para la transferencia.\n";
    }
}

    Cuenta* buscarCuentaLineal(const vector<Cuenta*>& cuentas, int numeroCuenta) {
    for (Cuenta* cuenta : cuentas) {
        if (cuenta->numeroCuenta == numeroCuenta) {
            return cuenta;
        }
    }
    return nullptr;  // Cuenta no encontrada
}
};

// Clase para manejar los usuarios
class Usuario {
public:
    string nombre;
    string direccion;
    vector<Cuenta*> cuentas;  // Un usuario puede tener varias cuentas

    Usuario(string nombre, string direccion) : nombre(nombre), direccion(direccion) {}

    void agregarCuenta(Cuenta* cuenta) {
        cuentas.push_back(cuenta);
    }

    void leerMontoYFecha(double &monto, string &fecha) {
    cout << "Ingrese monto: ";
    cin >> monto;
    cout << "Ingrese fecha (dd/mm/aaaa): ";
    cin >> fecha;
}

    int leerNumeroCuenta() {
    int numeroCuenta;
    cout << "Ingrese número de cuenta: ";
    cin >> numeroCuenta;
    return numeroCuenta;
}

    void abrirCuenta(Usuario* usuario) {
    int numeroCuenta = leerNumeroCuenta();
    double saldoInicial;
    cout << "Ingrese saldo inicial: ";
    cin >> saldoInicial;
    Cuenta* nuevaCuenta = new Cuenta(numeroCuenta, saldoInicial);
    usuario->agregarCuenta(nuevaCuenta);
    cout << "Cuenta creada exitosamente.\n";
}

    void mostrarCuentas() const {
        cout << "Cuentas de " << nombre << ":\n";
        for (const auto& cuenta : cuentas) {
            cout << "Cuenta No. " << cuenta->numeroCuenta << " con saldo $" << cuenta->obtenerSaldo() << endl;
        }
    }
};

// Clase para manejar las pilas (operaciones deshacer)
template <typename T>
class Pila {
private:
    vector<Transaccion> transacciones;
public:
    void apilar(const Transaccion& transaccion) {
        transacciones.push_back(transaccion);
    }

    Transaccion desapilar() {
        if (!estaVacia()) {
            Transaccion t = transacciones.back();
            transacciones.pop_back();
            return t;
        }
        throw runtime_error("Pila vacía");
    }

    bool estaVacia() const {
        return transacciones.empty();
    }

    Transaccion verUltima() const {
        if (!estaVacia()) {
            return transacciones.back();
        }
        throw runtime_error("Pila vacía");
    }
};

// Clase para manejar las colas (transacciones pendientes)
template <typename T>  // Esto hace que la clase sea una plantilla para cualquier tipo T
class Cola {
private:
    struct Nodo {
        T dato;
        Nodo* siguiente;

        Nodo(const T& dato) : dato(dato), siguiente(nullptr) {}
    };

    Nodo* frente;
    Nodo* final;

public:
    Cola() : frente(nullptr), final(nullptr) {}

    ~Cola() {
        while (frente != nullptr) {
            Nodo* temp = frente;
            frente = frente->siguiente;
            delete temp;
        }
    }

    void encolar(const T& dato) {
        Nodo* nuevo = new Nodo(dato);
        if (final != nullptr) {
            final->siguiente = nuevo;
        } else {
            frente = nuevo;
        }
        final = nuevo;
    }

    T desencolar() {
        if (frente != nullptr) {
            Nodo* temp = frente;
            T dato = frente->dato;
            frente = frente->siguiente;
            if (frente == nullptr) {
                final = nullptr;
            }
            delete temp;
            return dato;
        }
        throw underflow_error("Cola vacía");
    }

    bool estaVacia() const {
        return frente == nullptr;
    }
};

// Clase Lista estática para manejar cuentas de usuarios
template <typename T>
class ListaEstatica {
private:
    T* elementos;
    int capacidad;
    int tamano;

public:
    ListaEstatica(int capacidad) : capacidad(capacidad), tamano(0) {
        elementos = new T[capacidad];
    }

    ~ListaEstatica() {
        delete[] elementos;
    }

    void agregar(const T& elemento) {
        if (tamano < capacidad) {
            elementos[tamano++] = elemento;
        } else {
            cout << "Lista estática llena.\n";
        }
    }

    T obtener(int indice) const {
        if (indice >= 0 && indice < tamano) {
            return elementos[indice];
        }
        throw out_of_range("Índice fuera de rango");
    }

    int getTamano() const {
        return tamano;
    }
};

// Clase Grafo (esquemática para representar la relación entre cuentas o usuarios)
class Grafo {
public:
    map<int, vector<int>> adjList;  // Representación de un grafo como lista de adyacencia

    // Agregar una relación entre dos cuentas (ej., deudas o transferencias)
    void agregarRelacion(int cuentaOrigen, int cuentaDestino) {
        adjList[cuentaOrigen].push_back(cuentaDestino);
    }

    // Mostrar todas las relaciones de un usuario
    void mostrarRelaciones(int cuenta) {
        if (adjList.find(cuenta) != adjList.end()) {
            cout << "Relaciones para la cuenta " << cuenta << ":\n";
            for (int destino : adjList[cuenta]) {
                cout << "- Cuenta " << destino << endl;
            }
        } else {
            cout << "No hay relaciones para la cuenta " << cuenta << ".\n";
        }
    }
};

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
    sort(cuentas.begin(), cuentas.end(), [](Cuenta* a, Cuenta* b) { //algoritmo de ordenación
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
        if (!intercambio) break;  // Si no hubo intercambio, la lista ya está ordenada
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
        // Intercambiar la cuenta mínima con la primera no ordenada
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
        Transaccion t = pila.desapilar();  // Obtener la última transacción
        // Deshacer la operación según el tipo de transacción (deposición, retiro, etc.)
        cout << "Operación deshecha: " << t.tipo << " de $" << t.monto << endl;
    } else {
        cout << "No hay operaciones recientes para deshacer.\n";
    }
}
void mostrarMenu(){
    cout << "\n=== Menú Bancario ===\n";
    cout << "1. Abrir cuenta\n";
    cout << "2. Realizar depósito\n";
    cout << "3. Realizar retiro\n";
    cout << "4. Transferir dinero\n";
    cout << "5. Ver historial de transacciones\n";
    cout << "6. Deshacer última operación\n";
    cout << "7. Ver relaciones entre cuentas\n";
    cout << "8. Salir\n";
    cout << "Seleccione una opción: ";
}

// Función principal
int main() {
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
                cout << "Ingrese número de cuenta: ";
                cin >> numeroCuenta;
                cout << "Ingrese saldo inicial: ";
                cin >> saldoInicial;
                Cuenta* nuevaCuenta = new Cuenta(numeroCuenta, saldoInicial);
                usuario->agregarCuenta(nuevaCuenta);
                cout << "Cuenta creada exitosamente.\n";
                break;
            }
            case 2: {
                // Realizar depósito
                int numeroCuenta;
                double monto;
                string fecha;
                cout << "Ingrese número de cuenta: ";
                cin >> numeroCuenta;
                cout << "Ingrese monto a depositar: ";
                cin >> monto;
                cout << "Ingrese fecha (dd/mm/aaaa): ";
                cin >> fecha;
                Cuenta* cuenta = buscarCuentaBinaria(usuario->cuentas, numeroCuenta);
                if (cuenta != nullptr) {
                    cuenta->depositar(monto, fecha);
                    cout << "Depósito realizado.\n";
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
                cout << "Ingrese número de cuenta: ";
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
                cout << "Ingrese número de cuenta origen: ";
                cin >> numeroCuentaOrigen;
                cout << "Ingrese número de cuenta destino: ";
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
    // Deshacer la última operación (última transacción)
    if (!pilaTransacciones.estaVacia()) {
        Transaccion ultimaTransaccion = pilaTransacciones.desapilar(); // Desapilamos la última transacción

        // Lógica para deshacer la operación
        if (ultimaTransaccion.tipo == "Deposito") {
            Cuenta* cuenta = buscarCuentaLineal(usuario->cuentas, ultimaTransaccion.numeroCuenta);
            if (cuenta) {
                cuenta->saldo -= ultimaTransaccion.monto;  // Deshacer el depósito
                cout << "Depósito de $" << ultimaTransaccion.monto << " deshecho en cuenta " << ultimaTransaccion.numeroCuenta << ".\n";
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
    cout << "Ingrese el número de cuenta para ver sus relaciones: ";
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
                cout << "Opción no válida.\n";
        }
    }
}

