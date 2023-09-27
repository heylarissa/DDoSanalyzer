#ifndef __LOG__
#define __LOG__

#include "arff.h"

#define ATAQUES_FILE "R_ATAQUES.txt"
#define ENTIDADES_FILE "R_ENTIDADES.txt"
#define TAMANHOS_FILE "R_TAMANHO.txt"
#define BLACKLIST "BLACKLIST.bl"

#define TRUE 1
#define FALSE 0

typedef struct
{
    int ocorrencias;
    char *nome;

} log;

typedef struct
{
    log log_info;
    int sum_size;

} log_size_avg;
int busca_id_atributo(atributo *dados, int quantidade, char *atributo);
log *set_ataques(atributo elemento);
int get_id_ataque(log *ataques, char *token, int quantidade);
void write_log_entidades(log *data, int data_size, char *filename);
log_size_avg *set_ataques_log_size(atributo elemento);
void write_size_file(log_size_avg *ataques, int ataques_size, char *filename);
void write_blacklist(char **sources, int size, char *filename);

void get_ataques(atributo *dados, int quantidade, FILE *arquivo);
void get_entidades(atributo *dados, int quantidade, FILE *arquivo);
void get_tamanho(atributo *dados, int quantidade, FILE *arquivo);
void get_firewall(atributo *dados, int quantidade);

#endif