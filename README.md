# Grafos - GCC 262

Este projeto em C++ implementa uma estrutura de dados para representação e análise de grafos, com suporte à leitura de instâncias a partir de arquivos de dados e cálculo de diversas métricas úteis em teoria dos grafos.

## 📚 Funcionalidades

A classe `Grafo` implementa funcionalidades como:

- Leitura de grafos a partir de arquivos estruturados
- Cálculo de:
  - **Densidade**
  - **Grau mínimo e máximo dos vértices**
  - **Caminho médio**
  - **Diâmetro**
  - **Centralidade de intermediação (betweenness centrality)**
- Reconstrução de caminhos mínimos entre vértices
- Execução do algoritmo de **Floyd-Warshall** para cálculo de caminhos mínimos
- Armazenamento e manipulação de:
  - Vértices e arcos/arestas
  - Vértices, arestas e arcos requeridos

## 🧠 Algoritmos Utilizados

- **Floyd-Warshall**: para encontrar os caminhos mínimos entre todos os pares de vértices.
- **Reconstrução de caminhos**: baseada na matriz de predecessores gerada pelo algoritmo de Floyd-Warshall.
- **Centralidade de Intermediação**: cálculo baseado em quantas vezes um vértice aparece nos caminhos mínimos entre pares de vértices.

## 🗂 Estrutura do Código

- `grafo.h`: definição da classe `Grafo`
- `grafo.cpp`: implementação dos métodos da classe
- `main.cpp`: responsável pela execução e criação dos arquivos csv para visualização dos resultados

## ✅ Como Rodar

```bash
g++ -o programa main.cpp grafo.cpp
```

```bash
./programa 
```