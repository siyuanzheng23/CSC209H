#include <time.h>
#include <sys/timeb.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <fstream>
#include <map>
#include "library.h"
#include "library.cc"

int main(int argc, char ** argv) {
	if (argc != 6) {
		fprintf(stderr,
				"Usage: select <heapfile> <attribute_id> <start> <end> <page_size>\n");
		exit(1);
	}

	char* heap_name = argv[1];
	int attr_id = atoi(argv[2]);
	char* start = argv[3];
	char* end = argv[4];
	int page_size = atoi(argv[5]);

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

	/* holder to store matched attr */
	std::map<std::string, int> output;
	/* create a record iterator */
	RecordIterator record_i = RecordIterator(&heap);
	while (record_i.hasNext()) {
		Record r = record_i.next();
		/* init record */
		record_num++;
		/* check if the record fits */
		if (strcmp(r.at(attr_id), start) >= 0
				&& strcmp(r.at(attr_id), end) <= 0) {
			std::string str(r.at(attr_id), ATTR_LEN);
			std::string str2 = str.substr(1, 5);
			output[str2]++;
		}
	}

	fclose(heap_file);

	/* stop timing produce output */
	for (auto it = output.cbegin(); it != output.cend(); ++it) {
		std::cout << it->first << "\n";
	}
	long ftime = timenow();
	fprintf(stdout, "NUMBER OF RECORDS: %d\n", record_num);
	fprintf(stdout, "TIME: %ld milliseconds\n", ftime - stime);
}
