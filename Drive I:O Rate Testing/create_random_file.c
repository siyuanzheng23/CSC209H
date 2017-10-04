#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>
#include <sys/timeb.h>

#define length 1
#define iteration 10
/**
 * populate a random array (which is already
 * allocated with enough memory to hold n bytes.
 */
void random_array(char *array, long bytes);
long get_time();


int main(int argc, char **argv){

	if(argc == 4){
		long total_bytes = (long)(atoi(argv[2]));
		int block_size = atoi(argv[3]);

		// printf("first argv: %s\nsecond argv: %s\nthird argv: %s\nfourth argv: %s\n",argv[0],argv[1],argv[2],argv[3]);
		//printf("%d char's should be there for %d bytes\n",(int)(total_bytes/sizeof(char)),total_bytes);
		char *buffer = malloc(sizeof(char) * (int)(total_bytes/sizeof(char)));

		//populate the array
		random_array(buffer,total_bytes);

		//now start to write content from memory to disk 
		FILE *fp = fopen(argv[1], "w");
		if(fp == NULL){
			perror("Open file failed, please try again");
			return -1;
		}	
		long bytes_written = 0;
        //size_t fwrite(const void *ptr, size_t size, size_t nmemb,FILE *stream);
        //On success, fread() and fwrite() return the number of items read or written. This number equals the number of bytes transferred only when size is 1
        long t_begin = get_time();
        while(bytes_written < total_bytes){
        	if(total_bytes - bytes_written < block_size){
        		bytes_written += fwrite(buffer + bytes_written,1,total_bytes - bytes_written,fp);
        		fflush(fp);
        	}else{
        		bytes_written += fwrite(buffer + bytes_written, 1, block_size,fp);
        		fflush(fp);
        	}
        }
        long t_end= get_time();
		fclose(fp);
		printf("Time: %ld ms, File: %s, Size:%lu, Block Size: %d, Avg Rate:%f bytes/s\n\n",t_end - t_begin, argv[1],total_bytes, block_size, (double)total_bytes/(t_end - t_begin)* 1000);
		if(bytes_written != total_bytes){
			printf("bytes_written %lu is different than total_bytes %lu\n", bytes_written, total_bytes);
			return -1;
		}
		return 0;
	}else{
		printf("\nUsage: .\\create_random_file <filename> <total bytes> <block size>\nProgram terminated\n\n");
		return -1;

	}
}

void random_array(char *array, long bytes){
	int char_num = (int)(bytes/(sizeof(char)));
	struct timeb t;
	ftime(&t);

	long cur = t.time * 1000 + t.millitm;
	srand(cur);

	char ter = '\0';

	char *letters = "ABCDEFGHIJKLMNOPQRSTUVWXYZ";

	// void *memcpy(void *dest, const void *src, size_t n);
	int written_num = 0;
	while(written_num < char_num){
		memcpy(array + written_num, &letters[rand()%26], sizeof(char));
		written_num ++ ;	
	}
	//printf("number of char's written is  %d\n",written_num);
	memcpy(array + written_num, &ter,sizeof(char));
}

long get_time(){
	struct timeb t;
	ftime(&t);
	long now_in_ms = t.time * 1000 + t.millitm;
	return now_in_ms;
}