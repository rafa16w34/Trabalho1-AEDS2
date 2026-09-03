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

    void tratamento(string& word){

            //Trata a palavra para ficar com letras minúsculas

            transform(word.begin(), word.end(), word.begin(), [](unsigned char c) {
                return std::tolower(c);
            });


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

        void tratamento(string palavra){

            transform(palavra.begin(), palavra.end(), palavra.begin(), [](unsigned char c) {
                return tolower(c);
            });


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

            tratamento(palavra);

            if (palavra.empty()) {
                return;
            }

            int index = palavra[0] - 'a';
            PatriciaNode* pn = raiz->filho[index];

            if (pn == nullptr) {

                raiz->filho[index] = new PatriciaNode(palavra, true); //se tiver vazio já isere a palavra

                nosAlocados++;
                totalCriados++;


            }else{
                
                string prefixo = "";
                bool divergiu = false;
                
                int limite = min(palavra.size(), pn->rotulo.size());

                for (int i = 0; i < limite; i++){                    
                   
                    if(palavra[i] == pn->rotulo[i]){

                        prefixo += palavra[i]; 

                    }
                    else{

                        divergiu = true;
                        break;

                    }
                
                }

                string fimPrefixo = palavra;
                string fimRotulo = pn->rotulo;

                fimPrefixo.erase(0, prefixo.length());
                fimRotulo.erase(0, prefixo.length());


                PatriciaNode* antigo = new PatriciaNode(fimRotulo, pn->ehPalavra);  //Cria um filho que vai herdar a palavra antiga, e por isso os filhos antigo vão ser seu filhos
                for(int i = 0; i < 26; i++){
                    antigo->filho[i] = pn->filho[i];
                }

                PatriciaNode* novo   = new PatriciaNode(fimPrefixo, true);  //Cria um filho novo com a palavra recém colocada

                for (int i = 0; i < 26; i++) {  //Limpar os filhos de pn     
                    pn->filho[i] = nullptr;
                }

                pn->rotulo = prefixo;   //O nó original agora é só o prefixo e agora ele não é mais o final da palavra
                pn->ehPalavra = false;

                int indexAntigo = fimRotulo[0] - 'a';
                int indexNovo = fimPrefixo[0] - 'a';

                pn->filho[indexAntigo] = antigo;
                pn->filho[indexNovo] = novo;

                nosAlocados += 2;
                totalCriados += 2;

            }   

            
        }

        bool buscar(){

        }

        bool comecaCom(){

        }

        void remover(){

        }

        void estatisticas(){

        }

};