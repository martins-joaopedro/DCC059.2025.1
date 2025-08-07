#include "Gulosos.h"
#include "Gerenciador.h"

#include <iostream>
#include <fstream>
#include <sstream>
#include <map>
#include <vector>
#include <set>
#include <cstdlib>
#include <ctime>
#include <iomanip>
#include <unordered_map>
#include <unordered_set>

using namespace std;

//
//
//
/* GULOSO */
void Gulosos::resetar_dominacao(Grafo *grafo)
{
    for (auto no : grafo->lista_adj)
    {
        no->dominado = false;
    }
}

No *Gulosos::get_no(char id, Grafo *grafo)
{
    for (No *no : grafo->lista_adj)
    {
        if (no->id == id)
            return no;
    }
    return nullptr;
}

vector<char> Gulosos::get_vizinhos(char id_no, Grafo *grafo)
{
    vector<char> vizinhos;
    No *no = get_no(id_no, grafo);
    if (!no)
        return vizinhos;

    for (Aresta *aresta : no->arestas)
    {
        vizinhos.push_back(aresta->id_no_alvo);
    }
    return vizinhos;
}

bool Gulosos::conjunto_dominante(const vector<char> &D, Grafo *grafo)
{
    resetar_dominacao(grafo);

    for (char id : D)
    {
        No *no = get_no(id, grafo);
        if (no)
        {
            no->dominado = true;
            for (Aresta *aresta : no->arestas)
            {
                No *destino = get_no(aresta->id_no_alvo, grafo);
                if (destino)
                    destino->dominado = true;
            }
        }
    }
    for (No *no : grafo->lista_adj)
    {
        if (!no->dominado)
            return false;
    }
    return true;
}

bool Gulosos::conjunto_independente(const vector<char> &D, Grafo *grafo)
{
    set<char> conjunto(D.begin(), D.end());

    for (char id : D)
    {
        No *no = get_no(id, grafo);
        if (!no)
            continue;

        for (Aresta *aresta : no->arestas)
        {
            if (conjunto.count(aresta->id_no_alvo))
                return false;
        }
    }

    return true;
}
void Gulosos::reorganiza(vector<pair<char, int>> &vertice_grau_ordenado, Grafo *grafo)
{
    unordered_map<char, int> novo_grau;
    for (const auto &par : vertice_grau_ordenado)
    {
        novo_grau[par.first] = 0; // inicializa com 0, para calcular o "grau" depois
    }

    // Recalcula grau baseado apenas nos vértices restantes
    for (No *no : grafo->lista_adj)
    {
        char id = no->id;

        // Se o vértice já saiu, deixa o loop achar outro v
        if (novo_grau.find(id) == novo_grau.end())
            continue;

        int grau = 0;
        for (Aresta *a : no->arestas)
        {
            if (novo_grau.find(a->id_no_alvo) != novo_grau.end())
                grau++;
        }

        novo_grau[id] = grau;
    }

    // Atualiza o vetor original com os novos graus
    for (auto &par : vertice_grau_ordenado)
    {
        par.second = novo_grau[par.first];
    }

    stable_sort(vertice_grau_ordenado.begin(), vertice_grau_ordenado.end(),
         [](const auto &a, const auto &b)
         {
             return a.second > b.second;
         });
}

vector<char> Gulosos::heuristica_gulosa(Grafo *grafo, ofstream& file)
{
    vector<char> D; // solução vazia inicialmente

    vector<pair<char, int>> vertice_grau_ordenado;
    for (No *no : grafo->lista_adj)
    {
        vertice_grau_ordenado.push_back({no->id, no->arestas.size()});
    }

    stable_sort(vertice_grau_ordenado.begin(), vertice_grau_ordenado.end(), [](const auto &a, const auto &b)
         { return a.second > b.second; }); // ordena os vértices em ordem decrescente

    int i = 0;
    while (!conjunto_dominante(D, grafo) && !vertice_grau_ordenado.empty())
    {
        if (i != 0)
            reorganiza(vertice_grau_ordenado, grafo);

        for (const auto &pair : vertice_grau_ordenado)
        {
            file << "(" << pair.first << " - " << pair.second << ") " << endl;
        }

        file << "------------------------" << endl;
        pair<char, int> candidato = vertice_grau_ordenado[0];
        vector<char> vizinhos = get_vizinhos(candidato.first, grafo);

        D.push_back(candidato.first);
        vertice_grau_ordenado.erase(remove(vertice_grau_ordenado.begin(), vertice_grau_ordenado.end(), candidato), vertice_grau_ordenado.end());

        if (!vizinhos.empty())
        {
            for (char v : vizinhos)
            {
                for (auto it = vertice_grau_ordenado.begin(); it != vertice_grau_ordenado.end();)
                {
                    if (it->first == v)
                        it = vertice_grau_ordenado.erase(it);
                    else
                        ++it;
                }
            }
        }
        i++;
    }

    for (char i : D)
    {
        file << i << " - ";
    }

}

//
//
//
/* RANDOMIZADO ADAPTATIVO */
vector<char> Gulosos::randomized_heuristic(vector<char> LC, map<char, No *> &mapa_nos, int k, ofstream& file)
{

    map<char, int> degree;

    if (k == 0)
    {
        // calcula o grau de cada nó
        for (char no : LC)
            degree[no] = mapa_nos[no]->arestas.size();
    }
    else
    {
        // calcula o quanto cada grau pode dominar na lista de candidatos
        for (char no : LC)
        {
            int domain = 1; // considera ele mesmo
            for (Aresta *aresta : mapa_nos[no]->arestas)
            {
                if (!mapa_nos[aresta->id_no_alvo]->dominado)
                    domain++;
            }
            degree[no] = domain;
        }
    }

    // reordena os nós por grau e remonta a LC
    LC.clear();

    file << "\n=> LC: " << k << endl;
    while (!degree.empty())
    {
        auto max_it = degree.begin();
        for (auto it = degree.begin(); it != degree.end(); ++it)
        {
            if (it->second > max_it->second)
                max_it = it;
        }

        LC.push_back(max_it->first);
        file << "(" << max_it->first << " ; " << max_it->second << ") ";

        // remove do map pós iteração
        degree.erase(max_it);
    }
    file << endl;

    return LC;
}

void Gulosos::updates_domain(map<char, No *> &mapa_nos, vector<char> S)
{
    for (char no : S)
    {
        mapa_nos[no]->dominado = true;
        for (Aresta *aresta : mapa_nos[no]->arestas)
            mapa_nos[aresta->id_no_alvo]->dominado = true;
    }
}

// lógica adaptativa de atualização da lista LC
vector<char> Gulosos::updates_LC(map<char, No *> &mapa_nos, vector<char> LC, vector<char> S)
{

    set<char> neighborhood = set<char>();

    // critério de atualização: removo os nós vizinhos de v da LC
    for (char no : S)
    {
        for (Aresta *aresta : mapa_nos[no]->arestas)
        {
            char neighbor = aresta->id_no_alvo;
            neighborhood.insert(neighbor);
        }
    }

    vector<char> new_LC = vector<char>();

    for (char no : LC)
    {
        // se ele for vizinho de algum nó em S, não o adiciono
        if (find(neighborhood.begin(), neighborhood.end(), no) != neighborhood.end())
            continue;

        // nao posso recolocar candidatos de S
        if (find(S.begin(), S.end(), no) != S.end())
            continue;

        new_LC.push_back(no);
    }

    return new_LC;
}

// algoritmo adaptativo guloso randomizado
vector<char> Gulosos::randomized_adaptative_greedy(Grafo *grafo, float alfa, ofstream& file)
{

    bool debug = true;
    map<char, No *> mapa_nos;
    vector<char> LC;

    // monta o mapa de nos para facilitar e a lista de candidatos
    for (No *no : grafo->lista_adj)
    {
        no->dominado = false; // reseta a dominancia para não influenciar em outras itr
        mapa_nos[no->id] = no;
        LC.push_back(no->id);
    }

    // euristica: inicialmente ordena os candidatos pelo grau do no
    // depois para garantir dominancia, pelo grau de cobertura, ambos de forma crescente
    int k = 0;
    LC = randomized_heuristic(LC, mapa_nos, k, file);
    vector<char> S = vector<char>();

    while (!LC.empty())
    {

        k++;
        int LCR = max(1, int(alfa * LC.size()));
        int randomized = rand() % LCR;

        // seleciona o no e LC <- LC - {v}
        char no = LC[randomized];
        LC.erase(LC.begin() + randomized);

        if (debug)
            file << "Escolhendo no: " << no << endl;

        S.push_back(no);

        // atualiza dominancia dos nós
        updates_domain(mapa_nos, S);

        // garanto todas as condições para independencia
        LC = updates_LC(mapa_nos, LC, S);

        // reorganiza a LC
        LC = randomized_heuristic(LC, mapa_nos, k, file);

        if (debug)
        {
            file << "[ S:  " << k << " ]" << endl;
            for (char no : S)
                file << no << " ";
            file << endl;
        }

        if (debug)
        {
            file << "[ LC:  " << k << " ]" << endl;
            for (char no : LC)
                file << no << " ";
            file << endl;
        }
    }

    return S;
}

bool Gulosos::check_validity(vector<char> S, Grafo *grafo, ofstream& file)
{

    set<char> domain = set<char>();
    set<char> dominated = set<char>();

    // para cada no de S ver se ele domina todos
    for (char v : S)
        for (No *no : grafo->lista_adj)
        {
            domain.insert(no->id); // sempre que eu buscar por um no
            if (v == no->id)
            {
                dominated.insert(no->id); // sempre que eu achar um nó e tudo que ele domina
                for (Aresta *aresta : no->arestas)
                    dominated.insert(aresta->id_no_alvo);
            }
        }

    // ver se para todos da solução é independente
    bool cond = true;
    for (auto v : S)
        for (No *no : grafo->lista_adj)
            if (v == no->id)
                for (Aresta *aresta : no->arestas)
                    if (find(S.begin(), S.end(), aresta->id_no_alvo) != S.end())
                    {
                        cond = false;
                        file << "(X) nao eh independente" << endl;
                        break;
                    }

    file << "Vertices: ";
    for (auto no : domain)
        file << no << " ";

    file << "\nDominados: ";
    for (auto no : domain)
        file << no << " ";

    file << "\nSolucao: ";
    for (auto no : S)
        file << no << " ";
    file << "\n";

    if (domain == dominated && cond)
    {
        file << "===> Solucao VALIDA! |S| = " << S.size() << "\n";
        return true;
    }
    else
        file << "===> Solucao INVALIDA\n";
    return false;
}

//
//
//
/* REATIVO */
void Gulosos::updates_probability(vector<float> &P, vector<float> &M, int m, int solBest_size, ofstream& file)
{
    file << "\n\e[36m---------------- Atualizando Probabilidades ----------------\e[0m\n";

    vector<float> scores(m, 0.0); // transfoma media em pontuação, menores medias tem pontuação maior
    float sum_scores = 0.0;       // soma dos scores de todos os alphas
    int i;

    for (i = 0; i < m; i++)
    {
        if (M[i] != 0)
            scores[i] = float(solBest_size) / M[i];
        else
            scores[i] = 0.0;

        sum_scores += scores[i];
    }

    file << "\nSCORES: ";
    for (i = 0; i < m; i++)
    {
        if (sum_scores > 0.0)
            P[i] = scores[i] / sum_scores;
        else
            P[i] = 1.0f / m;

        file << scores[i] << " ; ";
    }
    file << endl
         << "SUM_SCORES: " << sum_scores << endl;
}

void Gulosos::updates_means(vector<float> &M, vector<int> &sum_sols, vector<int> &count, vector<char> &s, int index_alpha)
{
    count[index_alpha]++;
    sum_sols[index_alpha] += s.size();
    M[index_alpha] = float(sum_sols[index_alpha]) / count[index_alpha];
}

int Gulosos::choose_alpha(vector<float> &P)
{
    vector<float> P_interval(P.size() + 1); // vetor de intervalo probabilidades
    P_interval[0] = 0.0;

    float r = float(rand() % 100) / 100.0; // numero random de 0 a 1

    for (int i = 0; i < P.size(); i++)
    {
        P_interval[i + 1] = P_interval[i] + P[i]; // acumula probabilidades

        if (r >= P_interval[i] && r <= P_interval[i + 1]) // coloquei igual tbm caso r for 0 ou 1
            return i;
    }

    return -1;
}

void Gulosos::imprime_prob(vector<float> &alphas, vector<float> &P, vector<float> &M, vector<int> &sum_sols, vector<int> &count, int m, ofstream& file)
{
    file << endl
         << left; // alinha o texto à esquerda

    // Cabeçalho
    file << setw(10) << "Alfa"
         << setw(15) << "Probability"
         << setw(15) << "Mean"
         << setw(15) << "sum_sols"
         << setw(15) << "count"
         << endl;

    file << string(60, '-') << endl;

    for (int i = 0; i < m; i++)
    {
        file << setw(10) << alphas[i]
             << setw(15) << P[i]
             << setw(15) << M[i]
             << setw(15) << sum_sols[i]
             << setw(15) << count[i]
             << endl;
    }
}

// algoritmo randomizado adaptativo reativo
vector<char> Gulosos::randomized_adaptative_reactive_greedy(Grafo *grafo, vector<float> &alphas, int m, int nIter, int bloco, ofstream& file)
{

    vector<char> solBest, s;
    vector<float> P(m, (1.0 / m)); // probabilidade de alphas
    vector<float> M(m, 0.0);       // média da qualidade
    vector<int> sum_sols(m, 0.0);  // somatorio do tamanho da solução
    vector<int> count(m, 0.0);     // contagem de quantas vezes cada alpha foi usado
    int index_alpha = 0;           // index para acessar alfa nos vetores

    srand(time(0)); // inicializa semente com hora atual
    int i = 0;
    bool isValid;

    for (; index_alpha < m; index_alpha++)
    {
        file << "\n\e[33mALFA----------------------------------->: " << alphas[index_alpha] << "\e[0m" << endl;

        s = randomized_adaptative_greedy(grafo, alphas[index_alpha], file);
        isValid = check_validity(s, grafo, file);

        if (isValid)
        { // perguntar se tem que calcular mesmo se nao for valida, soluçoes vazias sao validas?
            updates_means(M, sum_sols, count, s, index_alpha);

            // if(solBest.empty() || s.size() < solBest.size()){
            //     solBest = s;
            // }
        }
        else
            file << "SOLUÇÃO INVALIDA" << endl;
    }

    // updates_probability(P, M, m, solBest.size());
    imprime_prob(alphas, P, M, sum_sols, count, m, file);

    while (i < nIter)
    {
        file << "\e[34m========================================================== i = "
             << i << " ==========================================================\e[0m" << endl;

        imprime_prob(alphas, P, M, sum_sols, count, m, file);

        if (i != 0 && i % bloco == 0)
        { // atualiza prob
            updates_probability(P, M, m, solBest.size(), file);
            imprime_prob(alphas, P, M, sum_sols, count, m, file);
        }

        i++;
        index_alpha = choose_alpha(P);
        file << "\n\e[33mALFA----------------------------------->: " << alphas[index_alpha] << "\e[0m" << endl;

        if (index_alpha == -1)
        {
            file << "Alfa invalido" << endl;
            continue;
        }

        s = randomized_adaptative_greedy(grafo, alphas[index_alpha], file);
        bool isValid = check_validity(s, grafo, file);

        if (isValid)
        { // perguntar se tem que calcular mesmo se nao for valida, soluçoes vazias sao validas?
            updates_means(M, sum_sols, count, s, index_alpha);

            if (solBest.empty() || s.size() < solBest.size())
            {
                solBest = s;
            }
        }
        else
            file << "SOLUÇÃO INVALIDA" << endl;

        file << "\n\e[32mBEST SOLUTION-------------------------------->: " << solBest.size() << "\e[0m\n";
        file << endl;
    }

    return solBest;
}

//
//
//
/* EXECUÇÃO */
void Gulosos::run_greedy(Grafo *grafo, ofstream& file)
{
    Gulosos g;
    g.heuristica_gulosa(grafo, file);
}

void Gulosos::run_randomized_adaptive_greedy(Grafo *grafo, ofstream& file)
{
    Gulosos g;
    map<float, vector<int>> sols;
    float alphas[] = {0.1, 0.2, 0.3, 0.4, 0.5, 0.6};
    int MAX_IT = 1;
    srand(time(0));

    for (auto alpha : alphas)
    {
        // setColor(1 + alpha * 10);
        file << "Alpha: " << alpha << endl;
        sols[alpha] = vector<int>();
        for (int i = 0; i < MAX_IT; i++)
        {
            file << "\n[ Iteracao: " << i + 1 << " ]" << endl;
            vector<char> S = g.randomized_adaptative_greedy(grafo, alpha, file);
            bool isValid = g.check_validity(S, grafo, file);

            if (isValid)
                sols[alpha].push_back(S.size());
            else
                file << "SOLUÇÃO INVALIDA" << endl;
        }
        file << endl;
    }

    for (auto alpha : alphas)
    {
        file << "[ Alpha: " << alpha << " ] ";
        for (auto S_size : sols[alpha])
            file << S_size << " ";
        file << endl;
    }
}

void Gulosos::run_randomized_adaptative_reactive_greedy(Grafo *grafo, ofstream& file)
{
    Gulosos g;
    int m = 3, nIter = 6, bloco = 2;
    vector<float> alphas = {0.1, 0.5, 0.9};

    // cout << "Numero de iterações: ";
    // cin >> nIter;

    // cout << "Tamanho do bloco: ";
    // cin >> bloco;

    // cout << "Quantidade de alfas: ";
    // cin >> m;

    // for(int i = 0; i < m; i++){
    //     cout << "Alfa[" << i << "]: ";
    //     cin >> alphas[i];
    // }

    file << "\n\e[31m========================================================= INICIO"
         << " ==========================================================\e[0m\n";

    vector<char> s = g.randomized_adaptative_reactive_greedy(grafo, alphas, m, nIter, bloco, file);
    file << "\e[35mMELHOR SOLUÇÃO: " << s.size() << endl;

    for (auto no : s)
        file << no << " ";
    file << endl;

    file << "\n\e[31m========================================================== FIM"
         << " ===========================================================\e[0m\n";
}