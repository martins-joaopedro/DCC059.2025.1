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

void unir(map<char,Subconjunto> & conjuntos, char id1, char id2){
    char raiz1 = encontrar(conjuntos, id1);
    char raiz2 = encontrar(conjuntos, id2);

    if(raiz1 == raiz2) return;

    if (conjuntos[raiz1].rank< conjuntos[raiz2].rank){
        conjuntos[raiz1].pai = raiz2;
    }
    else if (conjuntos[raiz1].rank> conjuntos[raiz2].rank){
        conjuntos[raiz2].pai=raiz1;
    }
    //se tem a mesma altura nn importa o pai??? -> conferir
    else{
        conjuntos[raiz2].pai = raiz1;
        conjuntos[raiz1].rank++;
    }
}

Grafo * Grafo::arvore_geradora_minima_kruskal(vector<char> ids_nos) {

    //configura infos agm
    Grafo* agm = new Grafo();
    agm->in_direcionado = this->in_direcionado;
    agm->in_ponderado_aresta = this->in_ponderado_aresta;
    agm->in_ponderado_vertice = this->in_ponderado_vertice;
    agm->ordem = ids_nos.size();

    if(agm->in_direcionado==true){
        cout<< "Nao eh possivel aplicar este algoritmo em digrafos";
        return nullptr;
    }

    //ver se os parametros vieram certinho
    cout<< "TESTE 1 - Parametros Grafo " <<endl <<agm->in_direcionado<<agm->in_ponderado_aresta<<agm->in_ponderado_vertice<<agm->ordem;

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
   map<pair<char, char>, int> melhor_arestas;
    
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
                //evita duplicado (deixa sempre em ordem alfabética)
                pair<char, char> par = minmax(id_a, id_b);
                
                if(melhor_arestas.find(par) == melhor_arestas.end()){
                    melhor_arestas[par]=a->peso;
                }
                else{
                    melhor_arestas[par]=min(melhor_arestas[par],a->peso);
                }
            }
        }

    }

    //ordena arestas por peso
    vector<tuple<int, char, char>> arestas;
    for(const pair<pair<char, char>, int>& entrada: melhor_arestas){
        arestas.push_back({entrada.second, entrada.first.first, entrada.first.second});
    }
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

    //Inicializa conjuntos
    map<char,Subconjunto> conjuntos;
    for (char id: ids_nos){
        conjuntos[id] = {id,0};
    }

    //Adiciona arestas sem formar ciclos
    for(const tuple<int, char, char>& element: arestas){
        
        int peso = get<0>(element);
        char id_a = get<1>(element);
        char id_b = get<2>(element);
        
        char raiz_a = encontrar (conjuntos, id_a);
        char raiz_b = encontrar (conjuntos, id_b);

        if(raiz_a != raiz_b){
            unir(conjuntos,raiz_a, raiz_b);

            Aresta*nova = new Aresta();
            nova->id_no_alvo= id_b;
            nova-> peso = peso;

            for(No* no: agm->lista_adj){
                if(no->id == id_a){
                    no->arestas.push_back(nova);
                    break;
                }
            }
            //se nao é digrafo tem que add na lista de adj do outro tmb
            if(!this->in_direcionado){
                Aresta* nova_b = new Aresta();
                nova_b->id_no_alvo = id_a;
                nova_b->peso = peso;

                for(No* no: agm->lista_adj){
                    if(no->id == id_b){
                        no->arestas.push_back(nova_b);
                        break;
                    }
                }
            }
        }    
    }

    //verifica se o conjunto é conexo
    if ((int)arestas.size() != ids_nos.size() - 1) {
    cout << "Nao foi possivel gerar uma AGM com os vertices fornecidos: o subconjunto dado nao eh conexo." << endl;
    delete agm;
    return nullptr;
    }
    else{
        cout <<endl<< "AGM - LISTA DE ADJACENCIA:"<<endl;
        for (No* no : agm->lista_adj) {
            cout << "Vertice: [" << no->id << "] com peso: " << no->peso << endl;
            for (Aresta* aresta : no->arestas) {
                cout << "   Aresta para [" << aresta->id_no_alvo << "] com peso: " << aresta->peso << endl;
            }
        }
    }
    
    return agm;
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
