#include "library.h"

// Section 2
int fixed_len_sizeof(Record *record) {
    return ATTR_LEN * ATTR_NUM;
}

void fixed_len_write(Record *record, void *buf) {
    char *dest = (char *) buf;
    for (int index = 0; index < ATTR_NUM; index++) {
        std::memcpy(dest, record->at(index), ATTR_LEN);
        dest += ATTR_LEN;
    }
}

void fixed_len_read(void *buf, int size, Record *record) {
    char *src = (char *) buf;
    char * record_block=(char*)malloc(sizeof(char)*1000); 
	memset(record_block, '\0' ,1000);
    for(int i = 0; i < 100; i++)
		record->push_back(record_block + i * 10);
    for (int index = 0; index < ATTR_NUM; index++) {
        //should not put & before record
        std::memcpy((char*) (record->at(index)), src, ATTR_LEN);
        src += ATTR_LEN;
    }
}

// section 3
void init_fixed_len_page(Page *page, int page_size, int slot_size) {
    page->page_size = page_size;
    page->slot_size = slot_size;
    page->data = ::operator new(page_size);
    *(char *) (page->data) = '\0';
}

int fixed_len_page_capacity(Page *page) {
    return (page->page_size) / (page->slot_size);
}

int fixed_len_page_freeslots(Page *page) {
    int found = 0;
    int slot_index = 0;
    while (slot_index < fixed_len_page_capacity(page)&&!found) {
        if (*((char *) (page->data) + slot_index * page->slot_size) != '\0') {
            slot_index++;
        } else {
            found = 1;
	}
}
    if (found) {
        return fixed_len_page_capacity(page) - slot_index;
    }
    return 0;
}

int add_fixed_len_page(Page *page, Record *r) {
    int free_slot = fixed_len_page_freeslots(page);
    if (free_slot > 0) {
        char *buf = (char *) (page->data)
                + (fixed_len_page_capacity(page) - free_slot) * page->slot_size;
        fixed_len_write(r, buf);
        return (fixed_len_page_capacity(page) - free_slot + 1) * page->slot_size;
    }
    return -1;}

void write_fixed_len_page(Page *page, int slot, Record *r) {
    char *offset = (char *) (page->data) + slot * (page->slot_size);
    fixed_len_write(r, offset);
}

void read_fixed_len_page(Page *page, int slot, Record *r) {
    fixed_len_read((char*) (page->data) + slot * page->slot_size,
            page->slot_size, r);
}

// section 4
// freespace is the number of free slots
void init_heapfile(Heapfile *heapfile, int page_size, FILE *file) {
    //Piazza @103
    fseek(file, 0L, SEEK_END);
    int file_size = ftell(file);
    rewind(file);

    heapfile->file_ptr = file;
    heapfile->page_size = page_size;

    if(file_size/page_size == 0){
        heapfile -> num_of_page = file_size/page_size;
    }else{
        heapfile -> num_of_page = file_size/page_size + 1 ;
    }
    

    //directory pages are linked lists
//     (heapfile->dir) = (Directory*) ::operator new(sizeof(Directory));
    
//     //get the file size
//     fseek(file, 0L, SEEK_END);
//     int file_size = ftell(file);
//     rewind(file); //reset the file pointer

//     //denote the last data page wont be full.
//     //if (file_size % page_size != 0) {
//       //  char null_ter = '\0';
//        // fwrite(&null_ter, 1, sizeof(null_ter), file);
//     //}
//     //how many data pages needed for file
//     int data_pages = -1;
//     if (file_size % page_size == 0) {
//         data_pages = file_size / page_size;
//     } else {
//         data_pages = file_size / page_size + 1;
//     }

// //    heapfile->num_of_page = data_pages;

//     //how many directory pages needed for page entry
//     int dir_pages = -1;
//     if (data_pages % ENTRY_PER_PAGE == 0) {
//         dir_pages = data_pages / ENTRY_PER_PAGE;
//     } else {
//         dir_pages = data_pages / ENTRY_PER_PAGE + 1;
//     }
//     heapfile->num_of_page = data_pages;
//     //initial the head(a struct representing directory page(not data))
//     //heapfile->dir->head = (Dir_Page*) ::operator new(sizeof(Dir_Page));

//     Dir_Page *cur ; //= heapfile->dir->head;

//     //initilize the structs that represent directory entry
//     for (int i = 0; i < dir_pages; i++) {
//         //allocate memory for dir_page-> data , for storing page entry
//         Dir_Page* new_dir_page = (Dir_Page*) ::operator new(sizeof(Dir_Page));

//         new_dir_page->data = (Page_Entry*) ::operator new(page_size);
//         new_dir_page->page_size = DIR_PAGE_SIZE;
//         new_dir_page->entry_size = sizeof(Page_Entry);
//         new_dir_page->full = 0;
//         new_dir_page->entry_offset = -1;

//         if( i == dir_pages -1 ){
//             new_dir_page -> tail = 1; //indicate that this page is the tail
//         }else{
//             new_dir_page -> tail = 0; //indicate that this page is not the tail
//         }


//         // malloc a new struct that represents Dir_Page

//         if(i == 0){
//             heapfile -> dir -> head = new_dir_page;
//             cur = heapfile -> dir -> head;
//         }else{
//             cur -> next = new_dir_page;
//             cur = cur -> next;
//         }
        
//         //cur->next = new_dir_page;
//        // cur = cur->next;
//     }

//     //set the last dir page's next as NULL
//     //cur->next = NULL;

//     //set the tail in the directory to the last directory page
//     heapfile->dir->tail = cur;

//     //set cur_dir_page to the first directory page
//     Dir_Page *cur_dir_page = heapfile->dir->head;

//     //index (offset) of the data page
//     int cur_index = 0;
//     // used to record the entry count within one directory page
//     int entry_count = 0;

//     for (int i = 0; i < dir_pages; i++) {
//         entry_count = 0;
//         /* check if the dir_page is full */
      

//         Page_Entry * cur_pe = (Page_Entry*) cur_dir_page->data;

//         for (int j = 0; j < static_cast<int>(ENTRY_PER_PAGE); j++) {
//             cur_pe->page_offset = cur_index;

//             if (file_size % page_size != 0 && cur_index == data_pages - 1) {
//                 cur_pe->freespace = ((data_pages * page_size) - file_size)
//                         / RECORD_SIZE;
//             } else {
//                 cur_pe->freespace = page_size / RECORD_SIZE;
//             }

//             //indicates the offset of the last page entry in the directory
//           if (j != ENTRY_PER_PAGE - 1) {
//                 cur_dir_page->entry_offset = j + 1;
//             } else {
//                 cur_dir_page->entry_offset = -1;
//             }

//             entry_count += 1;

//             if (i != dir_pages - 1 && static_cast<int>(ENTRY_PER_PAGE) <= entry_count) {
//                 cur->full = 1;
//             }

//             cur_index += 1;

//             if (cur_index >= data_pages) {
//                 break;
//             }

//             /*performs pointer arithmetic, since typeof(cur_pe) is *Page_Entry,
//              cur_pe + 1 will reach the next page entry */
//          if( entry_count < static_cast<int>(ENTRY_PER_PAGE) -2 ){
//                 cur_pe += 1;
//             }
//         }

//         if (entry_count == ENTRY_PER_PAGE) {
//             cur_dir_page->full = 1;
//         }

//         if (cur_index >= data_pages) {
//             break;
//         }

//         cur_dir_page = cur_dir_page->next;
//     }

}

//https://stackoverflow.com/questions/7775027/how-to-create-file-of-x-size
//heapfile -> file_ptr must be opened with fopen
PageID alloc_page(Heapfile *heapfile) {
    FILE *f = heapfile->file_ptr;
    //we assume that heapfile -> file % page_size == 0 



    //get the size of the file, file size is not used 
     fseek(f, 0L, SEEK_END);
     int file_size = ftell(f);
    // rewind(f);

    //int page_index = heapfile->num_of_page;
    
    //write additional page into the file
    fseek(f, file_size, SEEK_SET);
    fputc('\0', f);
    fflush(f);
    //write a new page entry into directory page

    // Dir_Page * cur = heapfile->dir->head;

    // int debug = 0;

    // while (cur -> tail != 1) {   //keep indexing into the tail(last) directory apge
    //     cur = cur->next;
    //     std::cout << debug << std::endl;
    //     debug += 1 ; 
    // }

    // //directory page is full, need a new one
    // if (cur->full == 1) {
    //     //The last direcotry page is already full , thus it is not the tail page any more
    //     cur -> tail = 0;

    //     Dir_Page * temp = (Dir_Page *) ::operator new(sizeof(Dir_Page));
    //     temp->page_size = DIR_PAGE_SIZE;
    //     temp->entry_size = sizeof(Page_Entry);
    //     temp->full = 0;
    //     temp->data = (Page_Entry*)::operator new(temp->page_size);
    //     temp->entry_offset = 0;
    //     temp->tail=1;

    //     cur->next = temp;
    //     cur = cur->next;
    // }

    // Page_Entry * cur_entry = (Page_Entry *) cur->data;

    // //perform pointer arithmetics
    // cur_entry = (Page_Entry*)cur->data + cur->entry_offset;// * cur->entry_size;
    // //indicate how many slots(records) can a data page hold
    // cur_entry->freespace = (cur->entry_size) / RECORD_SIZE;

    // heapfile -> num_of_page += 1 ; 
    // cur_entry->page_offset = heapfile->num_of_page;


	return file_size/heapfile->page_size + 1; 
    //return heapfile->num_of_page;//cur_entry->page_offset;
}

void read_page(Heapfile *heapfile, PageID pid, Page *page) {
    // int dir_page_index = pid / ENTRY_PER_PAGE;
    // Dir_Page* dir_page = heapfile->dir->head;
    // for (int i = 0; i < dir_page_index; i++) {
    //     dir_page = dir_page->next;
    // }
    // Page_Entry *pe = (Page_Entry *) (dir_page->data) + pid % ENTRY_PER_PAGE;

    fseek(heapfile->file_ptr, pid * (heapfile->page_size), SEEK_SET);
    fread(page->data, 1, heapfile->page_size, heapfile->file_ptr);
}

void write_page(Page *page, Heapfile *heapfile, PageID pid) {
    // int dir_page_index = pid / ENTRY_PER_PAGE;
    // Dir_Page *dir_page = heapfile->dir->head;
    // for (int i = 0; i < dir_page_index; i++) {
    //     dir_page = dir_page->next;
    // }
    // Page_Entry *pe = (Page_Entry *) (dir_page->data) + pid % ENTRY_PER_PAGE;

    fseek(heapfile->file_ptr, pid * (heapfile->page_size), SEEK_SET);
    fwrite(page->data, 1, heapfile->page_size, heapfile->file_ptr);
    fflush(heapfile->file_ptr);
}

//cause compile errors /warning
RecordIterator::RecordIterator(Heapfile* heapfile) {
    this->heapfile = heapfile;
    page = new Page;
init_fixed_len_page(page,heapfile->page_size, RECORD_SIZE);
    rID = new RecordID;
    rID->page_id = 0;
    rID->slot = 0;
}

Record RecordIterator::next() {
	record = new Record;
    if (hasNext()) {
	if(rID->slot == 0){
		read_page(heapfile, rID->page_id, page);
	}
        read_fixed_len_page(page, rID->slot, record);

        if (fixed_len_page_capacity(page) - fixed_len_page_freeslots(page) - 1
                == rID->slot) {
            rID->page_id++;
            rID->slot = 0;
        }else{
        rID->slot++;
    }}
    return *record;
}

bool RecordIterator::hasNext() {
	if(rID->slot == 0){
		read_page(heapfile, rID->page_id, page);
	}
  if ((rID->page_id >= heapfile->num_of_page)){
  	  return false;  
  }
  if((fixed_len_page_capacity(page) - fixed_len_page_freeslots(page) <= rID->slot)) {	
	return false;
   }
      return true;
}


//section 4 ends

/*********** helper ***********/
long timenow() {
    struct timeb t;
    ftime(&t);
    return t.time * 1000 + t.millitm;
}

//get number of free slots in one data page
int free_num_slots(FILE *f, int page_index, int page_size) {
    //calculate the free slots in the page
    rewind(f);
    Page* temp = (Page*) ::operator new(sizeof(Page));

    fseek(f, page_size * page_index, SEEK_SET);
    fread(temp->data, 1, page_size, f);
    rewind(f);

    return fixed_len_page_freeslots(temp);
}

int get_file_size(FILE *f){
    fseek(f, 0L, SEEK_END);
    int file_size = ftell(f);
    rewind(f); //reset the file pointer

    return file_size;
}
