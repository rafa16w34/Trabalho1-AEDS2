#include<iostream>
#include<vector>

#include "KDTree.cpp"

int main() {
    // Instancia uma KD-Tree 2D (k = 2)
    KDTree arvore(2);

    // Inserindo pontos: (x, y)
    arvore.inserir({51.0, 75.0});
    arvore.inserir({25.0, 40.0});
    arvore.inserir({70.0, 70.0});
    arvore.inserir({10.0, 30.0});
    arvore.inserir({35.0, 90.0});

    cout << "--- Apos Insercoes ---" << endl;
    arvore.estatisticas();
    arvore.exportarDOT("kdtree_inicial.dot");

    // Teste de Busca por Vizinho Mais Proximo (NNS)
    vector<double> alvo = {30.0, 45.0};
    vector<double> maisProximo = arvore.vizinhosProximos(alvo);
    cout << "\nVizinho mais proximo de (30, 45): (" 
         << maisProximo[0] << ", " << maisProximo[1] << ")" << endl;

    // Teste de Remocao
    arvore.remover({25.0, 40.0});
    cout << "\n--- Apos Remover (25, 40) ---" << endl;
    arvore.estatisticas();
    arvore.exportarDOT("kdtree_pos_remocao.dot");

    return 0;
}