#include "grafo.h"
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <filesystem>

namespace fs = std::filesystem;

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

// Função para processar um único arquivo de grafo
MetricasGrafo processarArquivoGrafo(const string &caminho_arquivo)
{
    Grafo grafo;
    grafo.lerArquivoDados(caminho_arquivo);

    MetricasGrafo metricas;

    // Salva as estatísticas do grafo
    metricas.nome = fs::path(caminho_arquivo).filename().string();
    metricas.num_vertices = grafo.getNumVertices();
    metricas.num_arestas = grafo.getNumEdges();
    metricas.num_arcos = grafo.getNumArcs();
    metricas.num_vertices_obrigatorios = grafo.getNumRequiredVertices();
    metricas.num_arestas_obrigatorias = grafo.getNumRequiredEdges();
    metricas.num_arcos_obrigatorios = grafo.getNumRequiredArcs();
    metricas.densidade = grafo.calcularDensidade();
    metricas.grau_minimo = grafo.grausMinimo();
    metricas.grau_maximo = grafo.grausMaximo();
    metricas.caminho_medio = grafo.calcularCaminhoMedio();
    metricas.diametro = grafo.calcularDiametro();
    metricas.intermediacao = grafo.calcularIntermediacao();

    return metricas;
}

// Função para exportar os resultados para um arquivo CSV
void exportarParaCSV(const vector<MetricasGrafo> &todas_metricas, const string &arquivo_saida)
{
    ofstream arquivo_csv(arquivo_saida);

    // Cabeçalho
    arquivo_csv << "Nome,Vertices,Arestas,Arcos,VerticesObrigatorios,ArestasObrigatorias,ArcosObrigatorios,Densidade,GrauMinimo,GrauMaximo,CaminhoMedio,Diametro\n";

    // Dados de cada grafo
    for (const auto &m : todas_metricas)
    {
        arquivo_csv << m.nome << ","
                    << m.num_vertices << ","
                    << m.num_arestas << ","
                    << m.num_arcos << ","
                    << m.num_vertices_obrigatorios << ","
                    << m.num_arestas_obrigatorias << ","
                    << m.num_arcos_obrigatorios << ","
                    << m.densidade << ","
                    << m.grau_minimo << ","
                    << m.grau_maximo << ","
                    << m.caminho_medio << ","
                    << m.diametro << "\n";
    }

    arquivo_csv.close();
}

int main()
{
    string diretorio_dados = "../data/";
    vector<MetricasGrafo> resultados;

    // Processar cada arquivo .dat no diretório
    for (const auto &entrada : fs::directory_iterator(diretorio_dados))
    {
        if (entrada.is_regular_file() && entrada.path().extension() == ".dat")
        {
            cout << "Processando: " << entrada.path().filename() << endl;
            resultados.push_back(processarArquivoGrafo(entrada.path().string()));
        }
    }

    // Exportar resultados
    exportarParaCSV(resultados, "resultados_metricas_grafos.csv");

    cout << "Processamento concluído. Resultados exportados para 'resultados_metricas_grafos.csv' e 'resultados_intermediacao.csv'\n";

    return 0;
}
