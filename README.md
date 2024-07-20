
# Estatísticas Computacionais em C++

Este é um programa avançado de estatística descritiva desenvolvido em C/C++. Ele processa dados quantitativos e qualitativos a partir de um arquivo CSV e gera um relatório em um arquivo de texto.

## Funcionalidades

O programa fornece as seguintes análises para dados quantitativos:
- Número de amostras (N)
- Média
- Mediana
- Moda
- Variância
- Desvio padrão
- Coeficiente de variação
- Skewness
- Curtose
- Quartis (Q1, Q3)
- Outliers

Para dados qualitativos, o programa fornece:
- Número de amostras (N)
- Moda
- Frequências
- Proporções

## Como usar

### Requisitos

- Compilador g++ (pode ser instalado via MinGW no Windows ou usando um gerenciador de pacotes como apt no Linux)
- Um arquivo CSV contendo os dados a serem analisados

### Compilação

1. Clone o repositório:

   ```sh
   git clone <URL_DO_SEU_REPOSITORIO>
   cd <NOME_DO_REPOSITORIO>
   ```

2. Compile o programa:

   ```sh
   g++ -o statistics main.cpp
   ```

### Execução

Execute o programa passando o nome do arquivo CSV como argumento:

```sh
./statistics <NOME_DO_ARQUIVO_CSV>
```

Exemplo:

```sh
./statistics teste_1.csv
```

### Saída

O programa gera um relatório chamado `report.txt` no mesmo diretório onde o programa é executado. Este relatório contém todas as análises descritivas realizadas nos dados.

## Estrutura do Projeto

- `main.cpp`: Código-fonte principal do programa.
- `report.txt`: Arquivo de saída contendo o relatório de análises descritivas.
- `<NOME_DO_ARQUIVO_CSV>`: Arquivo de entrada contendo os dados a serem analisados.
