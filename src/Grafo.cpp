#include "Grafo.h"
#include <tuple>
#include <limits>
#include <map>
#include <set>
#include <unordered_map>
#include <algorithm>
#include <queue>
#include <vector>
#include <functional> //
#define INFINITO = numeric_limits<int>::max();

Grafo::Grafo() {
}

Grafo::~Grafo() {
}

vector<char> Grafo::fecho_transitivo_direto(int id_no) {
    cout<<"Metodo nao implementado"<<endl;
    return {};
}

vector<char> Grafo::fecho_transitivo_indireto(int id_no) {
    cout<<"Metodo nao implementado"<<endl;
    return {};
}

vector<char> Grafo::caminho_minimo_dijkstra(int id_no_a, int id_no_b) {
    cout<<"Metodo nao implementado"<<endl;
    return {};
}

vector<char> Grafo::caminho_minimo_floyd(int id_no, int id_no_b) {
    cout<<"Metodo nao implementado"<<endl;
    return {};
}

bool Grafo::insere_aresta(tuple<int, char, char>& aresta_info, vector<No*>& lista){
    Aresta* aresta = new Aresta();
    int cont = 0;
    
    for(No* no : lista){
        if(no->id == get<1>(aresta_info)){
            aresta->peso = get<0>(aresta_info);
            aresta->id_no_alvo = get<2>(aresta_info);
            no->arestas.push_back(aresta);
            cont++;
        }
        else if(no->id == get<2>(aresta_info)){
            Aresta* aresta_b = new Aresta();
            aresta_b->id_no_alvo = get<1>(aresta_info);
            aresta_b->peso = get<0>(aresta_info);
            no->arestas.push_back(aresta_b);
            cont++;
        }
        else if(cont == 2){break;}
    }
    
    if(cont != 2){//grafo desconexo
        return false;
    }

    return true;
}

Grafo * Grafo::arvore_geradora_minima_prim(vector<char> ids_nos) {
    if(!this->in_ponderado_aresta){
        cout << "O grafo não possui pesos nas arestas." << endl;
        return nullptr;
    }

    if(this->in_direcionado){
        cout << "O grafo é direcionado." << endl;
        return nullptr;
    }

    if (this->lista_adj.empty()) {
        cout << "Grafo vazio." << endl;
        return nullptr;
    }

    Grafo* arvore = new Grafo();
    arvore->in_ponderado_aresta = this->in_ponderado_aresta;
    arvore->in_direcionado = this->in_direcionado;
    arvore->in_ponderado_vertice = this->in_ponderado_vertice;
    arvore->ordem = ids_nos.size();  

    for(char id : ids_nos){//preenche lista_adj da arvore
        for(No* no : this->lista_adj){
            if(no->id == id){
                No* novo_no = new No();
                novo_no->id = no->id;
                novo_no->peso = no->peso;
                arvore->lista_adj.push_back(novo_no);
                break;
            }
        }
    }

    set<char> visitados;
    priority_queue<tuple<int, char, char>, vector<tuple<int, char, char>>, greater<>> pq;

    char inicial = ids_nos[0];
    visitados.insert(inicial);

    // Insere arestas do nó inicial
    for (No* no : this->lista_adj){
        if (no->id == inicial){
            for (Aresta* a : no->arestas){
                if (find(ids_nos.begin(), ids_nos.end(), a->id_no_alvo) != ids_nos.end())
                    pq.push({a->peso, inicial, a->id_no_alvo});
            }
            break;
        }
    }

    while (visitados.size() < ids_nos.size() && !pq.empty()){
        auto [peso, u, v] = pq.top(); pq.pop();
        if (visitados.count(v)) continue;
        auto tupla = make_tuple(peso, u, v);

        if (!insere_aresta(tupla, arvore->lista_adj)){
            cout << "Erro ao inserir aresta: " << u << " - " << v << endl;
            return nullptr;
        }

        visitados.insert(v);

        // Adiciona novas arestas do nó recém-inserido
        for (No* no : this->lista_adj){
            if (no->id == v) {
                for (Aresta* a : no->arestas) {
                    if (!visitados.count(a->id_no_alvo) &&
                        find(ids_nos.begin(), ids_nos.end(), a->id_no_alvo) != ids_nos.end()){
                        pq.push({a->peso, v, a->id_no_alvo});
                    }
                }
                break;
            }
        }
    }

    if (visitados.size() != ids_nos.size()){
        cout << "Subgrafo desconexo.\n";
        return nullptr;
    }
    
    return arvore;
}

Grafo * Grafo::arvore_geradora_minima_kruskal(vector<char> ids_nos) {
    cout<<"Metodo nao implementado"<<endl;
    return nullptr;
}

Grafo * Grafo::arvore_caminhamento_profundidade(int id_no) {
    cout<<"Metodo nao implementado"<<endl;
    return nullptr;
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

void Grafo::imprime_lista_adj(vector<No*>& lista){
    for(No* no : lista){
        cout << "Vertice: [" << no->id << "] com peso: " << no->peso << endl;

        for(Aresta* aresta : no->arestas){
            cout << "   Existe uma aresta de [" << no->id << "] para [" << aresta->id_no_alvo
                << "] com peso: " << aresta->peso << endl;
        }
        cout << endl;
    }
}
