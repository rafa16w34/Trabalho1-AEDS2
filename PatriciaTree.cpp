#include <iostream>
#include <fstream>
#include <string>
#include <cctype>
#include <algorithm>

using namespace std;

//-------------------------------------------------------------------------------------------------------------------

struct PatriciaNode{

    string rotulo;
    PatriciaNode* filho[26];
    bool ehPalavra; 

    PatriciaNode(string rot = "", bool finalPalavra = false){

        rotulo = rot;
        ehPalavra = finalPalavra;

        for (int i = 0; i<26; i++){

            filho[i] = nullptr;

        }

    }

};

//----------------------------------------------------------------------------------------------------------------

class Patricia{
 
    private:

        int nosAlocados;
        int totalCriados;
        int acessos;

        PatriciaNode* raiz;
        
        void clear(PatriciaNode* pn){

            for (int i = 0; i<26; i++){

                if (pn->filho[i]){

                    clear(pn->filho[i]);

                }

            }
            delete(pn);

        }

        void tratamento(string& palavra){

            string limpa = "";
            for (char c : palavra) {
                if (isalpha(c)) {
                    limpa += tolower(c);
                }
            }
            palavra = limpa;

        }

        int calcularPrefixoComum(const string& a, const string& b ){
            
            int i = 0;
            int limite = min(a.size() , b.size());
            
            while(i < limite && a[i] == b[i] ){

                i++;

            } 
            return i;
        
        }

        //Inserção Recursiva
        void inserirRecursivo(PatriciaNode* nodeAtual, const string& palavra){

            acessos++;

            //Se a palavra for vazia, marca o node como o fim da palavra
            if (palavra.empty()){

                nodeAtual->ehPalavra = true;
                return ;

            }

            int index = palavra[0] - 'a';
            
            PatriciaNode* filho = nodeAtual->filho[index];

            //Caso 1: Ramo não existe, criar a folha
            if (filho == nullptr){

                acessos++;

                nodeAtual->filho[index] = new PatriciaNode(palavra, true);

                nosAlocados++;
                totalCriados++;

                return ;

            }
            
            //Caso 2: Ramo já existe, obtém o prefixo comum entre a palavra e rotulo do filho
            int k = calcularPrefixoComum(palavra, filho->rotulo);

            //Cenário A:Coincidencia total com o rotulo e o filho
            if (k == filho->rotulo.size()){

                if (k == palavra.size()){

                    acessos++;
                    filho->ehPalavra = true;
                
                }else{

                    inserirRecursivo(filho, palavra.substr(k));

                }

                return ;

            }     

            //Cenário B:Divergencia no meio do rotulo
            string prefixo = filho->rotulo.substr(0,k);
            string fimRotulo = filho->rotulo.substr(k);
            string fimPrefixo = palavra.substr(k);

            index = prefixo[0] - 'a';

            PatriciaNode* meio =new PatriciaNode(prefixo,false);
            nosAlocados++;
            totalCriados++;

            filho->rotulo = fimRotulo;
            int indexFimRotulo = fimRotulo[0]-'a';
            meio->filho[indexFimRotulo] = filho;


            if (fimPrefixo.empty()){

                meio->ehPalavra = true;

            }else{

                int indexFimPrefixo = fimPrefixo[0]-'a';
                meio->filho[indexFimPrefixo] = new PatriciaNode(fimPrefixo,true);

                nosAlocados++;
                totalCriados++;

            }

            nodeAtual->filho[index] = meio;
            return ;
        }

        //Busca Recursiva
        bool BuscarRecursiva(PatriciaNode* nodeAtual, const string& palavra){

            acessos++;

            if(palavra.empty()){

                return nodeAtual->ehPalavra;

            }

            int index = palavra[0] - 'a';
            PatriciaNode* filho = nodeAtual->filho[index];

            if (filho == nullptr){

                return false;

            }

            int k = calcularPrefixoComum(palavra, filho->rotulo);

            if(k<filho->rotulo.size()){

                return false;

            }

            return BuscarRecursiva(filho,palavra.substr(k));

        }

        //Começa Com ... Recursivo
        bool ComecaComRecursivo(PatriciaNode* nodeAtual, const string& prefixo){

            acessos++;

            if(prefixo.empty()){

                return true;

            }

            int index = prefixo[0] - 'a';
            PatriciaNode* filho = nodeAtual->filho[index];
            
            if(filho == nullptr){

                return false;

            }

            int k = calcularPrefixoComum(prefixo, filho->rotulo);

            if(k == prefixo.size()){

                return true;

            }

            if(k == filho->rotulo.size()){

                return ComecaComRecursivo(filho,prefixo.substr(k));

            }

            return false;


        }

    public:

        Patricia(){

            raiz = new PatriciaNode();

            nosAlocados = 1;
            totalCriados = 1;
            acessos = 0;

        }

        ~Patricia(){

            clear(raiz);

        }

        void inserir(string palavra){

            acessos = 0;

            tratamento(palavra);

            if (palavra.empty()) {
                return;
            }

            inserirRecursivo(raiz,palavra);
            
        }

        bool buscar(string palavra){

            acessos = 0;
            tratamento(palavra);

            if(palavra.empty()){
                return false;
            }

            return BuscarRecursiva(raiz,palavra);
            

        }

        bool comecaCom(string palavra){

                        acessos = 0;
            tratamento(palavra);

            if(palavra.empty()){
                return false;
            }

            return ComecaComRecursivo(raiz,palavra);

        }

        void remover(){

        }

        void estatisticas(){

        }

};