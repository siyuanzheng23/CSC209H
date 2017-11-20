#include <time.h>
#include <sys/timeb.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <fstream>
#include "library.h"
#include "library.cc"

int main(int argc, char * argv[]){
	if(argc!=4){
		fprintf(stderr,"Usage: write_fixed_len_pages <csv_file> <page_file> <page_size>\n");
		exit(1);
	}
	char *csv_name = argv[1];
	char *page_name = argv[2];
	int page_size = atoi(argv[3]);
	
	if (page_size <= 0) {
		fprintf(stderr, "Invalid argument <page_size> \n");
		exit(1);
	}

	/* Load csv_file */
	std::ifstream csv_file(csv_name);
	if (!csv_file.is_open()) {
		fprintf(stderr, "Could not open file %s\n", csv_name);
	}

	/* Write to page_file */
	FILE* page_file = fopen(page_name, "w");
	if (page_file == 0) {
		fprintf(stderr, "Could not open file %s\n", page_name);
	}

	/* start timing */
	long stime = timenow();
	int page_num = 0, record_num = 0;

	/* read from csv file and write each record to the page */
	Page page;
	/* init the page */
	init_fixed_len_page(&page, page_size, 1000);
	std::string line = "";

	char *buffer = (char*)malloc(sizeof(char)*11);
	memset(buffer,'\0', sizeof(char)*11);
	/* read lines and we init a record */
	while(getline(csv_file, line)){
		int cur_index = 0, last_index = 0, attr_num = 0;
		Record r;
        	/* init record */
		char * record_block=(char*)malloc(sizeof(char)*1000); 
        	memset(record_block, '\0' ,1000);
        	for(int i = 0; i < 100; i++)
                	r.push_back(record_block + i * 10);
   
		/* each line represents one record */
		while((unsigned)cur_index<line.capacity()){
				/* reading each attr */
				if(line.at(cur_index)==','){
					line.copy(buffer, 10,last_index);
					memcpy((char*)r.at(attr_num),(buffer),10);
					/* reset the buffer for each record*/
					memset(buffer, '\0', sizeof(char)*11);
					last_index += 11;
					attr_num++;
				}else if (attr_num==99){
					/* reach the end of line we should have 100 attrs now */
					line.copy(buffer, 10,last_index);
					memcpy((char*)r.at(attr_num),(buffer),10);
					/* reset the buffer for each record*/
					memset(buffer, '\0', sizeof(char)*11);
					attr_num++;
				}
				cur_index++;
		}
		/* now we have a record to be inserted in the page */
		record_num++;
		int next_record = add_fixed_len_page(&page, &r);
		if(next_record == -1){
			fprintf(stderr, "Could not insert the record \n");
		}
		
		/* check if the page is full now */
		if (fixed_len_page_freeslots(&page) == 0) {
		
			/* page is full, write the page to page_file
			and reinit a new page */
			fwrite(page.data, page_size,1, page_file);
            fflush(page_file);
			page_num++;
			init_fixed_len_page(&page, page_size, fixed_len_sizeof(&r));

		}
		/* else: page is not full -> read and insert next record */
	}

	/* we may left with not full page */
	int usedslot = fixed_len_page_capacity(&page)
			- fixed_len_page_freeslots(&page);
	if(usedslot>0){
		fwrite(page.data, 1, usedslot * page.slot_size, page_file);
        fflush(page_file);
		page_num++;
	}

	fclose(page_file);
	csv_file.close();
	/* stop timing produce output */
	long ftime = timenow();
	fprintf(stdout,"NUMBER OF RECORDS: %d\n", record_num);
	fprintf(stdout,"NUMBER OF PAGES: %d\n", page_num);
	fprintf(stdout,"TIME: %ld milliseconds\n", ftime-stime);

}
