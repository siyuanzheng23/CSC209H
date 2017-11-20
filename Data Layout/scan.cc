#include <time.h>
#include <sys/timeb.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <fstream>
#include "library.h"
#include "library.cc"

int main(int argc, char ** argv) {
	if (argc != 3) {
		fprintf(stderr, "Usage: scan <heapfile> <page_size>\n");
		exit(1);
	}

	char* heap_name = argv[1];
	int page_size = atoi(argv[2]);

	FILE* heap_file = fopen(heap_name, "r");
	if (heap_file == 0) {
		fprintf(stderr, "Could not open file %s\n", heap_name);
	}

	/* start timing */
	long stime = timenow();
	int record_num = 0;
	/* init heap file */
	Heapfile heap;
	init_heapfile(&heap, page_size, heap_file);

	/* create a record iterator */
	RecordIterator record_i = RecordIterator(&heap);
	
	while (record_i.hasNext()) {
		Record r = record_i.next();
		/* init record */
		record_num++;
		/* print the record in csv format */
		for (int i = 0; i < 100; i++) {
			char *buf = (char*)malloc(sizeof(char)*11);
			memset(buf,'\0',11);
			memcpy(buf,(char*)r.at(i),10);			
			if (i == 99) {
				printf("%s\n", buf);
			} else {
				printf("%s,", buf);
			}
		}
		
	}

	fclose(heap_file);
	/* stop timing produce output */
	long ftime = timenow();
	fprintf(stdout, "NUMBER OF RECORDS: %d\n", record_num);
	fprintf(stdout, "TIME: %ld milliseconds\n", ftime - stime);
}
