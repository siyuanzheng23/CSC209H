#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>
#include <sys/timeb.h>
#include <assert.h>
/**
 * file_ptr : the file pointer, ready to be read from.
 * hist: an array to hold 26 long integers.  hist[0] is the
 *       number of 'A', and hist[1] is the number of 'B', etc.
 * block_size: the buffer size to be used.
 * milliseconds: time it took to complete the file scan
 * total_bytes_read: the amount data in bytes read
 *
 * returns: -1 if there is an error.
 */
int get_histogram(
    FILE *file_ptr, 
    long hist[], 
    int block_size, 
    long *milliseconds, 
    long *total_bytes_read);
long get_size(FILE *fp);
long g_time();


int main(int argc, char **argv){

	if(argc == 3){
		FILE *file_ptr = fopen(argv[1], "r");

		if(file_ptr == NULL){
			perror("Invald file path");
			exit(-1);
		}else{
			int b_size = atoi(argv[2]);
			long hist[26];
			long milliseconds;
			long filelen;
			filelen = get_size(file_ptr);
			printf("\n\nFile:%s Block Size:%d \n\n",argv[1],b_size);

			for(int i = 0 ; i < 26; i ++){
				hist[i] = 0;
			}
			//printf("the size of the file %s is %lu\n",argv[1],filelen);
			
			int ret = get_histogram(file_ptr, hist, b_size,&milliseconds,&filelen);
			assert(! (ret < 0));
 
			printf("Computed the histogram in %ld ms.\n", milliseconds);
			for(int j=0; j < 26; j++) {
			    printf("%c : %ld\n", 'A' + j, hist[j]);
			}
			printf("Data rate: %f Bps\n", (double)filelen/milliseconds * 1000);
		}
	}else{
		printf("\nUsage: .\\get_histogram <filename> <blocksize>\nProgram terminated\n\n");
		return -1;
	}

	return 0;
}

long get_size(FILE *fp) {
    fseek(fp, 0, SEEK_END);
    long size = ftell(fp);
    rewind(fp);
    return size;
}

long g_time(){
	struct timeb t;
	ftime(&t);
	long now_in_ms = t.time * 1000 + t.millitm;
	return now_in_ms;
}

int get_histogram(FILE *file_ptr, long hist[], int block_size, long *milliseconds, long *total_bytes_read){
	long t_start = g_time();
	int idx = -1;
	
	char * buf = malloc(block_size/sizeof(char)   * sizeof(char));
	long bytes_read = 0;

	long f_size = get_size(file_ptr);

	while( f_size - bytes_read > 0){
		//cleaning the buffer
		bzero(buf,block_size);
		//keep reading
		if(f_size - bytes_read < block_size){
			bytes_read += fread(buf,1,f_size - bytes_read , file_ptr);		
			for(int i = 0 ; i < (f_size - bytes_read)/sizeof(char) ; i ++){
				// int pos = ch - 'a' + 1;
				idx = buf[i] - 'A' ;
				//printf("the char is %c and its idx is %d\n", buf[i],idx);
				hist[idx] += 1;
			}
		}else if(f_size - bytes_read >= block_size){

			bytes_read += fread(buf,1,block_size, file_ptr);

			for(int i = 0 ; i < (block_size/sizeof(char)); i ++){
				idx = buf[i] - 'A';
				//printf("the char is %c and its idx is %d\n", buf[i],idx);
				hist[idx] += 1;
			}


		}
	}
	//rewind(file_ptr);
	//fwrite(buf,1,100,file_ptr);
	// char ch = fgetc(file_ptr);
	// printf("the first char in the buffer is %c\n",ch);
	rewind(file_ptr);
	fclose(file_ptr);

	long t_end = g_time();
	*milliseconds = t_end - t_start;
	*total_bytes_read = bytes_read;
	if(f_size != bytes_read){
		printf("bytes read %lu is different than file size %lu\n",bytes_read,f_size);
		return -1;
	}
	printf("bytes read %lu is the same as file size %lu\n",bytes_read,f_size);
	return 0;
}

