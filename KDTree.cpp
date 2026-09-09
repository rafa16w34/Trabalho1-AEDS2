#include <iostream>
#include <vector>
#include <cmath>
#include <limits>
#include <fstream>

using namespace std;

struct Ponto{

    vector<double> coords;

    Ponto(){};
    Ponto(const vector <double>& c) : coords(c) {};

    double operator [] (int i) const{
        return coords[i];
    }

    double& operator[](int i){
        return coords[i];
    }

    size_t dimensao() const{
        return coords.size();
    }

    bool operator==(const Ponto& outro) const{
        return coords == outro.coords;
    }
};

struct KDNode{

    Ponto ponto;
    KDNode* esquerda;
    KDNode* direita;

    KDNode(Ponto p) : ponto(p), esquerda(nullptr), direita(nullptr){}

};

class KDTree{

    private:

    int acessos;
    int nosAlocados;
    int totalCriados;
    
    int kDimensoes;

    KDNode* raiz;

    KDNode* inserirRecursivo(KDNode* node, const Ponto& p, int profundidade){

        acessos++;

        if(node == nullptr){
            nosAlocados++;
            totalCriados++;
            return new KDNode(p);
        }

        if (node->ponto == p){
            return node;
        }

        int eixo = profundidade % kDimensoes;

        //Compara a coordenada correspondente com a dimensão atual
        if (p[eixo] < node->ponto[eixo]){
            node->esquerda = inserirRecursivo(node->esquerda, p, profundidade + 1);
        }else{
            node->direita = inserirRecursivo(node->direita, p, profundidade + 1);
        }

        return node;

    }

    double distanciaQuadrada(const Ponto& a, const Ponto& b){

        double soma = 0.0;

        for(int i = 0; i < kDimensoes; i++){
            double diff = a[i] - b[i];
            soma +=  diff * diff;
        }
        return soma;
    }

    void vizinhoMaisProximoRecursivo(KDNode* node, const Ponto& alvo, int profundidade, Ponto& melhorPonto, double& melhorDistQuadrada){

        if(node == nullptr) return;

        acessos++;

        double distAtual = distanciaQuadrada(node->ponto, alvo);

        if(distAtual<melhorDistQuadrada){
            melhorDistQuadrada = distAtual;
            melhorPonto = node->ponto;
        }

        int eixo = profundidade % kDimensoes;
        KDNode* proximoRamo = nullptr;
        KDNode* outroRamo = nullptr;
    
        //Distancia unidimensional perpendicular ao hiperplano de corte
        double diferencaEixo = alvo[eixo] - node->ponto[eixo];

        if (diferencaEixo < 0){

            proximoRamo = node->esquerda;
            outroRamo = node->direita;
        
        }else{

            proximoRamo = node->direita;
            outroRamo = node->esquerda;
        }

        //Explora lado promissor
        vizinhoMaisProximoRecursivo(proximoRamo, alvo, profundidade + 1, melhorPonto, melhorDistQuadrada);

        //Poda 
        if ((diferencaEixo * diferencaEixo) < melhorDistQuadrada){

            vizinhoMaisProximoRecursivo(outroRamo, alvo, profundidade + 1, melhorPonto, melhorDistQuadrada);

        }
    }

//-----------------------------------------------------------------------------------------------------------------------------------------------------------

    KDNode* encontrarMinimo(KDNode* node, int dimAlvo, int profundidade) {
        if (node == nullptr) return nullptr;

        acessos++;
        int eixoAtual = profundidade % kDimensoes;

        // Se o corte atual for na dimensão alvo, o menor só pode estar à esquerda
        if (eixoAtual == dimAlvo) {
            if (node->esquerda == nullptr) return node;
            return encontrarMinimo(node->esquerda, dimAlvo, profundidade + 1);
        }

        // Caso contrário, o menor pode estar em qualquer um dos lados; precisa checar ambos
        KDNode* minEsq = encontrarMinimo(node->esquerda, dimAlvo, profundidade + 1);
        KDNode* minDir = encontrarMinimo(node->direita, dimAlvo, profundidade + 1);

        KDNode* menor = node;
        if (minEsq != nullptr && minEsq->ponto[dimAlvo] < menor->ponto[dimAlvo]) {
            menor = minEsq;
        }
        if (minDir != nullptr && minDir->ponto[dimAlvo] < menor->ponto[dimAlvo]) {
            menor = minDir;
        }
        return menor;
    }

    KDNode* removerRecursivo(KDNode* node, const Ponto& p, int profundidade, bool& removido) {
        if (node == nullptr) return nullptr;

        acessos++;
        int eixo = profundidade % kDimensoes;

        // Ponto encontrado
        if (node->ponto == p) {
            removido = true;

            // Caso 1: Tem subárvore direita
            if (node->direita != nullptr) {
                // Encontra o menor na dimensão do corte atual dentro da subárvore direita
                KDNode* minDir = encontrarMinimo(node->direita, eixo, profundidade + 1);
                node->ponto = minDir->ponto; // Substitui os dados
                node->direita = removerRecursivo(node->direita, minDir->ponto, profundidade + 1, removido);
            }
            // Caso 2: Não tem direita, mas tem subárvore esquerda
            else if (node->esquerda != nullptr) {
                KDNode* minEsq = encontrarMinimo(node->esquerda, eixo, profundidade + 1);
                node->ponto = minEsq->ponto;
                
                // Transfere o ramo para a direita e anula o ramo esquerdo ANTES de descer
                node->direita = node->esquerda;
                node->esquerda = nullptr;
                
                // Remove o nó duplicado agora a partir do ramo direito
                node->direita = removerRecursivo(node->direita, minEsq->ponto, profundidade + 1, removido);
            }
            // Caso 3: É um nó folha
            else {
                delete node;
                nosAlocados--;
                return nullptr;
            }
            return node;
        }

        // Descida conforme a coordenada do corte
        if (p[eixo] < node->ponto[eixo]) {
            node->esquerda = removerRecursivo(node->esquerda, p, profundidade + 1, removido);
        } else {
            node->direita = removerRecursivo(node->direita, p, profundidade + 1, removido);
        }

        return node;
    }

    void clear(KDNode* node) {
        if (node != nullptr) {
            clear(node->esquerda);
            clear(node->direita);
            delete node;
            nosAlocados--;
        }
    }

    void gerarDotRecursivo(KDNode* node, int profundidade, ofstream& arquivo) {
        if (node == nullptr) return;

        int eixo = profundidade % kDimensoes;

        arquivo << "    node" << node << " [label=\"(";
        for (size_t i = 0; i < node->ponto.dimensao(); i++) {
            arquivo << node->ponto[i] << (i + 1 < node->ponto.dimensao() ? ", " : "");
        }
        arquivo << ") | Eixo " << eixo << "\"];\n";

        if (node->esquerda != nullptr) {
            arquivo << "    node" << node << " -> node" << node->esquerda << " [label=\"<\"];\n";
            gerarDotRecursivo(node->esquerda, profundidade + 1, arquivo);
        }
        if (node->direita != nullptr) {
            arquivo << "    node" << node << " -> node" << node->direita << " [label=\">=\"];\n";
            gerarDotRecursivo(node->direita, profundidade + 1, arquivo);
        }
    }

//-----------------------------------------------------------------------------------------------------------------------------------------------------------

    bool buscarRecursivo(KDNode* node, const Ponto& p, int profundidade){

        acessos++;

        if(node == nullptr)return false;
        if(node->ponto == p)return true;

        int eixo = profundidade % kDimensoes;

        if(p[eixo] < node->ponto[eixo]){
            return buscarRecursivo(node->esquerda, p, profundidade + 1);
        }else{
            return buscarRecursivo(node->direita, p, profundidade + 1);
        }

    }

    


    public:

        KDTree(int k) : raiz(nullptr), kDimensoes(k), nosAlocados(0), totalCriados(0), acessos(0){};
        
        ~KDTree(){

            clear(raiz);

        }

        void inserir(const vector<double>& coords){

            if (coords.size() != static_cast<size_t>(kDimensoes)) return ;

            acessos = 0;

            Ponto p(coords);
            raiz = inserirRecursivo(raiz,p,0);
        }

        bool buscar(const vector<double>& coords){

            if (coords.size() != static_cast<size_t>(kDimensoes)) return false ;
            acessos = 0;
            return buscarRecursivo(raiz, Ponto(coords), 0);

        }

//-----------------------------------------------------------------------------------------------------------------------------------------------------------
    
        bool remover(const vector<double>& coords) {
            if (coords.size() != static_cast<size_t>(kDimensoes) || raiz == nullptr) {
                return false;
            }
            acessos = 0;
            bool removido = false;

            Ponto p(coords);
            raiz = removerRecursivo(raiz,p,0,removido);
            return removido;
        }

        void exportarDOT(const string& nomeArquivo) {
            ofstream arq(nomeArquivo);
            if (!arq.is_open()) return;

            arq << "digraph KDTree {\n";
            arq << "    node [shape=record, style=filled, fillcolor=lightgreen];\n";
            if (raiz != nullptr) {
                gerarDotRecursivo(raiz, 0, arq);
            }
            arq << "}\n";
            arq.close();
        }

//-----------------------------------------------------------------------------------------------------------------------------------------------------------

        vector<double> vizinhosProximos(const vector<double>& coords){

            acessos = 0;

            if(raiz == nullptr || coords.size() != static_cast<size_t>(kDimensoes)){
                return{};
            }

            Ponto alvo(coords);
            Ponto melhorPonto = raiz->ponto;
            double melhorDist = distanciaQuadrada(raiz->ponto, alvo);

            vizinhoMaisProximoRecursivo(raiz, alvo, 0, melhorPonto, melhorDist);
            return melhorPonto.coords;

        }

        void estatisticas(){

        cout <<"Nos na memoria: "<< nosAlocados<<endl;
        cout <<"Total de nos criados: "<< totalCriados<<endl;
        cout <<"Acesos na ultima operacao: "<< acessos<<endl;

        }

};

