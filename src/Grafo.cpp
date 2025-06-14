#include "Grafo.h"
#include <tuple>
#include <limits>

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

tuple<No*, Aresta*> Grafo::aresta_menor_peso(){
    No* no_min = nullptr;
    Aresta* aresta_min = nullptr;
    int menor_peso = numeric_limits<int>::max();
    
    for(No* no : this->lista_adj){
        for(Aresta* aresta : no->arestas){
            if(aresta->peso < menor_peso){
                menor_peso = aresta->peso;
                aresta_min = aresta;
                no_min = no;
            }
        }
    }

    return make_tuple(no_min, aresta_min);
}

Aresta* Grafo::custo(No* no, char id_alvo){
    Aresta* aresta = new Aresta();
    aresta->peso = numeric_limits<int>::max();
    
    for(Aresta* aresta_i : no->arestas){
        if(aresta_i->id_no_alvo == id_alvo){
            aresta = aresta_i;
        }
    }

    return aresta;
}

Grafo * Grafo::arvore_geradora_minima_prim(vector<char> ids_nos) {
    cout<<"Metodo nao implementado"<<endl;

    if(!this->in_direcionado){
        cout << "there's nothing we can do" << endl;
        return nullptr;
    }

    Grafo* arvore = new Grafo();
    tuple min = aresta_menor_peso();
    
    No* raiz = get<0>(min);
    Aresta* aresta_menor = get<1>(min);

    raiz->arestas.push_back(aresta_menor);
    arvore->lista_adj.push_back(raiz);

    No* u = raiz;
    No* v;
    for(No* no : this->lista_adj){
        if(no->id == aresta_menor->id_no_alvo){
            v = no;
        }
    }

    vector<Aresta*> prox;

    for(No* no : this->lista_adj){
        if(custo(u, no->id)->peso < custo(v, no->id)->peso){
            
        }
    }

    return nullptr;
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
