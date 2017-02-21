#include <stdio.h>
#include <string.h>
#include <strings.h>
#include <stdlib.h>
#include <unistd.h>
#include "mapreduce.h"
#include "linkedlist.h"

void map_worker(int outfd, int infd) {

	char filenameIn[MAX_FILENAME];
	char chunk[READSIZE];
	int n, size;

	while ((n = read(infd, filenameIn, MAX_FILENAME)) != 0) {

		if (n == -1) {
			perror("Read");
			exit(-1);
		}

		/* Read from file */
		FILE *fileIn = fopen(filenameIn, "r");

		do {
			size = fscanf(fileIn, "%s", chunk);
			/* Add a final null-terminator to each chunk */
			chunk[strlen(chunk)] = '\0';

			/* Call function map() */
			map(chunk, outfd);
		} while (size != EOF);
	} 
}
