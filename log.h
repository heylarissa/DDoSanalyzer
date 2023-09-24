#ifndef __LOG__
#define __LOG__

#include "arff.h"

typedef struct
{
    int ocorrencias;
    char *nome_ataque;

} ataque;

void get_ataques(atributo *dados, int quantidade, FILE *arquivo);
void get_entidades(atributo *dados, int quantidade);
void get_tamanho(atributo *dados, int quantidade);
void get_firewall(atributo *dados, int quantidade);

#endif