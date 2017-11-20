#include "library.h"
#include "library.cc"
//silence warning
#include <cstdio>
#include <cstdint>
#include <vector>
#include <fstream>
#include <cassert>

int main(int argc, char ** argv){
	if(argc != 7){
		fprintf(stderr,"Usage: select3  <colstore_name> <attribute_id> <return_attribute_id> <start> <end> <page_size>\n");
		exit(1);
	}
	else{

		 long stime = timenow();
		//void push_back (const value_type& val);
		std::vector<int> idx_wit_col;
		//only the last page of the column file will contain free slots 
        std::string eleven_s = "           ";
		std::string ten_s =    "          ";

		//SELECT SUBSTRING(A, 1, 5) FROM T
    	//WHERE A >= start AND A <= end
        std::string null_ter = "\0";
		char *single_char = (char*)::operator new(1);



		std::string dir_name = argv[1];
		//std::string start = argv[4];
		//std::string end = argv[5];
		int page_size = atoi(argv[6]);

		//how mamy attributes can be in one page , either there is remainder or not 
		int attr_per_page = page_size/ATTR_LEN ; 


		std::string path = dir_name + '/' ; //+ attr_id ;  
		path += argv[2]; 


		std::string return_column_path = dir_name + '/';  //+ return_id;
		return_column_path += argv[3]; 

		
 		std::FILE* f = std::fopen((const char*)path.c_str(), "r");

 		if(f ==0){
 		 	std::cout << "File " << argv[2] <<" does not exist" << std::endl;
 		 	return -1;
 		 }


 		 int ch = 0, ch_count = 0; 
		while ((ch = std::fgetc(f)) != '\0' && ch != EOF) {
        	ch_count ++ ;
    	}

 		
 		std::FILE* return_f = std::fopen((const char*)return_column_path.c_str(), "r");

 		if(return_f ==0){
 		 	std::cout << "File " << argv[3] << " does not exist" << std::endl;
 		 	return -1;
 		 }




		//the buffer for reading every page of the column file 
		char *buffer = (char *)::operator new(page_size);
		char *cur = NULL;


		int file_size = get_file_size(f);
		int num_of_pages = -1;

		if (file_size/page_size == 0){
			num_of_pages = file_size / page_size;
		}else{ //the last page is not full for sure
			num_of_pages = file_size / page_size + 1 ;
		}

		//attribute is 10 bytes, reserve the 11th bytes for the null terminator(for print to work)
		char *buffer_for_print = (char*)::operator new(11);
		//int print_count = 0;
		//again, only the last page of the column file will contain free slots
		//loop over the page
		int counter = 0;

		for( int i = 0; i < num_of_pages ; i ++ ){

			std::fseek(f,  i * page_size, SEEK_SET);
			//size_t fread ( void * ptr, size_t size, size_t count, FILE * stream );
			// read from stream and write that into ptr

			//read the i-th(including i == 0) page of *reference column file*  into the buffer 
    		std::fread(buffer, 1, page_size, f);
    		std::rewind(f);


    		cur = buffer ; 
    		//the last page is full






    		//recording all qualifying records  in reference attributes 
    		int index_rec = 0 ;
    		while(counter <= attr_per_page -1  ){
    			std::memcpy(single_char,buffer+counter * ATTR_LEN, 1 ) ;
    			if (counter >= ch_count/ATTR_LEN){
    				break;
    			}else{
    				// std::memcpy(dest, source, sizeof dest);
    				std::memcpy(buffer_for_print, buffer + counter, 10);
    				buffer_for_print[10] = '\0';
    				if (std::strcmp((const char*)buffer_for_print, argv[4]) >= 0 &&  std::strcmp((const char*)buffer_for_print, argv[5]) <= 0  ){
    					if(std::strcmp((const char*)buffer_for_print, (const char*)&eleven_s) != 0  && std::strcmp((const char*)buffer_for_print, (const char*)&ten_s) != 0){
    						idx_wit_col.push_back(index_rec);
    					}
    				}
    				// returns a value < 0 (s1 is smaller then s2)
    				//if((s1.compare(s2)) < 0)
    			}

    			counter += 1;
    			index_rec += 1;
    		}




    		
    		//read the corresponding page(of returning attribute) into the buffer 
    		std::fseek(return_f,  i * page_size, SEEK_SET);
    		std::fread(buffer, 1, page_size, return_f);
    		//std::rewind(return_f);


    		cur = buffer ; 

    		//printing all qualifying records in the return attribtues 
    		for (std::vector<int>::iterator it = idx_wit_col.begin() ; it != idx_wit_col.end(); ++it){
    			//use *it to reference vector's content 
    			//* it is the number in the vector 
    			std::memcpy(buffer_for_print, cur + *it , 10);
    			buffer_for_print[10] = '\0';
    			std::cout << buffer_for_print << std::endl  ; //new line character 
    		}
    		//close returning column file



    		if (counter >= ch_count/ATTR_LEN){
    			break;
    		}

    		


		} 

		std::fclose(f);
    	std::fclose(return_f);


		::operator delete(buffer); //memory for a page
		::operator delete(buffer_for_print); //memory for single attribute

		long etime = timenow();
    	std::cout << "select3, Page Size : " << argv[6] << "Time: " << etime - stime << "ms" << std::endl ; 


		return 0; //place holder
	}
}