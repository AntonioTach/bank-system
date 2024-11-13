#include <iostream>
#include <vector>
#include <string>

// Angel Alberto Campos Pelayo 
// Antonio Viña Hernandez

using namespace std;


// Clase cuenta bancaria
class Cuenta {
	public: 
	int numeroCuenta;
	double saldo;
	// Contenedor dinamico STL - almacenar transacciones a la cuenta.
	//vector<Transaccion> historialTransacciones; // Historial Transacciones
	
	//Constructor de Inicializacion 
	Cuenta(int numeroCuenta, double saldoInicial) : numeroCuenta(numeroCuenta), saldo(saldoInicial) {
	}
	
	void depositar(double monto) {
	
	}
	
	void retirar(double monto) {
		
	}
	
	void mostrarHistorial() const {
		cout << "Historial de Transacciones para la cuenta " << numeroCuenta << ":\n";
	}
};

class Transaccion {
	
};

class Usuario {
	
};

int main(int argc, char** argv) {
	return 0;
}
