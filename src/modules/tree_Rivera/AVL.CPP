#include "avl.h"
#include <iostream>
#include <algorithm>
using namespace std;

AVLTree::AVLTree() {
    raiz = nullptr;
}

int AVLTree::altura(NodoAVL* nodo) {
    return nodo ? nodo->altura : 0;
}

int AVLTree::balance(NodoAVL* nodo) {
    return nodo ? altura(nodo->izq) - altura(nodo->der) : 0;
}

NodoAVL* AVLTree::rotarDer(NodoAVL* y) {
    NodoAVL* x = y->izq;
    NodoAVL* T2 = x->der;
    x->der = y;
    y->izq = T2;
    y->altura = max(altura(y->izq), altura(y->der)) + 1;
    x->altura = max(altura(x->izq), altura(x->der)) + 1;
    return x;
}

NodoAVL* AVLTree::rotarIzq(NodoAVL* x) {
    NodoAVL* y = x->der;
    NodoAVL* T2 = y->izq;
    y->izq = x;
    x->der = T2;
    x->altura = max(altura(x->izq), altura(x->der)) + 1;
    y->altura = max(altura(y->izq), altura(y->der)) + 1;
    return y;
}

// Inserción por hora y zona
NodoAVL* AVLTree::insertar(NodoAVL* nodo, const string& hora, int zona) {
    if (!nodo) {
        NodoAVL* nuevo = new NodoAVL(hora);
        nuevo->accesosPorZona[zona] = 1;
        return nuevo;
    }

    if (hora < nodo->hora)
        nodo->izq = insertar(nodo->izq, hora, zona);
    else if (hora > nodo->hora)
        nodo->der = insertar(nodo->der, hora, zona);
    else {
        // Si la hora ya existe, se incrementa el acceso a esa zona
        nodo->accesosPorZona[zona]++;
        return nodo;
    }

    // Balanceo AVL
    nodo->altura = 1 + max(altura(nodo->izq), altura(nodo->der));
    int b = balance(nodo);

    if (b > 1 && hora < nodo->izq->hora)
        return rotarDer(nodo);
    if (b < -1 && hora > nodo->der->hora)
        return rotarIzq(nodo);
    if (b > 1 && hora > nodo->izq->hora) {
        nodo->izq = rotarIzq(nodo->izq);
        return rotarDer(nodo);
    }
    if (b < -1 && hora < nodo->der->hora) {
        nodo->der = rotarDer(nodo->der);
        return rotarIzq(nodo);
    }

    return nodo;
}

void AVLTree::insertar(const string& hora, int zona) {
    raiz = insertar(raiz, hora, zona);
}

// Inorder para mostrar accesos por hora
void AVLTree::inOrder(NodoAVL* nodo) {
    if (!nodo) return;
    inOrder(nodo->izq);
    cout << "Hora " << nodo->hora << ":\n";
    for (auto& [zona, cantidad] : nodo->accesosPorZona) {
        cout << "  Zona " << zona << ": " << cantidad << " accesos\n";
    }
    inOrder(nodo->der);
}

void AVLTree::mostrar() {
    inOrder(raiz);
}

// Consultas por franja horaria
void AVLTree::mostrarRango(NodoAVL* nodo, const string& desde, const string& hasta) {
    if (!nodo) return;
    if (desde < nodo->hora)
        mostrarRango(nodo->izq, desde, hasta);

    if (desde <= nodo->hora && nodo->hora <= hasta) {
        cout << "Hora " << nodo->hora << ":\n";
        for (auto& [zona, cantidad] : nodo->accesosPorZona) {
            cout << "  Zona " << zona << ": " << cantidad << " accesos\n";
        }
    }

    if (hasta > nodo->hora)
        mostrarRango(nodo->der, desde, hasta);
}

void AVLTree::consultarRango(const string& desde, const string& hasta) {
    mostrarRango(raiz, desde, hasta);
}

// Recorre todo el árbol y acumula accesos por zona
void AVLTree::acumularZonas(NodoAVL* nodo, map<int, int>& totalZonas) {
    if (!nodo) return;
    for (auto& [zona, cantidad] : nodo->accesosPorZona) {
        totalZonas[zona] += cantidad;
    }
    acumularZonas(nodo->izq, totalZonas);
    acumularZonas(nodo->der, totalZonas);
}

int AVLTree::zonaConMasAccesos() {
    map<int, int> totalZonas;
    acumularZonas(raiz, totalZonas);

    int zonaMax = -1;
    int maxAcc = 0;

    for (auto& [zona, cantidad] : totalZonas) {
        if (cantidad > maxAcc) {
            maxAcc = cantidad;
            zonaMax = zona;
        }
    }

    return zonaMax;
}
