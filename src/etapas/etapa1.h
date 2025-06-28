#ifndef ETAPA1_H
#define ETAPA1_H

#include "../core/grafo.h"
#include <string>
#include <vector>
#include <map>

using namespace std;

// Estrutura para armazenar as métricas do grafo
struct MetricasGrafo
{
    string nome;
    int num_vertices;
    int num_arestas;
    int num_arcos;
    int num_vertices_obrigatorios;
    int num_arestas_obrigatorias;
    int num_arcos_obrigatorios;
    double densidade;
    int grau_minimo;
    int grau_maximo;
    double caminho_medio;
    double diametro;
    map<int, double> intermediacao;
};

// Função principal da Etapa 1
void executarEtapa1();

// Funções auxiliares
MetricasGrafo processarArquivoGrafo(const string &caminho_arquivo);
void exportarParaCSV(const vector<MetricasGrafo> &todas_metricas, const string &arquivo_saida);

#endif // ETAPA1_H