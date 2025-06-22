#include "Grafo.h"
#include <vector>
#include <stack>
#include <map>
#include <queue>
#include <iostream>

using namespace std;


Grafo::Grafo() {
}

Grafo::~Grafo() {
}

vector<char> Grafo::fecho_transitivo_direto(int id_no) {
    cout<<"Metodo nao implementado"<<endl;
    return {};
}

vector<char> Grafo::fecho_transitivo_indireto(int id_no) {
    
    vector<char> saida;
    queue<No*> fila;
    map<char, bool> marcados;

    if(!in_direcionado) {
        cout << "Grafo nao direcionado!" << endl;
        return saida;
    }

    // busca pelo nó inicial
    for(No* no : lista_adj)
        if(no->id == id_no) {
            fila.push(no);
            marcados[no->id] = true;
            break;
        }
    
    while(!fila.empty()) {

        No* no_atual = fila.front();
        fila.pop();

        // encontra todas as arestas que vão pro no atual
        for(No* no : lista_adj) {
            for (Aresta* aresta : no->arestas) {
                if(aresta->id_no_alvo == no_atual->id) {
                    if(!marcados[no->id]) {
                        // significa que esse vertice vai pro nó atual
                        fila.push(no); 
                        saida.emplace_back(no->id);    
                        marcados[no->id] = true; // evita repetição
                    }
                }
            }
        }
    }

    return saida;
}

vector<char> Grafo::caminho_minimo_dijkstra(int id_no_a, int id_no_b) {
    cout<<"Metodo nao implementado"<<endl;
    return {};
}

vector<char> Grafo::caminho_minimo_floyd(int id_no, int id_no_b) {
    cout<<"Metodo nao implementado"<<endl;
    return {};
}

Grafo * Grafo::arvore_geradora_minima_prim(vector<char> ids_nos) {
    cout<<"Metodo nao implementado"<<endl;
    return nullptr;
}

Grafo * Grafo::arvore_geradora_minima_kruskal(vector<char> ids_nos) {
    cout<<"Metodo nao implementado"<<endl;
    return nullptr;
}

Grafo * Grafo::arvore_caminhamento_profundidade(int id_no) {
    
    map<char, bool> visitados;
    map<char, int> profundidades;

    for(No* no : lista_adj) {
        visitados[no->id] = false;
        profundidades[no->id] = 0; 
    }
    
    // mantenho configurações do grafo original
    Grafo* grafo_saida = new Grafo();
        grafo_saida->ordem = ordem;
        grafo_saida->in_direcionado = in_direcionado;
        grafo_saida->in_ponderado_aresta = in_ponderado_aresta; 
        grafo_saida->in_ponderado_vertice = in_ponderado_vertice;

    // percorre até encontrar o no alvo para iniciar 
    for(No* no : lista_adj)
        if(no->id == id_no) 
            return caminho_profundidade(no, grafo_saida, visitados, id_no, profundidades);
    
    return grafo_saida; 
}

Grafo * Grafo::caminho_profundidade(No* no, Grafo* grafo_saida, map<char, bool>& visitados, char id_no_pai, map<char, int>& profundidades) {

    No* no_saida = new No();
    no_saida->id = no->id;
    no_saida->peso = no->peso;

    visitados[no->id] = true; 
    profundidades[no->id] = profundidades[id_no_pai] + 1; 

    // pra cada aresta, adiciono os aprofundamentos
    for(Aresta* aresta : no->arestas) {

        // ignora aresta que leva de volta ao pai
        if(aresta->id_no_alvo == id_no_pai)
            continue; 

        // encontra no alvo da aresta
        for(No* no_alvo : lista_adj) {
            if(no_alvo->id == aresta->id_no_alvo) {
                if(!visitados[no_alvo->id]) {
                    // chama recursivamente para o nó alvo
                    caminho_profundidade(no_alvo, grafo_saida, visitados, no->id, profundidades); 

                    Aresta* nova_aresta = new Aresta();
                        nova_aresta->id_no_alvo = no_alvo->id;
                        nova_aresta->peso = aresta->peso; 
                        nova_aresta->retorno = false; 
                    no_saida->arestas.push_back(nova_aresta);
                    
                // marca como aresta de retorno caso esteja num nivel menor e não seja o pai -> melhoria pra grafos direcionados e não direcionados
                } else if(id_no_pai != no_alvo->id and profundidades[no->id] > profundidades[no_alvo->id]) {
                    Aresta* nova_aresta = new Aresta();
                        nova_aresta->id_no_alvo = no_alvo->id;
                        nova_aresta->peso = aresta->peso;
                        nova_aresta->retorno = true;
                    no_saida->arestas.push_back(nova_aresta);
                } 

                break;
            }   
        }
    }

    grafo_saida->lista_adj.push_back(no_saida);

    return grafo_saida;
}

int Grafo::raio() {
    cout<<"Metodo nao implementado"<<endl;
    return 0;
}

int Grafo::diametro() {
    cout<<"Metodo nao implementado"<<endl;
    return 0;
}

vector<char> Grafo::centro() {
    cout<<"Metodo nao implementado"<<endl;
    return {};
}

vector<char> Grafo::periferia() {
    cout<<"Metodo nao implementado"<<endl;
    return {};
}

vector<char> Grafo::vertices_de_articulacao() {
    cout<<"Metodo nao implementado"<<endl;
    return {};
}
