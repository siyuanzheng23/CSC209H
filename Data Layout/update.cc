#include <time.h>
#include <sys/timeb.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <fstream>
#include "library.h"
#include "library.cc"


int main(int argc, char ** argv){
	if (argc != 7) {
		fprintf(stderr,
				"Usage: update <heapfile> <record_id.page_id> <record_id.slot_id> <attribute_id> <new_value> <page_size>\n");
		exit(1);
	}
	char *heap_name = argv[1];
	int record_page_id = atoi(argv[2]);
	int record_slot_id = atoi(argv[3]);
	int attr_id = atoi(argv[4]);
	char *new_value = argv[5];
	int page_size = atoi(argv[6]);

	if (page_size <= 0) {
		fprintf(stderr, "Invalid argument <page_size> \n");
		exit(1);
	}

	FILE* heap_file = fopen(heap_name, "r+");
	if (heap_file == 0) {
		fprintf(stderr, "Could not open file %s\n", heap_name);
	}

	/* init heap file */
	Heapfile heap;
	init_heapfile(&heap, page_size, heap_file);

	Page page;
	init_fixed_len_page(&page, page_size, RECORD_SIZE);
	Record r;
	/* read that page */
	read_page(&heap, record_page_id, &page);
	/* read that record from page */
	read_fixed_len_page(&page, record_slot_id, &r);
	/* modify the content */
	std::memcpy((char*) (r.at(attr_id)), new_value, ATTR_LEN);
	/* write that record to page */
	write_fixed_len_page(&page, record_slot_id, &r);
	/* write that page */
	write_page(&page, &heap, record_page_id);

	fclose(heap_file);
}
