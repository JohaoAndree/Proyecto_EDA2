#include <iostream>
#include "HashTable.h"

using namespace std;

HashTable::HashTable(int tam) {
    capacidad = tam;
    cantidad = 0;
    tabla = new Entrada[capacidad];

    for (int i = 0; i < capacidad; i++) {
        tabla[i].ocupado = false;
    }
}

HashTable::~HashTable() {
    delete[] tabla;
}

int HashTable::funcionHash(int clave) {
    int hash = clave % capacidad;
    return (hash < 0) ? hash + capacidad : hash; // soporte para negativos
}

int HashTable::resolverColision(int clave) {
    int i = 1;
    int hash = funcionHash(clave);
    int indice = hash;

    while (i <= capacidad) {
        indice = (hash + i) % capacidad;
        if (!tabla[indice].ocupado) {
            return indice;
        }
        i++;
    }
    return -1; // tabla llena
}

void HashTable::rehash() {
    int nuevaCapacidad = capacidad * 2;
    Entrada* nuevaTabla = new Entrada[nuevaCapacidad];

    // Inicializamos la nueva tabla
    for (int i = 0; i < nuevaCapacidad; i++) {
        nuevaTabla[i].ocupado = false;
    }

    // Reinserta elementos antiguos
    for (int i = 0; i < capacidad; i++) {
        if (tabla[i].ocupado) {
            int clave = tabla[i].clave;
            int hash = clave % nuevaCapacidad;
            if (hash < 0) hash += nuevaCapacidad;

            int j = 0;
            int indice = hash;
            while (nuevaTabla[indice].ocupado && j < nuevaCapacidad) {
                indice = (hash + ++j) % nuevaCapacidad;
            }

            nuevaTabla[indice].clave = clave;
            nuevaTabla[indice].ocupado = true;
        }
    }

    // Reemplazar tabla
    delete[] tabla;
    tabla = nuevaTabla;
    capacidad = nuevaCapacidad;

    cout << ">>> Rehashing completado. Nueva capacidad: " << capacidad << endl;
}

bool HashTable::insertar(int clave) {
    if (buscar(clave)) {
        cout << "Error: La clave " << clave << " ya existe. No se puede insertar duplicado.\n";
        return false;
    }

    if (cantidad >= capacidad) {
        rehash(); // ahora duplicamos tamaño
    }

    int hash = funcionHash(clave);
    int indice = (!tabla[hash].ocupado) ? hash : resolverColision(clave);
    if (indice == -1) return false;

    tabla[indice].clave = clave;
    tabla[indice].ocupado = true;
    cantidad++;
    return true;
}

bool HashTable::buscar(int clave) {
    int hash = funcionHash(clave);

    int i = 0;
    int indice = hash;

    while (i < capacidad) {
        indice = (hash + i) % capacidad;

        if (!tabla[indice].ocupado) return false; // espacio vacío → no existe
        if (tabla[indice].clave == clave) return true;

        i++;
    }

    return false;
}

void HashTable::mostrar() {
    cout << "\nTabla Hash:\n";
    for (int i = 0; i < capacidad; i++) {
        if (tabla[i].ocupado)
            cout << "[" << i << "] → " << tabla[i].clave << endl;
        else
            cout << "[" << i << "] → (vacío)" << endl;
    }
}
