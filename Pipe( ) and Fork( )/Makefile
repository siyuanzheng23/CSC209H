CFLAGS = gcc -Wall -Werror -std=c99

master: linkedlist.o word_freq.o reduceworker.o mapworker.o master.o
	$(CFLAGS) -o mapreduce linkedlist.o word_freq.o reduceworker.o mapworker.o master.o

linkedlist.o: linkedlist.c mapreduce.h linkedlist.h
	$(CFLAGS) -c linkedlist.c

word_freq.o: word_freq.c mapreduce.h linkedlist.h
	$(CFLAGS) -c word_freq.c

reduceworker.o: reduceworker.c mapreduce.h linkedlist.h
	$(CFLAGS) -c reduceworker.c

mapworker.o: mapworker.c mapreduce.h linkedlist.h
	$(CFLAGS) -c mapworker.c

master.o: master.c mapreduce.h linkedlist.h
	$(CFLAGS) -c master.c

clean:
	rm master *.o
