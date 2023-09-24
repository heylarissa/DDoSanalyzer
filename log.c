#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "arff.h"
#include "log.h"

/* Retorna o id/posição/coluna do atributo desejado */
int busca_id_atributo(atributo *dados, int quantidade, char *atributo)
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

ataque *set_ataques(atributo elemento)
{
    /* Inicializa o relatório de ataques */
    ataque *ataques_possiveis;
    ataques_possiveis = malloc((elemento.size_categorias) * sizeof(ataque));

    for (int i = 0; i < elemento.size_categorias; i++)
    {
        ataques_possiveis[i].nome_ataque = elemento.categorias[i];
        ataques_possiveis[i].ocorrencias = 0;
        printf("%s\n", ataques_possiveis[i].nome_ataque);
    }

    return ataques_possiveis;
}

/* Gera o relatório de ataque (nome_do_ataque; numero_de_ocorrências) */
void get_ataques(atributo *dados, int quantidade, FILE *arquivo)
{
    /*
    Etapa 1: Valida se existe o atributo com o rótulo PKT_CLASS
    Etapa 2: Busca nos dados o valor para esse atributo
    Etapa 3: Quando achar, busca na lista de ataques e soma 1 a ocorrências
    */

    ataque *ataques;

    char line[LINESIZE + 1];
    char *token;

    int id_atributo = busca_id_atributo(dados, quantidade, "PKT_CLASS");
    ataques = set_ataques(dados[id_atributo]);

    int i;

    while (fgets(line, sizeof(line), arquivo) != NULL)
    {
        i = 0;
        token = strtok(line, ",");
        while (token != NULL)
        {
            if (i == id_atributo && strcmp(token, "Normal\n") != 0)
            {
                
            }
            i++;
            token = strtok(NULL, ",");
        }
    }
}

void get_entidades(atributo *dados, int quantidade) {}
void get_tamanho(atributo *dados, int quantidade) {}
void get_firewall(atributo *dados, int quantidade) {}
