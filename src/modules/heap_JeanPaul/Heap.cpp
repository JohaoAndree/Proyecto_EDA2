#include "Heap.h"
#include "Utils.h"
#include <iostream>
using namespace std;

MaxHeap::MaxHeap() {}

void MaxHeap::swap(int a, int b) {
    std::swap(datos[a], datos[b]);
    mapaDniPos[datos[a].dni] = a;
    mapaDniPos[datos[b].dni] = b;
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
    int n = datos.size();
    while (true) {
        int izq = 2 * idx + 1, der = 2 * idx + 2, mayor = idx;
        if (izq < n && datos[izq].prioridad > datos[mayor].prioridad) mayor = izq;
        if (der < n && datos[der].prioridad > datos[mayor].prioridad) mayor = der;
        if (mayor == idx) break;
        swap(idx, mayor);
        idx = mayor;
    }
}

bool MaxHeap::existe(int dni) {
    return mapaDniPos.count(dni);
}

void MaxHeap::insertar(const Persona& p) {
    if (existe(p.dni)) {
        cout << "⚠️  DNI duplicado, ya fue registrado.\n";
        return;
    }
    datos.push_back(p);
    int idx = datos.size() - 1;
    mapaDniPos[p.dni] = idx;
    subir(idx);
}

Persona MaxHeap::extraerMax() {
    if (datos.empty()) return Persona();

    Persona max = datos[0];
    mapaDniPos.erase(max.dni);

    datos[0] = datos.back();
    mapaDniPos[datos[0].dni] = 0;
    datos.pop_back();

    bajar(0);
    return max;
}

bool MaxHeap::actualizarPrioridad(int dni, int nueva) {
    if (!existe(dni)) return false;

    int idx = mapaDniPos[dni];
    int anterior = datos[idx].prioridad;
    datos[idx].prioridad = nueva;

    if (nueva > anterior) subir(idx);
    else bajar(idx);

    return true;
}

void MaxHeap::mostrarTop5() {
    cout << "Top 5 prioridades:\n";
    int n = datos.size();
    for (int i = 0; i < 5 && i < n; i++) {
        cout << i + 1 << ". " << datos[i].nombre
             << " | DNI: " << datos[i].dni
             << " | Prioridad: " << etiquetaPrioridad(datos[i].prioridad)
             << " | Zona: " << datos[i].zona << "\n";
    }
}

int MaxHeap::tamano() const {
    return datos.size();
}
