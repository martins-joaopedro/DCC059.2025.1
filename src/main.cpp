#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>

#include "Gerenciador.h"
#include "No.h"
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
            cout << "TESTE DIRECIONADO: " << grafo->in_direcionado << endl;

            cout << "Grafo " << (ponderado_aresta ? "ponderado nas arestas" : "nao ponderado nas arestas") << endl;
            grafo->in_ponderado_aresta = ponderado_aresta;
            cout << "TESTE ponderado atesta: " << grafo->in_ponderado_aresta << endl;
            
            cout << "Grafo " << (ponderado_vertice ? "ponderado nos vertices" : "nao ponderado nos vertices") << endl;
            grafo->in_ponderado_vertice = ponderado_vertice;
            cout << "TESTE ponderado vertice: " << grafo->in_ponderado_vertice << endl;

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
            cout << "Vertice [ " << no->id << " ] ";

            if(ponderado_vertice) {
                iss >> peso;
                
                no->peso = peso;
                cout << "possui peso igual a " << no->peso << endl;
            } else {
                no->peso = -1; //colocar null ou -1 ou 0????????
                cout << endl;
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



            cout << "Existe uma aresta de " << id_no_a << " para " << id_no_b;

            if(ponderado_aresta) {
                iss >> peso;
                cout << " com peso: " << peso << endl;
            }
            else cout << endl;
        }        
    }
    cout << "TESTE LISTA: " ;
    for(No* no : grafo->lista_adj ){
        cout << no->id << " peso vertice: " << no->peso << endl;
    }
    
    Gerenciador::comandos(grafo);

    file.close();
    return 0;
}
