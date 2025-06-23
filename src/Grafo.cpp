#include "Grafo.h"
#include <algorithm>
#include <map>
#include <limits.h>

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

bool Grafo::execoes(char id_no_a, char id_no_b)
{
    if (id_no_a == id_no_b)
    {
        cout << "O caminho entre os mesmos vertices eh zero." << endl;
        return false;
    }

    if (this->lista_adj.empty())
    {
        cout << "Grafo vazio." << endl;
        return false;
    }

    for (No *no : this->lista_adj)
    {
        if (no->id == id_no_a && no->arestas.empty())
        {
            cout << "Vertice inicial nao possui conexao com nenhum outro vertice" << endl;
            return false;
        }
    }
    return true;
}
// antes estava como int nos ids, não faz sentido
vector<char> Grafo::caminho_minimo_dijkstra(char id_no_a, char id_no_b)
{
    vector<char> visitados;
    map<char, int> distancia;
    map<char, char> anterior;

    if (!execoes(id_no_a, id_no_b))
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
        {
            break;
        }

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

    cout << "distancia de " << id_no_a << " ate " << id_no_b << ": " << distancia[id_no_b] << endl;

    vector<char> caminho;
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

vector<char> Grafo::caminho_minimo_floyd(char id_no_a, char id_no_b)
{
    int n = this->lista_adj.size();
    vector<vector<int>> distancia(n, vector<int>(n, INT_MAX));
    map<char, int> mapa_indice;

    vector<vector<char>> penultimo(n, vector<char>(n, '-'));

    if (!execoes(id_no_a, id_no_b))
        return {};

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
                if (distancia[i][k] != INT_MAX && distancia[k][j] != INT_MAX && distancia[i][k] + distancia[k][j] < distancia[i][j])
                {
                    distancia[i][j] = distancia[i][k] + distancia[k][j];
                    penultimo[i][j] = penultimo[k][j];
                }
            }
        }
    }

    for (int i = 0; i < n; i++)
    {
        for (int j = 0; j < n; j++)
        {
            cout << penultimo[i][j] << " | ";
        }
        cout << endl;
    }
    cout << endl;

    if (distancia[mapa_indice[id_no_a]][mapa_indice[id_no_b]] == INT_MAX)
    {
        cout << "Nao exite caminho entre esses vertices!" << endl;
        return {};
    }

    for (int i = 0; i < n; i++)
    {
        for (int j = 0; j < n; j++)
        {
            if (distancia[i][j] == INT_MAX)
                cout << "*" << " | ";
            else
                cout << distancia[i][j] << " | ";
        }
        cout << endl;
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
