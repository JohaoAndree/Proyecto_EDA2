#include "HashTable.h"
#include <iostream>

using namespace std;

int main() {
    HashTable tabla(4); // Pequeña a propósito para forzar rehash

    tabla.insertar(12345678);
    tabla.insertar(23456789);
    tabla.insertar(-111111);
    tabla.insertar(20202020); // aquí debe hacer rehash
    tabla.insertar(99999999);

    tabla.mostrar();

    cout << ">>> Rehashing completado. Nueva capacidad: " << capacidad << endl;

    return 0;
}
