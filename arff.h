#ifndef __ARFF__
#define __ARFF__

#define LINESIZE 1024
#include <stdio.h>

typedef struct
{
  char *rotulo;
  char *tipo;
  char **categorias;
  int size_categorias;
} atributo;

int linhaEstaEmBranco(const char *linha);
void exibe_atributos(atributo *infos, int quantidade);
int conta_atributos(FILE *arff);
void processa_categorias(atributo *elemento, char *categorias);
atributo *processa_atributos(FILE *arff, int quantidade);
void valida_arff(FILE *arff, atributo *atributos, int quantidade);

#endif