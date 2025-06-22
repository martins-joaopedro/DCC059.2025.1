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

// antes estava como int nos ids, não faz sentido
vector<char> Grafo::caminho_minimo_dijkstra(char id_no_a, char id_no_b)
{
    if (this->lista_adj.empty())
    {
        cout << "Grafo vazio." << endl;
        return {};
    }

    for (No *no : this->lista_adj)
    {
        if (no->id == id_no_a && no->arestas.empty())
        {
            cout << "Vertice inicial nao possui conexao com nenhum outro vertice" << endl;
            return {};
        }
    }

    vector<char> visitados;
    map<char, int> distancia;
    map<char, char> anterior;
    vector<char> caminho;

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
        {
            break;
        }

        visitados.push_back(atual);
        cout << atual << " aq" << endl;

        for (No *no : this->lista_adj)
        {
            if (no->id == atual)
            {
                for (Aresta *a : no->arestas)
                {
                    char vizinho = a->id_no_alvo;
                    int peso = a->peso;

                    cout << "vizinhos: " << vizinho << "-- " << "peso: " << peso << endl;
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

    cout << "distancia de: " << id_no_a << " ate " << id_no_b << ": " << distancia[id_no_b] << endl;

    for (char v = id_no_b; v != id_no_a; v = anterior[v])
    {
        caminho.insert(caminho.begin(), v);
    }

    caminho.insert(caminho.begin(), id_no_a);

    cout << "caminho: ";
    for (char c : caminho)
    {
        cout << c << " --- ";
    }
    cout << endl;
    return caminho;
}

vector<char> Grafo::caminho_minimo_floyd(char id_no, char id_no_b)
{
    int n = this->lista_adj.size();
    vector<vector<int>> distancia(n, vector<int>(n, INT_MAX));
    map<char, int> mapa_indice;
    int valor = 0;

    for (No *no : this->lista_adj)
    {
        mapa_indice[no->id] = valor;
        valor++;
    }

    for (No *no : this->lista_adj)
    {
        for (Aresta *a : no->arestas)
        {
            distancia[mapa_indice[no->id]][mapa_indice[a->id_no_alvo]] = a->peso;
        }
        distancia[mapa_indice[no->id]][mapa_indice[no->id]] = 0;
    }

    for (int k = 0; k < n; k++)
    {
        for (int i = 0; i < n; i++)
        {
            for (int j = 0; j < n; j++)
            {
                if ( distancia[i][k] !=INT_MAX  && distancia[k][j] != INT_MAX && distancia[i][k] + distancia[k][j] < distancia[i][j])
                    distancia[i][j] = distancia[i][k] + distancia[k][j];
            }
        }
    }

    for (int i = 0; i < n; i++)
    {
        for (int j = 0; j < n; j++)
        {
            if(distancia[i][j] == INT_MAX)
                cout<< "*" << " | ";
            else
                cout << distancia[i][j] << " | ";
        }
        cout << endl;
    }
    return {};
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

bool Grafo::usarFloyd() {
    for (No* no: this->lista_adj){
        for(Aresta* aresta:no->arestas){
            //se tiver aresta negativa tem q usar floyd
            if(aresta->peso<0) return true;
        }
    }
    int V = this->lista_adj.size();
    int E = 0;
    for(No* no: this->lista_adj){
        //ver pra direcionado e nao direcionado
        E += no->arestas.size();
    }
    //static_cast serve para converção segura, so precisa aplicar em um, pq float e int da float
    float densidade = static_cast<float>(E)/ (V*(V-1));
    if (!in_direcionado) densidade /= 2.0f;

    //Se tiver poucos vertices ou for um grafico denso (mais das metade dos pares de vertices estão conectados) é recomendado usar floyd
    return densidade>0.5f || V<=100;
}

vector<vector<int>> Grafo::matriz_distancias() {
    int n = this->lista_adj.size();

}

int Grafo::raio()
{
    cout << "Metodo nao implementado" << endl;
    return 0;
}

int Grafo::diametro()
{
    cout << "Metodo nao implementado" << endl;
    return 0;
}

vector<char> Grafo::centro()
{
    cout << "Metodo nao implementado" << endl;
    return {};
}

vector<char> Grafo::periferia()
{
    cout << "Metodo nao implementado" << endl;
    return {};
}

vector<char> Grafo::vertices_de_articulacao()
{
    cout << "Metodo nao implementado" << endl;
    return {};
}
