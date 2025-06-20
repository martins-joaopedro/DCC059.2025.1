#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>

#include "Gerenciador.h"
using namespace std;

int main(int argc, char *argv[]) {
    
    Grafo* grafo = new Grafo();

    if(argc < 2) {
        cout << "Nenhum arquivo de entrada foi informado." << endl;
        cout << "Execucao finalizada!" << endl;
        return 0;
    }
    
    // So lê um arquivo de entrada por vez?
    string file_name = argv[1];
    fstream file = fstream(file_name);
    
    if (!file.is_open()) {
        cout << "Erro ao abrir o arquivo: " << file_name << endl;
        return 1;
    }

    int header = 0;
    string line;

    int direcionado, ponderado_aresta, ponderado_vertice;
    int ordem, peso;
    int k = 0; // contador de vertices lidos
    char id, id_no_a, id_no_b; 

    while (getline(file, line, '\n'))  {
        istringstream iss(line);
        
        // Lendo informações iniciais do grafo
        if (header == 0) {
            cout << "Cabecalho do Grafo: " << endl;
            iss >> direcionado >> ponderado_aresta >> ponderado_vertice;

            cout << "Grafo " << (direcionado ? "direcionado" : "nao direcionado") << endl;
            grafo->in_direcionado = direcionado;

            cout << "Grafo " << (ponderado_aresta ? "ponderado nas arestas" : "nao ponderado nas arestas") << endl;
            grafo->in_ponderado_aresta = ponderado_aresta;
            
            cout << "Grafo " << (ponderado_vertice ? "ponderado nos vertices" : "nao ponderado nos vertices") << endl;
            grafo->in_ponderado_vertice = ponderado_vertice;

            cout << "----------------------------------------" << endl;
            // Avança para a leitura de outras informações
            header++;
        }
        // Lendo ordem do grafo
        else if (header == 1) {
            iss >> ordem;

            grafo->ordem = ordem;
            cout << "Grafo de Ordem: " << grafo->ordem << endl;

            header++;
        }
        // Lendo vertices do grafo
        else if (header == 2) {
            iss >> id;

            No* no = new No();//novo no
            no->id = id;

            if(ponderado_vertice) {
                iss >> peso;
                no->peso = peso;
            } else {
                no->peso = 1;
            }

            grafo->lista_adj.push_back(no); //adiciona novo vertice no fim da lista

            // Lê todos os vertices
            k++;
            if(k == ordem)
                header++;
        }
        
        // Lendo arestas do grafo
        else if (header == 3) {
            iss >> id_no_a >> id_no_b;

            Aresta* aresta = new Aresta();
            
            //adiciona aresta de a pra b
            for(No* no : grafo->lista_adj ){
                if(no->id == id_no_a){                        
                    aresta->id_no_alvo = id_no_b;
                    no->arestas.push_back(aresta);
                }
            }

            if(ponderado_aresta) {
                iss >> peso;
                aresta->peso = peso;
            } else {
                aresta->peso = 1;
            }
            
            //se nao direcionado, adiciona aresta de b pra a
            if(!grafo->in_direcionado){
                Aresta* aresta_b = new Aresta();
                
                for(No* no : grafo->lista_adj){
                    if(no->id == id_no_b){                        
                        aresta_b->id_no_alvo = id_no_a;
                        aresta_b->peso = aresta->peso;
                        no->arestas.push_back(aresta_b);
                    }
                }
            }
        }        
    }

    // cout << "\nLISTA DE ADJ: " << endl ;
    // for(No* no : grafo->lista_adj ){
    //     cout << "Vertice: [" << no->id << "] com peso: " << no->peso << endl;

    //     for(Aresta* aresta : no->arestas){
    //         cout << "   Existe uma aresta de [" << no->id << "] para [" << aresta->id_no_alvo
    //             << "] com peso: " << aresta->peso << endl;
    //     }
    //     cout << endl;
    // }
    
    // TODO: Finalizar corrigindo o salvamento de acordo com os formatos especificados
    Gerenciador::salvar_lista(grafo->lista_adj, "TESTE_lista_adj.txt");
    Gerenciador::salvar_grafo(grafo, "TESTE_grafo.txt");

    Gerenciador::comandos(grafo);

    file.close();
    return 0;
}
