#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "arff.h"

void exibe_atributos(atributo *infos, int quantidade)
{
  // Função do A1 (com modificações para o atributo de categorias)

  if (infos == NULL)
  {
    printf("O arquivo ARFF fornecido é inválido!\n");
    exit(0);
  }

  printf("=== ATRIBUTOS DO ARQUIVO ===\n");
  for (int i = 0; i < quantidade; i++)
  {
    printf("-> Atributo #%d\n", i + 1);
    printf("Rótulo: %s\n", infos[i].rotulo);
    printf("Tipo: %s\n", infos[i].tipo);
    // mudar para usar categorias como vetor
    // if (infos[i].categorias)
    //   printf("Categorias: %s\n", infos[i].categorias);
    if (i < quantidade - 1)
      printf("------------------------------\n");
  }
  printf("===============================\n");
}

int conta_atributos(FILE *arff)
{
  // Função do A1
  char line[LINESIZE];
  int attributeCount = 0;

  while (fgets(line, sizeof(line), arff) != NULL)
  {
    if (strstr(line, "@attribute") == line)
    {
      attributeCount++;
    }
    else if (strstr(line, "@data") != NULL)
    {
      break;
    }
  }

  return attributeCount;
}

void processa_categorias(atributo *elemento, char *categorias)
{
  // Recebe uma string com as categorias e atualiza o elemento
  // com um vetor de strings (modificar a struct)
  char *vetor[10]; // Suponhamos que você tenha um máximo de 10 elementos
  int numElementos = 0;
  // Remove os caracteres '{' e '}' da string de entrada
  if (strcmp(strdup(elemento->tipo), "categoric\n") == 0)
  {
    int i, j = 0;
    int length = strlen(categorias);

    for (i = 0; i < length; i++)
    {
      if (categorias[i] != '{' && categorias[i] != '}')
      {
        categorias[j++] = categorias[i];
      }
    }

    categorias[j] = '\0'; // Adiciona o terminador nulo para indicar o fim da nova string

    char *token = strtok(categorias, ","); // Divide a string na primeira vírgula

    while (token != NULL)
    {
      if (numElementos < 10)
      {
        vetor[numElementos] = token; // Armazena o elemento no vetor
        numElementos++;
      }
      token = strtok(NULL, ","); // Divide a string no próximo elemento
    }

    // Imprime os elementos do vetor
    for (int i = 0; i < numElementos; i++)
    {
      printf("Elemento %d: %s\n", i, vetor[i]);
    }
  }

  elemento->categorias = vetor;
}

atributo *processa_atributos(FILE *arff, int quantidade)
{
  // Função do A1 (com modificações para o atributo de categorias)

  char line[256];             // Tamanho da linha arbitrário
  atributo *atributos = NULL; // vetor
  int numAtributos = 0;
  rewind(arff);

  while (fgets(line, sizeof(line), arff) != NULL)
  {
    if (strstr(line, "@attribute") == line)
    {

      atributo novoAtributo;
      char *token = strtok(line, " ");
      token = strtok(NULL, " ");           // Pula o "@attribute"
      novoAtributo.rotulo = strdup(token); // Copia o rótulo
      token = strtok(NULL, " ");           // Pega o nome
      // se o tipo for categórico, deve constar "categoric"

      if ((strcmp(token, "string\n") != 0) && (strcmp(token, "numeric\n") != 0))
      {
        novoAtributo.tipo = strdup("categoric\n");
      }
      else
      {
        novoAtributo.tipo = strdup(token); // Copia o tipo
      }
      processa_categorias(&novoAtributo, token);

      numAtributos++;
      atributos = realloc(atributos, numAtributos * sizeof(atributo));
      atributos[numAtributos - 1] = novoAtributo;
    }
    else if (strstr(line, "@data") != NULL)
    {
      break; // Sai do loop quando "@data" é encontrado
    }
  }

  return atributos;
}

void valida_arff(FILE *arff, atributo *atributos, int quantidade)
{
  // Recebe um arquivo arff com ponteiro de leitura antes do "@data";
  // passa por todas as linhas de dados e valida cada elementos de cada coluna em
  // relação ao vetor de atributos também fornecido como argumento.

  char line[LINESIZE];
  char *token;
  int elementos, linhas = 0;

  while (fgets(line, sizeof(line), arff) != NULL)
  {
    elementos = 0;
    linhas++;

    // Use strtok para dividir a string por vírgulas
    token = strtok(line, ",");

    while (token != NULL)
    {
      // Para obter o próximo elemento
      token = strtok(NULL, ",");
      elementos++;
    }

    if (quantidade != elementos)
    {
      fprintf(stderr, "Erro: linha %d do arquivo arff\n", linhas);
      exit(EXIT_FAILURE);
    }
  }
}
