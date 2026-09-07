#include <iostream>
#include <fstream>
#include <cstdlib>
#include <ctime>

using namespace std;

struct SplayNode{

    int chave;

    SplayNode* esquerda;
    SplayNode* direita;

    SplayNode(int k) : chave(k), esquerda(nullptr), direita(nullptr){}

};

class Splay{

    private:

    int nosAlocados;
    int totalCriados;
    int acessos;
    int rotacoes;

    SplayNode* raiz;

    //Zig
    SplayNode* rotacaoEsquerda(SplayNode* node){

        SplayNode* x = node->direita;
        SplayNode* aux = x->esquerda;

        x->esquerda = node;
        node->direita = aux;

        rotacoes++;
        acessos+=2;

        return x;
    }
    SplayNode* rotacaoDireita(SplayNode* node){

        SplayNode* x= node->esquerda;
        SplayNode* aux = x->direita;

        x->direita = node;
        node->esquerda = aux;

        rotacoes++;
        acessos+=2;

        return x;
    }
    //fim dos zig - mesma implementação de rotação da treap

    SplayNode* inserirRecursivo(SplayNode* node, int chave){}
    SplayNode* removerRecursivo(SplayNode* node){}

    SplayNode* splay(SplayNode* node, int chave){

        //Caso base: subárvore vazia ou chave já esxite na raiz
        if (node == nullptr || node->chave == chave){
            return node;
        }

        //Ramo esquerdo
        if(chave < node->chave){

            //Não existe
            if(node->esquerda == nullptr){
                return node;
            }

            //Zig-Zig
            if(chave < node->esquerda->chave){

                node->esquerda->esquerda = splay(node->esquerda->esquerda, chave);                
                node = rotacaoDireita(node);
            }

            //Zig-Zag
            else if(chave > node->esquerda->chave){

                node->esquerda->direita = splay(node->esquerda->direita, chave);
                if(node->esquerda->direita != nullptr){
                    node->esquerda = rotacaoEsquerda(node->esquerda);
                }
            }
            //Segunda rotação(Zig final para alinhar na raiz da subarvore)
            return(node->esquerda == nullptr) ? node : rotacaoDireita(node);// Se o nó da esquerda for nulo o retorne, caso o contrario faça uma rotação para direita e retorne o resultado
        }

        else{

            //chave não está na árvore
            if(node->direita == nullptr){
                return node;
            }

            //Zig-Zag - (Direita Esquerda)
            if(chave < node->direita->chave){

                node->direita->esquerda = splay(node->direita->esquerda, chave);
                if(node->direita->esquerda != nullptr){
                    node->direita = rotacaoDireita(node->direita);
                }
            }
            
            //Zig Zig (Direita Direita)
            else if(chave > node->direita->chave){

                node->direita->direita = splay(node->direita->direita, chave);

                node = rotacaoEsquerda(node);
            }

            return (node->direita == nullptr) ? node : rotacaoEsquerda(node);
        }
    }

    void clear(SplayNode* node){

        if(node != nullptr){

            clear(node->esquerda);
            clear(node->direita);
            delete node;
            nosAlocados--;
        }
    }

    public:

    Splay() : raiz(nullptr), nosAlocados(0), totalCriados(0), acessos(0), rotacoes(0){}

    ~Splay(){

        clear(raiz);

    }

    void inserir(int chave){

        acessos = 0;
        if(raiz == nullptr){
            raiz = new SplayNode(chave);

            nosAlocados++;
            totalCriados++;
        }

        raiz = splay(raiz, chave);

        if (chave == raiz->chave){
            return;
        }

        SplayNode* novo = new SplayNode(chave);
        nosAlocados++;
        totalCriados++;

        if(chave > raiz->chave){
            novo->direita = raiz;
            novo->esquerda = raiz->esquerda;
            raiz->esquerda = nullptr;
        }
        else{

            novo->esquerda = raiz;
            novo->direita = raiz->direita;
            raiz->direita = nullptr;
        }

        raiz = novo;

    }

    bool buscar(int chave){

        acessos = 0;
        if(raiz == nullptr){
            return false;
        }

        raiz = splay(raiz, chave);

        return (raiz->chave == chave);

    }


    bool remover(int chave){

        acessos = 0;

        if (raiz==nullptr){
            return false;
        }

        raiz = splay(raiz,chave);

        if(raiz->chave != chave){
            return false;
        }

        SplayNode* aux = raiz;

        if(raiz->esquerda == nullptr){
            raiz = raiz->direita;
        }else{
            SplayNode* subDir = raiz->direita;

            raiz = splay(raiz->esquerda, chave);
            raiz->direita = subDir;
        }

        delete aux;
        nosAlocados--;
        return true;

    }

    void estatisticas(){

        cout <<"Nos na memoria: "<< nosAlocados<<endl;
        cout <<"Total de nos criados: "<< totalCriados<<endl;
        cout <<"Acesos na ultima operacao: "<< acessos<<endl;
        cout <<"Total de rotacoes: "<< rotacoes<<endl;

    }

};