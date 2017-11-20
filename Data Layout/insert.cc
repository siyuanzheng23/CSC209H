#include <time.h>
#include <sys/timeb.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <fstream>
#include "library.h"
#include "library.cc"

int main(int argc, char ** argv) {
	if (argc != 4) {
		fprintf(stderr, "Usage: insert <heapfile> <csv_file> <page_size>\n");
		exit(1);
	}
	char *heap_name = argv[1];
	char *csv_name = argv[2];
	int page_size = atoi(argv[3]);

	if (page_size <= 0) {
		fprintf(stderr, "Invalid argument <page_size> \n");
		exit(1);
	}

	std::ifstream csv_file(csv_name);
	if (!csv_file.is_open()) {
		fprintf(stderr, "Could not open file %s\n", csv_name);
		exit(1);
	}

	FILE* heap_file = fopen(heap_name, "r+");
	if (heap_file == 0) {
		fprintf(stderr, "Could not open heap file %s\n", heap_name);
		exit(1);
	}

	/* start timing */
	long stime = timenow();
	int record_num = 0, page_num = 0;

	/* init heap file */
	Heapfile heap;
	init_heapfile(&heap, page_size, heap_file);

	/* find an available slot in the page */
	Page page;
	/* init the page */
	init_fixed_len_page(&page, page_size, 1000);
	read_page(&heap, page_num, &page);

	/* read from csv file and insert each record to the page */
	std::string line = "";
	Record r;
        /* init record */
	char * record_block=(char*)malloc(sizeof(char)*1000); 
        memset(record_block, '\0' ,1000);
        for(int i = 0; i < 100; i++)
               	r.push_back(record_block + i * 10);
	char *buffer = (char *) malloc(sizeof(char) * 11);
	/* read lines and we init a record */
	while (getline(csv_file, line)) {
		int cur_index = 0, last_index = 0, attr_num = 0;

		/* each line represents one record */
		while ((unsigned) cur_index < line.capacity()) {

			/* reading each attr */
			if (line.at(cur_index) == ',') {
				line.copy(buffer, 10, last_index);
				memcpy((char*) r.at(attr_num), buffer, 10);
				/* reset the buffer for each record*/
				memset(buffer, '\0', sizeof(char)*11);
				last_index += 11;
				attr_num++;
			} else if (attr_num==99) {
				/* reach the end of line we should have 100 attrs now */
				line.copy(buffer, 10, last_index);
				memcpy((char*) r.at(attr_num), buffer, 10);
				/* reset the buffer for each record*/
				memset(buffer, '\0', sizeof(char) * 11);
				attr_num++;
			}
			cur_index++;
		}

		/* now we have a record to be inserted in the heap */
		while (fixed_len_page_freeslots(&page) == 0) {
			/* check if this page is full -> we go to next page */
			page_num++;
			if (page_num <= heap.num_of_page) {
				/* get the next page */
				read_page(&heap, page_num, &page);
			} else {
				/* need to alloc a new page */
				page_num = alloc_page(&heap);
				read_page(&heap, page_num, &page);
			}
		}
		/* now we have an empty slot in this page */
		record_num++;
		int next_record = add_fixed_len_page(&page, &r);
		if (next_record == -1) {
			fprintf(stderr, "Could not insert the record \n");
		}

		/* check if the page is full now */
		if (fixed_len_page_freeslots(&page) == 0) {
			/* page is full, write the page to heap_file */
			write_page(&page, &heap, alloc_page(&heap));
			page_num++;
		}
		/* else: page is not full -> read and insert next record */

	}

	/* we may left with not full page */
	int usedslot = fixed_len_page_capacity(&page)
			- fixed_len_page_freeslots(&page);
	if (usedslot > 0) {
		write_page(&page, &heap, alloc_page(&heap));
		page_num++;
	}

	fclose(heap_file);
	csv_file.close();
	/* stop timing produce output */
	long ftime = timenow();
	fprintf(stdout, "NUMBER OF RECORDS: %d\n", record_num);
	fprintf(stdout, "NUMBER OF PAGES: %d\n", page_num);
	fprintf(stdout, "TIME: %ld milliseconds\n", ftime - stime);
}
