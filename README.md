# ARFF Reader

Formato ARFF

Dividido em duas seções:
- atributos (colunas) definidas no começo do arquivo (a linha inicia com @attribute)
        - numeric, string, categórico {cat1, cat2, cat3}
        @attribute Nome string
        @attribute Idade numeric
        @attribute Grau {BC,MC,Bacharel,Mestre,Doutor}
- dados (@data)

Linhas em branco antes dos atributos, entre atributos e entre um atributo e o 
token @data devem ser ignoradas

1. Abre um arquivo no formato ARFF
2. Lê a sua seção de atributos
3. Para cada atributo armazena o seu rótulo, tipo e categorias em uma estrutura 
como a definida a seguir

typedef struct {
    char *rotulo;
    char *tipo;
    char *categorias;
} atributo;

Se o tipo for númerico ou textual, em "tipo" deve constar, respectivamente "numeric" e 
"string", em "categorias" deve existir zero (NULL);
Se o tipo for categórico, em "tipo" deve constar "categoric" e em categorias deve constar 
um vetor com todas as categorias.

Considere que:
- Nenhuma linha de atributo terá mais do que 1024 caracteres;
- Haverá exatamente um espaço entre um elemento e outro em uma linha de atributo;
- Não haverão espaços no início e no final de uma linha de atributos;
- Atributos categóricos terão seus valores sempre definidos corretamente

- Uma linha de atributo pode conter menos ou mais elementos, além dos necessários 
(o programa deve tratar esse possível erro)
- Uma linha de atributo não iniciada com "@atributte" deve ser indicada como erro.

# Avaliação 2
ObjetivoS: 
- Analisar e validar um arquivo no formato ARFF e usar esse
analisador para extrair estatísticas relacionadas a ataques DDoS de um log de rede. 
- Criar uma lista negra (blacklist) de endereços maliciosos para alimentar um
firewall iptables

1. Realizar a modificação na estrutura de dados dos atributos ARFF. Agora, para atributos
categórico, em vez de armazenar uma única string contendo todas as categorias (por
exemplo, "{a, b, c}"), devemos armazenar um vetor de strings, com cada categoria
separada (por exemplo, na primeira posição "a", na segunda "b" e na terceira "c"). 
Essa separação deve ser feita por meio da função "processa_categorias" e deve ser
chamada no contexto da função "processa_atributos" (observem que essa modificação
terá impacto em outras funções, como "exibe_atributos").

2. Realizar a validação da seção de dados do arquivo ARFF. Ou seja, linha por linha do
arquivo, é necessário verificar se a quantidade adequada de atributos existe e se cada
um desses atributos apresenta um dado compatível com o tipo designado para ele na
seção de definição dos atributos.

3. Criar arquivos de código-fonte (log.c e log.h) para a análise de um arquivo de log de
rede ARFF validado (podem usar o "netlog.arff" como referência). As seguintes
funcionalidades devem ser implementadas e disponibilizadas nas respectivas seções da
função principal (main.c):
    a. Gerar um relatório de todos os ataques ocorridos e o número de ocorrências no
    conjunto de dados (nome do arquivo de saída: "R_ATAQUES.txt");
    b. Gerar um relatório dos endereços de origem maliciosos, potencialmente
    maliciosos e benignos (nome do arquivo de saída: "R_ENTIDADES.txt");
    c. Gerar um relatório com a média da média do tamanho dos pacotes para cada
    tipo de ataque (nome do arquivo de saída: "R_TAMANHO.txt");
    d. Gerar uma lista negra (blacklist) de endereços de origem considerados
    maliciosos (nome do arquivo de saída: "BLACKLIST.bl").