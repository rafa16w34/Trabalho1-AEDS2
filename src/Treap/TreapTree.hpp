#pragma once

#include <iostream>
#include <fstream>
#include <string>
#include <random>

using namespace std;

struct TreapNode {
    int chave;
    int prioridade;
    TreapNode* esquerda;
    TreapNode* direita;

    TreapNode(int k, int p) : chave(k), prioridade(p), esquerda(nullptr), direita(nullptr) {}
};

class Treap {
private:
    int nosAlocados;
    int totalCriados;
    int acessos;
    int rotacoes;

    TreapNode* raiz;
    mt19937 rng;

    TreapNode* rotacaoEsquerda(TreapNode* node) {
        TreapNode* x = node->direita;
        TreapNode* aux = x->esquerda;

        x->esquerda = node;
        node->direita = aux;

        rotacoes++;
        acessos += 2;

        return x;
    }

    TreapNode* rotacaoDireita(TreapNode* node) {
        TreapNode* x = node->esquerda;
        TreapNode* aux = x->direita;

        x->direita = node;
        node->esquerda = aux;

        rotacoes++;
        acessos += 2;

        return x;
    }

    TreapNode* inserirRecursivo(TreapNode* node, int chave, int prioridade) {
        acessos++;

        if (node == nullptr) {
            nosAlocados++;
            totalCriados++;
            return new TreapNode(chave, prioridade);
        }

        // Tratamento de chaves duplicadas: ignora inserção duplicada
        if (chave == node->chave) {
            return node;
        }

        if (chave < node->chave) {
            node->esquerda = inserirRecursivo(node->esquerda, chave, prioridade);
            if (node->esquerda->prioridade > node->prioridade) {
                node = rotacaoDireita(node);
            }
        } else {
            node->direita = inserirRecursivo(node->direita, chave, prioridade);
            if (node->direita->prioridade > node->prioridade) {
                node = rotacaoEsquerda(node);
            }
        }

        return node;
    }

    TreapNode* removerRecursivo(TreapNode* node, int chave, bool& removido) {
        if (node == nullptr) {
            removido = false;
            return nullptr;
        }

        acessos++;

        if (chave < node->chave) {
            node->esquerda = removerRecursivo(node->esquerda, chave, removido);
        } else if (chave > node->chave) {
            node->direita = removerRecursivo(node->direita, chave, removido);
        } else {
            removido = true;

            if (node->esquerda == nullptr) {
                TreapNode* aux = node->direita;
                delete node;
                nosAlocados--;
                return aux;
            } else if (node->direita == nullptr) {
                TreapNode* aux = node->esquerda;
                delete node;
                nosAlocados--;
                return aux;
            }

            if (node->esquerda->prioridade > node->direita->prioridade) {
                node = rotacaoDireita(node);
                node->direita = removerRecursivo(node->direita, chave, removido);
            } else {
                node = rotacaoEsquerda(node);
                node->esquerda = removerRecursivo(node->esquerda, chave, removido);
            }
        }

        return node;
    }

    void clear(TreapNode* node) {
        if (node != nullptr) {
            clear(node->esquerda);
            clear(node->direita);
            delete node;
            nosAlocados--;
        }
    }

    void gerarDotRecursivo(TreapNode* node, ofstream& arquivo) {
        if (node == nullptr) return;

        arquivo << "    node" << node << " [label=\"" << node->chave << "\\np=" << node->prioridade << "\"];\n";

        if (node->esquerda != nullptr) {
            arquivo << "    node" << node << " -> node" << node->esquerda << " [label=\"L\"];\n";
            gerarDotRecursivo(node->esquerda, arquivo);
        }
        if (node->direita != nullptr) {
            arquivo << "    node" << node << " -> node" << node->direita << " [label=\"R\"];\n";
            gerarDotRecursivo(node->direita, arquivo);
        }
    }

public:
    Treap() : nosAlocados(0), totalCriados(0), acessos(0), rotacoes(0), raiz(nullptr), rng(random_device{}()) {}

    ~Treap() {
        clear(raiz);
    }

    void inserir(int chave) {
        acessos = 0;
        int prioridade = static_cast<int>(rng() & 0x7FFFFFFF);
        raiz = inserirRecursivo(raiz, chave, prioridade);
    }

    bool buscar(int chave) {
        acessos = 0;
        TreapNode* node = raiz;

        while (node != nullptr) {
            acessos++;
            if (chave == node->chave) {
                return true;
            } else if (chave < node->chave) {
                node = node->esquerda;
            } else {
                node = node->direita;
            }
        }

        return false;
    }

    bool remover(int chave) {
        acessos = 0;
        bool removido = false;
        raiz = removerRecursivo(raiz, chave, removido);
        return removido;
    }

    void exportarDOT(const string& nomeArquivo) {
        ofstream arq(nomeArquivo);
        if (!arq.is_open()) return;

        arq << "digraph TreapTree {\n";
        arq << "    node [shape=record, style=filled, fillcolor=lightblue];\n";
        if (raiz != nullptr) {
            gerarDotRecursivo(raiz, arq);
        }
        arq << "}\n";
        arq.close();
    }

    void estatisticas() {
        cout << "Nos na memoria: " << nosAlocados << endl;
        cout << "Total de nos criados: " << totalCriados << endl;
        cout << "Acessos na ultima operacao: " << acessos << endl;
        cout << "Total de rotacoes: " << rotacoes << endl;
    }
};
