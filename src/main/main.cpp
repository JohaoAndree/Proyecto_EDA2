#include <iostream>
#include <string>
#include <fstream>
#include <vector>
#include "../modules/tree_Rivera/AVL.h"
#include "../modules/heap_JeanPaul/Heap.h"
#include "../modules/hash_Johao/hashTable.h"

using namespace std;

struct Persona {
    string dni;
    string nombre;
    int edad;
    int zona;
    string hora;
    int prioridad;
};

// Instancias globales de las estructuras
AVLTree avl;
MaxHeap heap;
HashTable tablaHash(1009);

// Función para registrar acceso manual
void registrarAccesoManual() {
    Persona p;
    cout << "DNI: "; cin >> p.dni;
    cout << "Nombre: "; cin >> p.nombre;
    cout << "Edad: "; cin >> p.edad;
    cout << "Zona: "; cin >> p.zona;
    cout << "Hora (HH:MM): "; cin >> p.hora;
    cout << "Prioridad (1-100): "; cin >> p.prioridad;
    // Insertar en HashTable (DNI como int, hora como string)
    tablaHash.insertar(stoi(p.dni), p.nombre, p.edad, p.hora);
    // Insertar en Heap
    heap.insertar(p);
    // Insertar en AVL
    avl.insertar(p.hora, p.zona);
    cout << "Acceso registrado exitosamente.\n";
}

// Cargar accesos desde archivo
void cargarAccesosArchivo() {
    string archivo;
    cout << "Nombre del archivo: ";
    cin >> archivo;
    ifstream fin(archivo);
    if (!fin) {
        cout << "No se pudo abrir el archivo.\n";
        return;
    }
    Persona p;
    while (fin >> p.dni >> p.nombre >> p.edad >> p.zona >> p.hora >> p.prioridad) {
        tablaHash.insertar(stoi(p.dni), p.nombre, p.edad, p.hora);
        heap.insertar(p);
        avl.insertar(p.hora, p.zona); // AVL solo usa hora y zona
    }
    cout << "Carga masiva completada.\n";
}

// Buscar persona por DNI
void buscarPorDNI() {
    string dni;
    cout << "Ingrese DNI: ";
    cin >> dni;
    auto datos = tablaHash.buscar(stoi(dni));
    if (datos) {
        cout << "Nombre: " << datos->nombre << ", Edad: " << datos->edad << ", Hora: " << datos->horaCompra << endl;
    } else {
        cout << "No encontrado.\n";
    }
}

// Validar existencia por DNI
void validarExistenciaDNI() {
    string dni;
    cout << "Ingrese DNI: ";
    cin >> dni;
    if (tablaHash.buscar(stoi(dni)))
        cout << "La persona existe en el sistema.\n";
    else
        cout << "No existe.\n";
}

// Mostrar top 5 prioridades (Heap)
void mostrarTop5Prioridades() {
    heap.mostrarTop5();
}

// Extraer acceso de mayor prioridad (Heap)
void extraerMayorPrioridad() {
    Persona p = heap.extraerMax();
    cout << "Extraído: " << p.dni << " - " << p.nombre << " (Prioridad: " << p.prioridad << ")\n";
}

// Consultar accesos por zona y hora (AVL)
void consultarAccesosPorZonaHora() {
    string desde, hasta;
    cout << "Desde hora (HH:MM): "; cin >> desde;
    cout << "Hasta hora (HH:MM): "; cin >> hasta;
    avl.consultarRango(desde, hasta);
}

// Mostrar zona con más accesos (AVL)
void mostrarZonaMasAccesos() {
    int zona = avl.zonaConMasAccesos();
    cout << "Zona con más accesos: " << zona << endl;
}

int main() {
    int opcion;
    do {
        cout << "\n--- SISTEMA DE CONTROL DE ACCESOS ---\n";
        cout << "1. Registrar acceso manual (individual)\n";
        cout << "2. Cargar accesos desde archivo (masivo)\n";
        cout << "3. Buscar persona por DNI (Hash)\n";
        cout << "4. Validar existencia por DNI (Hash)\n";
        cout << "5. Mostrar top 5 prioridades (Heap)\n";
        cout << "6. Extraer acceso de mayor prioridad (Heap)\n";
        cout << "7. Consultar accesos por zona y hora (AVL)\n";
        cout << "8. Mostrar zona con más accesos (AVL)\n";
        cout << "9. Consultar accesos por rango de horas (AVL)\n";
        cout << "0. Salir\n";
        cout << "Seleccione opción: ";
        cin >> opcion;
        switch (opcion) {
            case 1: registrarAccesoManual(); break;
            case 2: cargarAccesosArchivo(); break;
            case 3: buscarPorDNI(); break;
            case 4: validarExistenciaDNI(); break;
            case 5: mostrarTop5Prioridades(); break;
            case 6: extraerMayorPrioridad(); break;
            case 7: consultarAccesosPorZonaHora(); break;
            case 8: mostrarZonaMasAccesos(); break;
            case 9: {
                string desde, hasta;
                cout << "Desde (HH:MM): ";
                cin >> desde;
                cout << "Hasta (HH:MM): ";
                cin >> hasta;
                avl.consultarRango(desde, hasta);
                break;
            }
            case 0: cout << "Saliendo...\n"; break;
            default: cout << "Opción inválida.\n";
        }
    } while (opcion != 0);
    return 0;
}
