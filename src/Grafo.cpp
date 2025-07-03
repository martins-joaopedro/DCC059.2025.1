#include "Grafo.h"
#include <algorithm>
#include <map>
#include <limits.h>
#include <climits>
#include <vector>
#include <stack>
#include <map>
#include <queue>
#include <iostream>
#include <tuple>
#include <set>
#include <functional>
#include <algorithm>

using namespace std;

#define INF INT_MAX

Grafo::Grafo()
{
}

Grafo::~Grafo()
{
}

void Grafo::aux_fecho_transitivo_direto(map<char, bool> &C, char descendente)
{
    for (No *no : this->lista_adj)
    {
        if (no->id == descendente)
        {
            for (Aresta *aresta : no->arestas)
            {
                char filho = aresta->id_no_alvo;
                if (!C[aresta->id_no_alvo])
                {
                    C[aresta->id_no_alvo] = true;
                    aux_fecho_transitivo_direto(C, filho);
                }
            }
            break;
        }
    }
}

vector<char> Grafo::fecho_transitivo_direto(char id_no)
{
    vector<char> fecho;

    if (!this->in_direcionado)
    {
        cout << "Grafo nao direcionado!" << endl
             << endl;
        return {};
    }

    map<char, bool> C;
    C[id_no] = true;
    aux_fecho_transitivo_direto(C, id_no);

    for (const auto &par : C)
    {
        if (par.second && par.first != id_no)
        {
            fecho.push_back(par.first);
        }
    }

    return fecho;
}

vector<char> Grafo::fecho_transitivo_indireto(char id_no)
{

    vector<char> saida;
    queue<No *> fila;
    map<char, bool> marcados;

    if (!in_direcionado)
    {
        cout << "Grafo nao direcionado!" << endl;
        return saida;
    }

    // busca pelo nó inicial
    for (No *no : lista_adj)
        if (no->id == id_no)
        {
            fila.push(no);
            marcados[no->id] = true;
            break;
        }

    while (!fila.empty())
    {

        No *no_atual = fila.front();
        fila.pop();

        // encontra todas as arestas que vão pro no atual
        for (No *no : lista_adj)
        {
            for (Aresta *aresta : no->arestas)
            {
                if (aresta->id_no_alvo == no_atual->id)
                {
                    if (!marcados[no->id])
                    {
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

bool Grafo::insere_aresta(tuple<int, char, char> &aresta_info, vector<No *> &lista)
{
    Aresta *aresta = new Aresta();
    int cont = 0;

    for (No *no : lista)
    {
        if (no->id == get<1>(aresta_info))
        {
            aresta->peso = get<0>(aresta_info);
            aresta->id_no_alvo = get<2>(aresta_info);
            no->arestas.push_back(aresta);
            cont++;
        }
        else if (no->id == get<2>(aresta_info))
        {
            Aresta *aresta_b = new Aresta();
            aresta_b->id_no_alvo = get<1>(aresta_info);
            aresta_b->peso = get<0>(aresta_info);
            no->arestas.push_back(aresta_b);
            cont++;
        }
        else if (cont == 2)
        {
            break;
        }
    }

    if (cont != 2)
    {
        return false;
    }

    return true;
}

Grafo *Grafo::arvore_geradora_minima_prim(vector<char> ids_nos)
{
    if (!this->in_ponderado_aresta)
    {
        cout << "O grafo não possui pesos nas arestas." << endl;
        return nullptr;
    }

    if (this->in_direcionado)
    {
        cout << "O grafo eh direcionado." << endl;
        return nullptr;
    }

    if (this->lista_adj.empty())
    {
        cout << "Grafo vazio." << endl;
        return nullptr;
    }

    Grafo *arvore = new Grafo();
    arvore->in_ponderado_aresta = this->in_ponderado_aresta;
    arvore->in_direcionado = this->in_direcionado;
    arvore->in_ponderado_vertice = this->in_ponderado_vertice;
    arvore->ordem = ids_nos.size();

    for (char id : ids_nos)
    { // preenche lista_adj da arvore
        for (No *no : this->lista_adj)
        {
            if (no->id == id)
            {
                No *novo_no = new No();
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

    for (No *no : this->lista_adj)
    {
        if (no->id == inicial)
        {
            for (Aresta *aresta : no->arestas)
            {
                if (find(ids_nos.begin(), ids_nos.end(), aresta->id_no_alvo) != ids_nos.end())
                    pq.push({aresta->peso, inicial, aresta->id_no_alvo});
            }
            break;
        }
    }

    while (visitados.size() < ids_nos.size() && !pq.empty())
    {
        auto aux = pq.top();
        int peso = get<0>(aux);
        char u = get<1>(aux);
        char v = get<2>(aux);

        pq.pop();

        if (visitados.count(v))
            continue;

        auto tupla = make_tuple(peso, u, v);

        if (!insere_aresta(tupla, arvore->lista_adj))
        {
            cout << "Erro ao inserir aresta: " << u << " - " << v << endl;
            return nullptr;
        }

        visitados.insert(v);

        for (No *no : this->lista_adj)
        {
            if (no->id == v)
            {
                for (Aresta *aresta : no->arestas)
                {
                    if (!visitados.count(aresta->id_no_alvo) &&
                        find(ids_nos.begin(), ids_nos.end(), aresta->id_no_alvo) != ids_nos.end())
                    {

                        pq.push({aresta->peso, v, aresta->id_no_alvo});
                    }
                }
                break;
            }
        }
    }

    if (visitados.size() != ids_nos.size())
    {
        cout << "Subgrafo desconexo.\n\n";
        return nullptr;
    }

    return arvore;
}

struct Subconjunto
{
    char pai;
    int rank;
};

// função recursiva pra agrar pais dos subconjuntos -> melhora eficiência
char encontrar(map<char, Subconjunto> &conjuntos, char id)
{
    if (conjuntos[id].pai != id)
    {
        conjuntos[id].pai = encontrar(conjuntos, conjuntos[id].pai);
    }
    return conjuntos[id].pai;
}

void unir(map<char, Subconjunto> &conjuntos, char id1, char id2)
{
    char raiz1 = encontrar(conjuntos, id1);
    char raiz2 = encontrar(conjuntos, id2);

    if (raiz1 == raiz2)
        return;

    if (conjuntos[raiz1].rank < conjuntos[raiz2].rank)
    {
        conjuntos[raiz1].pai = raiz2;
    }
    else if (conjuntos[raiz1].rank > conjuntos[raiz2].rank)
    {
        conjuntos[raiz2].pai = raiz1;
    }
    // se tem a mesma altura nn importa o pai??? -> conferir
    else
    {
        conjuntos[raiz2].pai = raiz1;
        conjuntos[raiz1].rank++;
    }
}

Grafo *Grafo::arvore_geradora_minima_kruskal(vector<char> ids_nos)
{

    // configura infos agm
    Grafo *agm = new Grafo();
    agm->in_direcionado = this->in_direcionado;
    agm->in_ponderado_aresta = this->in_ponderado_aresta;
    agm->in_ponderado_vertice = this->in_ponderado_vertice;
    agm->ordem = ids_nos.size();

    if (agm->in_direcionado == true)
    {
        cout << "Nao eh possivel aplicar este algoritmo em digrafos";
        return nullptr;
    }

    // ver se os parametros vieram certinho
    cout << "TESTE 1 - Parametros Grafo " << endl
         << agm->in_direcionado << agm->in_ponderado_aresta << agm->in_ponderado_vertice << agm->ordem;

    // copiar nos escolhidos pra lista da agm
    for (char id : ids_nos)
    {
        for (No *no : this->lista_adj)
        {
            if (no->id == id)
            {
                No *novo = new No();
                novo->id = id;
                novo->peso = no->peso;
                agm->lista_adj.push_back(novo);
                break;
            }
        }
    }

    // Copia apenas arestas presentes no subconjunto agm
    map<pair<char, char>, int> melhor_arestas;

    for (char id_a : ids_nos)
    {
        No *no_a = nullptr;
        for (No *no : this->lista_adj)
        {
            if (no->id == id_a)
            {
                no_a = no;
                break;
            }
        }
        if (!no_a)
            continue;

        for (Aresta *a : no_a->arestas)
        {
            char id_b = a->id_no_alvo;

            // ve se o id b ta no ids nos selecionados
            if (find(ids_nos.begin(), ids_nos.end(), id_b) != ids_nos.end())
            {
                // evita duplicado (deixa sempre em ordem alfabética)
                pair<char, char> par = minmax(id_a, id_b);

                if (melhor_arestas.find(par) == melhor_arestas.end())
                {
                    melhor_arestas[par] = a->peso;
                }
                else
                {
                    melhor_arestas[par] = min(melhor_arestas[par], a->peso);
                }
            }
        }
    }

    // ordena arestas por peso
    vector<tuple<int, char, char>> arestas;
    for (const pair<pair<char, char>, int> &entrada : melhor_arestas)
    {
        arestas.push_back({entrada.second, entrada.first.first, entrada.first.second});
    }
    sort(arestas.begin(), arestas.end());

    cout << "TESTE2 - Imprimir Arestas e pesos " << endl;
    for (const tuple<int, char, char> &element : arestas)
    {
        cout << "("
             << get<0>(element) << ", "
             << get<1>(element) << ", "
             << get<2>(element)
             << ") ";
    }

    // verificar se os nós do subconjunto se conectam (ciclos) e add arestas válidas

    // Inicializa conjuntos
    map<char, Subconjunto> conjuntos;
    for (char id : ids_nos)
    {
        conjuntos[id] = {id, 0};
    }

    int arestas_agm = 0;
    // Adiciona arestas sem formar ciclos
    for (const tuple<int, char, char> &element : arestas)
    {

        int peso = get<0>(element);
        char id_a = get<1>(element);
        char id_b = get<2>(element);

        char raiz_a = encontrar(conjuntos, id_a);
        char raiz_b = encontrar(conjuntos, id_b);

        if (raiz_a != raiz_b)
        {
            unir(conjuntos, raiz_a, raiz_b);

            Aresta *nova = new Aresta();
            nova->id_no_alvo = id_b;
            nova->peso = peso;

            for (No *no : agm->lista_adj)
            {
                if (no->id == id_a)
                {
                    no->arestas.push_back(nova);
                    break;
                }
            }

            // Tem que add na lista de adj do outro tmb
            Aresta *nova_b = new Aresta();
            nova_b->id_no_alvo = id_a;
            nova_b->peso = peso;

            for (No *no : agm->lista_adj)
            {
                if (no->id == id_b)
                {
                    no->arestas.push_back(nova_b);
                    break;
                }
            }
            arestas_agm++;
        }
    }

    // verifica se o conjunto é conexo
    if (arestas_agm != ids_nos.size() - 1)
    {
        cout << "Nao foi possivel gerar uma AGM com os vertices fornecidos: o subconjunto dado nao eh conexo." << endl;
        delete agm;
        return nullptr;
    }
    else
    {
        cout << endl
             << "AGM - LISTA DE ADJACENCIA:" << endl;
        for (No *no : agm->lista_adj)
        {
            cout << "Vertice: [" << no->id << "] com peso: " << no->peso << endl;
            for (Aresta *aresta : no->arestas)
            {
                cout << "   Aresta para [" << aresta->id_no_alvo << "] com peso: " << aresta->peso << endl;
            }
        }
    }

    return agm;
}

Grafo *Grafo::arvore_caminhamento_profundidade(int id_no)
{

    map<char, No *> mapa_nos;
    map<char, bool> visitados;
    map<char, int> profundidades;

    for (No *no : lista_adj)
    {
        visitados[no->id] = false;
        profundidades[no->id] = 0;
    }

    // mantenho configurações do grafo original
    Grafo *grafo_saida = new Grafo();
    grafo_saida->ordem = ordem;
    grafo_saida->in_direcionado = in_direcionado;
    grafo_saida->in_ponderado_aresta = in_ponderado_aresta;
    grafo_saida->in_ponderado_vertice = in_ponderado_vertice;

    // percorre até encontrar o no alvo para iniciar
    for (No *no : lista_adj)
        if (no->id == id_no)
        {
            grafo_saida = caminho_profundidade(no, grafo_saida, visitados, id_no, profundidades);
            break;
        }

    // pos processamento pra cada tornar a lista de adj simétrica
    if (grafo_saida != nullptr and !grafo_saida->in_direcionado)
    {
        for (No *no_saida : grafo_saida->lista_adj)
        {
            // uso pra evitar replicações
            visitados.clear();

            // estrutura aux pra facilitar montar as simetrias
            for (No *no : grafo_saida->lista_adj)
                mapa_nos[no->id] = no;

            for (Aresta *aresta : no_saida->arestas)
            {
                if (!visitados[aresta->id_no_alvo])
                {
                    bool itExists = false;

                    // se a aresta que eu quero criar ja existe
                    for (Aresta *aresta : mapa_nos[aresta->id_no_alvo]->arestas)
                        if (aresta->id_no_alvo == no_saida->id)
                            itExists = true;

                    if (!itExists)
                    {
                        Aresta *nova_aresta = new Aresta();
                        nova_aresta->id_no_alvo = no_saida->id;
                        nova_aresta->peso = aresta->peso;
                        nova_aresta->retorno = aresta->retorno;
                        mapa_nos[aresta->id_no_alvo]->arestas.push_back(nova_aresta);
                    }
                }
            }
            visitados[no_saida->id] = true;
        }
    }
    return grafo_saida;
}

Grafo *Grafo::caminho_profundidade(No *no, Grafo *grafo_saida, map<char, bool> &visitados, char id_no_pai, map<char, int> &profundidades)
{

    // de onde saio para me aprofundar
    No *no_saida = new No();
    no_saida->id = no->id;
    no_saida->peso = no->peso;

    visitados[no->id] = true;
    profundidades[no->id] = profundidades[id_no_pai] + 1;

    // pra cada aresta, adiciono os aprofundamentos
    for (Aresta *aresta : no->arestas)
    {

        // ignora aresta que leva de volta ao pai
        if (aresta->id_no_alvo == id_no_pai)
            continue;

        // encontra no alvo da aresta
        for (No *no_alvo : lista_adj)
        {
            if (no_alvo->id == aresta->id_no_alvo)
            {
                if (!visitados[no_alvo->id])
                {
                    // chama recursivamente para o nó alvo
                    caminho_profundidade(no_alvo, grafo_saida, visitados, no->id, profundidades);

                    Aresta *nova_aresta = new Aresta();
                    nova_aresta->id_no_alvo = no_alvo->id;
                    nova_aresta->peso = aresta->peso;
                    nova_aresta->retorno = false;
                    no_saida->arestas.push_back(nova_aresta);

                    // marca como aresta de retorno caso esteja num nivel maior e não seja o pai do no atual
                }
                else if (id_no_pai != no_alvo->id and profundidades[no->id] > profundidades[no_alvo->id])
                {
                    Aresta *nova_aresta = new Aresta();
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
        if (no->id == id_no_a || no->id == id_no_b)
            existe = true;
    }

    if (!existe)
    {
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

// matriz utilizada para o desenvolvimento da letra h
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

// Evita ter que construir excentricidades 4x para um mesmo grafo
const map<char, int> &Grafo::get_excentricidades()
{
    if (!excentricidades_validas)
    {
        armazena_excentricidades.clear();
        armazena_excentricidades = calcular_excentricidades();
        excentricidades_validas = true;
    }
    return armazena_excentricidades;
}

map<char, int> Grafo::calcular_excentricidades()
{
    if (lista_adj.empty())
    {
        cout << "Grafo vazio - nao e possivel calcular excentricidades." << endl;
        return {};
    }

    int n = this->lista_adj.size();

    vector<vector<int>> dist(n, vector<int>(n, INT_MAX));
    cria_matriz_floyd(dist, false);

    for (int i = 0; i < n; i++)
    {
        for (int j = 0; j < n; j++)
        {
            cout << dist[i][j] << " ";
        }
        cout << endl;
    }

    map<char, int> excentricidades;
    map<int, char> id_por_indice;
    int i = 0;

    for (No *no : this->lista_adj)
    {
        id_por_indice[i] = no->id;
        i++;
    }

    for (int i = 0; i < dist.size(); i++)
    {
        int excentricidade = 0;
        // evita vertice isolado recebe exc 0
        bool alcanca_algum = false;
        bool alcanca_todos = true; // para grafos direcionados

        for (int j = 0; j < dist.size(); j++)
        {
            if (i == j)
                continue;

            if (dist[i][j] != INT_MAX)
            {
                excentricidade = max(excentricidade, dist[i][j]);
                alcanca_algum = true;
            }
            else
            {
                // Se for direcionado e não alcança algum vértice, marca false
                if (this->in_direcionado)
                {
                    alcanca_todos = false;
                }
            }
        }
        if (!alcanca_algum)
        { // vértice isolado
            excentricidades[id_por_indice[i]] = INT_MIN;
        }
        else
        {
            if (this->in_direcionado && !alcanca_todos)
            {
                // Se direcionado e não alcança todos, excentricidade "infinita"
                excentricidades[id_por_indice[i]] = INT_MAX;
            }
            else
            {
                excentricidades[id_por_indice[i]] = excentricidade;
            }
        }
    }
    return excentricidades;
}

void Grafo::calcula_caracteristicas()
{
    map<char, int> excentricidades = calcular_excentricidades();

    if (excentricidades.empty())
    {
        cout << "Raio indefinido - grafo sem vertices ou excentricidades." << endl;
        return;
    }

    for (pair<const char, int> &par : excentricidades)
    {
        cout << " char: " << par.first << " Num: " << par.second;
    }
    cout << endl;

    // calcula raio e diametro
    this->raio = INT_MAX;
    this->diametro = INT_MIN;
    periferia.clear();
    centro.clear();

    for (pair<const char, int> &par : excentricidades)
    {
        if (par.second != INT_MIN && par.second != INT_MAX)
            this->diametro = max(this->diametro, par.second);
    }

    for (pair<const char, int> &par : excentricidades)
    {
        if (par.second != INT_MIN && par.second != INT_MAX && par.second <= this->diametro)
            this->raio = min(this->raio, par.second);
    }

    if (this->raio == INT_MAX)
    {
        cout << "Raio indefinido — nenhum vértice alcança outro." << endl;
        this->raio = 0;
    }

    // calcula centro e periferia
    for (pair<const char, int> &par : excentricidades)
    {
        if (par.second == this->diametro)
        {
            periferia.push_back(par.first);
        }
        if (par.second == this->raio)
        {
            centro.push_back(par.first);
        }
    }
}
