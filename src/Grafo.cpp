#include "Grafo.h"
#include <tuple>
#include <limits>
#include <unordered_map>

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

tuple<No*, Aresta*> Grafo::aresta_menor_peso(vector<No*>& lista){
    No* no_min = nullptr;
    Aresta* aresta_min = nullptr;
    int menor_peso = numeric_limits<int>::max();
    
    for(No* no : lista){
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

tuple<No*, Aresta*> Grafo::minimo_prox(unordered_map<No*, Aresta*>& prox){
    No* no_min = nullptr;
    Aresta* aresta_min = nullptr;
    int menor_peso = numeric_limits<int>::max();

    for(const auto& par : prox){
        if(par.second != nullptr && par.second->peso < aresta_min->peso){
            no_min = par.first;
            aresta_min = par.second;
        }
    }

    return make_tuple(no_min, aresta_min);
}

No* Grafo::acha_no(char id, vector<No*>& lista){
    for(No* no : lista){
        if(no->id == id){
            return no;
        }
    }
    return nullptr;
}

Grafo * Grafo::arvore_geradora_minima_prim(vector<char> ids_nos) {
    if(!this->in_ponderado_aresta){
        cout << "O grafo não possui pesos nas arestas." << endl;
        return nullptr;
    }
    this->imprime_lista_adj(this->lista_adj);
    
    

    if (this->lista_adj.empty()) {
        cout << "Grafo vazio." << endl;
        return nullptr;
    }

    Grafo* arvore = new Grafo();
    arvore->in_ponderado_aresta = true;
    vector<No*> nova_lista;    

    for(char id : ids_nos){//preenche nova lista
        for(No* no : this->lista_adj){
            if(no->id == id){
                nova_lista.push_back(no);
                cout<<"muito\n";
            }
        }
    }
    imprime_lista_adj(nova_lista);

    tuple min = aresta_menor_peso(nova_lista);//achr aresta menor peso
    cout<<"vertice: "<<get<0>(min)->id<<", aresta: "<<get<1>(min)->id_no_alvo<<endl;
    
    // No* raiz = get<0>(min);
    // Aresta* aresta_menor = get<1>(min);

    // vector<Aresta*> novas_arestas;
    // raiz->arestas = novas_arestas;
    // raiz->arestas.push_back(aresta_menor);
    // arvore->lista_adj.push_back(raiz);

    // No* u = raiz;
    // No* v = acha_no(aresta_menor->id_no_alvo, nova_lista);

    // unordered_map<No*, Aresta*> prox;

    // for(No* no : this->lista_adj){
    //     Aresta* aresta_u = custo(no, u->id);
    //     Aresta* aresta_v = custo(no, v->id);
        
    //     if(aresta_u->peso < aresta_v->peso){
    //         prox.emplace(no, aresta_u);
    //     } else {
    //         prox.emplace(no, aresta_v);
    //     }
    // }
    // prox.at(u) = nullptr;
    // prox.at(v) = nullptr;
    
    // int cont = 0;

    // while(cont < this->lista_adj.size() - 2){
    //     min = minimo_prox(prox);

    //     No* novo_no = get<0>(min);
    //     Aresta* novo_aresta = get<1>(min);
    //     novo_no->arestas.push_back(novo_aresta);
    //     arvore->lista_adj.push_back(novo_no);

    //     prox.at(novo_no) = nullptr;

    //     for(No* no : this->lista_adj){
    //         Aresta* aresta_prox_i = custo(no, prox.at(no)->id_no_alvo);
    //         Aresta* aresta_j = custo(no, novo_no->id);
            
    //         if(aresta_prox_i->peso < aresta_j->peso){
    //             prox.emplace(no, aresta_prox_i);
    //         } else {
    //             prox.emplace(no, aresta_j);
    //         }
    //     }

    //     cont++;
    // }

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
