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
    Grafo();

    int getNumVertices() const { return num_vertices; }
    int getNumEdges() const { return arestas.size(); }
    int getNumArcs() const { return arcos.size(); }
    int getNumRequiredVertices() const { return vertices_requeridos.size(); }
    int getNumRequiredEdges() const { return arestas_requeridas.size(); }
    int getNumRequiredArcs() const { return arcos_requeridos.size(); }
    string getNome() const { return nome; }

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

    void lerArquivoDados(const string &nome_arquivo);
};

#endif // GRAFO_H