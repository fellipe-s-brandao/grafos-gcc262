#ifndef LEITOR_ARQUIVO_H
#define LEITOR_ARQUIVO_H

#include "estruturas_grafo.h"
#include <fstream>
#include <sstream>

class LeitorArquivo
{
public:
    static DadosGrafo lerArquivoDados(const string &nome_arquivo);

private:
    static void lerCabecalho(ifstream &arquivo, DadosGrafo &dados);
    static void lerQuantidadesRequeridas(ifstream &arquivo, int &num_vertices_req,
                                         int &num_arestas_req, int &num_arcos_req);
    static void lerNosRequeridos(ifstream &arquivo, DadosGrafo &dados,
                                 int num_vertices_requeridos, int &id_servico);
    static void lerArestasRequeridas(ifstream &arquivo, DadosGrafo &dados,
                                     int num_arestas_requeridas, int &id_servico);
    static void lerArestasNaoRequeridas(ifstream &arquivo, DadosGrafo &dados);
    static void lerArcosRequeridos(ifstream &arquivo, DadosGrafo &dados,
                                   int num_arcos_requeridos, int &id_servico);
    static void lerArcosNaoRequeridos(ifstream &arquivo, DadosGrafo &dados);
    static void pularAteSecao(ifstream &arquivo, const string &secao);
    static void inicializarVertices(DadosGrafo &dados);
};

#endif // LEITOR_ARQUIVO_H