#include "Grafo.h"
#include <algorithm>
#include <map>

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

Grafo * Grafo::arvore_geradora_minima_prim(vector<char> ids_nos) {
    cout<<"Metodo nao implementado"<<endl;
    return nullptr;
}


struct Subconjunto {
    char pai;
    int rank;
};

//função recursiva pra agrar pais dos subconjuntos -> melhora eficiência
char encontrar(map<char,Subconjunto> & conjuntos, char id ){
    if (conjuntos[id].pai != id){
        conjuntos[id].pai =  encontrar(conjuntos, conjuntos[id].pai);
    }
    return conjuntos[id].pai;
}

Grafo * Grafo::arvore_geradora_minima_kruskal(vector<char> ids_nos) {

    //configura infos agm
    Grafo* agm = new Grafo();
    agm->in_direcionado = this->in_direcionado;
    agm->in_ponderado_aresta = this->in_ponderado_aresta;
    agm->in_ponderado_vertice = this->in_ponderado_vertice;
    agm->ordem = ids_nos.size();

    //ver se os parametros vieram certinho
    cout<< "TESTE 1 - Parâmetros Grafo " <<endl <<agm->in_direcionado<<agm->in_ponderado_aresta<<agm->in_ponderado_vertice<<agm->ordem;

    //copiar nos escolhidos pra lista da agm
    for(char id:ids_nos){
        for (No* no: this->lista_adj){
            if(no->id == id){
                No* novo = new No();
                novo->id = id;
                novo->peso = no->peso;
                agm->lista_adj.push_back(novo);
                break;
            }
        }
    }

    //Copia apenas arestas presentes no subconjunto agm
    vector<tuple<int,char,char>>arestas;
    
    for(char id_a:ids_nos){
        No* no_a = nullptr;
        for(No* no: this-> lista_adj){
            if (no->id == id_a){
                no_a = no;
                break;
            }
        }
        if(!no_a) continue;

        for(Aresta* a: no_a->arestas){
            char id_b = a->id_no_alvo;

            //ve se o id b ta no ids nos selecionados
            if(find(ids_nos.begin(),ids_nos.end(),id_b)!= ids_nos.end()){
                //evita duplicado -> conferir dps multiaresta
                if(id_a < id_b || this->in_direcionado){
                    arestas.push_back({a->peso,id_a,id_b});
                }
            }
        }

    }
    //ordena arestas por peso
    sort(arestas.begin(),arestas.end());

    cout<<"TESTE2 - Imprimir Arestas e pesos "<< endl;
    for (const tuple<int, char, char>& element : arestas) {
            cout << "("
                      << get<0>(element) << ", "
                      << get<1>(element) << ", "
                      << get<2>(element)
                      << ") ";
        }
    


    //verificar se os nós do subconjunto se conectam (ciclos) e add arestas válidas
    
    //função de unir
    


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
