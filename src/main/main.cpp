#include <iostream>
#include <string>
#include <fstream>
#include "../modules/tree_Rivera/AVL.h"
#include "../modules/heap_JeanPaul/Heap.h"
#include "../modules/hash_Johao/hashTable.h"
#include "../modules/Persona.h" 

using namespace std;

AVLTree avl;
MaxHeap heap;
HashTable tablaHash(1009);

//aca se registra el acceso de las personas de manera manual
void registrarAccesoManual() {
    Persona p;
    cout << "DNI: "; cin >> p.dni;
    cout << "Nombre: "; cin >> p.nombre;
    cout << "Edad: "; cin >> p.edad;
    cout << "Zona: "; cin >> p.zona;
    cout << "Hora (HH:MM): "; cin >> p.hora;
    cout << "Prioridad (1-100): "; cin >> p.prioridad;

    //Se inserta tabla hash
    tablaHash.insertar(stoi(p.dni), p.nombre, p.edad, p.hora);
    // se inserta en el heap
    heap.insertar(p);


    //Insertar en AVL
    avl.insertar(p.hora, p.zona);
    cout << "Acceso registrado exitosamente.\n";
}

//Se hace la carga del archivo de usuarios masivos de manera automatica
void cargarAccesosArchivo() {
    string archivo = "c:/Users/Bruno Tupia Canales/Desktop/PROGRA/Proyecto_EDA2/src/main/usuarios_masivos.txt";
    ifstream fin(archivo);
    if (!fin) {
        cout << "No se pudo abrir el archivo masivo en la ruta predefinida.\n";
        return;
    }
    Persona p;

    while (fin >> p.dni >> p.nombre >> p.edad >> p.zona >> p.hora >> p.prioridad) {
        tablaHash.insertar(stoi(p.dni), p.nombre, p.edad, p.hora);
        heap.insertar(p);
        avl.insertar(p.hora, p.zona);
    }

    cout << "Carga masiva completada desde archivo predefinido.\n";


}

//Opcion 3: Buscar persona por DNI
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

//Opcion 4: validar existencia por DNI
void validarExistenciaDNI() {
    string dni;
    cout << "Ingrese DNI: ";
    cin >> dni;
    if (tablaHash.buscar(stoi(dni)))
        cout << "La persona existe en el sistema.\n";
    else
        cout << "No existe.\n";
}

// OPcion 5: Mostrar top 5 prioridades
void mostrarTop5Prioridades() {
    heap.mostrarTop5() ;
}


//opcion 6: Extraer acceso de mayor prioridad
void extraerMayorPrioridad() {
    Persona p = heap.extraerMax();
    cout << "Extraido: " << p.dni << " - " << p.nombre << " (Prioridad: " << p.prioridad << ")\n";
}

// OPcion 7. Consultar accesos por zona y hora (AVL)
void consultarAccesosPorZonaHora() {

    string desde, hasta;
    cout << "Desde hora (HH:MM): "; cin >> desde;
    
    cout << "Hasta hora (HH:MM): "; cin >> hasta;
    avl.consultarRango(desde, hasta);
}

//Opcion 8: Mostrar zona con más accesos (AVL)
void mostrarZonaMasAccesos() {
    int zona = avl.zonaConMasAccesos();

    cout << "Zona con mas accesos: " << zona << endl;
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
        cout << "8. Mostrar zona con mas accesos (AVL)\n";
        cout << "0. Salir\n";
        cout << "Seleccione opcion: ";
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
            case 0: cout << "Saliendo...\n"; break;
            default: cout << "Opcion invalida.\n";
        }

    } while (opcion != 0);

    return 0;

}
