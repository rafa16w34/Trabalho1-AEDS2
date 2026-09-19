#include <iostream>
#include <vector>
#include <string>
#include <chrono>
#include <random>
#include <iomanip>
#include <fstream>
#include <algorithm>

#include "KDTree/KDTree.hpp"
#include "Patricia/PatriciaTree.hpp"
#include "Splay/SplayTree.hpp"
#include "Treap/TreapTree.hpp"
#include "Trie/TrieTree.hpp"

using namespace std;
using namespace std::chrono;

// Gerador de strings aleatórias simples
string gerarPalavraAleatoria(mt19937& rng, int idx) {
    string chars = "abcdefghijklmnopqrstuvwxyz";
    string res = "";
    int len = 5 + (idx % 8);
    for (int i = 0; i < len; i++) {
        res += chars[rng() % 26];
    }
    return res;
}

// Gerador de palavras com prefixos muito longos compartilhados (Caso ideal para Patricia)
string gerarPalavraComPrefixoLongo(mt19937& rng, int idx) {
    string prefixoBase = "httpapiempresa-sistemainternov1recurso-";
    string res = prefixoBase + to_string(idx) + "-" + string(1, 'a' + (rng() % 26));
    return res;
}

void rodarBenchmarkCompleto() {
    cout << "==========================================================================" << endl;
    cout << "      AVALIAÇÃO EXPERIMENTAL COMPLETA: DIVERSOS PADRÕES E DISTRIBUIÇÕES   " << endl;
    cout << "==========================================================================" << endl;

    vector<int> tamanhos = {1000, 10000, 100000};
    mt19937 rng(42);

    ofstream csv("output/benchmark_results_detalhado.csv");
    csv << "Estrutura,N,Cenario,Operacao,Tempo_ms,Metrica_Secundaria\n";

    for (int N : tamanhos) {
        cout << "\n==========================================================================" << endl;
        cout << ">>> TESTANDO PARA VOLUME DE DADOS N = " << N << endl;
        cout << "==========================================================================" << endl;

        // ---------------------------------------------------------------------
        // 1. SPLAY TREE: ALEATÓRIO vs. ORDENADO (Pior Caso) vs. ALTA LOCALIDADE (Favorável)
        // ---------------------------------------------------------------------
        {
            cout << "\n--- [SPLAY TREE] Avaliando Padrões de Acesso ---" << endl;
            // A) Aleatório
            Splay splayRand;
            vector<int> chavesRand(N);
            for (int i = 0; i < N; i++) chavesRand[i] = rng() % (N * 10);

            auto t0 = high_resolution_clock::now();
            for (int k : chavesRand) splayRand.inserir(k);
            auto t1 = high_resolution_clock::now();
            double tInsRand = duration<double, milli>(t1 - t0).count();

            // B) Inserção Sequencial Ordenada (Caso Desfavorável para BST comum, Splay ajusta)
            Splay splaySeq;
            t0 = high_resolution_clock::now();
            for (int i = 1; i <= N; i++) splaySeq.inserir(i);
            t1 = high_resolution_clock::now();
            double tInsSeq = duration<double, milli>(t1 - t0).count();

            // C) Busca com Alta Localidade Temporal (80% das buscas em 20% das chaves - Pareto)
            vector<int> chavesFrequentes;
            for (int i = 0; i < N / 5; i++) chavesFrequentes.push_back(chavesRand[i]);
            
            t0 = high_resolution_clock::now();
            for (int i = 0; i < N / 10; i++) {
                int chaveAlvo = chavesFrequentes[rng() % chavesFrequentes.size()];
                splayRand.buscar(chaveAlvo);
            }
            t1 = high_resolution_clock::now();
            double tBuscaLocalidade = duration<double, milli>(t1 - t0).count();

            cout << "  Splay Insercao Aleatoria: " << tInsRand << " ms" << endl;
            cout << "  Splay Insercao Sequencial Ordenada: " << tInsSeq << " ms" << endl;
            cout << "  Splay Busca com Alta Localidade (80/20): " << tBuscaLocalidade << " ms" << endl;

            csv << "Splay," << N << ",Aleatorio,Insercao," << tInsRand << ",-\n";
            csv << "Splay," << N << ",Sequencial_Ordenado,Insercao," << tInsSeq << ",-\n";
            csv << "Splay," << N << ",Alta_Localidade,Busca," << tBuscaLocalidade << ",-\n";
        }

        // ---------------------------------------------------------------------
        // 2. TREAP: DADOS ORDENADOS (Validação da Invariante de Heap Estocástica)
        // ---------------------------------------------------------------------
        {
            cout << "\n--- [TREAP] Avaliando Resiliência a Dados Ordenados ---" << endl;
            Treap treapSeq;
            auto t0 = high_resolution_clock::now();
            for (int i = 1; i <= N; i++) treapSeq.inserir(i);
            auto t1 = high_resolution_clock::now();
            double tInsSeq = duration<double, milli>(t1 - t0).count();

            t0 = high_resolution_clock::now();
            for (int i = 0; i < N / 10; i++) treapSeq.buscar(1 + (rng() % N));
            t1 = high_resolution_clock::now();
            double tBuscaSeq = duration<double, milli>(t1 - t0).count();

            cout << "  Treap Insercao Sequencial Ordenada: " << tInsSeq << " ms" << endl;
            cout << "  Treap Busca em Estrutura Ordenada: " << tBuscaSeq << " ms" << endl;

            csv << "Treap," << N << ",Sequencial_Ordenado,Insercao," << tInsSeq << ",-\n";
            csv << "Treap," << N << ",Sequencial_Ordenado,Busca," << tBuscaSeq << ",-\n";
        }

        // ---------------------------------------------------------------------
        // 3. PATRICIA vs. TRIE: PREFIXOS LONGOS COMPARTILHADOS (Compactação de Memória)
        // ---------------------------------------------------------------------
        {
            cout << "\n--- [PATRICIA vs TRIE] Comparativo com Prefixos Longos ---" << endl;
            vector<string> palavrasPrefixo(N);
            for (int i = 0; i < N; i++) palavrasPrefixo[i] = gerarPalavraComPrefixoLongo(rng, i);

            // Patricia com Prefixos Longos
            Patricia pat;
            auto t0 = high_resolution_clock::now();
            for (const auto& w : palavrasPrefixo) pat.inserir(w);
            auto t1 = high_resolution_clock::now();
            double tInsPat = duration<double, milli>(t1 - t0).count();

            // Trie com Prefixos Longos
            Trie trie;
            t0 = high_resolution_clock::now();
            for (const auto& w : palavrasPrefixo) trie.inserir(w);
            t1 = high_resolution_clock::now();
            double tInsTrie = duration<double, milli>(t1 - t0).count();

            cout << "  Patricia Insercao Prefixos Longos: " << tInsPat << " ms" << endl;
            cout << "  Trie Padrão Insercao Prefixos Longos: " << tInsTrie << " ms" << endl;

            csv << "Patricia," << N << ",Prefixos_Longos,Insercao," << tInsPat << ",-\n";
            csv << "Trie," << N << ",Prefixos_Longos,Insercao," << tInsTrie << ",-\n";
        }

        // ---------------------------------------------------------------------
        // 4. KD-TREE: PONTOS UNIFORMES vs. PONTOS AGRUPADOS (CLUSTERS)
        // ---------------------------------------------------------------------
        {
            cout << "\n--- [KD-TREE] PONTOS UNIFORMES vs CLUSTERIZADOS ---" << endl;
            KDTree kdtCluster(2);
            vector<vector<double>> pontosCluster(N);
            // 80% dos pontos em um cluster denso [0..10] x [0..10], 20% dispersos
            for (int i = 0; i < N; i++) {
                if (i < (N * 4) / 5) {
                    pontosCluster[i] = {(double)(rng() % 100) / 10.0, (double)(rng() % 100) / 10.0};
                } else {
                    pontosCluster[i] = {(double)(rng() % 10000) / 10.0, (double)(rng() % 10000) / 10.0};
                }
            }

            auto t0 = high_resolution_clock::now();
            for (const auto& p : pontosCluster) kdtCluster.inserir(p);
            auto t1 = high_resolution_clock::now();
            double tInsCluster = duration<double, milli>(t1 - t0).count();

            t0 = high_resolution_clock::now();
            for (int i = 0; i < N / 10; i++) kdtCluster.vizinhosProximos({5.0, 5.0});
            t1 = high_resolution_clock::now();
            double tNNSCluster = duration<double, milli>(t1 - t0).count();

            cout << "  KD-Tree Insercao Clusterizada: " << tInsCluster << " ms" << endl;
            cout << "  KD-Tree NNS em Regiao Densa (Cluster): " << tNNSCluster << " ms" << endl;

            csv << "KD-Tree," << N << ",Clusterizado,Insercao," << tInsCluster << ",-\n";
            csv << "KD-Tree," << N << ",Clusterizado,NNS," << tNNSCluster << ",-\n";
        }
    }

    csv.close();
    cout << "\n==========================================================================" << endl;
    cout << "  Experimentos completos concluídos! Resultados salvos em:                " << endl;
    cout << "  output/benchmark_results_detalhado.csv                                  " << endl;
    cout << "==========================================================================" << endl;
}

int main() {
    rodarBenchmarkCompleto();
    return 0;
}
