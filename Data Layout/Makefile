# Makefile
CC = g++ 
CCFLAGS = -Wall -g -D_LARGEFILE_SOURCE -D_FILE_OFFSET_BITS=64 -std=c++11

Targets = write_fixed_len_pages read_fixed_len_pages csv2heapfile scan insert update delete select csv2colstore select2 select3

all: $(Targets)

library.o: library.cc library.h
	$(CC) $(CCFLAGS) -o $@ -c $< library.h

write_fixed_len_pages: write_fixed_len_pages.cc library.o
	$(CC) $(CCFLAGS) -o $@ $<
	
read_fixed_len_pages: read_fixed_len_pages.cc library.o
	$(CC) $(CCFLAGS) -o $@ $< 
 
csv2heapfile: csv2heapfile.cc library.o
	$(CC) $(CCFLAGS) -o $@ $< 
 
scan: scan.cc library.o
	$(CC) $(CCFLAGS) -o $@ $< 
 
insert: insert.cc library.o
	$(CC) $(CCFLAGS) -o $@ $< 
 
update: update.cc library.o
	$(CC) $(CCFLAGS) -o $@ $< 
 
delete: delete.cc library.o
	$(CC) $(CCFLAGS) -o $@ $< 
 
select: select.cc library.o
	$(CC) $(CCFLAGS) -o $@ $<
 
csv2colstore: csv2colstore.cc library.o
	$(CC) $(CCFLAGS) -o $@ $<
 
select2: select2.cc library.o
	$(CC) $(CCFLAGS) -o $@ $<
 
select3: select3.cc library.o
	$(CC) $(CCFLAGS) -o $@ $< 

clean: 
	rm -r -f select csv2colstore select2 select3 delete update insert scan csv2heapfile write_fixed_len_pages read_fixed_len_pages *.o


