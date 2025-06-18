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

Aresta* Grafo::custo1(No* no, char id_alvo){
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

tuple<int, char, char> Grafo::custo(char id_origem, char id_destino, vector<tuple<int,char,char>>& arestas){
    for (const tuple<int, char, char>& element : arestas){
        cout<<"to aqui"<<endl;
        if((get<1>(element) == id_origem || get<2>(element) == id_origem) 
        && (get<1>(element) == id_destino || get<2>(element) == id_destino)){
            return element;
        }
    }
    return make_tuple(numeric_limits<int>::max(), id_origem, '\0');
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

    // vector<tuple<int,char,char>>arestas;

    // for(char id_a:ids_nos){
    //     No* no_a = nullptr;
    //     for(No* no: this-> lista_adj){
    //         if (no->id == id_a){
    //             no_a = no;
    //             break;
    //         }
    //     }
    //     if(!no_a) continue;

    //     for(Aresta* a: no_a->arestas){
    //         char id_b = a->id_no_alvo;

    //         //ve se o id b ta no ids nos selecionados
    //         if(find(ids_nos.begin(),ids_nos.end(),id_b)!= ids_nos.end()){
    //             //evita duplicado -> conferir dps multiaresta
    //             if(id_a < id_b || this->in_direcionado){
    //                 arestas.push_back({a->peso,id_a,id_b});
    //             }
    //         }
    //     }
    // }

    // if(arestas.empty()){
    //     cout << "O subconjunto é desconexo" << endl;
    //     return nullptr;
    // }

    // imprime_lista_adj(arvore->lista_adj);

    // //ordena arestas por peso
    // sort(arestas.begin(),arestas.end());

    // cout<<"TESTE2 - Imprimir Arestas e pesos "<< endl;
    // for (const tuple<int, char, char>& element : arestas) {
    //     cout << "("
    //               << get<0>(element) << ", "
    //               << get<1>(element) << ", "
    //               << get<2>(element)
    //               << ") ";
    // }

    // if(!insere_aresta(arestas[0], arvore->lista_adj)){
    //     cout << "Não foi pissível inserir a aresta de "<< get<1>(arestas[0])<< " para " 
    //         << get<1>(arestas[0]) <<endl;
    //     return nullptr;
    // }

    // char u = get<1>(arestas[0]), v = get<2>(arestas[0]);
    // arestas.erase(arestas.begin());//remove a primerira aresta(de menor peso)

    // map<char, char> prox;
    // map<char, int
    // set<char> visitados;
    // // vector<tuple<int, char, char>> prox;

    
    // for(char id : ids_nos){
    //     tuple<int, char, char> aresta_u = custo(id, u, arestas);
    //     tuple<int, char, char> aresta_v = custo(id, v, arestas);

    //     if(get<2>(aresta_u) != '/0' || get<2>(aresta_v) != '/0'){
    //         if(get<0>(aresta_u) < get<0>(aresta_v)){
    //             prox[{get<0>(aresta_u), id}] = u;
    //         } else {
    //             prox[{get<0>(aresta_v), id}] = v;
    //         }
    //     } else {
    //         prox[{get<0>(aresta_u), id}] = '/0';
    //     }
    // }

    // prox[{numeric_limits<int>::max(), u}] = '\0';
    // prox[{numeric_limits<int>::max(), v}] = '\0';
    // int cont = 0;
    
    // while(cont < ids_nos.size() -2){
    //     auto it = prox.begin();
    //     auto tupla = std::make_tuple(it->first.first, it->first.second, it->second);

        
    //     if(!insere_aresta(tupla, arvore->lista_adj)){
    //         cout << "Não foi pissível inserir a aresta de "<< get<1>(arestas[0])<< " para " 
    //         << get<1>(arestas[0]) <<endl;
    //         return nullptr;
    //     }

    //     it->second = '\0';

    //     for(char id : ids_nos){
    //         tuple<int, char, char> aresta_i_j = custo(id, it->first.first, arestas);

    //         if(prox.at() != '/0' || get<2>(aresta_i_j) != '/0'){
    //             if(get<0>(aresta_u) < get<0>(aresta_v)){
    //                 prox[{get<0>(aresta_u), id}] = u;
    //             } else {
    //                 prox[{get<0>(aresta_v), id}] = v;
    //             }
    //         } else {
    //             prox[{get<0>(aresta_u), id}] = '/0';
    //         }
    //     }
    // }



    //     if(get<1>(element) == u || get<2>(element) == u){
    //         aresta_u = element;
    //     } else {
    //         get<0>(aresta_u) = numeric_limits<int>::max();
    //     }
        
    //     if((get<1>(element) == v || get<2>(element) == v)){
    //         aresta_v = element;
    //     } else {
    //         get<0>(aresta_v) = numeric_limits<int>::max();
    //     }

    //     if(get<0>(aresta_u) < get<0>(aresta_v)){
    //         prox.push_back(aresta_u);
    //     } else {
    //         prox.push_back(aresta_v);
    //     }
    // }

    //prox.push_back(make_tuple(0, ))
    // int cont = 0;

    // while (cont < ids_nos.size()-2){
    //     sort(prox.begin(), prox.end());

    //     if(!insere_aresta(prox[0], arvore->lista_adj)){
    //         cout << "Não foi pissível inserir a aresta de "<< get<1>(arestas[0])<< " para " 
    //         << get<1>(arestas[0]) <<endl;
    //         return nullptr;
    //     }

    //     char j = get<1>(prox[0]);
    //     prox.erase(prox.begin());//remove a primerira aresta(de menor peso)
    //     cout<<"TESTE4 - Imprimir Arestas e pesos "<< endl;
    //     for (const tuple<int, char, char>& element : arestas) {
    //         cout << "("
    //                 << get<0>(element) << ", "
    //                 << get<1>(element) << ", "
    //                 << get<2>(element)
    //                 << ") ";
    //     }      

    //     for(const tuple<int, char, char>& element : arestas){
    //     //cout<<"to aqui"<<endl;
    //         tuple<int, char, char> aresta_prox_i, aresta_i_j;

    //         if(get<1>(element) == j || get<2>(element) == j){
    //             aresta_i_j = element;
    //         }

    //         if(get<1>(element) == get<1>(aresta_i_j) || get<1>(element) == get<2>(aresta_i_j) &&
    //             get<0>(aresta_i_j) < get<0>(element)){
    //             prox.push_back(aresta_i_j);
    //         }
    //     }

    //     cont++;
    // }
    
    imprime_lista_adj(arvore->lista_adj);
    return arvore;
    
    // tuple min = aresta_menor_peso(arvore->lista_adj);//achr aresta menor peso
    // cout<<"vertice: "<<get<0>(min)->id<<", aresta: "<<get<1>(min)->id_no_alvo<<endl;

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
