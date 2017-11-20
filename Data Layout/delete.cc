#include <time.h>
#include <sys/timeb.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <fstream>
#include "library.h"
#include "library.cc"

int main(int argc, char ** argv) {
	if (argc != 5) {
		fprintf(stderr,
				"Usage: delete <heapfile> <record_page_id> <record_slot_id> <page_size>\n");
		exit(1);
	}
	char *heap_name = argv[1];
	int record_page_id = atoi(argv[2]);
	int record_slot_id = atoi(argv[3]);
	int page_size = atoi(argv[4]);

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
	int total_records = fixed_len_page_capacity(&page);
	/* read that page */
	read_page(&heap, record_page_id, &page);
	/* we want to shift the records after the deleted record forward */
	char* temp = (char *) (malloc(page_size));
	
	memcpy(temp, (char*) page.data, page_size); // copy scr records
	//if(record_slot_id != page.slot_size - 1){
	memcpy(((char*) (page.data) + (record_slot_id * page.slot_size)),// deleted slot
			(temp + ((record_slot_id + 1) * page.slot_size)),// shifted records
			(total_records - record_slot_id - 1) * page.slot_size);
	//}
	memset(((char*) (page.data) + (total_records - 1) * page.slot_size),
			'\0',
			page.slot_size);

	/* write that page */
	write_page(&page, &heap, record_page_id);

	fclose(heap_file);

}
