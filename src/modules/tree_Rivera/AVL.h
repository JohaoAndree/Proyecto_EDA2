#ifndef AVL_H
#define AVL_H

#include <string>
#include <map>
#include "../Persona.h"
using namespace std;

struct NodoAVL {
    string hora; // clave principal del árbol
    map<int, int> accesosPorZona; // zona -> cantidad de accesos
    int altura;
    NodoAVL* izq;
    NodoAVL* der;

    NodoAVL(const string& h) : hora(h), altura(1), izq(nullptr), der(nullptr) {}
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
    void acumularZonas(NodoAVL* nodo, map<int, int>& totalZonas);

public:
    AVLTree();

    void insertar(const string& hora, int zona); // insertar acceso (hora y zona)
    void mostrar();                              // mostrar todos los accesos por hora
    void consultarRango(const string& desde, const string& hasta); // por franja horaria
    int zonaConMasAccesos();                     // zona con más accesos globalmente
};

#endif

