#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <ctype.h>

#include "log.h"
#include "arff.h"

int linhaEstaEmBranco(const char *linha)
{
  while (*linha)
  {
    if (!isspace(*linha))
    {
      return FALSE; // O caractere não é espaço em branco
    }
    linha++;
  }
  return TRUE; // Todos os caracteres são espaços em branco ou caracteres de nova linha
}

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

    if (infos[i].categorias != NULL)
    {
      printf("Categorias: ");

      for (int j = 0; j < infos[i].size_categorias; j++)
      {
        printf("%s - ", infos[i].categorias[j]);
      }
    }
    printf("\n");

    if (i < quantidade - 1)
      printf("------------------------------\n");
  }
  printf("===============================\n");
}

int conta_atributos(FILE *arff)
{
  // tratamento de exceção: 1) ter só 2 campos na linha de atributo
  // Função do A1
  char line[LINESIZE + 1];
  int attributeCount = 0;

  while (fgets(line, sizeof(line), arff) != NULL)
  {
    if (strstr(line, "@attribute") == line)
    {
      char *token;
      int contador_palavras = 0;

      // Primeira chamada ao strtok
      token = strtok(line, " ");

      // Subsequentemente, use NULL como primeiro argumento para continuar a dividir
      while (token != NULL)
      {
        contador_palavras++;
        token = strtok(NULL, " ");
      }

      if (contador_palavras != 3)
      {
        fprintf(stderr, "Arquivo mal formado! A linha de atributos não possui o número adequado de elementos\n");
        exit(EXIT_FAILURE);
      }

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
  // com um vetor de strings
  elemento->size_categorias = 0;

  int i;
  categorias[strlen(categorias) - 2] = '\0';
  categorias++; // Avance além do '{'
  char *cat_copy = strdup(categorias);

  char *token = strtok(categorias, ",");
  int num_elementos = 0;

  while (token != NULL)
  {
    num_elementos++;
    token = strtok(NULL, ",");
  }

  token = strtok(cat_copy, ",");
  i = 0;
  elemento->categorias = malloc((num_elementos + 1) * sizeof(char *));
  elemento->size_categorias = num_elementos;

  while (token != NULL)
  {
    elemento->categorias[i] = strdup(token);

    token = strtok(NULL, ",");
    i++;
  }
  free(cat_copy);
}

atributo *processa_atributos(FILE *arff, int quantidade)
{
  // Função do A1 (com modificações para o atributo de categorias)

  char line[LINESIZE + 1];
  atributo *atributos = NULL;
  int numAtributos = 0;
  rewind(arff);

  while (fgets(line, sizeof(line), arff) != NULL)
  {
    if (linhaEstaEmBranco(line))
    {
      continue;
    }

    if (strstr(line, "@attribute") == line)
    {
      atributo novoAtributo;
      char *token = strtok(line, " ");
      token = strtok(NULL, " ");           // Pula o "@attribute"
      novoAtributo.rotulo = strdup(token); // Copia o rótulo
      token = strtok(NULL, " ");           // Pega o nome

      if ((strcmp(token, "string\n") != 0) && (strcmp(token, "numeric\n") != 0))
      {
        novoAtributo.tipo = strdup("categoric\n");
        processa_categorias(&novoAtributo, token);
      }

      else
      {
        novoAtributo.categorias = NULL;
        novoAtributo.size_categorias = 0;
        novoAtributo.tipo = strdup(token); // Copia o tipo
      }

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

  char line[LINESIZE + 1];
  char *token;
  int elementos, linhas = 0;

  while (fgets(line, sizeof(line), arff) != NULL)
  {
    if (linhaEstaEmBranco(line))
    {
      continue;
    }
    elementos = 0;
    linhas++;

    token = strtok(line, ","); // separa a string

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
