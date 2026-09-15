#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <string>
#include <filesystem>
#include <cstdlib>

#include "KDTree/KDTree.hpp"
#include "Patricia/PatriciaTree.hpp"
#include "Splay/SplayTree.hpp"
#include "Treap/TreapTree.hpp"
#include "Trie/TrieTree.hpp"

using namespace std;
namespace fs = std::filesystem;

void criarEstruturaPastas() {
    vector<string> estruturas = {"kdtree", "patricia", "trie", "splay", "treap"};
    for (const auto& est : estruturas) {
        fs::create_directories("output/dots/" + est);
        fs::create_directories("output/images/" + est);
    }
}

void gerarImagemDot(const string& dotPath, const string& pngPath) {
    string cmd = "dot -Tpng \"" + dotPath + "\" -o \"" + pngPath + "\" 2>/dev/null";
    int res = system(cmd.c_str());
    if (res == 0) {
        cout << "  [Visual] Imagem gerada: " << pngPath << endl;
    } else {
        cout << "  [Visual] Falha ao converter .dot em .png (verifique se Graphviz dot esta instalado)." << endl;
    }
}

void processarKDTree(const string& arquivoInput) {
    cout << "\n==========================================" << endl;
    cout << "          PROCESSANDO: KD-TREE" << endl;
    cout << "==========================================" << endl;

    KDTree arvore(2);
    ifstream arq(arquivoInput);

    if (!arq.is_open()) {
        cout << "Erro ao abrir o arquivo de entrada: " << arquivoInput << endl;
        return;
    }

    string linha;

    while (getline(arq, linha)) {
        if (linha.empty() || linha[0] == '#') continue;

        stringstream ss(linha);
        string comando;
        ss >> comando;

        if (comando == "INSERT") {
            double x, y;
            if (ss >> x >> y) {
                arvore.inserir({x, y});
                cout << "Comando INSERT: (" << x << ", " << y << ")" << endl;
            }
        } else if (comando == "SEARCH") {
            double x, y;
            if (ss >> x >> y) {
                bool achou = arvore.buscar({x, y});
                cout << "Comando SEARCH (" << x << ", " << y << "): " << (achou ? "Encontrado" : "Nao Encontrado") << endl;
            }
        } else if (comando == "NNS") {
            double x, y;
            if (ss >> x >> y) {
                vector<double> nns = arvore.vizinhosProximos({x, y});
                if (!nns.empty()) {
                    cout << "Comando NNS (" << x << ", " << y << "): (" << nns[0] << ", " << nns[1] << ")" << endl;
                }
            }
        } else if (comando == "RANGE") {
            double minX, minY, maxX, maxY;
            if (ss >> minX >> minY >> maxX >> maxY) {
                auto pontos = arvore.buscaPorAlcance({minX, minY}, {maxX, maxY});
                cout << "Comando RANGE [" << minX << ", " << maxX << "] x [" << minY << ", " << maxY << "]: ";
                for (const auto& p : pontos) cout << "(" << p[0] << ", " << p[1] << ") ";
                cout << endl;
            }
        } else if (comando == "REMOVE") {
            double x, y;
            if (ss >> x >> y) {
                // Exportar estado intermediário antes da remoção
                string dotInter = "output/dots/kdtree/estado_antes_remocao.dot";
                string pngInter = "output/images/kdtree/estado_antes_remocao.png";
                arvore.exportarDOT(dotInter);
                gerarImagemDot(dotInter, pngInter);

                bool removido = arvore.remover({x, y});
                cout << "Comando REMOVE (" << x << ", " << y << "): " << (removido ? "Removido" : "Falhou") << endl;
            }
        }
    }

    arvore.estatisticas();
    string dotFinal = "output/dots/kdtree/estado_final.dot";
    string pngFinal = "output/images/kdtree/estado_final.png";
    arvore.exportarDOT(dotFinal);
    gerarImagemDot(dotFinal, pngFinal);
}

void processarPatricia(const string& arquivoInput) {
    cout << "\n==========================================" << endl;
    cout << "        PROCESSANDO: ARVORE PATRICIA" << endl;
    cout << "==========================================" << endl;

    Patricia arvore;
    ifstream arq(arquivoInput);

    if (!arq.is_open()) {
        cout << "Erro ao abrir o arquivo de entrada: " << arquivoInput << endl;
        return;
    }

    string linha;
    while (getline(arq, linha)) {
        if (linha.empty() || linha[0] == '#') continue;

        stringstream ss(linha);
        string comando, palavra;
        ss >> comando >> palavra;

        if (comando == "INSERT") {
            arvore.inserir(palavra);
            cout << "Comando INSERT: '" << palavra << "'" << endl;
        } else if (comando == "SEARCH") {
            bool achou = arvore.buscar(palavra);
            cout << "Comando SEARCH '" << palavra << "': " << (achou ? "Encontrado" : "Nao Encontrado") << endl;
        } else if (comando == "STARTSWITH") {
            bool tem = arvore.comecaCom(palavra);
            cout << "Comando STARTSWITH '" << palavra << "': " << (tem ? "Sim" : "Nao") << endl;
        } else if (comando == "REMOVE") {
            string dotInter = "output/dots/patricia/estado_antes_remocao.dot";
            string pngInter = "output/images/patricia/estado_antes_remocao.png";
            arvore.exportarDOT(dotInter);
            gerarImagemDot(dotInter, pngInter);

            bool removido = arvore.remover(palavra);
            cout << "Comando REMOVE '" << palavra << "': " << (removido ? "Removido" : "Falhou") << endl;
        }
    }

    arvore.estatisticas();
    string dotFinal = "output/dots/patricia/estado_final.dot";
    string pngFinal = "output/images/patricia/estado_final.png";
    arvore.exportarDOT(dotFinal);
    gerarImagemDot(dotFinal, pngFinal);
}

void processarTrie(const string& arquivoInput) {
    cout << "\n==========================================" << endl;
    cout << "          PROCESSANDO: TRIE" << endl;
    cout << "==========================================" << endl;

    Trie arvore;
    ifstream arq(arquivoInput);

    if (!arq.is_open()) {
        cout << "Erro ao abrir o arquivo de entrada: " << arquivoInput << endl;
        return;
    }

    string linha;
    while (getline(arq, linha)) {
        if (linha.empty() || linha[0] == '#') continue;

        stringstream ss(linha);
        string comando, palavra;
        ss >> comando >> palavra;

        if (comando == "INSERT") {
            arvore.inserir(palavra);
            cout << "Comando INSERT: '" << palavra << "'" << endl;
        } else if (comando == "SEARCH") {
            bool achou = arvore.buscar(palavra);
            cout << "Comando SEARCH '" << palavra << "': " << (achou ? "Encontrado" : "Nao Encontrado") << endl;
        } else if (comando == "STARTSWITH") {
            bool tem = arvore.comecaCom(palavra);
            cout << "Comando STARTSWITH '" << palavra << "': " << (tem ? "Sim" : "Nao") << endl;
        } else if (comando == "REMOVE") {
            string dotInter = "output/dots/trie/estado_antes_remocao.dot";
            string pngInter = "output/images/trie/estado_antes_remocao.png";
            arvore.exportarDOT(dotInter);
            gerarImagemDot(dotInter, pngInter);

            bool removido = arvore.remover(palavra);
            cout << "Comando REMOVE '" << palavra << "': " << (removido ? "Removido" : "Falhou") << endl;
        }
    }

    arvore.estatisticas();
    string dotFinal = "output/dots/trie/estado_final.dot";
    string pngFinal = "output/images/trie/estado_final.png";
    arvore.exportarDOT(dotFinal);
    gerarImagemDot(dotFinal, pngFinal);
}

void processarSplay(const string& arquivoInput) {
    cout << "\n==========================================" << endl;
    cout << "        PROCESSANDO: ARVORE SPLAY" << endl;
    cout << "==========================================" << endl;

    Splay arvore;
    ifstream arq(arquivoInput);

    if (!arq.is_open()) {
        cout << "Erro ao abrir o arquivo de entrada: " << arquivoInput << endl;
        return;
    }

    string linha;
    while (getline(arq, linha)) {
        if (linha.empty() || linha[0] == '#') continue;

        stringstream ss(linha);
        string comando;
        int chave;
        ss >> comando >> chave;

        if (comando == "INSERT") {
            arvore.inserir(chave);
            cout << "Comando INSERT: " << chave << endl;
        } else if (comando == "SEARCH") {
            bool achou = arvore.buscar(chave);
            cout << "Comando SEARCH " << chave << " (Splayed para a Raiz): " << (achou ? "Encontrado" : "Nao Encontrado") << endl;
            string dotSplay = "output/dots/splay/estado_pos_busca_" + to_string(chave) + ".dot";
            string pngSplay = "output/images/splay/estado_pos_busca_" + to_string(chave) + ".png";
            arvore.exportarDOT(dotSplay);
            gerarImagemDot(dotSplay, pngSplay);
        } else if (comando == "REMOVE") {
            string dotInter = "output/dots/splay/estado_antes_remocao.dot";
            string pngInter = "output/images/splay/estado_antes_remocao.png";
            arvore.exportarDOT(dotInter);
            gerarImagemDot(dotInter, pngInter);

            bool removido = arvore.remover(chave);
            cout << "Comando REMOVE " << chave << ": " << (removido ? "Removido" : "Falhou") << endl;
        }
    }

    arvore.estatisticas();
    string dotFinal = "output/dots/splay/estado_final.dot";
    string pngFinal = "output/images/splay/estado_final.png";
    arvore.exportarDOT(dotFinal);
    gerarImagemDot(dotFinal, pngFinal);
}

void processarTreap(const string& arquivoInput) {
    cout << "\n==========================================" << endl;
    cout << "        PROCESSANDO: ARVORE TREAP" << endl;
    cout << "==========================================" << endl;

    Treap arvore;
    ifstream arq(arquivoInput);

    if (!arq.is_open()) {
        cout << "Erro ao abrir o arquivo de entrada: " << arquivoInput << endl;
        return;
    }

    string linha;
    while (getline(arq, linha)) {
        if (linha.empty() || linha[0] == '#') continue;

        stringstream ss(linha);
        string comando;
        int chave;
        ss >> comando >> chave;

        if (comando == "INSERT") {
            arvore.inserir(chave);
            cout << "Comando INSERT: " << chave << endl;
        } else if (comando == "SEARCH") {
            bool achou = arvore.buscar(chave);
            cout << "Comando SEARCH " << chave << ": " << (achou ? "Encontrado" : "Nao Encontrado") << endl;
        } else if (comando == "REMOVE") {
            string dotInter = "output/dots/treap/estado_antes_remocao.dot";
            string pngInter = "output/images/treap/estado_antes_remocao.png";
            arvore.exportarDOT(dotInter);
            gerarImagemDot(dotInter, pngInter);

            bool removido = arvore.remover(chave);
            cout << "Comando REMOVE " << chave << ": " << (removido ? "Removido" : "Falhou") << endl;
        }
    }

    arvore.estatisticas();
    string dotFinal = "output/dots/treap/estado_final.dot";
    string pngFinal = "output/images/treap/estado_final.png";
    arvore.exportarDOT(dotFinal);
    gerarImagemDot(dotFinal, pngFinal);
}

int main() {
    cout << "=========================================================" << endl;
    cout << "  TRABALHO PRATICO I: ESTRUTURAS EM ARVORES AVANCADAS   " << endl;
    cout << "=========================================================" << endl;

    // Garante que as pastas de saida output/dots/ e output/images/ existam
    criarEstruturaPastas();

    // Processamento das arvores lendo dos arquivos em input/
    processarKDTree("input/kdtree_input.txt");
    processarPatricia("input/patricia_input.txt");
    processarTrie("input/trie_input.txt");
    processarSplay("input/splay_input.txt");
    processarTreap("input/treap_input.txt");

    cout << "\n=========================================================" << endl;
    cout << "  Execucao concluida!                                   " << endl;
    cout << "  - Arquivos .dot salvos em: output/dots/<estrutura>/   " << endl;
    cout << "  - Imagens .png salvas em:  output/images/<estrutura>/ " << endl;
    cout << "=========================================================" << endl;

    return 0;
}