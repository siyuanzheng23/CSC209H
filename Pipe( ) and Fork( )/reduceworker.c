#include <stdio.h>
#include <string.h>
#include <strings.h>
#include <stdlib.h>
#include <unistd.h>
#include "mapreduce.h"
#include "linkedlist.h"

void reduce_worker(int outfd, int infd) {

	/* Initialization */
	char filename[MAX_FILENAME + 1];
	char processpid[MAX_FILENAME + 1];
	int p = getpid();
	Pair pair, another_buffer;
	LLKeyValues *curr = NULL;
	FILE *f;

	/* Call the function insert_into_keys()*/
	while (read(infd, &pair, sizeof(Pair)) == sizeof(Pair)) {
		insert_into_keys(&curr, pair);
	}

	/* Convert integer to string */
	sprintf(processpid, "%d", p);
	strcpy(filename, processpid);
	strcat(filename, ".out");

	/* Open file */
	if ((f = fopen(filename, "wb")) == NULL) {
		perror("cannot create or open the file");
		exit(1);
	}

	/* Reduce and write */
	while (curr != NULL) {
		another_buffer = reduce(curr -> key, curr -> head_value);
		fwrite(&another_buffer, sizeof(Pair), 1, f);
		curr = curr -> next;
	}

	/* Free */
	free_key_values_list(curr);

}