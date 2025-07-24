#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <map>
#include <set>

#include "Gerenciador.h"
using namespace std;

Grafo* ler_grafo(string file_name) {
    
    Grafo* grafo = new Grafo();
    
    fstream file = fstream("../instancias/"+file_name);
    
    if (!file.is_open()) {
        cout << "Erro ao abrir o arquivo: " << file_name << endl;
        return nullptr;
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

    file.close();

    return grafo;
} 

vector<char> heuristic(Grafo* grafo) {
    vector<char> LC = vector<char>();
    map<char, int> grau;
    
    // calcula o grau de cada nó
    for(No* no : grafo->lista_adj) {
        for(Aresta* aresta : no->arestas) {
            if(grau.find(no->id) != grau.end())
                grau[no->id]++; 
            else grau[no->id] = 1; // inicializa o grau do nó se não existir
        }
    }

    // ordena os nós por grau e monta a LC
    while (!grau.empty()) {
        auto max_it = grau.begin();
        for (auto it = grau.begin(); it != grau.end(); ++it) {
            if (it->second > max_it->second)
                max_it = it;
            
        }

        LC.push_back(max_it->first);
        cout << "Escolhido: " << max_it->first << " com grau: " << max_it->second << endl;

        // remove do map
        grau.erase(max_it);
    }

    return LC;
}

// lógica adaptativa de atualização da lista LC 
vector<char> updates_LC(map<char, No*>& mapa_nos, vector<char> LC, vector<char> S) {
    
    set<char> neighbourhood = set<char>();

    // critério de atualização: removo os nós vizinhos de v
    for (char v : S) {
        if (mapa_nos.find(v) != mapa_nos.end()) {
            No* no = mapa_nos[v];
            for (Aresta* aresta : no->arestas) {
                char vizinho = aresta->id_no_alvo;
                neighbourhood.insert(vizinho);
            }
        }
    }

    vector<char> new_LC = vector<char>();

    for (char v : LC) {
        // se ele for vizinho de algum nó em S, não o adiciono
        if(find(neighbourhood.begin(), neighbourhood.end(), v) != neighbourhood.end())
            continue;
        
        new_LC.push_back(v);
    }

    return new_LC;
}

// algoritmo adaptativo guloso randomizado
void randomized_adaptative_greedy(Grafo* grafo, int randomized) {
    
    map<char, No*> mapa_nos;

    for (No* no : grafo->lista_adj) {
        mapa_nos[no->id] = no;
    }

    // euristica: ordenar os candidatos por grau de forma crescente
    vector<char> LC = heuristic(grafo);

    // solução
    vector<char> S = vector<char>(); 

    while(!LC.empty()) {

        char no = LC[0];
        cout << "Escolhendo no: " << no << endl;

        S.push_back(no);

        LC.erase(LC.begin()); // apago no em questão
        LC = updates_LC(mapa_nos, LC, S);

        cout << "S atual" << endl;
        for (char id : S)
            cout << id << " ";
        cout << endl;

        cout << "LC atual: " << endl;
        for (char v : LC)
            cout << v << " ";
        cout << endl;
    }

    cout << "Solucao encontrada: ";
    for (char id : S) {
        cout << id << " ";
    }
}

void run(Grafo* grafo) {
    randomized_adaptative_greedy(grafo, 0);
}

int main(int argc, char *argv[]) {
    
    string path = "../instancias/teste.txt";

    Grafo * grafo = ler_grafo(path);
    Gerenciador::imprimir_grafo(grafo);
    
    run(grafo);
    delete grafo;
            
    return 0;
}