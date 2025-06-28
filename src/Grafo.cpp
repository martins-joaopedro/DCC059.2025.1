#include "Grafo.h"
#include <algorithm>
#include <map>
#include <limits.h>
#include <climits>


Grafo::Grafo()
{
}

Grafo::~Grafo()
{
}

vector<char> Grafo::fecho_transitivo_direto(int id_no)
{
    cout << "Metodo nao implementado" << endl;
    return {};
}

vector<char> Grafo::fecho_transitivo_indireto(int id_no)
{
    cout << "Metodo nao implementado" << endl;
    return {};
}

bool Grafo::execoes_caminho_minimo(char id_no_a, char id_no_b)
{
    bool existe;
    if (this->lista_adj.empty())
    {
        cout << "Grafo vazio." << endl;
        return false;
    }

    for (No *no : this->lista_adj)
    {
        if (no->id == id_no_a && no->arestas.empty())
        {
            cout << "Vertice inicial nao possui conexao com nenhum outro vertice." << endl;
            return false;
        }
        if(no->id == id_no_a || no->id == id_no_b)
            existe = true;
    }

    if(!existe){
        cout << "Algum vertice digitado nao esta contido no grafo." << endl;
        return false;
    }

    return true;
}

vector<char> Grafo::caminho_minimo_dijkstra(char id_no_a, char id_no_b)
{
    vector<char> visitados;
    map<char, int> distancia;
    map<char, char> anterior;

    if (!execoes_caminho_minimo(id_no_a, id_no_b))
        return {};

    for (No *no : this->lista_adj)
    {
        distancia.insert({no->id, INT_MAX});
    }

    distancia[id_no_a] = 0;

    while (visitados.size() < this->lista_adj.size())
    {
        char atual;
        int menorDistancia = INT_MAX;

        for (auto const &par : distancia)
        {
            if (find(visitados.begin(), visitados.end(), par.first) == visitados.end() && par.second < menorDistancia)
            {
                menorDistancia = par.second;
                atual = par.first;
            }
        }

        // caso o ultimo vertice acessado nao alcançar mais ninguem, o proximo atual terá distancia infinita, podendo parar o loop
        if (menorDistancia == INT_MAX)
            break;

        visitados.push_back(atual);

        for (No *no : this->lista_adj)
        {
            if (no->id == atual)
            {
                for (Aresta *a : no->arestas)
                {
                    char vizinho = a->id_no_alvo;
                    int peso = a->peso;
                    int novaDist = distancia[atual] + peso;
                    // verifica todos os seus vizinhos, mesmo aqueles que já foram visitados
                    if (novaDist < distancia[vizinho])
                    {
                        distancia[vizinho] = novaDist;
                        anterior[vizinho] = atual;
                        if (find(visitados.begin(), visitados.end(), vizinho) != visitados.end())
                        {
                            visitados.erase(remove(visitados.begin(), visitados.end(), vizinho), visitados.end()); // remove de visitados para seus vizinhos serem recalculados
                        }
                    }
                }
                break;
            }
        }
    }

    if (distancia[id_no_b] == INT_MAX)
    {
        cout << "Nao existe caminho ligando esses dois vertices" << endl;
        return {};
    }

    cout << "Distancia de " << id_no_a << " ate " << id_no_b << ": " << distancia[id_no_b] << endl;

    vector<char> caminho;
    for (char v = id_no_b; v != id_no_a; v = anterior[v])
    {
        caminho.insert(caminho.begin(), v);
    }

    caminho.insert(caminho.begin(), id_no_a);

    cout << "Caminho: ";
    for (char c : caminho)
    {
        cout << c << " --- ";
    }
    cout << endl;
    return caminho;
}

//matriz utilizada para o desenvolvimento da letra h
vector<vector<char>> Grafo::cria_matriz_floyd(vector<vector<int>> &distancia, bool verificacao)
{
    int n = this->lista_adj.size();
    map<char, int> mapa_indice;
    int valor = 0;

    for (No *no : this->lista_adj)
    {
        mapa_indice[no->id] = valor;
        valor++;
    }

    vector<vector<char>> penultimo(n, vector<char>(n, '-'));

    for (No *no : this->lista_adj)
    {
        for (Aresta *a : no->arestas)
        {
            distancia[mapa_indice[no->id]][mapa_indice[a->id_no_alvo]] = a->peso;
            if (verificacao)
                penultimo[mapa_indice[no->id]][mapa_indice[a->id_no_alvo]] = no->id;
        }
        distancia[mapa_indice[no->id]][mapa_indice[no->id]] = 0;
    }

    for (int k = 0; k < n; k++)
    {
        for (int i = 0; i < n; i++)
        {
            for (int j = 0; j < n; j++)
            {
                if (distancia[i][k] != INT_MAX && distancia[k][j] != INT_MAX && k != i && k != j && i != j && distancia[i][k] + distancia[k][j] < distancia[i][j])
                {
                    distancia[i][j] = distancia[i][k] + distancia[k][j];
                    if (verificacao)
                        penultimo[i][j] = penultimo[k][j];
                }
            }
        }
    }

    if (verificacao)
        return penultimo;
    else
        return {};
}

vector<char> Grafo::caminho_minimo_floyd(char id_no_a, char id_no_b)
{
    if (!execoes_caminho_minimo(id_no_a, id_no_b))
        return {};

    int n = this->lista_adj.size();
    vector<vector<int>> distancia(n, vector<int>(n, INT_MAX));
    vector<vector<char>> penultimo(n, vector<char>(n, '-')); // penúltimo vértice do caminho i, j
    map<char, int> mapa_indice;
    int valor = 0;

    for (No *no : this->lista_adj)
    {
        mapa_indice[no->id] = valor;
        valor++;
    }

    penultimo = cria_matriz_floyd(distancia, true); // booleano true para indicar que eu quero o retorno da função, que é a matriz de penultimo para reconstrução do caminho

    if (distancia[mapa_indice[id_no_a]][mapa_indice[id_no_b]] == INT_MAX)
    {
        cout << "Nao exite caminho entre esses vertices!" << endl;
        return {};
    }

    vector<char> caminho;
    char inicio = id_no_a;
    char fim = id_no_b;

    caminho.insert(caminho.begin(), fim);
    while (inicio != fim)
    {
        char p = penultimo[mapa_indice[inicio]][mapa_indice[fim]];
        caminho.insert(caminho.begin(), p);
        fim = p;
    }

    for (char c : caminho)
    {
        cout << c << " --- ";
    }
    cout << endl;

    cout << "Caminho de " << id_no_a << " ate " << id_no_b << " eh: " << distancia[mapa_indice[id_no_a]][mapa_indice[id_no_b]] << endl;
    return caminho;
}

Grafo *Grafo::arvore_geradora_minima_prim(vector<char> ids_nos)
{
    cout << "Metodo nao implementado" << endl;
    return nullptr;
}

Grafo *Grafo::arvore_geradora_minima_kruskal(vector<char> ids_nos)
{
    cout << "Metodo nao implementado" << endl;
    return nullptr;
}

Grafo *Grafo::arvore_caminhamento_profundidade(int id_no)
{
    cout << "Metodo nao implementado" << endl;
    return nullptr;
}

//Evita ter que construir excentricidades 4x para um mesmo grafo
const map<char, int>& Grafo::get_excentricidades() {
    if (!excentricidades_validas) {
        armazena_excentricidades.clear();
        armazena_excentricidades = calcular_excentricidades();
        excentricidades_validas = true;
    }
    return armazena_excentricidades;
}


map<char, int> Grafo::calcular_excentricidades() {
    if(lista_adj.empty()){
        cout << "Grafo vazio - nao e possivel calcular excentricidades." << endl;
        return {};
    }

    int n = this->lista_adj.size();
    
    vector<vector<int>> dist(n, vector<int>(n, INT_MAX));
    cria_matriz_floyd(dist, false);

    map<char,int> excentricidades;
    map<int,char> id_por_indice;
    int i = 0; 

    for(No*no: this->lista_adj){
        id_por_indice[i] = no->id;
        i++;
    }

    for(int i=0;i<dist.size();i++){
        int excentricidade=0;
        //evita vertice isolado recebe exc 0
        bool alcanca_algum=false;

        for (int j=0; j<dist.size();j++){
            if( i!=j && dist[i][j]!= INT_MAX){
                excentricidade=max(excentricidade,dist[i][j]);
                alcanca_algum=true;
            }
        }
        if(alcanca_algum) {
            excentricidades[id_por_indice[i]] = excentricidade;
        }
        else{
            excentricidades[id_por_indice[i]] = 0;
        }       
    }
    return excentricidades;
}

void Grafo::calcula_caracteristicas(){
    map<char, int> excentricidades = calcular_excentricidades();

    if (excentricidades.empty()) {
        cout << "Raio indefinido - grafo sem vertices ou excentricidades." << endl;
        return;
    }

    //calcula raio e diametro
    this->raio = INT_MAX;
    this->diametro = INT_MIN;

    for (pair<const char, int>& par : excentricidades) {
        if(par.second > 0)
            this->raio = min(this->raio, par.second);
        
        this->diametro = max(this->diametro, par.second);
    }

    if(this->raio == INT_MAX) raio = 0;

    //calcula centro e periferia
    for (pair<const char, int>& par : excentricidades) {
        if(par.second==this->diametro){
            periferia.push_back(par.first);
        }
        if(par.second==this->raio){
            centro.push_back(par.first);
        }
    }
}
