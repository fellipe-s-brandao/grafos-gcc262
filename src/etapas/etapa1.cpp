#include "etapa1.h"
#include <iostream>
#include <fstream>
#include <filesystem>

namespace fs = std::filesystem;

void executarEtapa1()
{
    cout << "Iniciando Etapa 1 - Análise de Métricas dos Grafos" << endl;

    string diretorio_dados = "./dados/";
    vector<MetricasGrafo> resultados;

    cout << "Buscando arquivos .dat em: " << diretorio_dados << endl;

    if (!fs::exists(diretorio_dados))
    {
        cerr << "Erro: Diretório de dados não encontrado: " << diretorio_dados << endl;
        cerr << "Certifique-se de que o diretório existe e contém arquivos .dat" << endl;
        return;
    }

    int total_arquivos = 0;
    int arquivos_processados = 0;

    // Contar arquivos primeiro
    for (const auto &entrada : fs::directory_iterator(diretorio_dados))
    {
        if (entrada.is_regular_file() && entrada.path().extension() == ".dat")
        {
            // Pular arquivos de solução
            if (entrada.path().filename().string().substr(0, 4) != "sol-")
            {
                total_arquivos++;
            }
        }
    }

    if (total_arquivos == 0)
    {
        cerr << "Nenhum arquivo .dat encontrado no diretório: " << diretorio_dados << endl;
        return;
    }

    cout << "Total de arquivos a processar: " << total_arquivos << endl;

    // Processar cada arquivo .dat no diretório
    for (const auto &entrada : fs::directory_iterator(diretorio_dados))
    {
        if (entrada.is_regular_file() && entrada.path().extension() == ".dat")
        {
            string nome_arquivo = entrada.path().filename().string();

            // Pular arquivos de solução
            if (nome_arquivo.substr(0, 4) == "sol-")
            {
                continue;
            }

            arquivos_processados++;
            cout << "\n[" << arquivos_processados << "/" << total_arquivos << "] Processando: " << nome_arquivo << endl;

            try
            {
                resultados.push_back(processarArquivoGrafo(entrada.path().string()));
                cout << "✓ Processado com sucesso!" << endl;
            }
            catch (const exception &e)
            {
                cerr << "✗ Erro ao processar " << nome_arquivo << ": " << e.what() << endl;
            }
        }
    }

    if (resultados.empty())
    {
        cerr << "Nenhum arquivo foi processado com sucesso!" << endl;
        return;
    }

    // Exportar resultados
    cout << "\nExportando resultados..." << endl;
    exportarParaCSV(resultados, "resultados_metricas_grafos.csv");

    cout << "\n=== ETAPA 1 CONCLUÍDA ===" << endl;
    cout << "Arquivos processados: " << resultados.size() << "/" << total_arquivos << endl;
    cout << "Resultados exportados para:" << endl;
    cout << "- resultados_metricas_grafos.csv" << endl;
    cout << "- resultados_intermediacao.csv" << endl;
}

MetricasGrafo processarArquivoGrafo(const string &caminho_arquivo)
{
    Grafo grafo;
    grafo.lerArquivoDados(caminho_arquivo);

    MetricasGrafo metricas;

    // Salva as estatísticas do grafo
    metricas.nome = grafo.getNomeArquivo();
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

void exportarParaCSV(const vector<MetricasGrafo> &todas_metricas, const string &arquivo_saida)
{
    ofstream arquivo_csv(arquivo_saida);

    if (!arquivo_csv.is_open())
    {
        throw runtime_error("Não foi possível criar o arquivo: " + arquivo_saida);
    }

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

    // Exportar a centralidade de intermediação separadamente
    ofstream arquivo_intermediacao("resultados_intermediacao.csv");

    if (!arquivo_intermediacao.is_open())
    {
        throw runtime_error("Não foi possível criar o arquivo: resultados_intermediacao.csv");
    }

    // Cabeçalho
    arquivo_intermediacao << "NomeGrafo,Vertice,CentralidadeIntermediacao\n";

    // Dados de intermediação para cada grafo
    for (const auto &m : todas_metricas)
    {
        for (const auto &[vertice, centralidade] : m.intermediacao)
        {
            arquivo_intermediacao << m.nome << ","
                                  << vertice << ","
                                  << centralidade << "\n";
        }
    }

    arquivo_intermediacao.close();
}