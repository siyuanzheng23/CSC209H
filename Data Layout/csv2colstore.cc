#include "library.h"
#include "library.cc"
#include <algorithm>

int main(int argc, char **argv) {
    if (argc != 4) {
        fprintf(stderr, "Usage: csv2colstore <csv_file> <colstore_name> <page_size>\n");
        exit(1);
    }
    //start time 
    long stime = timenow();

    std::ifstream input(argv[1]);
    std::string dir_name = argv[2];
    std::string sys_command = "mkdir " + dir_name;
    system(sys_command.c_str());
    // Init page list for each heapfile.
     char *page_list[100];
     for (int i = 0; i < 100; i ++) {
         page_list[i] = new char[atoi(argv[3])];
    }
    
    int tuple_per_page = atoi(argv[3]) / 10;
    int tuple_load = 0;
    std::string buffer;
    while (std::getline(input, buffer)) {
        // Remove comma.
        buffer.erase(std::remove(buffer.begin(), buffer.end(), ','), buffer.end());
        
        // Put attribute into buffer.
        for (int i = 0; i < 100; i ++) {
            buffer.copy(page_list[i]+tuple_load*10, 10, i*10);
        }
        tuple_load ++;
        // Flush page to file.
        if (tuple_load == tuple_per_page) {
            for (int i = 0; i < 100; i ++) {
                std::string file_name = dir_name + '/' + std::to_string(i);
                std::ofstream file;
                file.open(file_name.c_str(), std::ofstream::app);
                file.write(page_list[i], atoi(argv[3]));
                file.flush();
                file.close();
            }
            tuple_load = 0;
        }
    }
    if (tuple_load != 0) {
        for (int i = 0; i < 100; i ++) {
            std::string file_name = dir_name + '/' + std::to_string(i);
            std::ofstream file;
            file.open(file_name, std::fstream::app);
            file.write(page_list[i], atoi(argv[3]));
            if (tuple_load != 100) {
                file.put('\0');
            }
            file.flush();
            file.close();
        }
    }
    
    for(int i = 0; i < 100; i ++) {
        delete page_list[i];
    }
    input.close();
    //start time 
    long etime = timenow();
    std::cout << "csv2colstore, Page Size : " << argv[3] << "Time: " << etime - stime << "ms" << std::endl ; 
    return 0;
}
