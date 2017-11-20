#include <time.h>
#include <sys/timeb.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <fstream>
#include "library.h"
#include "library.cc"

int main(int argc, char * argv[]) {
	if (argc != 3) {
		fprintf(stderr, "Usage: read_fixed_len_page <page_file> <page_size>\n");
		exit(1);
	}

	char *page_name = argv[1];
	int page_size = atoi(argv[2]);

	if (page_size <= 0) {
		fprintf(stderr, "Invalid argument <page_size> \n");
		exit(1);
	}

	/* Read page_file */
	FILE* page_file = fopen(page_name, "r");
	if (page_file == 0) {
		fprintf(stderr, "Could not open file %s\n", page_name);
	}

	/* start timing */
	long stime = timenow();
	int page_num = 0, record_num = 0;

	/* read from page file and write each record to the csv file */
	Page page;
	/* init the page */
	init_fixed_len_page(&page, page_size, 1000);
	/* read the first page */

	int total_records = fixed_len_page_capacity(&page);

	/* Each time we read one page from the page file*/
	fread(page.data, page.page_size, 1, page_file);
	while(!feof(page_file)){
		Record r;
		/* init record */
		char * record_block = (char*) malloc(sizeof(char) * 1000);
		memset(record_block, '\0', 1000);
		for (int i = 0; i < 100; i++)
			r.push_back(record_block + i * 10);
		for (int index = 0; index < total_records; index++) {
			/* read the index-th record from the page */
			read_fixed_len_page(&page, index, &r);
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
		if(record_num % total_records == 0)
			page_num++;
		fread(page.data, page.page_size, 1, page_file);
	}

	fclose(page_file);
	/* stop timing produce output */
	long ftime = timenow();
	fprintf(stdout, "NUMBER OF RECORDS: %d\n", record_num);
	fprintf(stdout, "NUMBER OF PAGES: %d\n", page_num);
	fprintf(stdout, "TIME: %ld milliseconds\n", ftime - stime);

}
