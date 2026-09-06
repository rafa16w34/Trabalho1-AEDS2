#include <iostream>
#include <fstream>
#include <cstdlib>
#include <ctime>

using namespace std;


struct TreapNode{

    int chave;
    int prioridade;
    TreapNode* esquerda;
    TreapNode* direita;

    TreapNode(int k, int p) : chave(k), prioridade(p), esquerda(nullptr), direita(nullptr) {}
    //evita que as variaveis permanecam com lixo de memória

};


class Treap{

    private:

    int nosAlocados;
    int totalCriados;
    int acessos;
    int rotacoes;

    TreapNode* raiz;

    TreapNode* rotacaoEsquerda(TreapNode* node){

        TreapNode* x = node->direita;
        TreapNode* aux = x->esquerda;

        x->esquerda = node;
        node->direita = aux;

        rotacoes ++;
        acessos +=2;

        return x;
        

    }
    TreapNode* rotacaoDireita(TreapNode* node){

        TreapNode* x = node->esquerda;
        TreapNode* aux = x->direita;

        x->direita = node;
        node->esquerda = aux;

        rotacoes ++;
        acessos +=2;

        return x;

    }

    TreapNode* inserirRecursivo(TreapNode* node, int chave, int prioridade){

        acessos++;

        //Caso Base - folha vazia
        if (node == nullptr){

            nosAlocados++;
            totalCriados++;

            return new TreapNode(chave, prioridade);
        }

        //Descida BST
        if (chave < node->chave){

            node->esquerda = inserirRecursivo(node->esquerda, chave, prioridade);

            if(node->esquerda->prioridade > node->prioridade){

                node = rotacaoDireita(node);

            }
        }
        else if( chave > node->chave){

            node->direita = inserirRecursivo(node->direita, chave, prioridade);

            if(node->direita->prioridade > node->prioridade){

                node = rotacaoEsquerda(node);

            }

        }

        return node;

    }

    TreapNode* removerRecursivo(TreapNode* node, int chave){

        acessos ++;

        //Caso Base - Nn encontrada
        if(node == nullptr){
            return nullptr;
        }

        //Busca pela chave
        if(chave < node->chave){
            node->esquerda = removerRecursivo(node->esquerda, chave);
        }else if(chave > node->chave){
            node->direita = removerRecursivo(node->direita, chave);
        }
        else{

            if(node->esquerda == nullptr){

                TreapNode* aux = node->direita;
                delete node;
                nosAlocados --;
                return aux;

            }else if(node->direita == nullptr){

                TreapNode* aux = node->esquerda;
                delete node;
                nosAlocados--;
                return aux;

            }

            if( node->esquerda->prioridade > node->direita->prioridade){

                node = rotacaoDireita(node);
                node->direita = removerRecursivo(node->direita,chave);

            }else{

                node = rotacaoEsquerda(node);
                node->esquerda = removerRecursivo(node->esquerda,chave);
            }
        }

        return node;

    }
    
    void clear(TreapNode* node){
        if (node != nullptr){
            clear(node->esquerda);
            clear(node->direita);
            delete node;
            nosAlocados--;
        }
    }
    
    public:

 Treap() : raiz(nullptr), nosAlocados(0), totalCriados(0), acessos(0) {
        srand(time(nullptr));
    }

    ~Treap(){

        clear(raiz);

    }

    void inserir(int chave){

        acessos = 0;
        int prioridade = rand();
        raiz = inserirRecursivo(raiz, chave, prioridade);   

    }

    bool buscar(int chave){

        acessos = 0;

        TreapNode* node = raiz;

        while (node != nullptr){

            acessos++;

            if(chave == node->chave){

                return true;

            }else if( chave < node->chave){

                node = node->esquerda;

            }else{

                node = node->direita;

            }
        }

        return false;

    }

    void remover(int chave){

        acessos = 0;
        raiz = removerRecursivo(raiz, chave);

    }

    void estatisticas(){

        cout << "Nos na memoria: " << nosAlocados << endl;
        cout << "Total de nos criados: " << totalCriados << endl;
        cout << "Acessos na ultima operacao: " << acessos << endl;
        cout << "Total de rotações: " << rotacoes << endl;

    }

};