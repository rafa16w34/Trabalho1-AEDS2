#pragma once

#include <iostream>
#include <fstream>
#include <string>

using namespace std;

struct SplayNode {
    int chave;
    SplayNode* esquerda;
    SplayNode* direita;

    SplayNode(int k) : chave(k), esquerda(nullptr), direita(nullptr) {}
};

class Splay {
private:
    int nosAlocados;
    int totalCriados;
    int acessos;
    int rotacoes;

    SplayNode* raiz;

    // Rotação à esquerda (Zag)
    SplayNode* rotacaoEsquerda(SplayNode* node) {
        SplayNode* x = node->direita;
        SplayNode* aux = x->esquerda;

        x->esquerda = node;
        node->direita = aux;

        rotacoes++;
        acessos += 2;

        return x;
    }

    // Rotação à direita (Zig)
    SplayNode* rotacaoDireita(SplayNode* node) {
        SplayNode* x = node->esquerda;
        SplayNode* aux = x->direita;

        x->direita = node;
        node->esquerda = aux;

        rotacoes++;
        acessos += 2;

        return x;
    }

    SplayNode* splay(SplayNode* node, int chave) {
        if (node == nullptr || node->chave == chave) {
            return node;
        }

        acessos++;

        // Ramo esquerdo
        if (chave < node->chave) {
            if (node->esquerda == nullptr) {
                return node;
            }

            // Zig-Zig (Esquerda Esquerda)
            if (chave < node->esquerda->chave) {
                node->esquerda->esquerda = splay(node->esquerda->esquerda, chave);
                node = rotacaoDireita(node);
            }
            // Zig-Zag (Esquerda Direita)
            else if (chave > node->esquerda->chave) {
                node->esquerda->direita = splay(node->esquerda->direita, chave);
                if (node->esquerda->direita != nullptr) {
                    node->esquerda = rotacaoEsquerda(node->esquerda);
                }
            }
            return (node->esquerda == nullptr) ? node : rotacaoDireita(node);
        }
        else { // Ramo direito
            if (node->direita == nullptr) {
                return node;
            }

            // Zig-Zag (Direita Esquerda)
            if (chave < node->direita->chave) {
                node->direita->esquerda = splay(node->direita->esquerda, chave);
                if (node->direita->esquerda != nullptr) {
                    node->direita = rotacaoDireita(node->direita);
                }
            }
            // Zig-Zig (Direita Direita)
            else if (chave > node->direita->chave) {
                node->direita->direita = splay(node->direita->direita, chave);
                node = rotacaoEsquerda(node);
            }
            return (node->direita == nullptr) ? node : rotacaoEsquerda(node);
        }
    }

    void clear(SplayNode* node) {
        if (node != nullptr) {
            clear(node->esquerda);
            clear(node->direita);
            delete node;
            nosAlocados--;
        }
    }

    void gerarDotRecursivo(SplayNode* node, ofstream& arquivo) {
        if (node == nullptr) return;

        arquivo << "    node" << node << " [label=\"" << node->chave << "\"];\n";

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
    Splay() : nosAlocados(0), totalCriados(0), acessos(0), rotacoes(0), raiz(nullptr) {}

    ~Splay() {
        clear(raiz);
    }

    void inserir(int chave) {
        acessos = 0;
        if (raiz == nullptr) {
            raiz = new SplayNode(chave);
            nosAlocados++;
            totalCriados++;
            return;
        }

        raiz = splay(raiz, chave);

        if (chave == raiz->chave) {
            return; // Chave já presente
        }

        SplayNode* novo = new SplayNode(chave);
        nosAlocados++;
        totalCriados++;

        if (chave < raiz->chave) {
            novo->direita = raiz;
            novo->esquerda = raiz->esquerda;
            raiz->esquerda = nullptr;
        } else {
            novo->esquerda = raiz;
            novo->direita = raiz->direita;
            raiz->direita = nullptr;
        }

        raiz = novo;
    }

    bool buscar(int chave) {
        acessos = 0;
        if (raiz == nullptr) {
            return false;
        }

        raiz = splay(raiz, chave);
        return (raiz->chave == chave);
    }

    bool remover(int chave) {
        acessos = 0;
        if (raiz == nullptr) {
            return false;
        }

        raiz = splay(raiz, chave);

        if (raiz->chave != chave) {
            return false;
        }

        SplayNode* aux = raiz;

        if (raiz->esquerda == nullptr) {
            raiz = raiz->direita;
        } else {
            SplayNode* subDir = raiz->direita;
            raiz = splay(raiz->esquerda, chave);
            raiz->direita = subDir;
        }

        delete aux;
        nosAlocados--;
        return true;
    }

    void exportarDOT(const string& nomeArquivo) {
        ofstream arq(nomeArquivo);
        if (!arq.is_open()) return;

        arq << "digraph SplayTree {\n";
        arq << "    node [shape=circle, style=filled, fillcolor=lightyellow];\n";
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
