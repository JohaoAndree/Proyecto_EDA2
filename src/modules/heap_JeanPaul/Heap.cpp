#include "Heap.h"
#include <iostream>
using namespace std;

MaxHeap::MaxHeap() {
    size = 0;
    mapaSize = 0;
}

int MaxHeap::buscarDni(int dni) {
    for (int i = 0; i < mapaSize; ++i) {
        if (mapaDniPos[i].dni == dni)
            return i;
    }
    return -1;
}

void MaxHeap::swap(int a, int b) {
    Persona tmp = datos[a];
    datos[a] = datos[b];
    datos[b] = tmp;
    // Actualizar posiciones en el mapa
    for (int i = 0; i < mapaSize; ++i) {
        if (mapaDniPos[i].dni == stoi(datos[a].dni)) mapaDniPos[i].pos = a;
        if (mapaDniPos[i].dni == stoi(datos[b].dni)) mapaDniPos[i].pos = b;
    }
}

void MaxHeap::subir(int idx) {
    while (idx > 0) {
        int padre = (idx - 1) / 2;
        if (datos[idx].prioridad <= datos[padre].prioridad) break;
        swap(idx, padre);
        idx = padre;
    }
}

void MaxHeap::bajar(int idx) {
    while (true) {
        int izq = 2 * idx + 1, der = 2 * idx + 2, mayor = idx;
        if (izq < size && datos[izq].prioridad > datos[mayor].prioridad) mayor = izq;
        if (der < size && datos[der].prioridad > datos[mayor].prioridad) mayor = der;
        if (mayor == idx) break;
        swap(idx, mayor);
        idx = mayor;
    }
}

bool MaxHeap::existe(int dni) {
    return buscarDni(dni) != -1;
}

void MaxHeap::insertar(const Persona& p) {
    if (size >= HEAP_MAX) {
        cout << "Heap lleno.\n";
        return;
    }
    if (existe(stoi(p.dni))) {
        cout << "⚠️  DNI duplicado, ya fue registrado.\n";
        return;
    }
    datos[size] = p;
    mapaDniPos[mapaSize].dni = stoi(p.dni);
    mapaDniPos[mapaSize].pos = size;
    ++mapaSize;
    subir(size);
    ++size;
}

Persona MaxHeap::extraerMax() {
    if (size == 0) return Persona();
    Persona max = datos[0];
    // Eliminar del mapa
    int idx = buscarDni(stoi(max.dni));
    if (idx != -1) {
        for (int i = idx; i < mapaSize - 1; ++i)
            mapaDniPos[i] = mapaDniPos[i + 1];
        --mapaSize;
    }
    datos[0] = datos[size - 1];
    // Actualizar mapa para el nuevo root
    int nuevoDni = stoi(datos[0].dni);
    int idxNuevo = buscarDni(nuevoDni);
    if (idxNuevo != -1) mapaDniPos[idxNuevo].pos = 0;
    --size;
    bajar(0);
    return max;
}

void MaxHeap::mostrarTop5() {
    int n = (size < 5) ? size : 5;
    cout << "Top " << n << " prioridades:\n";
    for (int i = 0; i < n; ++i) {
        cout << datos[i].dni << " - " << datos[i].nombre << " (Prioridad: " << datos[i].prioridad << ")\n";
    }
}

bool MaxHeap::actualizarPrioridad(int dni, int nueva) {
    int idx = buscarDni(dni);
    if (idx == -1) return false;
    int pos = mapaDniPos[idx].pos;
    datos[pos].prioridad = nueva;
    subir(pos);
    bajar(pos);
    return true;
}

int MaxHeap::tamano() const {
    return size;
}
