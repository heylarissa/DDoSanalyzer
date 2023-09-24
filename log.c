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

log *set_ataques(atributo elemento)
{
    /* Inicializa o relatório de ataques */
    log *ataques_possiveis;
    ataques_possiveis = malloc((elemento.size_categorias) * sizeof(log));

    for (int i = 0; i < elemento.size_categorias; i++)
    {
        ataques_possiveis[i].nome = elemento.categorias[i];
        ataques_possiveis[i].ocorrencias = 0;
    }

    return ataques_possiveis;
}

int get_id_ataque(log *ataques, char *token, int quantidade)
{
    for (int i = 0; i < quantidade; i++)
    {
        if (strcmp(token, ataques[i].nome) == 0)
        {
            return i;
        }
    }

    fprintf(stderr, "O ataque %s não existe na definição dos atributos. Não foi possível continuar o relatório.\n", token);
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

    log *ataques;
    FILE *output;
    output = fopen(ATAQUES_FILE, "w");

    int size_ataques;

    char line[LINESIZE + 1];
    char *token;

    int id_atributo = busca_id_atributo(dados, quantidade, "PKT_CLASS");
    ataques = set_ataques(dados[id_atributo]);
    size_ataques = dados[id_atributo].size_categorias;

    int i;

    while (fgets(line, sizeof(line), arquivo) != NULL)
    {
        i = 0;
        token = strtok(line, ",");
        while (token != NULL)
        {
            token[strlen(token) - 1] = '\0'; // remove \n do final do token

            if (i == id_atributo && strcmp(token, "Normal") != 0)
            {
                int id_ataque = get_id_ataque(ataques, token, size_ataques);
                ataques[id_ataque].ocorrencias++;
            }
            i++;
            if (i == 50)
                break;
            token = strtok(NULL, ",");
        }
    }

    for (int k = 0; k < size_ataques; k++)
    {
        if (strcmp(ataques[k].nome, "Normal") != 0)
        {
            printf("Ataque: %s\nOcorrencias: %d\n\n", ataques[k].nome, ataques[k].ocorrencias);
            char *escrita;
            char ocorrencias[LINESIZE + 1];
            escrita = strcat(ataques[k].nome, ";");
            sprintf(ocorrencias, "%d", ataques[k].ocorrencias);

            escrita = strcat(escrita, ocorrencias);
            escrita = strcat(escrita, "\n");

            fputs(escrita, output);
        }
    }

    fclose(output);
}

void get_entidades(atributo *dados, int quantidade) {}
void get_tamanho(atributo *dados, int quantidade) {}
void get_firewall(atributo *dados, int quantidade) {}
