# Sistema Bancario - Simulador en C++

Realizado por 
- Angel Alberto Campos Pelayo
- Antonio Viña Hernández

Este proyecto es un simulador de sistema bancario diseñado para aplicar conceptos de estructuras de datos en C++. El simulador permite a los usuarios realizar operaciones financieras básicas, como abrir cuentas, realizar depósitos y retiros, transferencias, y consultar su saldo e historial de transacciones. Es ideal para practicar y comprender estructuras de datos fundamentales, tales como listas, pilas, colas y grafos.

## Funcionalidades Principales

1. **Creación de Usuarios y Cuentas**: Permite crear usuarios con múltiples cuentas bancarias.
2. **Transacciones Financieras**: Soporta depósitos, retiros, transferencias y consultas de saldo.
3. **Historial de Transacciones**: Almacena el historial de transacciones para cada cuenta.
4. **Almacenamiento en Archivos**: Guarda los datos de usuarios y transacciones en archivos para persistencia.
5. **Deshacer/Rehacer Transacciones**: Usa una pila para permitir operaciones de deshacer y rehacer transacciones.

## Estructura del Proyecto

### 1. Contenedores de Datos
   - **Implementación**: Utiliza `struct` o `class` para definir datos estructurados como `Usuario`, `Cuenta`, y `Transacción`.
   - **Anidaciones**: Cada `Usuario` contiene un `vector<Cuenta>` para almacenar múltiples cuentas.

### 2. Tipos Abstractos de Datos (TAD)
   - **TDA Lista, TDA Pila, TDA Cola** (implementación estática): 
      - Lista estática para manejar cuentas y transacciones, con operaciones básicas (agregar, eliminar, buscar).
      - Pila para deshacer operaciones recientes.
      - Cola para gestionar operaciones pendientes o en espera.

### 3. Almacenamiento Secundario
   - Usa archivos para guardar la información de usuarios y transacciones. Al iniciar el programa, carga la información desde un archivo y guarda cambios al finalizar.

### 4. Búsquedas
   - **Búsqueda Lineal**: Para encontrar cuentas o usuarios en la lista.
   - **Búsqueda Binaria**: Mejora la eficiencia de búsqueda en listas ordenadas, por ejemplo, por número de cuenta.

### 5. Ordenamiento
   - Implementa algoritmos de ordenamiento iterativos y recursivos, como burbuja o selección, para ordenar cuentas o transacciones por diferentes campos (fecha, monto, etc.).

### 6. Memoria Dinámica
   - Utiliza `new` y `delete` para asignar memoria a listas, pilas y colas dinámicas que manejan usuarios y cuentas.

### 7. Apuntadores en Estructuras de Datos
   - Emplea apuntadores para gestionar estructuras de datos dinámicas como listas enlazadas, pilas y colas.

### 8. Tipos Abstractos de Datos (TAD) Dinámicos
   - **Listas Dinámicas**: Implementa una lista enlazada para gestionar cuentas y transacciones de forma dinámica.
   - **Pila Dinámica**: Usa nodos enlazados para una pila que crezca según sea necesario.
   - **Cola Dinámica**: Utiliza una cola enlazada para manejar transacciones eficientemente.

### 9. Grafos
   - Representa relaciones entre usuarios y cuentas, o entre transacciones, usando un grafo. Se puede usar una lista o matriz de adyacencia.

## Ejemplo de Flujo de Operaciones

1. **Crear un Usuario**: Almacena en una lista dinámica.
2. **Crear Cuentas**: Cada cuenta se almacena en un `vector` dentro del usuario.
3. **Realizar Transacciones**: Usa pilas para gestionar operaciones de deshacer/rehacer.
4. **Consultar Saldo**: Implementa búsquedas en la lista de cuentas.
5. **Almacenar Datos en Archivos**: Guarda toda la información al cerrar el programa.

## Archivos del Proyecto

- **main.cpp**: Archivo principal con el código fuente del programa.
- **Makefile.win** (opcional): Para compilar el proyecto en entornos Windows.
- **Project1.dev** (opcional): Archivo de proyecto de Dev-C++.
