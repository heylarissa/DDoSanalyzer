CC = gcc
CFLAGS = -Wall -std=c99 # gerar "warnings" detalhados e infos de depuração
 
objs = log.o arff.o
 
# regra default (primeira regra)
all: ddosanalyzer
 
# regras de ligacao
ddosanalyzer: $(objs)
 
# regras de compilação
main.o: main.c
log.o:   log.c log.h
arff.o: arff.c arff.h
 
# remove arquivos temporários
clean:
	-rm -f $(objs) *~
 
# remove tudo o que não for o código-fonte
purge: clean
	-rm -f hello