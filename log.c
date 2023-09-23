#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "arff.h"
#include "log.h"

/* Retorna o id/posição/coluna do atributo desejado */
int busca_atributo(atributo *dados, int quantidade, char *atributo)
{
    for (int i = 0; i < quantidade; i++)
    {
        if (strcmp(dados[i].rotulo, atributo) == 0)
        {
            return i;
        }
    }
    fprintf(stderr, "ERRO: Não existe o atributo PKT_CLASS. Por isso, não foi possível gerar o relatório de ataques.\n");
    exit(EXIT_FAILURE);
}

/* Gera o relatório de ataque (nome_do_ataque; numero_de_ocorrências) */
void get_ataques(atributo *dados, int quantidade, FILE *arquivo)
{
    /*
    Etapa 1: Valida se existe o atributo com o rótulo PKT_CLASS
    Etapa 2: Busca nos dados o valor para esse atributo
    Etapa 3: Quando achar, busca na lista de ataques e soma 1 a ocorrências
    */

    ataque *ataque;
    ataque = NULL;
    char line[LINESIZE + 1];
    char *token;

    int id_atributo = busca_atributo(dados, quantidade, "PKT_CLASS");

    int i = 0;
    while (fgets(line, sizeof(line), arquivo) != NULL)
    {
        token = strtok(line, ",");
        while (token != NULL)
        {
            if (i == id_atributo && strcmp(token, "Normal") != 0)
            {
                printf("%s", token);
            }
            i++;
            token = strtok(NULL, ",");
        }
        break;
    }
}

void get_entidades(atributo *dados, int quantidade) {}
void get_tamanho(atributo *dados, int quantidade) {}
void get_firewall(atributo *dados, int quantidade) {}
