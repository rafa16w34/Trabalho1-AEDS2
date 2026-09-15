#pragma once

#include <iostream>
#include <fstream>
#include <string>
#include <cctype>
#include <algorithm>

using namespace std;

struct PatriciaNode {
    string rotulo;
    PatriciaNode* filho[26];
    bool ehPalavra;

    PatriciaNode(string rot = "", bool finalPalavra = false) {
        rotulo = rot;
        ehPalavra = finalPalavra;
        for (int i = 0; i < 26; i++) {
            filho[i] = nullptr;
        }
    }
};

class Patricia {
private:
    int nosAlocados;
    int totalCriados;
    int acessos;
    PatriciaNode* raiz;

    void clear(PatriciaNode* pn) {
        if (!pn) return;
        for (int i = 0; i < 26; i++) {
            if (pn->filho[i]) {
                clear(pn->filho[i]);
            }
        }
        delete pn;
        nosAlocados--;
    }

    void tratamento(string& palavra) {
        string limpa = "";
        for (char c : palavra) {
            if (isalpha(static_cast<unsigned char>(c))) {
                limpa += static_cast<char>(tolower(static_cast<unsigned char>(c)));
            }
        }
        palavra = limpa;
    }

    int calcularPrefixoComum(const string& a, const string& b) {
        int i = 0;
        int limite = min(static_cast<int>(a.size()), static_cast<int>(b.size()));
        while (i < limite && a[i] == b[i]) {
            i++;
        }
        return i;
    }

    void inserirRecursivo(PatriciaNode* nodeAtual, const string& palavra) {
        acessos++;

        if (palavra.empty()) {
            nodeAtual->ehPalavra = true;
            return;
        }

        int index = palavra[0] - 'a';
        if (index < 0 || index >= 26) return;

        PatriciaNode* filho = nodeAtual->filho[index];

        // Caso 1: Ramo não existe, criar a folha
        if (filho == nullptr) {
            acessos++;
            nodeAtual->filho[index] = new PatriciaNode(palavra, true);
            nosAlocados++;
            totalCriados++;
            return;
        }

        // Caso 2: Ramo já existe, obtém o prefixo comum entre a palavra e rótulo do filho
        int k = calcularPrefixoComum(palavra, filho->rotulo);

        // Cenário A: Coincidência total com o rótulo do filho
        if (k == static_cast<int>(filho->rotulo.size())) {
            if (k == static_cast<int>(palavra.size())) {
                acessos++;
                filho->ehPalavra = true;
            } else {
                inserirRecursivo(filho, palavra.substr(k));
            }
            return;
        }

        // Cenário B: Divergência no meio do rótulo (divisão de nó)
        string prefixo = filho->rotulo.substr(0, k);
        string fimRotulo = filho->rotulo.substr(k);
        string fimPrefixo = palavra.substr(k);

        PatriciaNode* meio = new PatriciaNode(prefixo, false);
        nosAlocados++;
        totalCriados++;

        filho->rotulo = fimRotulo;
        int indexFimRotulo = fimRotulo[0] - 'a';
        meio->filho[indexFimRotulo] = filho;

        if (fimPrefixo.empty()) {
            meio->ehPalavra = true;
        } else {
            int indexFimPrefixo = fimPrefixo[0] - 'a';
            meio->filho[indexFimPrefixo] = new PatriciaNode(fimPrefixo, true);
            nosAlocados++;
            totalCriados++;
        }

        nodeAtual->filho[index] = meio;
    }

    bool buscarRecursivo(PatriciaNode* nodeAtual, const string& palavra) {
        acessos++;

        if (palavra.empty()) {
            return nodeAtual->ehPalavra;
        }

        int index = palavra[0] - 'a';
        if (index < 0 || index >= 26) return false;

        PatriciaNode* filho = nodeAtual->filho[index];
        if (filho == nullptr) {
            return false;
        }

        int k = calcularPrefixoComum(palavra, filho->rotulo);
        if (k < static_cast<int>(filho->rotulo.size())) {
            return false;
        }

        return buscarRecursivo(filho, palavra.substr(k));
    }

    bool comecaComRecursivo(PatriciaNode* nodeAtual, const string& prefixo) {
        acessos++;

        if (prefixo.empty()) {
            return true;
        }

        int index = prefixo[0] - 'a';
        if (index < 0 || index >= 26) return false;

        PatriciaNode* filho = nodeAtual->filho[index];
        if (filho == nullptr) {
            return false;
        }

        int k = calcularPrefixoComum(prefixo, filho->rotulo);
        if (k == static_cast<int>(prefixo.size())) {
            return true;
        }
        if (k == static_cast<int>(filho->rotulo.size())) {
            return comecaComRecursivo(filho, prefixo.substr(k));
        }

        return false;
    }

    int contarFilhos(PatriciaNode* node) {
        int cont = 0;
        for (int i = 0; i < 26; i++) {
            if (node->filho[i] != nullptr) {
                cont++;
            }
        }
        return cont;
    }

    int obterIndexFilho(PatriciaNode* node) {
        for (int i = 0; i < 26; i++) {
            if (node->filho[i] != nullptr) {
                return i;
            }
        }
        return -1;
    }

    bool remocaoRecursiva(PatriciaNode* node, const string& palavra) {
        acessos++;

        if (palavra.empty()) {
            if (!node->ehPalavra) {
                return false;
            }
            node->ehPalavra = false;
            return true;
        }

        int index = palavra[0] - 'a';
        if (index < 0 || index >= 26) return false;

        PatriciaNode* filho = node->filho[index];
        if (filho == nullptr) {
            return false;
        }

        int k = calcularPrefixoComum(palavra, filho->rotulo);
        if (k < static_cast<int>(filho->rotulo.size())) {
            return false;
        }

        bool removido = remocaoRecursiva(filho, palavra.substr(k));
        if (!removido) {
            return false;
        }

        // Pós-Ordem: fusão e limpeza de nós
        int nFilhos = contarFilhos(filho);

        if (nFilhos == 0 && !filho->ehPalavra) {
            delete filho;
            node->filho[index] = nullptr;
            nosAlocados--;
        } else if (nFilhos == 1 && !filho->ehPalavra) {
            int indexFilhoFilho = obterIndexFilho(filho);
            PatriciaNode* filhoFilho = filho->filho[indexFilhoFilho];

            filhoFilho->rotulo = filho->rotulo + filhoFilho->rotulo;
            node->filho[index] = filhoFilho;

            delete filho;
            nosAlocados--;
        }

        return true;
    }

    void gerarDotRecursivo(PatriciaNode* node, const string& parentId, ofstream& arq, int& nodeCounter) {
        if (!node) return;

        string currentId = "node" + to_string(nodeCounter++);
        string labelText = node->rotulo.empty() ? "ROOT" : node->rotulo;
        arq << "    " << currentId << " [label=\"" << labelText
            << "\", shape=" << (node->ehPalavra ? "doublecircle" : "box") << "];\n";

        if (!parentId.empty()) {
            arq << "    " << parentId << " -> " << currentId << ";\n";
        }

        for (int i = 0; i < 26; i++) {
            if (node->filho[i]) {
                gerarDotRecursivo(node->filho[i], currentId, arq, nodeCounter);
            }
        }
    }

public:
    Patricia() : nosAlocados(1), totalCriados(1), acessos(0) {
        raiz = new PatriciaNode();
    }

    ~Patricia() {
        clear(raiz);
    }

    void inserir(string palavra) {
        acessos = 0;
        tratamento(palavra);
        if (palavra.empty()) return;

        inserirRecursivo(raiz, palavra);
    }

    bool buscar(string palavra) {
        acessos = 0;
        tratamento(palavra);
        if (palavra.empty()) return false;

        return buscarRecursivo(raiz, palavra);
    }

    bool comecaCom(string palavra) {
        acessos = 0;
        tratamento(palavra);
        if (palavra.empty()) return false;

        return comecaComRecursivo(raiz, palavra);
    }

    bool remover(string palavra) {
        acessos = 0;
        tratamento(palavra);
        if (palavra.empty()) return false;

        return remocaoRecursiva(raiz, palavra);
    }

    void exportarDOT(const string& nomeArquivo) {
        ofstream arq(nomeArquivo);
        if (!arq.is_open()) return;

        arq << "digraph PatriciaTree {\n";
        arq << "    node [style=filled, fillcolor=lightcoral];\n";
        int nodeCounter = 0;
        gerarDotRecursivo(raiz, "", arq, nodeCounter);
        arq << "}\n";
        arq.close();
    }

    void estatisticas() {
        cout << "Nos na memoria: " << nosAlocados << endl;
        cout << "Total de nos criados: " << totalCriados << endl;
        cout << "Acessos na ultima operacao: " << acessos << endl;
    }
};
