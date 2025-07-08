#ifndef HEAP_H
#define HEAP_H

#include <vector>
#include <unordered_map>
#include "../Persona.h"

class MaxHeap {
private:
    std::vector<Persona> datos;
    std::unordered_map<int, int> mapaDniPos;  // DNI -> índice en el heap

    void subir(int idx);
    void bajar(int idx);
    void swap(int a, int b);

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
