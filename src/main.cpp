#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <map>
#include <set>
#include <queue>
#include <algorithm>
#include <limits>
#include <cmath>
#include <string>
#include <iomanip>

using namespace std;

class Grafo
{
private:
    // estrutura grafo
    int num_vertices;
    set<int> vertices;
    vector<pair<int, int>> arestas;          // Arestas
    vector<pair<int, int>> arcos;            // Arestas direcionadas
    map<int, vector<pair<int, double>>> adj; // Lista de adjacência com pesos

    // obrigatórios
    set<int> vertices_requeridos;
    set<pair<int, int>> arestas_requeridas;
    set<pair<int, int>> arcos_requeridos;

    // deposito inicial
    int deposito;

    string nome;

public:
    Grafo() : num_vertices(0), deposito(0) {}

    double calcularDensidade()
    {

        int num_arestas = arestas.size();
        int num_arcos = arcos.size();

        double densidade = (2.0 * num_arestas + num_arcos) / (num_vertices * (num_vertices - 1));

        return densidade;
    }

    map<int, int> calcularGraus()
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

    int grausMinimo()
    {
        map<int, int> graus = calcularGraus();

        int grau_minimo = numeric_limits<int>::max();
        for (const auto &[v, grau] : graus)
        {
            grau_minimo = min(grau_minimo, grau);
        }
        return grau_minimo;
    }

    int grausMaximo()
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
    //  Retorna uma matriz de distâncias e uma matriz de predecessores
    pair<vector<vector<double>>, vector<vector<int>>> floydWarshall()
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

    vector<int> reconstruirCaminho(const vector<vector<int>> &pred, int origem, int destino)
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
    map<int, double> calcularIntermediacao()
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

    void lerArquivoDados(const string &nome_arquivo)
    {
        ifstream arquivo(nome_arquivo);
        if (!arquivo.is_open())
        {
            cerr << "Erro ao abrir o arquivo: " << nome_arquivo << endl;
            return;
        }

        string linha;

        // Nome da instância
        getline(arquivo, linha);
        nome = linha.substr(linha.find_first_of(":") + 1);
        nome = nome.substr(nome.find_first_not_of(" \t"));

        getline(arquivo, linha); // Optimal value
        getline(arquivo, linha); // #Vehicles
        getline(arquivo, linha); // Capacity

        // Depósito
        getline(arquivo, linha);
        deposito = stoi(linha.substr(linha.find_first_of(":") + 1));

        // Número de nós
        getline(arquivo, linha);
        num_vertices = stoi(linha.substr(linha.find_first_of(":") + 1));

        // Número de arestas e arcos
        int num_arestas, num_arcos;
        getline(arquivo, linha);
        num_arestas = stoi(linha.substr(linha.find_first_of(":") + 1));

        getline(arquivo, linha);
        num_arcos = stoi(linha.substr(linha.find_first_of(":") + 1));

        // Quantidade de elementos requeridos
        int num_vertices_requeridos, num_arestas_requeridas, num_arcos_requeridos;

        getline(arquivo, linha);
        num_vertices_requeridos = stoi(linha.substr(linha.find_first_of(":") + 1));

        getline(arquivo, linha);
        num_arestas_requeridas = stoi(linha.substr(linha.find_first_of(":") + 1));

        getline(arquivo, linha);
        num_arcos_requeridos = stoi(linha.substr(linha.find_first_of(":") + 1));

        for (int i = 1; i <= num_vertices; i++)
        {
            vertices.insert(i);
        }

        // Ler até encontrar "ReN."
        while (getline(arquivo, linha) && linha.find("ReN.") == string::npos)
            ;

        // Ler nós requeridos
        for (int i = 0; i < num_vertices_requeridos; i++)
        {
            getline(arquivo, linha);
            stringstream ss(linha);
            string nome_no;
            int demanda, custo_servico;

            ss >> nome_no >> demanda >> custo_servico;

            int node = stoi(nome_no.substr(1)); // Remove o 'N'
            vertices_requeridos.insert(node);
        }

        // Ler até encontrar "ReE."
        while (getline(arquivo, linha) && linha.find("ReE.") == string::npos)
            ;

        // Ler arestas requeridas
        for (int i = 0; i < num_arestas_requeridas; i++)
        {
            getline(arquivo, linha);
            stringstream ss(linha);
            string edge_name;
            int u, v;
            double custo_travessia, demanda, custo_servico;

            ss >> edge_name >> u >> v >> custo_travessia >> demanda >> custo_servico;

            arestas.push_back({u, v});
            arestas_requeridas.insert({u, v});
            adj[u].push_back({v, custo_travessia});
            adj[v].push_back({u, custo_travessia});
        }

        // Pular até "EDGE"
        while (getline(arquivo, linha) && linha.find("EDGE") == string::npos)
            ;

        // Ler arestas não requeridas
        for (int i = 0; i < num_arestas - num_arestas_requeridas; i++)
        {
            getline(arquivo, linha);
            if (linha.empty())
                break;

            stringstream ss(linha);
            string edge_name;
            int u, v;
            double custo_travessia;

            ss >> edge_name >> u >> v >> custo_travessia;

            arestas.push_back({u, v});
            adj[u].push_back({v, custo_travessia});
            adj[v].push_back({u, custo_travessia});
        }

        // Pular até "ReA."
        while (getline(arquivo, linha) && linha.find("ReA.") == string::npos)
            ;

        // Arcos requeridos
        for (int i = 0; i < num_arcos_requeridos; i++)
        {
            getline(arquivo, linha);
            stringstream ss(linha);
            string arc_name;
            int u, v;
            double custo_travessia, demanda, custo_servico;

            ss >> arc_name >> u >> v >> custo_travessia >> demanda >> custo_servico;

            arcos.push_back({u, v});
            arcos_requeridos.insert({u, v});
            adj[u].push_back({v, custo_travessia});
        }

        // Pular até "ARC"
        while (getline(arquivo, linha) && linha.find("ARC") == string::npos)
            ;

        // Arcos não requeridos
        for (int i = 0; i < num_arcos - num_arcos_requeridos; i++)
        {
            getline(arquivo, linha);
            if (linha.empty())
                break;

            stringstream ss(linha);
            string arc_name;
            int u, v;
            double custo_travessia;

            ss >> arc_name >> u >> v >> custo_travessia;

            arcos.push_back({u, v});
            adj[u].push_back({v, custo_travessia});
        }

        arquivo.close();

        cout << "\n===== RESUMO DO GRAFO =====" << endl;
        cout << "1. Quantidade de vértices: " << num_vertices << endl;
        cout << "2. Quantidade de arestas: " << arestas.size() << endl;
        cout << "3. Quantidade de arcos: " << arcos.size() << endl;
        cout << "4. Quantidade de vértices requeridos: " << vertices_requeridos.size() << endl;
        cout << "5. Quantidade de arestas requeridas: " << arestas_requeridas.size() << endl;
        cout << "6. Quantidade de arcos requeridos: " << arcos_requeridos.size() << endl;
        cout << "7. Densidade do grafo (order strength): " << calcularDensidade() << endl;
        cout << "9. Grau mínimo dos vértices: " << grausMinimo() << endl;
        cout << "10. Grau máximo dos vértices: " << grausMaximo() << endl;
        map<int, double> intermediacao = calcularIntermediacao();
        cout << "11. Intermediação dos vértices:" << endl;
        for (const auto &[v, cent] : intermediacao)
        {
            cout << "   Vértice " << v << ": " << fixed << setprecision(2) << cent << endl;
        }
    }
};

int main()
{
    Grafo grafo;
    grafo.lerArquivoDados("../../data/BHW1.dat");

    return 0;
}