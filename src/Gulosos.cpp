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


//Capturar todas as soluçõe e tempos dos algoritmos gulosos para cada alpha
//Ajuda a manter os resustados das 30 iterações
map<float, vector<int>> Gulosos::sols;
map<float, vector<double>> Gulosos::tempos;

//Transforma dominação de todas os nós para false
void Gulosos::reset_domain(Grafo *grafo)
{
    for (auto no : grafo->lista_adj)
    {
        no->dominado = false;
    }
}


No *Gulosos::get_node(char id, Grafo *grafo)
{
    for (No *no : grafo->lista_adj)
    {
        if (no->id == id)
            return no;
    }
    return nullptr;
}

//Retorna todos os vizinhos de um nó
vector<char> Gulosos::get_neighbors(char id_no, Grafo *grafo)
{
    vector<char> vizinhos;
    No *no = get_node(id_no, grafo);
    if (!no)
        return vizinhos;

    for (Aresta *aresta : no->arestas)
    {
        vizinhos.push_back(aresta->id_no_alvo);
    }
    return vizinhos;
}

//Confere se todos os nós estão dominados ou tem vizinhos dominados
bool Gulosos::dominant_set(const vector<char> &D, Grafo *grafo)
{
    reset_domain(grafo);
    
    for (char id : D)
    {
        No *no = get_node(id, grafo);
        if (no)
        {
            no->dominado = true;
            for (Aresta *aresta : no->arestas)
            {
                No *destino = get_node(aresta->id_no_alvo, grafo);
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

//Confere que dois nós dominantes nao sao adjacentes
bool Gulosos::independent_set(const vector<char> &D, Grafo *grafo)
{
    set<char> conjunto(D.begin(), D.end());

    for (char id : D)
    {
        No *no = get_node(id, grafo);
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

//Ordena vetor em ordem decrescente de grau
void Gulosos::reorganize(vector<pair<char, int>> &vertice_grau_ordenado, Grafo *grafo)
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


vector<char> Gulosos::greedy_heuristic(Grafo *grafo, ofstream &file)
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
    //Enquanto o conjunto não é dominano e ainda existem candidatos na lista
    while (!dominant_set(D, grafo) && !vertice_grau_ordenado.empty())
    {
        if (i != 0)
            reorganize(vertice_grau_ordenado, grafo);

        for (const auto &pair : vertice_grau_ordenado)
        {
            file << "(" << pair.first << " - " << pair.second << ") " << endl;
        }

        file << "------------------------" << endl;
        pair<char, int> candidato = vertice_grau_ordenado[0];
        vector<char> vizinhos = get_neighbors(candidato.first, grafo);

        D.push_back(candidato.first);
        vertice_grau_ordenado.erase(remove(vertice_grau_ordenado.begin(), vertice_grau_ordenado.end(), candidato), vertice_grau_ordenado.end());

        //Tira da lista de condidatos os vizinhos do nó colocado no conjunto solução
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

    file << endl;

    file << "[ Conjunto |S| ]: " << D.size() << endl;

    return D;
}

//
//
//
/* RANDOMIZADO ADAPTATIVO */
vector<char> Gulosos::randomized_heuristic(vector<char> LC, map<char, No *> &mapa_nos, int k, ofstream &file)
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

//Atualiza dominio 
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
vector<char> Gulosos::randomized_adaptative_greedy(Grafo *grafo, float alfa, ofstream &file)
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

bool Gulosos::check_validity(vector<char> S, Grafo *grafo, ofstream &file)
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

//atualiza propabilidades
void Gulosos::updates_probability(vector<float> &P, vector<float> &M, int m, int solBest_size, ofstream &file)
{
    file << "\n---------------- Atualizando Probabilidades ----------------\n";

    vector<float> scores(m, 0.0); // transfoma media em pontuação, menores medias tem maior pontuação
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

//atualia medias
void Gulosos::updates_means(vector<float> &M, vector<int> &sum_sols, vector<int> &count, vector<char> &s, int index_alpha)
{
    count[index_alpha]++; //incrementa contador do alfa referido
    sum_sols[index_alpha] += s.size(); //adiciona nova solucao ao somatorio
    M[index_alpha] = float(sum_sols[index_alpha]) / count[index_alpha]; //calcula media
}

// escolhe o alfa
int Gulosos::choose_alpha(vector<float> &P)
{
    vector<float> P_interval(P.size() + 1); // vetor de intervalo probabilidades
    P_interval[0] = 0.0;

    float r = float(rand() % 100) / 100.0; // gera numero random de 0 a 1

    for (int i = 0; i < P.size(); i++)
    {
        P_interval[i + 1] = P_interval[i] + P[i]; // acumula probabilidades

        if (r >= P_interval[i] && r <= P_interval[i + 1]) // coloquei igual tbm caso r for 0 ou 1
            return i;
    }

    return -1;
}

//funcao para imprimir tabela de probabilidades dos alfas
void Gulosos::imprime_prob(vector<float> &alphas, vector<float> &P, vector<float> &M, vector<int> &sum_sols, vector<int> &count, int m, ofstream &file)
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

    //informações
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
vector<char> Gulosos::randomized_adaptative_reactive_greedy(Grafo *grafo, vector<float> &alphas, int m, int nIter, int bloco, ofstream &file)
{

    vector<char> solBest, s;
    vector<float> P(m, (1.0 / m)); // probabilidade de alphas
    vector<float> M(m, 0.0);       // média da qualidade
    vector<int> sum_sols(m, 0.0);  // somatorio do tamanho da solução
    vector<int> count(m, 0.0);     // contagem de quantas vezes cada alpha foi usado
    int index_alpha = 0;           // index para acessar alfa nos vetores

    int i = 0;
    bool isValid;

    /* roda a primeira vez com todos o alfas para garantir que cada um é usado pelo menos uma vez
        caso contrario teriamos divisoes por zero nno calculo da probabilidade da ja que a media seria zero
    */
    for (; index_alpha < m; index_alpha++)
    {
        file << "\nALFA----------------------------------->: " << alphas[index_alpha] << endl;

        s = randomized_adaptative_greedy(grafo, alphas[index_alpha], file);
        isValid = check_validity(s, grafo, file);

        if (isValid)
        {
            updates_means(M, sum_sols, count, s, index_alpha);
        }
        else
            file << "SOLUÇÃO INVALIDA" << endl;
    }

    // updates_probability(P, M, m, solBest.size());
    imprime_prob(alphas, P, M, sum_sols, count, m, file);
    
    //laço principal
    while (i < nIter)
    {
        file << "========================================================== i = "
             << i << " ==========================================================" << endl;

        imprime_prob(alphas, P, M, sum_sols, count, m, file);

        if (i != 0 && i % bloco == 0)
        { // atualiza probabilidades a cada bloco de execução
            updates_probability(P, M, m, solBest.size(), file);
            imprime_prob(alphas, P, M, sum_sols, count, m, file);
        }

        i++;
        index_alpha = choose_alpha(P);
        file << "\nALFA----------------------------------->: " << alphas[index_alpha] << endl;

        if (index_alpha == -1)
        {// conferese o indice é valido, se nao pula para proxima iteracao
            file << "Alfa invalido" << endl;
            continue;
        }

        s = randomized_adaptative_greedy(grafo, alphas[index_alpha], file);
        bool isValid = check_validity(s, grafo, file);

        if (isValid)
        { //checa se a solução gerada é valida
            updates_means(M, sum_sols, count, s, index_alpha);

            if (solBest.empty() || s.size() < solBest.size())
            {
                solBest = s;
            }
        }
        else
            file << "SOLUÇÃO INVALIDA" << endl;

        file << "\nBEST SOLUTION-------------------------------->: " << solBest.size() << endl;
        file << endl;
    }
    file << "==================================================== RESULTADO FINAL "
         << "====================================================" << endl;
    imprime_prob(alphas, P, M, sum_sols, count, m, file);
    file << endl;

    return solBest;
}

//
//
//
/* EXECUÇÃO */

vector<char> Gulosos::run_greedy(Grafo *grafo, ofstream &file)
{
    Gulosos g;
    vector<char> S = g.greedy_heuristic(grafo, file);
    return S;
}

void Gulosos::run_randomized_adaptive_greedy(Grafo *grafo, ofstream &file)
{
    // abre o arquivo para internamente salvar cada solução de cada alpha
    ofstream file_stats = ofstream("output/stats.txt", ios::app);
    Gulosos g;
    float alphas[] = {0.1, 0.3, 0.5};
    int MAX_IT = 30;

    for (auto alpha : alphas) {   
        clock_t start_time = clock();
        file << "Alpha: " << alpha << endl;
        file_stats << "ALPHA: " << alpha << endl;
        for (int i = 0; i < MAX_IT; i++) {
            file << "\n[ Iteracao: " << i + 1 << " ]" << endl;
            vector<char> S = g.randomized_adaptative_greedy(grafo, alpha, file);
            file_stats << "|S|: " << S.size() << " ";
            bool isValid = g.check_validity(S, grafo, file);

            if (isValid)
                sols[alpha].push_back(S.size());
            else
                file << "SOLUÇÃO INVALIDA" << endl;
        }
        file << endl;
        clock_t end_time = clock();
        double tempo = double(end_time - start_time) / CLOCKS_PER_SEC;
        file_stats << "|T PARA 30 ITERACOES |: " << tempo << endl;
        tempos[alpha].push_back(tempo);
    }
    file_stats.close();
}

// para cada alfa calcula as médias e tempos ao final das N interações
void Gulosos::print_means_randomized_greedy() {

    ofstream file_stats = ofstream("output/stats.txt", ios::app);
    float alphas[] = {0.1, 0.3, 0.5};

    for(auto alpha : alphas) {
        
        file_stats << "[ ALPHA: " << alpha << " ] ";
        double soma_tempos = 0.0;
        double soma_sol = 0.0;
        
        file_stats << "\nSOLUÇÕES : " << endl;
        for (auto S_size : sols[alpha]) {
           
            file_stats << S_size << " ";
            soma_sol += S_size;
        }

        file_stats << "\nTEMPOS : ";
        for (auto tempo : tempos[alpha]) {
          
            file_stats << tempo << " ";
            soma_tempos += tempo;
        }

        file_stats << "\n\nMEDIA SOLUÇÕES " << soma_sol / (30*10) << endl;
        file_stats << "MEDIA TEMPO " << soma_tempos / (30*10) << endl << endl;
    }
    file_stats.close();
}

vector<char> Gulosos::run_randomized_adaptative_reactive_greedy(Grafo *grafo, ofstream &file) {    
    Gulosos g;
    int m = 3, nIter = 300, bloco = 50;
    vector<float> alphas = {0.1, 0.3, 0.5};

    file << "\n========================================================= INICIO"
         << " ==========================================================\n";

    vector<char> s = g.randomized_adaptative_reactive_greedy(grafo, alphas, m, nIter, bloco, file);
    file << "MELHOR SOLUÇÃO: " << s.size() << endl;

    for (auto no : s)
        file << no << " ";
    file << endl;

    file << "\n========================================================== FIM"
         << " ===========================================================\n";

    return s;
}