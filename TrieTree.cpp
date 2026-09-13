#include <iostream>
#include <fstream>
#include <string>
#include <cctype>
#include <algorithm>

using namespace std;

struct TrieNode{

    TrieNode *children[26];
    bool isWord;

    TrieNode(){
        for (auto &c : children) {
            c = nullptr;
        }
        isWord = false;
    }
};

class Trie{

    private:

        int nosAlocados;
        int totalCriados;

        int acessos;

        TrieNode *root;

        void clear(TrieNode *tn){

            for(int i = 0; i < 26; i++){
                if(tn->children[i]){
                    clear( tn->children[i] );
                }
            }
            delete(tn);
        }

    //-------------------------------------------

        TrieNode* find(string &s){

            TrieNode* tn = root;

            acessos++;

            for(auto &c : s){

                if(!tn->children[c - 'a']){
                    return nullptr;
                }

                tn = tn->children[c - 'a'];

                acessos++;
            }

            return tn;
        }


        bool temFilho(TrieNode* node){

            for (int i = 0; i < 26; i++) {
                if (node->children[i] != nullptr) {
                    return true;
                }
            }

            return false;

        }

        bool remover(TrieNode* node, string& word, int depth){

            if (!node){
                return false;
            }

            acessos++;

            if (depth == word.size()){

                node->isWord = false;
                return !temFilho(node);

            }

            char c = word[depth];
            int index = c - 'a';

            if(remover(node->children[index], word, depth +1)){

                delete node->children[index];
                node->children[index] = nullptr;

                nosAlocados--;
            }

            return !node->isWord && !temFilho(node);

        }

        void tratamento(string& word){

            //Trata a palavra para ficar com letras minúsculas

            transform(word.begin(), word.end(), word.begin(), [](unsigned char c) {
                return std::tolower(c);
            });


        }

    public:

        Trie(){ //Constructor

            // Complexidade de espaço = O(M*N); M = Maior tamanho de uma palavra ; N = Quantidade de palavras

            root = new TrieNode();

            nosAlocados = 1;     
            totalCriados = 1;   

            acessos = 0;

        }
    
    //------------------------------------------------------

        ~Trie(){ //Destructor

            clear(root);

        }
    
    //------------------------------------------------------

        void insert(string word){
            //Complexidade de tempo = O(n); n = tamanho da palavra

            acessos = 0;

            tratamento(word);   

            TrieNode *tn = root;

            acessos++; 

            for (auto &c : word){

                if (!tn->children[c - 'a']){

                    tn->children[c - 'a'] = new TrieNode();

                    nosAlocados++;
                    totalCriados++;

                }
                tn = tn->children[c - 'a'];
                acessos++;      
            }
            tn->isWord = true;

            

        }

    //------------------------------------------------------

        bool search(string word){

            acessos = 0;

            tratamento(word); 

            TrieNode *end = find(word);

            if (end && end->isWord){
                return true;
            }
            return false;

        }

    //------------------------------------------------------

        bool startsWith(string prefix){

            acessos = 0;

            tratamento(prefix); 

            if (find(prefix)){
                return true;
            }
            return false;

        }

    //------------------------------------------------------

        void remover(string word){

            acessos = 0;

            tratamento(word); 

            remover(root,word,0);

            
            
        }

        void estatisticas() {
            cout << "Nos na memoria: " << nosAlocados << endl;
            cout << "Total de nos criados: " << totalCriados << endl;
            cout << "Acessos na ultima operacao: " << acessos << endl;
        }


};