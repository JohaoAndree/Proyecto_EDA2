#ifndef AVL_H
#define AVL_H

#include <string>
#include "../Persona.h"
#define MAX_ZONAS 100
using namespace std;

struct ZonaAcceso {
    int zona;
    int cantidad;
};

struct NodoAVL {
    string hora; // clave principal del árbol
    ZonaAcceso accesosPorZona[MAX_ZONAS]; // zona -> cantidad de accesos
    int numZonas;
    int altura;
    NodoAVL* izq;
    NodoAVL* der;

    NodoAVL(const string& h) : hora(h), numZonas(0), altura(1), izq(nullptr), der(nullptr) {}
};

class AVLTree {
private:
    NodoAVL* raiz;

    NodoAVL* insertar(NodoAVL* nodo, const string& hora, int zona);
    void inOrder(NodoAVL* nodo);
    int altura(NodoAVL* nodo);
    int balance(NodoAVL* nodo);
    NodoAVL* rotarIzq(NodoAVL* x);
    NodoAVL* rotarDer(NodoAVL* y);
    void mostrarRango(NodoAVL* nodo, const string& desde, const string& hasta);
    void acumularZonas(NodoAVL* nodo, int* totalZonas, int& maxZona, int& maxCantidad);
    void agregarZona(NodoAVL* nodo, int zona);

public:
    AVLTree();

    void insertar(const string& hora, int zona); // insertar acceso (hora y zona)
    void mostrar();                              // mostrar todos los accesos por hora
    void consultarRango(const string& desde, const string& hasta); // por franja horaria
    int zonaConMasAccesos();                     // zona con más accesos globalmente
};

#endif

