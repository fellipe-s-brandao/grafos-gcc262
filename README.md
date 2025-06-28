# Grafos - GCC 262

Este projeto em C++ implementa um sistema completo para análise de grafos e otimização de problemas de roteamento, desenvolvido para a disciplina GCC262. O sistema oferece três etapas principais: análise de métricas, geração de soluções iniciais e otimização com algoritmos de busca local.

## 📚 Funcionalidades

### Etapa 1 - Análise de Métricas
A classe `Grafo` implementa funcionalidades para análise estrutural:

- Leitura de grafos a partir de arquivos estruturados (.dat)
- Cálculo de métricas fundamentais:
  - **Densidade do grafo**
  - **Grau mínimo e máximo dos vértices**
  - **Caminho médio entre vértices**
  - **Diâmetro do grafo**
  - **Centralidade de intermediação (betweenness centrality)**
- Exportação de resultados em formato CSV para análise
- Processamento em lote de múltiplas instâncias

### Etapa 2 - Geração de Soluções Iniciais
Sistema de roteamento para problemas de capacitated arc routing:

- Algoritmo construtivo guloso (nearest neighbor)
- Respeitoa restrições de capacidade dos veículos
- Tratamento de serviços em nós, arcos e arestas
- Geração automática de rotas otimizadas
- Salvamento de soluções em formato padronizado

### Etapa 3 - Otimização com 2-opt
Aprimoramento das soluções através de busca local:

- Algoritmo 2-opt para melhoria de rotas
- Manutenção de todas as restrições de capacidade
- Estatísticas detalhadas de melhoria
- Comparação entre soluções iniciais e otimizadas
- Análise de performance e convergência

## 🧠 Algoritmos Utilizados

### Análise de Grafos
- **Floyd-Warshall**: Cálculo de caminhos mínimos entre todos os pares de vértices
- **Reconstrução de caminhos**: Baseada na matriz de predecessores
- **Centralidade de Intermediação**: Contagem de participação em caminhos mínimos

### Roteamento e Otimização
- **Algoritmo Construtivo Guloso**: Construção de soluções iniciais viáveis
- **2-opt**: Busca local para melhoria de rotas através de reconexões
- **Validação de Restrições**: Verificação contínua de capacidade e viabilidade

## 🗂 Estrutura do Projeto

```
grafos-gcc262/
├── src/                           # Código fonte
│   ├── main.cpp                   # Programa principal com menu interativo
│   ├── core/                      # Módulos principais
│   │   ├── grafo.h               # Interface da classe Grafo
│   │   └── grafo.cpp             # Implementação da análise de grafos
│   ├── io/                       # Entrada e saída
│   │   ├── estruturas_grafo.h    # Estruturas de dados
│   │   └── leitor_arquivo.cpp    # Leitura de arquivos .dat
│   └── etapas/                   # Módulos das etapas
│       ├── etapa1.h/.cpp         # Análise de métricas
│       ├── etapa2.h/.cpp         # Geração de soluções
│       └── etapa3.h/.cpp         # Otimização 2-opt
├── dados/                        # Instâncias de teste (.dat)
├── solucao/                      # Soluções da Etapa 2
├── solucao_otimizada/           # Soluções otimizadas da Etapa 3
├── build/                       # Arquivos de compilação
├── Makefile                     # Sistema de build
└── README.md                    # Este arquivo
```

## ✅ Como Compilar e Executar

### Compilação Manual
```bash
# Compilar todos os módulos manualmente
g++ -std=c++17 -Wall -Wextra -O2 \
    -Isrc -Isrc/core -Isrc/io -Isrc/etapas \
    src/main.cpp \
    src/core/grafo.cpp \
    src/io/leitor_arquivo.cpp \
    src/etapas/etapa1.cpp \
    src/etapas/etapa2.cpp \
    src/etapas/etapa3.cpp \
    -o grafos

# Executar
./grafos
```

### Preparação dos Dados
```bash
# Criar diretórios necessários
mkdir -p dados solucao solucao_otimizada

# Colocar seus arquivos .dat na pasta dados/
cp /caminho/para/instancias/*.dat dados/
```

## 🎮 Menu de Execução

Ao executar o programa, você terá acesso ao menu interativo:

```
=======================================
    SISTEMA DE ANÁLISE DE GRAFOS
=======================================
1. Etapa 1 - Análise de Métricas dos Grafos
2. Etapa 2 - Geração de Soluções Iniciais
3. Etapa 3 - Otimização com 2-opt
9. Informações sobre as Etapas
0. Sair
=======================================
```

## 📊 Arquivos de Saída

### Etapa 1 - Análise
- `resultados_metricas_grafos.csv` - Métricas gerais dos grafos
- `resultados_intermediacao.csv` - Centralidade de intermediação

### Etapa 2 - Soluções Iniciais
- `solucao/sol-[instancia].dat` - Arquivos de solução inicial

### Etapa 3 - Soluções Otimizadas
- `solucao_otimizada/opt-[instancia].dat` - Soluções otimizadas
- `resultados_otimizacao_2opt.csv` - Estatísticas de melhoria

## 🔧 Requisitos do Sistema

- **Compilador**: g++ com suporte a C++17
- **Sistema Operacional**: Linux (testado), macOS, Windows (com adaptações)
- **Dependências**: Biblioteca padrão C++ (filesystem, iostream, etc.)

## 📈 Exemplos de Uso

### Processar uma única etapa:
```bash
./grafos
# Escolher opção 1, 2 ou 3
```

### Pipeline completo (recomendado):
```bash
./grafos
# Escolher opção 5 para executar todas as etapas sequencialmente
```

### Apenas otimização:
```bash
./grafos
# Escolher opção 4 para executar construção + otimização
```

## 👥 Contribuição

Este projeto foi desenvolvido como parte da disciplina GCC262. Para contribuições:

1. Mantenha a estrutura modular
2. Documente novas funcionalidades
3. Teste com múltiplas instâncias
4. Siga os padrões de código estabelecidos

## 📝 Licença

Projeto acadêmico desenvolvido para fins educacionais na disciplina GCC262.