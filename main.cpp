#include <iostream>
#include <iomanip>
#include <limits>
#include <string>
using namespace std;

const int N = 4;
int cuentas[N] = {1001, 1002, 1003, 1004};
int pines[N] = {1234, 2345, 3456, 4567};
double saldos[N] = {1500.00, 2000.00, 3500.00, 800.00};
bool bloqueadas[N] = {false, false, false, false};

int leerEntero(const string& mensaje) {
    int valor;
    while (true) {
        cout << mensaje;
        if (cin >> valor) return valor;
        cout << "Entrada invalida. Ingrese un numero entero.\n";
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
    }
}

double leerMonto(const string& mensaje) {
    double monto;
    while (true) {
        cout << mensaje;
        if (cin >> monto && monto > 0 && monto <= 1000000000.0) return monto;
        cout << "Monto invalido. Ingrese una cantidad positiva valida.\n";
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
    }
}

int buscarCuenta(int numero) {
    for (int i = 0; i < N; i++) {
        if (cuentas[i] == numero) return i;
    }
    return -1;
}

void mostrarMensajeBloqueo() {
    cout << "\n========================================\n";
    cout << "           CUENTA BLOQUEADA\n";
    cout << "========================================\n";
    cout << "Ha superado el numero maximo de intentos permitidos\n";
    cout << "o esta cuenta ya se encuentra bloqueada.\n";
    cout << "Por motivos de seguridad, no puede ingresar.\n";
    cout << "Para recuperar el acceso, comuniquese con su banco.\n";
    cout << "========================================\n";
}

int iniciarSesion() {
    cout << "\n========== INICIO DE SESION ==========\n";
    int numero = leerEntero("Numero de cuenta: ");
    int indice = buscarCuenta(numero);
    if (indice == -1) {
        cout << "Cuenta inexistente.\n";
        return -1;
    }
    if (bloqueadas[indice]) {
        mostrarMensajeBloqueo();
        return -1;
    }
    for (int intento = 1; intento <= 3; intento++) {
        int pin = leerEntero("Ingrese su PIN: ");
        if (pin == pines[indice]) {
            cout << "Acceso correcto. Bienvenido, cuenta " << numero << ".\n";
            return indice;
        }
        cout << "PIN incorrecto. Intentos restantes: " << 3 - intento << "\n";
    }
    bloqueadas[indice] = true;
    mostrarMensajeBloqueo();
    return -1;
}

void consultarSaldo(int indice) {
    cout << "\nCuenta: " << cuentas[indice]
         << "\nSaldo disponible: Q" << saldos[indice] << "\n";
}

void depositar(int indice) {
    double monto = leerMonto("Monto a depositar (Q): ");
    saldos[indice] += monto;
    cout << "Deposito exitoso. Nuevo saldo: Q" << saldos[indice] << "\n";
}

void retirar(int indice) {
    double monto = leerMonto("Monto a retirar (Q): ");
    if (monto > saldos[indice]) {
        cout << "Fondos insuficientes. Saldo: Q" << saldos[indice] << "\n";
        return;
    }
    saldos[indice] -= monto;
    cout << "Retiro exitoso. Nuevo saldo: Q" << saldos[indice] << "\n";
}

void transferir(int origen) {
    int destinoNumero = leerEntero("Numero de cuenta destino: ");
    int destino = buscarCuenta(destinoNumero);
    if (destino == -1) {
        cout << "La cuenta destino no existe.\n";
        return;
    }
    if (destino == origen) {
        cout << "No puede transferir a la misma cuenta.\n";
        return;
    }
    if (bloqueadas[destino]) {
        cout << "La cuenta destino esta bloqueada.\n";
        return;
    }
    double monto = leerMonto("Monto a transferir (Q): ");
    if (monto > saldos[origen]) {
        cout << "Fondos insuficientes para transferir.\n";
        return;
    }
    saldos[origen] -= monto;
    saldos[destino] += monto;
    cout << "Transferencia exitosa a la cuenta " << destinoNumero
         << ". Nuevo saldo: Q" << saldos[origen] << "\n";
}

void cambiarPIN(int indice) {
    cout << "\n========== CAMBIAR PIN ==========\n";
    int pinActual = leerEntero("Ingrese su PIN actual: ");
    if (pinActual != pines[indice]) {
        cout << "PIN actual incorrecto. No se realizaron cambios.\n";
        return;
    }
    int nuevoPIN = leerEntero("Ingrese su nuevo PIN de 4 digitos: ");
    if (nuevoPIN < 1000 || nuevoPIN > 9999) {
        cout << "Error: El PIN debe tener exactamente 4 digitos.\n";
        return;
    }
    if (nuevoPIN == pinActual) {
        cout << "El nuevo PIN debe ser diferente al actual.\n";
        return;
    }
    int confirmarPIN = leerEntero("Confirme su nuevo PIN: ");
    if (nuevoPIN != confirmarPIN) {
        cout << "Los PIN no coinciden. No se realizaron cambios.\n";
        return;
    }
    pines[indice] = nuevoPIN;
    cout << "PIN cambiado exitosamente.\n";
}

void reporteAdministrativo() {
    int mayor = 0;
    double total = 0;
    for (int i = 0; i < N; i++) {
        total += saldos[i];
        if (saldos[i] > saldos[mayor]) mayor = i;
    }
    cout << "\n========== REPORTE ADMINISTRATIVO ==========\n";
    cout << "Cuenta con mayor saldo: " << cuentas[mayor] << "\n";
    cout << "Saldo de esa cuenta: Q" << saldos[mayor] << "\n";
    cout << "Saldo total del banco: Q" << total << "\n";
}

void menu(int indice) {
    int opcion;
    do {
        cout << "\n========== CAJERO AUTOMATICO ==========\n"
             << "1. Consultar saldo\n"
             << "2. Depositar dinero\n"
             << "3. Retirar dinero\n"
             << "4. Transferir dinero\n"
             << "5. Reporte administrativo\n"
             << "6. Cambiar PIN\n"
             << "7. Cerrar sesion\n";
        opcion = leerEntero("Seleccione una opcion: ");
        switch (opcion) {
            case 1: consultarSaldo(indice); break;
            case 2: depositar(indice); break;
            case 3: retirar(indice); break;
            case 4: transferir(indice); break;
            case 5: reporteAdministrativo(); break;
            case 6: cambiarPIN(indice); break;
            case 7: cout << "Sesion cerrada.\n"; break;
            default: cout << "Opcion invalida. Intente nuevamente.\n";
        }
    } while (opcion != 7);
}

int main() {
    cout << fixed << setprecision(2);
    cout << "SIMULADOR DE CAJERO AUTOMATICO - PROYECTO C++\n";
    int opcion;
    do {
        cout << "\n1. Iniciar sesion\n2. Salir del programa\n";
        opcion = leerEntero("Seleccione una opcion: ");
        if (opcion == 1) {
            int indice = iniciarSesion();
            if (indice != -1) menu(indice);
        } else if (opcion == 2) {
            cout << "Gracias por utilizar el simulador.\n";
        } else {
            cout << "Opcion invalida.\n";
        }
    } while (opcion != 2);
    return 0;
}
