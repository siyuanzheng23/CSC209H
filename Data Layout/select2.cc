#include "library.h"
#include "library.cc"
//silent warning
#include <cstdio>
#include <cstdint>
#include <vector>
#include <fstream>
#include <cassert>

int main(int argc, char ** argv){
	if(argc != 6){
		fprintf(stderr,"Usage: select2  <colstore_name> <attribute_id> <start> <end> <page_size>\n");
		exit(1);
	}
	else{
		long stime = timenow();
		//only the last page of the column file will contain free slots 

		std::string eleven_s = "           ";
		std::string ten_s =    "          ";
		//SELECT SUBSTRING(A, 1, 5) FROM T
    	//WHERE A >= start AND A <= end

		std::string dir_name = argv[1];
		//std::string attr_id = atoi(argv[2]);
		//std::string start = argv[3];
		//std::string end = argv[4];
		int page_size = atoi(argv[5]);

		std::string null_ter = "\0";
		char *single_char = (char*)::operator new(1);




		//how mamy attributes can be in one page , either there is remainder or not 
		int attr_per_page = page_size/ATTR_LEN ; 


		std::string path = dir_name + '/' ;
		path += argv[2];



		//std::cout << path << std::endl;
		
   		 //not sure whether work ...
 		 std::FILE* f = std::fopen((const char*)(path.c_str()), "r"); 

 		 if(f ==0){
 		 	std::cout << "File " << argv[2]  << " does not exist" << std::endl;
 		 	return -1;
 		 }



 		 //int ch = 0 , null_count = 0;
 		 int ch = 0, ch_count = 0; 
		while ((ch = std::fgetc(f)) != '\0' && ch != EOF) {
        	ch_count ++ ;
    	}



 		 //std::cout << f << std::endl;


		//the buffer for reading every page of the column file 
		char *buffer = (char*)::operator new(page_size);

		//std::cout << "seg fault here  45" << std::endl;

		std::fseek(f, 0L, SEEK_END);
    	int file_size = std::ftell(f);
    	std::rewind(f); //reset the file pointer
		//int file_size = get_file_size(f);
		int num_of_pages = -1;

		//std::cout << "seg fault here  35" << std::endl;
		if (file_size/page_size == 0){
			num_of_pages = file_size / page_size;
		}else{ //the last page is not full for sure
			num_of_pages = file_size / page_size + 1 ;
		}

		//std::cout << "seg fault here  53" << std::endl;
		//attribute is 10 bytes, reserve the 11th bytes for the null terminator(for print to work)
		char *buffer_for_print = (char*)::operator new(11);
		//int print_count = 0;
		//again, only the last page of the column file will contain free slots
		//loop over the page
		int counter = 0;
		for( int i = 0; i < num_of_pages ; i ++ ){

			std::fseek(f,  i * page_size, SEEK_SET);
			//std::rewind(f);
			//size_t fread ( void * ptr, size_t size, size_t count, FILE * stream );
			// read from stream and write that into ptr
    		std::fread(buffer, 1, page_size, f);
    		std::rewind(f);

    		//the last page is full


    		//printing out all the qualifying attributes 
    		//int counter = 0; //use 1 instead of 0 
    		while(counter <= attr_per_page -1  ){
    			std::memcpy(single_char,buffer+counter * ATTR_LEN, 1 ) ;
    			if (counter >= ch_count/ATTR_LEN){
    				break;
    			}else{
    				// std::memcpy(dest, source, sizeof dest);
    				std::memcpy(buffer_for_print, buffer + counter, 10);
    				buffer_for_print[10] = '\0';
    				if (std::strcmp((const char*)buffer_for_print, argv[3]) >= 0 &&  std::strcmp((const char*)buffer_for_print, argv[4]) <= 0  ){
    					if(std::strcmp((const char*)buffer_for_print, (const char*)&eleven_s) != 0  && std::strcmp((const char*)buffer_for_print, (const char*)&ten_s) != 0){
    						std::cout << buffer_for_print << std::endl;
    					}
    				}
    				// returns a value < 0 (s1 is smaller then s2)
    				//if((s1.compare(s2)) < 0)
    			}

    			counter += 1;
    		}

    		if (counter >= ch_count/ATTR_LEN){
    			break;
    		}



		} 

		::operator delete(buffer); //memory for a page
		::operator delete(buffer_for_print); //memory for single attribute
		
		//std::cout << print_count << std::endl;
		long etime = timenow();
    	std::cout << "select2, Page Size : " << argv[5] << "Time: " << etime - stime << "ms" << std::endl ; 

		return 0; //place holder
	}
}