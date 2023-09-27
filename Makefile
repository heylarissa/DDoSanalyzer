CC = gcc
CFLAGS = -Wall -g

objs = main.o arff.o log.o

all: ddosanalyzer

ddosanalyzer: $(objs)
	$(CC) $(CFLAGS) -o ddosanalyzer $(objs)

main.o: main.c
	$(CC) $(CFLAGS) -c main.c

arff.o: arff.c arff.h log.h
	$(CC) $(CFLAGS) -c arff.c

log.o: log.c log.h arff.h
	$(CC) $(CFLAGS) -c log.c

clean:
	-rm -f $(objs) *~

purge: clean
	-rm -f ddosanalyzer
