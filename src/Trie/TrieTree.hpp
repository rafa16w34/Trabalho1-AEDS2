#pragma once

#include <iostream>
#include <fstream>
#include <string>
#include <cctype>

using namespace std;

struct TrieNode {
    TrieNode* children[26];
    bool isWord;

    TrieNode() : isWord(false) {
        for (auto& c : children) {
            c = nullptr;
        }
    }
};

class Trie {
private:
    int nosAlocados;
    int totalCriados;
    int acessos;
    TrieNode* root;

    void clear(TrieNode* tn) {
        if (!tn) return;
        for (int i = 0; i < 26; i++) {
            if (tn->children[i]) {
                clear(tn->children[i]);
            }
        }
        delete tn;
        nosAlocados--;
    }

    void tratamento(string& word) {
        string limpa = "";
        for (char c : word) {
            if (isalpha(static_cast<unsigned char>(c))) {
                limpa += static_cast<char>(tolower(static_cast<unsigned char>(c)));
            }
        }
        word = limpa;
    }

    TrieNode* find(const string& s) {
        TrieNode* tn = root;
        acessos++;

        for (char c : s) {
            int idx = c - 'a';
            if (idx < 0 || idx >= 26 || !tn->children[idx]) {
                return nullptr;
            }
            tn = tn->children[idx];
            acessos++;
        }

        return tn;
    }

    bool temFilho(TrieNode* node) {
        for (int i = 0; i < 26; i++) {
            if (node->children[i] != nullptr) {
                return true;
            }
        }
        return false;
    }

    bool removerRecursivo(TrieNode* node, const string& word, size_t depth, bool& removido) {
        if (!node) {
            return false;
        }

        acessos++;

        if (depth == word.size()) {
            if (node->isWord) {
                node->isWord = false;
                removido = true;
            }
            return !temFilho(node);
        }

        int index = word[depth] - 'a';
        if (index < 0 || index >= 26 || !node->children[index]) {
            return false;
        }

        if (removerRecursivo(node->children[index], word, depth + 1, removido)) {
            delete node->children[index];
            node->children[index] = nullptr;
            nosAlocados--;
        }

        return !node->isWord && !temFilho(node);
    }

    void gerarDotRecursivo(TrieNode* node, const string& parentId, char edgeLabel, ofstream& arq, int& nodeCounter) {
        if (!node) return;

        string currentId = "node" + to_string(nodeCounter++);
        arq << "    " << currentId << " [label=\"" << (edgeLabel == '\0' ? "ROOT" : string(1, edgeLabel))
            << "\", shape=" << (node->isWord ? "doublecircle" : "circle") << "];\n";

        if (!parentId.empty()) {
            arq << "    " << parentId << " -> " << currentId << ";\n";
        }

        for (int i = 0; i < 26; i++) {
            if (node->children[i]) {
                gerarDotRecursivo(node->children[i], currentId, static_cast<char>('a' + i), arq, nodeCounter);
            }
        }
    }

public:
    Trie() : nosAlocados(1), totalCriados(1), acessos(0) {
        root = new TrieNode();
    }

    ~Trie() {
        clear(root);
    }

    void inserir(string palavra) {
        acessos = 0;
        tratamento(palavra);
        if (palavra.empty()) return;

        TrieNode* tn = root;
        acessos++;

        for (char c : palavra) {
            int idx = c - 'a';
            if (idx < 0 || idx >= 26) continue;

            if (!tn->children[idx]) {
                tn->children[idx] = new TrieNode();
                nosAlocados++;
                totalCriados++;
            }
            tn = tn->children[idx];
            acessos++;
        }
        tn->isWord = true;
    }

    bool buscar(string palavra) {
        acessos = 0;
        tratamento(palavra);
        if (palavra.empty()) return false;

        TrieNode* endNode = find(palavra);
        return (endNode != nullptr && endNode->isWord);
    }

    bool comecaCom(string prefixo) {
        acessos = 0;
        tratamento(prefixo);
        if (prefixo.empty()) return false;

        return (find(prefixo) != nullptr);
    }

    bool remover(string palavra) {
        acessos = 0;
        tratamento(palavra);
        if (palavra.empty()) return false;

        bool removido = false;
        removerRecursivo(root, palavra, 0, removido);
        return removido;
    }

    void exportarDOT(const string& nomeArquivo) {
        ofstream arq(nomeArquivo);
        if (!arq.is_open()) return;

        arq << "digraph TrieTree {\n";
        arq << "    node [style=filled, fillcolor=lightgreen];\n";
        int nodeCounter = 0;
        gerarDotRecursivo(root, "", '\0', arq, nodeCounter);
        arq << "}\n";
        arq.close();
    }

    void estatisticas() {
        cout << "Nos na memoria: " << nosAlocados << endl;
        cout << "Total de nos criados: " << totalCriados << endl;
        cout << "Acessos na ultima operacao: " << acessos << endl;
    }
};
