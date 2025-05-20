#ifndef GRAFO_H
#define GRAFO_H

#include <iostream>
#include <vector>
#include <map>
#include <set>
#include <string>
#include <limits>
#include <algorithm>

using namespace std;

// Estrutura para representar um serviço
struct Servico
{
    int id;
    char tipo; // 'N', 'E', ou 'A'
    int origem;
    int destino;
    int demanda;
    int custo_servico;
    int custo_transporte;
};

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

    // Novos campos para a Etapa 2
    vector<Servico> servicos; // Lista de todos os serviços
    int valor_otimo;          // Valor ótimo da solução
    int num_veiculos;         // Número de veículos
    int capacidade;           // Capacidade dos veículos
    int deposito;             // Nó depósito

    vector<vector<double>> matriz_dist; // Matriz de distâncias
    vector<vector<int>> matriz_pred;    // Matriz de predecessores

    string nome;

public:
    Grafo();

    int getNumVertices() const { return num_vertices; }
    string getNomeArquivo() const { return nome; }
    int getNumEdges() const { return arestas.size(); }
    int getNumArcs() const { return arcos.size(); }
    int getNumRequiredVertices() const { return vertices_requeridos.size(); }
    int getNumRequiredEdges() const { return arestas_requeridas.size(); }
    int getNumRequiredArcs() const { return arcos_requeridos.size(); }
    string getNome() const { return nome; }

    // Novos getters
    int getCapacidade() const { return capacidade; }
    int getDeposito() const { return deposito; }
    const vector<Servico> &getServicos() const { return servicos; }

    double calcularDensidade();
    map<int, int> calcularGraus();
    int grausMinimo();
    int grausMaximo();

    // Calcular caminhos mínimos de todas as origens para todos os destinos
    pair<vector<vector<double>>, vector<vector<int>>> floydWarshall();

    // Reconstruir o caminho mais curto entre dois vértices
    vector<int> reconstruirCaminho(const vector<vector<int>> &pred, int origem, int destino);

    // Cálculo da intermediação (betweenness centrality)
    map<int, double> calcularIntermediacao();

    // Calcular o caminho médio do grafo
    double calcularCaminhoMedio();

    // Calcular o diâmetro do grafo
    double calcularDiametro();

    // Novos métodos
    void calcularCaminhosMaisCurtos();
    double getDistancia(int origem, int destino) const;
    vector<int> obterCaminho(int origem, int destino) const;

    void lerArquivoDados(const string &nome_arquivo);
};

#endif // GRAFO_H