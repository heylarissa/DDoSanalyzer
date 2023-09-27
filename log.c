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

    if (ataques_possiveis == NULL)
    {
        perror("Erro ao alocar memória para ataques_possiveis");
        exit(1);
    }

    for (int i = 0; i < elemento.size_categorias; i++)
    {
        ataques_possiveis[i].nome = strdup(elemento.categorias[i]);
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

            // Aloca memória para a string concatenada
            escrita = malloc(strlen(data[k].nome) + 32); // +32 para acomodar os caracteres adicionais

            strcpy(escrita, data[k].nome);
            strcat(escrita, ";");

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
            strcat(escrita, "\n");

            fputs(escrita, output);

            // Libera a memória alocada para data[k].nome
            free(escrita);
        }
    }

    fclose(output);
}

void get_ataques(atributo *dados, int quantidade, FILE *arquivo)
{
    log *ataques;

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

    FILE *output;
    output = fopen(ATAQUES_FILE, "w+");

    for (int k = 0; k < size_ataques; k++)
    {
        if (strcmp(ataques[k].nome, "Normal") != 0)
        {
            char ocorrencias[LINESIZE + 1];
            sprintf(ocorrencias, "%d", ataques[k].ocorrencias);

            int tamanho_escrita = snprintf(NULL, 0, "%s;%s\n", ataques[k].nome, ocorrencias) + 1;

            char *escrita = malloc(tamanho_escrita);
            if (escrita == NULL)
            {
                perror("Erro ao alocar memória para escrita");
                exit(1);
            }

            snprintf(escrita, tamanho_escrita, "%s;%s\n", ataques[k].nome, ocorrencias);
            fprintf(stdin, "%s", escrita);

            fputs(escrita, output);
            printf("%s", escrita);
            free(escrita);
        }
    }
    fclose(output);
    if (ataques != NULL)
    {
        for (int i = 0; i < size_ataques; i++)
        {
            free(ataques[i].nome);
        }
        free(ataques);
    }
}

void get_entidades(atributo *dados, int quantidade, FILE *arquivo)
{
    log *entidades = NULL;
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
        char *nome = NULL;
        int ocorrencias = 0;

        while (token != NULL)
        {
            if (i == id_pkt_class && strcmp(token, "Normal\n") != 0) // se for malicioso
            {
                // coluna do pkt_class
                ocorrencias++;
            }
            else if (i == id_src_add && nome == NULL)
            {
                // coluna do src_add
                nome = strdup(token);
            }
            i++;
            token = strtok(NULL, ",");
        }

        int exist = FALSE;
        for (int k = 0; k < entidades_tam; k++)
        {
            if ((nome != NULL) && strcmp(entidades[k].nome, nome) == 0)
            {
                entidades[k].ocorrencias = entidades[k].ocorrencias + ocorrencias;
                exist = TRUE;
                break;
            }
        }
        if (exist == FALSE)
        {
            entidades_tam += 1;
            entidades = realloc(entidades, entidades_tam * sizeof(log));
            entidades[entidades_tam - 1].nome = strdup(nome);
            entidades[entidades_tam - 1].ocorrencias = ocorrencias;
            free(nome);
        }
    }

    write_log_entidades(entidades, entidades_tam, ENTIDADES_FILE);

    for (int k = 0; k < entidades_tam; k++)
    {
        free(entidades[k].nome);
    }

    free(entidades);
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
        if (ataques[k].log_info.ocorrencias != 0)
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
    }
    fclose(output);
}

void get_tamanho(atributo *dados, int quantidade, FILE *arquivo)
{
    char line[2 * LINESIZE + 1];
    char *token;
    int col;

    int id_pkt = busca_id_atributo(dados, quantidade, "PKT_CLASS");
    int id_avg_size = busca_id_atributo(dados, quantidade, "PKT_AVG_SIZE");

    log_size_avg *ataques;
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
        novoLog.log_info.nome = NULL;
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
                novoLog.sum_size = atoi(token);
            }
            col++;
            token = strtok(NULL, ",");
        }
        // adiciona o ataque ao vetor ou soma a um ja existente
        if (novoLog.log_info.nome != NULL)
        {
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
        else
        {
            fprintf(stderr, "Não há valores de dados no arquivo de entrada.\n");
        }
    }
    write_size_file(ataques, ataques_size, TAMANHOS_FILE);

    free(ataques);
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
        **sources = NULL;
    int src_size = 0;
    FILE *entidades;
    entidades = fopen(ENTIDADES_FILE, "r");
    if (entidades == NULL)
    {
        perror("Erro ao abrir o arquivo de entidades");
        exit(EXIT_FAILURE);
    }
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

    free(sources);
}
