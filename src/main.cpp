#include <iostream>
#include <fstream>
#include <sstream>

#include "Gerenciador.h"
using namespace std;

int main(int argc, char *argv[])
{
    
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
            cout << "Grafo " << (ponderado_aresta ? "ponderado nas arestas" : "nao ponderado nas arestas") << endl;
            cout << "Grafo " << (ponderado_vertice ? "ponderado nos vertices" : "nao ponderado nos vertices") << endl;
            cout << "----------------------------------------" << endl;
            // Avança para a leitura de outras informações
            header++;
        }
        // Lendo ordem do grafo
        else if (header == 1) {
            iss >> ordem;
            cout << "Grafo de Ordem: " << ordem << endl;

            header++;
        }
        // Lendo vertices do grafo
        else if (header == 2) {
            iss >> id;
            cout << "Vertice [ " << id << " ] ";

            if(ponderado_vertice) {
                iss >> peso;
                cout << "possui peso igual a " << peso << endl;
            } else cout << endl;

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
    
    Grafo* grafo = new Grafo();
    Gerenciador::comandos(grafo);

    file.close();
    return 0;
}
