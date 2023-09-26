#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#include "arff.h"
#include "log.h"

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

void write_log_entidades(log *data, int data_size, char *filename)
{
    FILE *output;
    output = fopen(filename, "w");

    for (int k = 0; k < data_size; k++)
    {
        if (strcmp(data[k].nome, "Normal") != 0)
        {
            printf("SRC_ADD: %s\nOcorrências: %d\n\n", data[k].nome, data[k].ocorrencias);
            char *escrita;
            escrita = strcat(data[k].nome, ";");

            if (data[k].ocorrencias <= 5)
            {
                strcat(escrita, "potencialmente maliciosa");
            }
            else if (data[k].ocorrencias > 5)
            {
                strcat(escrita, "maliciosa");
            }
            else
            {
                strcat(escrita, "benigna");
            }
            escrita = strcat(escrita, "\n");

            fputs(escrita, output);
        }
    }

    fclose(output);
}

void get_ataques(atributo *dados, int quantidade, FILE *arquivo)
{
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
        if (linhaEstaEmBranco(line))
        {
            continue;
        }
        i = 0;
        line[strlen(line) - 1] = '\0'; // remove \n do final do token

        token = strtok(line, ",");
        while (token != NULL)
        {
            if (i == id_atributo && strcmp(token, "Normal") != 0)
            {
                int id_ataque = get_id_ataque(ataques, token, size_ataques);
                ataques[id_ataque].ocorrencias++;
            }
            i++;
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

void get_entidades(atributo *dados, int quantidade, FILE *arquivo)
{
    log *entidades = malloc(sizeof(log));
    char line[LINESIZE + 1], *token;
    int i, entidades_tam = 0;

    int id_pkt_class = busca_id_atributo(dados, quantidade, "PKT_CLASS");
    int id_src_add = busca_id_atributo(dados, quantidade, "SRC_ADD");

    while (fgets(line, sizeof(line), arquivo) != NULL)
    {
        if (linhaEstaEmBranco(line))
        {
            continue;
        }
        i = 0;
        token = strtok(line, ",");
        log novaEntidade;
        novaEntidade.ocorrencias = 0;

        while (token != NULL)
        {
            if (i == id_pkt_class && strcmp(token, "Normal\n") != 0) // se for malicioso
            {
                // coluna do pkt_class
                novaEntidade.ocorrencias++;
            }
            else if (i == id_src_add)
            {
                // coluna do src_add
                novaEntidade.nome = strdup(token);
            }
            i++;
            token = strtok(NULL, ",");
        }

        if (entidades_tam > 0) // busca elemento e soma as ocorrencias
        {
            int exist = FALSE;
            for (int k = 0; k < entidades_tam; k++)
            {
                if (strcmp(entidades[k].nome, novaEntidade.nome) == 0)
                {
                    entidades[k].ocorrencias = entidades[k].ocorrencias + novaEntidade.ocorrencias;
                    exist = TRUE;
                    break;
                }
            }
            if (exist == FALSE)
            {
                entidades_tam += 1;
                entidades = realloc(entidades, entidades_tam * sizeof(log));
                entidades[entidades_tam - 1] = novaEntidade;
            }
        }
        else // adiciona elemento
        {
            entidades_tam += 1;
            entidades = realloc(entidades, entidades_tam * sizeof(log));
            entidades[entidades_tam - 1] = novaEntidade;
        }
    }

    // escreve entidades
    write_log_entidades(entidades, entidades_tam, ENTIDADES_FILE);
}

log_size_avg *set_ataques_log_size(atributo elemento)
{
    log_size_avg *ataques;

    ataques = malloc((elemento.size_categorias) * sizeof(log_size_avg));
    // validar alocação
    for (int i = 0; i < elemento.size_categorias; i++)
    {
        ataques[i].log_info.nome = elemento.categorias[i];
        ataques[i].log_info.ocorrencias = 0;
        ataques[i].sum_size = 0;
    }

    return ataques;
}

void write_size_file(log_size_avg *ataques, int ataques_size, char *filename)
{
    FILE *output;
    output = fopen(filename, "w");
    // validar abertura do arquivo

    for (int k = 0; k < ataques_size; k++)
    {
        char *escrita, num[LINESIZE + 1];
        int avg_size = ataques[k].sum_size / ataques[k].log_info.ocorrencias;
        escrita = strcat(ataques[k].log_info.nome, ";");
        sprintf(num, "%d", avg_size);
        escrita = strcat(escrita, num);
        escrita = strcat(escrita, "\n");
        printf("%s", escrita);

        fputs(escrita, output);
    }
    fclose(output);
}

void get_tamanho(atributo *dados, int quantidade, FILE *arquivo)
{
    char line[LINESIZE + 1];
    char *token;
    int col;

    int id_pkt = busca_id_atributo(dados, quantidade, "PKT_CLASS");
    int id_avg_size = busca_id_atributo(dados, quantidade, "PKT_AVG_SIZE");

    log_size_avg *ataques = malloc(sizeof(log_size_avg));
    ataques = set_ataques_log_size(dados[id_pkt]);
    int ataques_size = dados[id_pkt].size_categorias;

    while (fgets(line, sizeof(line), arquivo) != NULL)
    {
        if (linhaEstaEmBranco(line))
        {
            continue;
        }

        col = 0;
        line[strlen(line) - 1] = '\0'; // remove \n do final do token

        token = strtok(line, ",");

        log_size_avg novoLog;
        novoLog.log_info.ocorrencias = 0;

        while (token != NULL)
        {

            if (col == id_pkt)
            {
                novoLog.log_info.nome = strdup(token);
                novoLog.log_info.ocorrencias++;
            }
            else if (col == id_avg_size)
            {
                novoLog.sum_size = atoi(strdup(token));
            }
            col++;
            token = strtok(NULL, ",");
        }
        // adiciona o ataque ao vetor ou soma a um ja existente
        for (int k = 0; k < ataques_size; k++)
        {
            if (strcmp(ataques[k].log_info.nome, novoLog.log_info.nome) == 0)
            {
                ataques[k].log_info.ocorrencias = ataques[k].log_info.ocorrencias + novoLog.log_info.ocorrencias;
                ataques[k].sum_size = ataques[k].sum_size + novoLog.sum_size;
                break;
            }
        }
    }
    write_size_file(ataques, ataques_size, TAMANHOS_FILE);
}

void write_blacklist(char **sources, int size, char *filename)
{
    FILE *output;
    output = fopen(filename, "w");
    for (int n = 0; n < size; n++)
    {
        char *escrita;
        printf("%s\n", sources[n]);
        escrita = strdup(sources[n]);
        escrita = strcat(escrita, "\n");

        fputs(escrita, output);
    }
    fclose(output);
}

void get_firewall(atributo *dados, int quantidade)
{
    // lê R_ENTIDADES.txt e pega os endereços considerados MALICIOSOS
    char line[LINESIZE + 1],
        *token,
        **sources = (char **)malloc(sizeof(char *));
    int src_size = 0;
    FILE *entidades;
    entidades = fopen(ENTIDADES_FILE, "r");

    while (fgets(line, sizeof(line), entidades) != NULL)
    {
        if (linhaEstaEmBranco(line))
            continue;

        line[strlen(line) - 1] = '\0'; // remove \n do final da linha

        token = strtok(line, ";");
        int col = 0;
        char *src;
        char *classificacao;
        while (token != NULL)
        {
            if (col == 0)
            {
                src = strdup(token);
            }
            else if (col == 1)
            {
                classificacao = strdup(token);
                if (strcmp(classificacao, "maliciosa") == 0)
                {
                    src_size++;
                    sources = realloc(sources, src_size * LINESIZE);
                    sources[src_size - 1] = strdup(src);
                }
            }

            col++;
            token = strtok(NULL, ";");
        }
    }
    write_blacklist(sources, src_size, BLACKLIST);
    fclose(entidades);
}
