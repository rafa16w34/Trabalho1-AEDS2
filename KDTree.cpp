#include <iostream>
#include <vector>
#include <cmath>
#include <limits>
#include <fstream>

using namespace std;

struct Ponto{

    double x,y;

    Ponto(double _x = 0, double _y = 0) : x(_x), y(_y) {}

    bool operator == (const Ponto& outro) const{
        return x == outro.x && y == outro.y;
    }

};

struct KDNode{

    Ponto ponto;
    KDNode* esquerda;
    KDNode* direita;

    KDNode(Ponto p) : ponto(p), esquerda(nullptr), direita(nullptr){}

};

class KDTree
{
private:
    /* data */
public:
    KDTree(/* args */);
    ~KDTree();
};

KDTree::KDTree(/* args */)
{
}

KDTree::~KDTree()
{
}
