# Estruturas em Árvores Avançadas: Modelagem, Implementação e Análise Comparativa

[![C++17](https://img.shields.io/badge/Language-C%2B%2B17-blue.svg)](https://isocpp.org/)
[![Graphviz](https://img.shields.io/badge/Visualization-Graphviz-orange.svg)](https://graphviz.org/)
[![License](https://img.shields.io/badge/License-MIT-green.svg)](LICENSE)

Este repositório contém a implementação completa, suíte de testes de desempenho empírico, geradores de visualização gráfica e o artigo acadêmico em formato LaTeX (IEEEtran) referente ao **Trabalho Prático I: Estruturas em Árvores Avançadas**.

O objetivo do projeto é analisar, comparar e implementar cinco estruturas de dados hierárquicas não convencionais, avaliando suas propriedades teóricas, custos assintóticos, transformações estruturais e desempenho prático diante de diferentes volumes de dados ($N \in \{1.000, 10.000, 100.000\}$).

---

## 🌳 Estruturas Implementadas

1. **Trie (Árvore de Prefixos)**
   - Representação eficiente de conjuntos de strings através do compartilhamento de prefixos comuns.
   - Operações: Inserção, Busca, Verificação de Prefixo (`comecaCom`) e Remoção.

2. **Árvore Patricia (Compact Radix Tree)**
   - Versão compactada da Trie que elimina nós internos com único filho, reduzindo drasticamente o consumo de memória de $O(N \cdot L \cdot |\Sigma|)$ para $O(N \cdot |\Sigma|)$.
   - Operações: Inserção com divisão de nós (*split*), Busca, Prefixo e Remoção com fusão pós-ordem (*merge*).

3. **Árvore Splay**
   - Árvore Binária de Busca autoajustável desenvolvida por Sleator e Tarjan (1985).
   - Movimenta qualquer nó consultado para a raiz através de rotações locais (*Zig*, *Zig-Zig*, *Zig-Zag*), garantindo custo amortizado $O(\log N)$ e otimizando cenários com alta localidade temporal de acesso.

4. **Árvore Treap (Tree + Heap)**
   - Estrutura híbrida (Seidel & Aragon, 1996) que combina as propriedades de uma BST nas chaves com a propriedade de um Max-Heap em prioridades estocásticas geradas via `std::mt19937`.
   - Garante balanceamento probabilístico rasa altura com implementação simples mantida por rotações locais.

5. **KD-Tree ($k$-Dimensional Tree)**
   - Árvore binária de busca espacial 2D ($k=2$) para organização e busca de pontos espaciais com particionamento ortogonal ciclicamente alternado ($\text{profundidade} \bmod k$).
   - Operações: Inserção, Busca, Vizinho Mais Próximo (NNS - *Nearest Neighbor Search*) com poda por hiper-esfera euclidiana, Busca por Alcance (*Range Search*) e Remoção com tratamento semiaberto $[<, \ge)$ via `findMin` e transferência de ramo (Samet/OpenDSA).

---

## 📊 Resultados do Benchmark Empírico

Tempos de execução mensurados em milissegundos com compilação de alta performance (`g++ -O2`):

| Estrutura | Operação | $N = 1.000$ | $N = 10.000$ | $N = 100.000$ |
| :--- | :--- | :---: | :---: | :---: |
| **KD-Tree** | Inserção ($N$) / Busca ($N/10$) | $0,704\text{ ms} / 0,024\text{ ms}$ | $9,153\text{ ms} / 0,290\text{ ms}$ | $183,839\text{ ms} / 5,800\text{ ms}$ |
| **Patricia Trie** | Inserção ($N$) / Busca ($N/10$) | $0,797\text{ ms} / 0,041\text{ ms}$ | $8,490\text{ ms} / 0,505\text{ ms}$ | $118,137\text{ ms} / 6,183\text{ ms}$ |
| **Trie Padrão** | Inserção ($N$) / Busca ($N/10$) | $1,708\text{ ms} / 0,068\text{ ms}$ | $20,848\text{ ms} / 0,676\text{ ms}$ | $209,157\text{ ms} / 7,549\text{ ms}$ |
| **Árvore Splay** | Inserção ($N$) / Busca ($N/10$) | $0,331\text{ ms} / 0,026\text{ ms}$ | $5,715\text{ ms} / 0,591\text{ ms}$ | $107,546\text{ ms} / 7,267\text{ ms}$ |
| **Treap** | Inserção ($N$) / Busca ($N/10$) | $0,325\text{ ms} / 0,009\text{ ms}$ | $4,408\text{ ms} / 0,166\text{ ms}$ | $80,724\text{ ms} / 4,395\text{ ms}$ |

---

## 📁 Estrutura de Arquivos do Repositório

```
.
├── input/                            # Arquivos de entrada de dados customizáveis
│   ├── kdtree_input.txt
│   ├── patricia_input.txt
│   ├── trie_input.txt
│   ├── splay_input.txt
│   └── treap_input.txt
├── output/                           # Saídas geradas dinamicamente
│   ├── dots/                         # Arquivos Graphviz (.dot) separados por estrutura
│   │   ├── kdtree/
│   │   ├── patricia/
│   │   ├── trie/
│   │   ├── splay/
│   │   └── treap/
│   ├── images/                       # Imagens (.png) organizadas em subpastas
│   │   ├── kdtree/
│   │   ├── patricia/
│   │   ├── trie/
│   │   ├── splay/
│   │   └── treap/
│   └── benchmark_results.csv         # Dados brutos dos experimentos em CSV
├── src/                              # Código-fonte e cabeçalhos modulares em C++
│   ├── KDTree/                       # Implementação da KD-Tree (KDTree.hpp, KDTree.cpp)
│   ├── Patricia/                     # Implementação da Patricia Trie
│   ├── Splay/                        # Implementação da Splay Tree
│   ├── Treap/                        # Implementação da Treap
│   ├── Trie/                         # Implementação da Trie
│   ├── KDTree.hpp                    # Cabeçalhos de encaminhamento
│   ├── PatriciaTree.hpp
│   ├── SplayTree.hpp
│   ├── TreapTree.hpp
│   ├── TrieTree.hpp
│   ├── main.cpp                      # Programa interativo de demonstração
│   └── benchmark.cpp                 # Suíte automatizada de testes de desempenho
├── relatorio_tecnico.tex             # Artigo científico completo em LaTeX (IEEEtran)
├── IEEEtran.cls                      # Classe oficial de estilização da IEEE
├── Trabalho_rvores.pdf               # Especificação e diretrizes do trabalho
└── README.md                         # Documentação do repositório
```

---

## 🚀 Como Compilar e Executar

### Pré-requisitos
- Compilador C++ com suporte a **C++17** (`g++` 7.0+ ou `clang++`).
- [Graphviz](https://graphviz.org/) instalado no sistema (opcional, para conversão automática de `.dot` em `.png`).

### 1. Execução do Programa Principal
O programa principal lê os arquivos de comando da pasta `input/`, executa as operações, imprime estatísticas de acesso e memória no console e gera os diagramas `.dot` e imagens `.png` em `output/`:

```bash
g++ -Wall -Wextra -std=c++17 src/main.cpp -o main
./main
```

### 2. Execução da Suíte de Benchmarks
Para rodar os testes de desempenho com $N = 1.000, 10.000, 100.000$ e exportar o CSV de métricas:

```bash
g++ -O2 -std=c++17 src/benchmark.cpp -o benchmark
./benchmark
```

### 3. Compilação do Artigo em LaTeX
Para compilar o artigo técnico em formato PDF:

```bash
pdflatex relatorio_tecnico.tex
```

---

## 📚 Referências Bibliográficas Canônicas

- **Morrison, D. R. (1968)**. *PATRICIA—Practical Algorithm To Retrieve Information Coded in Alphanumeric*. Journal of the ACM (JACM), 15(4), 514-534.
- **Seidel, R., & Aragon, C. R. (1996)**. *Randomized Search Trees*. Algorithmica, 16(4), 464-497.
- **Sleator, D. D., & Tarjan, R. E. (1985)**. *Self-Adjusting Binary Search Trees*. Journal of the ACM (JACM), 32(3), 652-686.
- **Bentley, J. L. (1975)**. *Multidimensional Binary Search Trees Used for Associative Searching*. Communications of the ACM, 18(9), 509-517.
- **Samet, H. (2006)**. *Foundations of Multidimensional and Metric Data Structures*. Morgan Kaufmann.
- **Cormen, T. H. et al. (2009)**. *Introduction to Algorithms* (3rd ed.). MIT Press.
