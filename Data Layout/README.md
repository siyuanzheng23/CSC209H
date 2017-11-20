# Data-Layout
We would like to be graded on 1 day late with 10% penalty.

NOTE: Please make sure the precondition of the executable is fulfilled in order to avoid seg fault.
      For e.g.      If the folder name  'new_folder' is empty
      				=>  calling select3 new_folder attrid returnid start end pagesize       => will cause seg fault

## Instruction
- `make`
### Part 3:
- `python mkcsv.py <outfile> <number of tuples>`

e.g.
`python mkcsv.py src.csv 10`

- `write_fixed_len_pages <csv_file> <page_file> <page_size>`

e.g.
`write_fixed_len_pages src.csv page_file 1000`

- `read_fixed_len_page <page_file> <page_size>`

e.g.
`read_fixed_len_page page_file 1000`

### Part 4: 
- `csv2heapfile <csv_file> <heapfile> <page_size>`

e.g.
`csv2heapfile src.csv heap_file 1000`

- `scan <heapfile> <page_size>`

e.g. 
`scan heap_file 1000`

- `insert <heapfile> <csv_file> <page_size>`   (If the process does not terminate itself, try kill it and run it again)

e.g.
`python mkcsv.py one.csv 1`
`insert heap_file one.csv 1000`

- `update <heapfile> <record_id.page_id> <record_id.slot_id> <attribute_id> <new_value> <page_size>`

e.g.
`update heap_file 0 0 0 aaaaaaaaaa 1000`

- `delete <heapfile> <record_page_id> <record_slot_id> <page_size>`

e.g.
`delete heap_file 0 0 1000`

- `select <heapfile> <attribute_id> <start> <end> <page_size>`

e.g.
 `select heapfile 0 AAAAAAAAAA VVVVVVVVVV <page_size>`

### Part 5:

- `csv2colstore <csv_file> <colstore_name> <page_size>`

e.g.
`csv2colstore scr.csv col_file 1000`

- `select2  <colstore_name> <attribute_id> <start> <end> <page_size>`

e.g. select2 col_file 2 AAA VVV 1000

- `select3  <colstore_name> <attribute_id> <return_attribute_id> <start> <end> <page_size>`

e.g. select3 col_file 2 99 AAA VVV 1000

