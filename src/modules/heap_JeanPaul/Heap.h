#ifndef HEAP_H
#define HEAP_H

#include "../Persona.h"

#define HEAP_MAX 1000
#define MAPA_MAX 1000

struct MapaDniPos {
    int dni;
    int pos;
};

class MaxHeap {
private:
    Persona datos[HEAP_MAX];
    int size;
    MapaDniPos mapaDniPos[MAPA_MAX]; // Array simple para mapear DNI a posición
    int mapaSize;

    void subir(int idx);
    void bajar(int idx);
    void swap(int a, int b);
    int buscarDni(int dni);

public:
    MaxHeap();
    void insertar(const Persona& p);
    Persona extraerMax();
    void mostrarTop5();
    bool actualizarPrioridad(int dni, int nueva);
    bool existe(int dni);
    int tamano() const;
};

#endif
