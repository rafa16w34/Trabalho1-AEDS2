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

// Gerador de strings aleatórias (comprimento 5 a 12)
string gerarPalavraAleatoria(mt19937& rng, int idx) {
    string chars = "abcdefghijklmnopqrstuvwxyz";
    string res = "";
    int len = 5 + (idx % 8);
    for (int i = 0; i < len; i++) {
        res += chars[rng() % 26];
    }
    return res;
}

void rodarBenchmark() {
    cout << "==========================================================================" << endl;
    cout << "                  EXECECUTAO DOS EXPERIMENTOS BENCHMARK                   " << endl;
    cout << "==========================================================================" << endl;

    vector<int> tamanhos = {1000, 10000, 100000};
    mt19937 rng(42); // Semente fixa para reprodutibilidade

    ofstream csv("output/benchmark_results.csv");
    csv << "Estrutura,N,Operacao,Tempo_ms,Nos_Memoria,Total_Criados,Acessos,Rotacoes\n";

    for (int N : tamanhos) {
        cout << "\n>>> TESTANDO PARA N = " << N << endl;

        // -------------------------------------------------------------
        // 1. BENCHMARK KD-TREE (2D)
        // -------------------------------------------------------------
        {
            KDTree kdt(2);
            vector<vector<double>> pontos(N);
            for (int i = 0; i < N; i++) {
                pontos[i] = {(double)(rng() % 100000) / 10.0, (double)(rng() % 100000) / 10.0};
            }

            // Inserção
            auto t0 = high_resolution_clock::now();
            for (const auto& p : pontos) kdt.inserir(p);
            auto t1 = high_resolution_clock::now();
            double tempoIns = duration<double, milli>(t1 - t0).count();

            // Busca
            t0 = high_resolution_clock::now();
            for (int i = 0; i < N / 10; i++) kdt.buscar(pontos[i]);
            t1 = high_resolution_clock::now();
            double tempoBusca = duration<double, milli>(t1 - t0).count();

            // Remoção
            t0 = high_resolution_clock::now();
            for (int i = 0; i < N / 10; i++) kdt.remover(pontos[i]);
            t1 = high_resolution_clock::now();
            double tempoRem = duration<double, milli>(t1 - t0).count();

            cout << "KD-Tree (N=" << N << "): Insercao=" << tempoIns << "ms, Busca(N/10)=" << tempoBusca << "ms, Remocao(N/10)=" << tempoRem << "ms" << endl;
            csv << "KD-Tree," << N << ",Insercao," << tempoIns << ",-,-,-,-\n";
            csv << "KD-Tree," << N << ",Busca," << tempoBusca << ",-,-,-,-\n";
            csv << "KD-Tree," << N << ",Remocao," << tempoRem << ",-,-,-,-\n";
        }

        // -------------------------------------------------------------
        // 2. BENCHMARK PATRICIA
        // -------------------------------------------------------------
        {
            Patricia pat;
            vector<string> palavras(N);
            for (int i = 0; i < N; i++) palavras[i] = gerarPalavraAleatoria(rng, i);

            // Inserção
            auto t0 = high_resolution_clock::now();
            for (const auto& w : palavras) pat.inserir(w);
            auto t1 = high_resolution_clock::now();
            double tempoIns = duration<double, milli>(t1 - t0).count();

            // Busca
            t0 = high_resolution_clock::now();
            for (int i = 0; i < N / 10; i++) pat.buscar(palavras[i]);
            t1 = high_resolution_clock::now();
            double tempoBusca = duration<double, milli>(t1 - t0).count();

            // Remoção
            t0 = high_resolution_clock::now();
            for (int i = 0; i < N / 10; i++) pat.remover(palavras[i]);
            t1 = high_resolution_clock::now();
            double tempoRem = duration<double, milli>(t1 - t0).count();

            cout << "Patricia (N=" << N << "): Insercao=" << tempoIns << "ms, Busca(N/10)=" << tempoBusca << "ms, Remocao(N/10)=" << tempoRem << "ms" << endl;
            csv << "Patricia," << N << ",Insercao," << tempoIns << ",-,-,-,-\n";
            csv << "Patricia," << N << ",Busca," << tempoBusca << ",-,-,-,-\n";
            csv << "Patricia," << N << ",Remocao," << tempoRem << ",-,-,-,-\n";
        }

        // -------------------------------------------------------------
        // 3. BENCHMARK TRIE
        // -------------------------------------------------------------
        {
            Trie trie;
            vector<string> palavras(N);
            for (int i = 0; i < N; i++) palavras[i] = gerarPalavraAleatoria(rng, i);

            // Inserção
            auto t0 = high_resolution_clock::now();
            for (const auto& w : palavras) trie.inserir(w);
            auto t1 = high_resolution_clock::now();
            double tempoIns = duration<double, milli>(t1 - t0).count();

            // Busca
            t0 = high_resolution_clock::now();
            for (int i = 0; i < N / 10; i++) trie.buscar(palavras[i]);
            t1 = high_resolution_clock::now();
            double tempoBusca = duration<double, milli>(t1 - t0).count();

            // Remoção
            t0 = high_resolution_clock::now();
            for (int i = 0; i < N / 10; i++) trie.remover(palavras[i]);
            t1 = high_resolution_clock::now();
            double tempoRem = duration<double, milli>(t1 - t0).count();

            cout << "Trie (N=" << N << "): Insercao=" << tempoIns << "ms, Busca(N/10)=" << tempoBusca << "ms, Remocao(N/10)=" << tempoRem << "ms" << endl;
            csv << "Trie," << N << ",Insercao," << tempoIns << ",-,-,-,-\n";
            csv << "Trie," << N << ",Busca," << tempoBusca << ",-,-,-,-\n";
            csv << "Trie," << N << ",Remocao," << tempoRem << ",-,-,-,-\n";
        }

        // -------------------------------------------------------------
        // 4. BENCHMARK SPLAY
        // -------------------------------------------------------------
        {
            Splay splay;
            vector<int> chaves(N);
            for (int i = 0; i < N; i++) chaves[i] = rng() % (N * 10);

            // Inserção
            auto t0 = high_resolution_clock::now();
            for (int k : chaves) splay.inserir(k);
            auto t1 = high_resolution_clock::now();
            double tempoIns = duration<double, milli>(t1 - t0).count();

            // Busca (Com reestruturação Splay)
            t0 = high_resolution_clock::now();
            for (int i = 0; i < N / 10; i++) splay.buscar(chaves[i]);
            t1 = high_resolution_clock::now();
            double tempoBusca = duration<double, milli>(t1 - t0).count();

            // Remoção
            t0 = high_resolution_clock::now();
            for (int i = 0; i < N / 10; i++) splay.remover(chaves[i]);
            t1 = high_resolution_clock::now();
            double tempoRem = duration<double, milli>(t1 - t0).count();

            cout << "Splay (N=" << N << "): Insercao=" << tempoIns << "ms, Busca(N/10)=" << tempoBusca << "ms, Remocao(N/10)=" << tempoRem << "ms" << endl;
            csv << "Splay," << N << ",Insercao," << tempoIns << ",-,-,-,-\n";
            csv << "Splay," << N << ",Busca," << tempoBusca << ",-,-,-,-\n";
            csv << "Splay," << N << ",Remocao," << tempoRem << ",-,-,-,-\n";
        }

        // -------------------------------------------------------------
        // 5. BENCHMARK TREAP
        // -------------------------------------------------------------
        {
            Treap treap;
            vector<int> chaves(N);
            for (int i = 0; i < N; i++) chaves[i] = rng() % (N * 10);

            // Inserção
            auto t0 = high_resolution_clock::now();
            for (int k : chaves) treap.inserir(k);
            auto t1 = high_resolution_clock::now();
            double tempoIns = duration<double, milli>(t1 - t0).count();

            // Busca
            t0 = high_resolution_clock::now();
            for (int i = 0; i < N / 10; i++) treap.buscar(chaves[i]);
            t1 = high_resolution_clock::now();
            double tempoBusca = duration<double, milli>(t1 - t0).count();

            // Remoção
            t0 = high_resolution_clock::now();
            for (int i = 0; i < N / 10; i++) treap.remover(chaves[i]);
            t1 = high_resolution_clock::now();
            double tempoRem = duration<double, milli>(t1 - t0).count();

            cout << "Treap (N=" << N << "): Insercao=" << tempoIns << "ms, Busca(N/10)=" << tempoBusca << "ms, Remocao(N/10)=" << tempoRem << "ms" << endl;
            csv << "Treap," << N << ",Insercao," << tempoIns << ",-,-,-,-\n";
            csv << "Treap," << N << ",Busca," << tempoBusca << ",-,-,-,-\n";
            csv << "Treap," << N << ",Remocao," << tempoRem << ",-,-,-,-\n";
        }
    }

    csv.close();
    cout << "\nBenchmark concluido! Resultados salvos em output/benchmark_results.csv" << endl;
}

int main() {
    rodarBenchmark();
    return 0;
}
