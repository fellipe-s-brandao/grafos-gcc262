#include "grafo.h"
#include <fstream>
#include <sstream>
#include <iomanip>
#include <queue>
#include <cmath>

Grafo::Grafo() : num_vertices(0), valor_otimo(-1), num_veiculos(-1), capacidade(0), deposito(0) {}

double Grafo::calcularDensidade()
{
    int num_arestas = arestas.size();
    int num_arcos = arcos.size();

    double densidade = (2.0 * num_arestas + num_arcos) / (num_vertices * (num_vertices - 1));

    return densidade;
}

map<int, int> Grafo::calcularGraus()
{
    map<int, int> graus;

    // Inicializar todos os vértices com grau 0
    for (int v : vertices)
    {
        graus[v] = 0;
    }

    // Contar arestas (cada aresta adiciona 1 ao grau de ambos os vértices)
    for (const auto &aresta : arestas)
    {
        int u = aresta.first;
        int v = aresta.second;
        graus[u]++;
        graus[v]++;
    }

    // Contar arcos (arco de saída adiciona 1 ao grau de saída, arco de entrada adiciona 1 ao grau de entrada)
    for (const auto &arco : arcos)
    {
        int u = arco.first;
        int v = arco.second;
        graus[u]++;
        graus[v]++;
    }

    return graus;
}

int Grafo::grausMinimo()
{
    map<int, int> graus = calcularGraus();

    int grau_minimo = numeric_limits<int>::max();
    for (const auto &[v, grau] : graus)
    {
        grau_minimo = min(grau_minimo, grau);
    }
    return grau_minimo;
}

int Grafo::grausMaximo()
{
    map<int, int> graus = calcularGraus();

    int grau_maximo = 0;
    for (const auto &[v, grau] : graus)
    {
        grau_maximo = max(grau_maximo, grau);
    }
    return grau_maximo;
}

// Calcular caminhos mínimos de todas as origens para todos os destinos
pair<vector<vector<double>>, vector<vector<int>>> Grafo::floydWarshall()
{
    // Inicializa matriz de distância com infinito
    vector<vector<double>> dist(num_vertices + 1, vector<double>(num_vertices + 1, numeric_limits<double>::infinity()));

    // Inicializa matriz de predecessores com -1
    vector<vector<int>> pred(num_vertices + 1, vector<int>(num_vertices + 1, -1));

    // A distância de um vértice para ele mesmo é 0
    for (int i = 1; i <= num_vertices; i++)
    {
        dist[i][i] = 0;
    }

    // Define as distâncias iniciais com base na lista de adjacência
    for (const auto &[u, vizinhos] : adj)
    {
        for (const auto &[v, peso] : vizinhos)
        {
            dist[u][v] = peso;
            pred[u][v] = u;
        }
    }

    // Algoritmo de Floyd-Warshall
    for (int k = 1; k <= num_vertices; k++)
    {
        for (int i = 1; i <= num_vertices; i++)
        {
            for (int j = 1; j <= num_vertices; j++)
            {
                if (dist[i][k] != numeric_limits<double>::infinity() &&
                    dist[k][j] != numeric_limits<double>::infinity() &&
                    dist[i][k] + dist[k][j] < dist[i][j])
                {
                    dist[i][j] = dist[i][k] + dist[k][j];
                    pred[i][j] = pred[k][j];
                }
            }
        }
    }

    return {dist, pred};
}

// Reconstruir o caminho mais curto entre dois vértices
vector<int> Grafo::reconstruirCaminho(const vector<vector<int>> &pred, int origem, int destino)
{
    vector<int> caminho;

    // Se não existe caminho
    if (pred[origem][destino] == -1)
    {
        return caminho;
    }

    // Reconstruir o caminho do destino até a origem
    int atual = destino;
    while (atual != origem)
    {
        caminho.push_back(atual);
        atual = pred[origem][atual];
    }
    caminho.push_back(origem);

    // Inverter o caminho para ficar da origem para o destino
    reverse(caminho.begin(), caminho.end());

    return caminho;
}

// Cálculo da intermediação (betweenness centrality)
map<int, double> Grafo::calcularIntermediacao()
{
    map<int, double> intermediacao;

    // Inicializa a intermediação de todos os vértices com 0
    for (int v : vertices)
    {
        intermediacao[v] = 0.0;
    }

    // Obter caminhos mais curtos entre todos os pares de vértices
    auto [dist, pred] = floydWarshall();

    // Para cada par de vértices (s,t)
    for (int s : vertices)
    {
        for (int t : vertices)
        {
            // Ignorar quando s = t ou quando não há caminho
            if (s == t || dist[s][t] == numeric_limits<double>::infinity())
            {
                continue;
            }

            // Contar vértices intermediários no caminho mais curto de s a t
            vector<int> caminho = reconstruirCaminho(pred, s, t);

            // Incrementar a intermediação de cada vértice no caminho (exceto s e t)
            for (size_t i = 1; i < caminho.size() - 1; i++)
            {
                intermediacao[caminho[i]] += 1.0;
            }
        }
    }

    return intermediacao;
}

// Calcular o caminho médio do grafo
double Grafo::calcularCaminhoMedio()
{
    // Obter matriz de distâncias usando Floyd-Warshall
    auto [dist, pred] = floydWarshall();

    double soma_distancias = 0.0;
    int count = 0;

    // Somar as distâncias entre todos os pares de vértices
    for (int i = 1; i <= num_vertices; i++)
    {
        for (int j = 1; j <= num_vertices; j++)
        {
            // Ignorar o próprio vértice e caminhos inexistentes
            if (i != j && dist[i][j] != numeric_limits<double>::infinity())
            {
                soma_distancias += dist[i][j];
                count++;
            }
        }
    }

    // Calcular a média
    double caminho_medio = (count > 0) ? (soma_distancias / count) : 0.0;

    return caminho_medio;
}

// Calcular o diâmetro do grafo
double Grafo::calcularDiametro()
{
    // Obter matriz de distâncias usando Floyd-Warshall
    auto [dist, pred] = floydWarshall();

    double diametro = 0.0;

    // Encontrar a maior distância finita entre quaisquer dois vértices
    for (int i = 1; i <= num_vertices; i++)
    {
        for (int j = 1; j <= num_vertices; j++)
        {
            // Ignorar o próprio vértice e caminhos inexistentes
            if (i != j && dist[i][j] != numeric_limits<double>::infinity())
            {
                diametro = max(diametro, dist[i][j]);
            }
        }
    }

    return diametro;
}

// Método para calcular caminhos mais curtos usando o resultado de Floyd-Warshall
void Grafo::calcularCaminhosMaisCurtos()
{
    cout << "Iniciando cálculo de caminhos mais curtos..." << endl;
    
    // Inicializar as matrizes com o tamanho correto
    matriz_dist.clear();
    matriz_pred.clear();
    
    matriz_dist.resize(num_vertices + 1, vector<double>(num_vertices + 1, numeric_limits<double>::infinity()));
    matriz_pred.resize(num_vertices + 1, vector<int>(num_vertices + 1, -1));
    
    // A distância de um vértice para ele mesmo é 0
    for (int i = 1; i <= num_vertices; i++)
    {
        matriz_dist[i][i] = 0;
        matriz_pred[i][i] = i;  // Predecessor de um nó para ele mesmo é o próprio nó
    }
    
    // Define as distâncias iniciais com base na lista de adjacência
    for (const auto &[u, vizinhos] : adj)
    {
        for (const auto &[v, peso] : vizinhos)
        {
            matriz_dist[u][v] = peso;
            matriz_pred[u][v] = u;
        }
    }
    
    // Algoritmo de Floyd-Warshall
    for (int k = 1; k <= num_vertices; k++)
    {
        cout << "Processando vértice intermediário " << k << "/" << num_vertices << endl;
        for (int i = 1; i <= num_vertices; i++)
        {
            for (int j = 1; j <= num_vertices; j++)
            {
                if (matriz_dist[i][k] != numeric_limits<double>::infinity() &&
                    matriz_dist[k][j] != numeric_limits<double>::infinity() &&
                    matriz_dist[i][k] + matriz_dist[k][j] < matriz_dist[i][j])
                {
                    matriz_dist[i][j] = matriz_dist[i][k] + matriz_dist[k][j];
                    matriz_pred[i][j] = matriz_pred[k][j];
                }
            }
        }
    }
    
    cout << "Cálculo de caminhos mais curtos concluído!" << endl;
}
// Método para obter a distância entre dois vértices
double Grafo::getDistancia(int origem, int destino) const
{
    // Verificação de limites
    if (origem <= 0 || origem > num_vertices || destino <= 0 || destino > num_vertices)
    {
        cerr << "Erro em getDistancia: Índices fora dos limites - origem=" << origem << ", destino=" << destino << ", num_vertices=" << num_vertices << endl;
        return numeric_limits<double>::infinity();
    }
    
    // Verifica se a matriz está inicializada corretamente
    if (matriz_dist.empty() || matriz_dist.size() <= (size_t)origem || 
        matriz_dist[origem].empty() || matriz_dist[origem].size() <= (size_t)destino)
    {
        cerr << "Erro em getDistancia: Matriz de distâncias não inicializada corretamente!" << endl;
        cerr << "matriz_dist.size()=" << matriz_dist.size() << ", origem=" << origem << endl;
        if (!matriz_dist.empty() && matriz_dist.size() > (size_t)origem) {
            cerr << "matriz_dist[origem].size()=" << matriz_dist[origem].size() << ", destino=" << destino << endl;
        }
        return numeric_limits<double>::infinity();
    }
    
    return matriz_dist[origem][destino];
}

// Método para obter o caminho entre dois vértices
vector<int> Grafo::obterCaminho(int origem, int destino) const
{
    if (origem == destino) {
        return {origem}; // Retorna um caminho contendo apenas o próprio nó
    }
    
    // Verificação de limites
    if (origem <= 0 || origem > num_vertices || destino <= 0 || destino > num_vertices)
    {
        cerr << "Erro em obterCaminho: Índices fora dos limites - origem=" << origem << ", destino=" << destino << ", num_vertices=" << num_vertices << endl;
        return vector<int>();
    }
    
    // Verifica se a matriz está inicializada corretamente
    if (matriz_pred.empty() || matriz_pred.size() <= (size_t)origem || 
        matriz_pred[origem].empty() || matriz_pred[origem].size() <= (size_t)destino)
    {
        cerr << "Erro em obterCaminho: Matriz de predecessores não inicializada corretamente!" << endl;
        cerr << "matriz_pred.size()=" << matriz_pred.size() << ", origem=" << origem << endl;
        if (!matriz_pred.empty() && matriz_pred.size() > (size_t)origem) {
            cerr << "matriz_pred[origem].size()=" << matriz_pred[origem].size() << ", destino=" << destino << endl;
        }
        return vector<int>();
    }
    
    vector<int> caminho;
    
    // Se não existe caminho
    if (matriz_pred[origem][destino] == -1)
    {
        cerr << "Aviso: Não existe caminho de " << origem << " para " << destino << endl;
        return caminho;
    }
    
    // Reconstruir o caminho do destino até a origem
    int atual = destino;
    while (atual != origem)
    {
        caminho.push_back(atual);
        atual = matriz_pred[origem][atual];
        
        // Evitar loop infinito se houver inconsistência
        if (static_cast<int>(caminho.size()) > num_vertices)
        {
            cerr << "Erro em obterCaminho: Loop detectado na reconstrução do caminho de " << origem << " para " << destino << endl;
            return vector<int>();
        }
    }
    caminho.push_back(origem);
    
    // Inverter o caminho para ficar da origem para o destino
    reverse(caminho.begin(), caminho.end());
    
    return caminho;
}

void Grafo::lerArquivoDados(const string &nome_arquivo)
{
    // Limpar dados anteriores
    vertices.clear();
    vertices_requeridos.clear();
    arestas.clear();
    arestas_requeridas.clear();
    arcos.clear();
    arcos_requeridos.clear();
    adj.clear();
    servicos.clear();
    matriz_dist.clear();
    matriz_pred.clear();
    
    // Usar o LeitorArquivo para ler os dados
    DadosGrafo dados = LeitorArquivo::lerArquivoDados(nome_arquivo);
    
    // Carregar os dados na classe
    carregarDados(dados);
    
    // Calcular caminhos mais curtos
    calcularCaminhosMaisCurtos();
}

void Grafo::carregarDados(const DadosGrafo &dados)
{
    // Copiar estrutura básica
    num_vertices = dados.num_vertices;
    vertices = dados.vertices;
    arestas = dados.arestas;
    arcos = dados.arcos;
    adj = dados.adj;
    
    // Copiar elementos requeridos
    vertices_requeridos = dados.vertices_requeridos;
    arestas_requeridas = dados.arestas_requeridas;
    arcos_requeridos = dados.arcos_requeridos;
    
    // Copiar metadados
    nome = dados.nome;
    valor_otimo = dados.valor_otimo;
    num_veiculos = dados.num_veiculos;
    capacidade = dados.capacidade;
    deposito = dados.deposito;
    
    // Copiar serviços
    servicos = dados.servicos;
}