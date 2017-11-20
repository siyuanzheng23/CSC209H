#ifndef LIBRARY_H
#define LIBRARY_H

/*  #include <memcpy> */
#include <iostream>
#include<fstream>
#include <string>
#include <ostream>
#include <cstdint>
#include <cstring>
#include <cstdio>
#include <cstdlib>
#include <sys/timeb.h>
#include <time.h>

#include <vector>


typedef const char* V;
typedef std::vector<V> Record;

typedef struct {
    void *data;
    int page_size;
    int slot_size;
} Page;


#define ATTR_LEN 10
#define ATTR_NUM 100
#define RECORD_SIZE  ATTR_NUM * ATTR_LEN
#define DIR_PAGE_SIZE 1024
#define ENTRY_PER_PAGE  DIR_PAGE_SIZE/sizeof(Page_Entry)


/**
 * Compute the number of bytes required to serialize record
 */
int fixed_len_sizeof(Record *record);

/**
 * Serialize the record to a byte array to be stored in buf.
 */
void fixed_len_write(Record *record, void *buf);

/**
    * Deserializes `size` bytes from the buffer, `buf`, and
    * stores the record in `record`.
    */
void fixed_len_read(void *buf, int size, Record *record);




/*  Section 3 

*/

/**
 * Initializes a page using the given slot size
 */
void init_fixed_len_page(Page *page, int page_size, int slot_size);
 
/**
 * Calculates the maximal number of records that fit in a page
 */
int fixed_len_page_capacity(Page *page);
 
/**
 * Calculate the free space (number of free slots) in the page
 */
int fixed_len_page_freeslots(Page *page);
 
/**
 * Add a record to the page
 * Returns:
 *   record slot offset if successful,
 *   -1 if unsuccessful (page full)
 */
int add_fixed_len_page(Page *page, Record *r);
 
/**
 * Write a record into a given slot.
 */
void write_fixed_len_page(Page *page, int slot, Record *r);
 
/**
 * Read a record from the page from a given slot.
 */
void read_fixed_len_page(Page *page, int slot, Record *r);

/* Section 4 */




//this doesn't work
// typedef struct{
//     void *data; //can be cast to Page_Entry * 
//     int page_size;
//     int entry_size;
//     int full ; //flag, 1 for full and 0 for not 
//     Dir_Page *next;
// } Dir_Page;

//define the struct to represent every directory page, and directory pages are linked lists
// struct Dir_Page{
//     void *data; //can be cast to Page_Entry * 
//     int page_size;
//     int entry_size;
//     int full ; //flag, 1 for full and 0 for not 
//     int entry_offset;//the offset of the last page entry in current directory page 
//     Dir_Page *next;
//     int tail; //1 if this directory page is tail, 0 otherwise
// };

//define the directory struct that is used in Heapfile
// typedef struct{
//     Dir_Page* head; //do not modify while adding directory pages , it always points to the first one 
//     Dir_Page* tail; //we may not use it
// } Directory; 

//define the struct to represents data page entry (page_offset, freespace)
// typedef struct {
//     int page_offset ; 
//     int freespace ; //it means the number of free slots 
// } Page_Entry;

typedef struct {
    FILE *file_ptr;
    int page_size;
    int num_of_page;
    //Directory *dir;
} Heapfile;









typedef int PageID;
 
typedef struct {
    int page_id;
    int slot;
} RecordID;




/**
 * Initalize a heapfile to use the file and page size given.
 */
void init_heapfile(Heapfile *heapfile, int page_size, FILE *file);

/**
 * Allocate another page in the heapfile.  This grows the file by a page.
 */
PageID alloc_page(Heapfile *heapfile);

/**
 * Read a page into memory
 */
void read_page(Heapfile *heapfile, PageID pid, Page *page);

/**
 * Write a page from memory to disk
 */
void write_page(Page *page, Heapfile *heapfile, PageID pid);

class RecordIterator {
    public:
    RecordIterator(Heapfile *heapfile);
    Record next();
    bool hasNext();
    Heapfile *heapfile;
    Page *page;
    RecordID *rID;
    Record *record;
};




/**
 * return the time now in milliseconds.
 */
long timenow();
int free_num_slots(FILE *f, int page_index, int page_size);
int get_file_size(FILE *f);



#endif
