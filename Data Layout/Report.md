# Experiments Report

## Part 2
- Size of fixed length serialization of records in the table: 1000 bytes.
- `fixed_len_sizeof()` agrees with our calculation.

## Part 3
- Plot the performance (records/second) versus page size for write and read.

### write_fixed_len_pages
Write: (csv_size: 100,000records)

|    pageSize |  time             |
|-------------|:-----------------:|
|    1000     |     15071         |
|    2000     |     12214         |
|    3000     |     10284         |
|    4000     |     7571          |
|    5000     |     9378          |
|    6000     |     7955          |
|    7000     |     8332          |
|    8000     |     7770          |
|    9000     |     7709          |
|    10000    |    7415           |
    
|records|ms  |pagesize| rec/ms  |
|------|-----|-------|----------|
| 1000 | 125 | 1000  | 8        |
| 100  | 56  | 1000  | 1.785714 |
| 10   | 10  | 1000  | 1        |
| 1000 | 125 | 1000  | 8        |
| 100  | 40  | 2000  | 2.5      |
| 100  | 16  | 3000  | 6.25     |
| 100  | 27  | 4000  | 3.703704 |
| 1000 | 70  | 6000  | 14.28571 |
| 10   | 9   | 10000 | 1.111111 |
| 100  | 19  | 10000 | 5.263158 |
| 100  | 40  | 10000 | 2.5      |


![write_fixed_len_pages_plot](https://github.com/ZhuaMei/Data-Layout/blob/master/write_fixed_len_pages_plot.png)

### read_fixed_len_pages
Read: (csv_size: 10000records)  

|    pageSize | time             |
|-------------|:----------------:|
|    1000     |     5496         |
|    2000     |     5365         |
|    3000     |     5283         |
|    4000     |     5693         |
|    5000     |     5117         |
|    6000     |     5005         |
|    7000     |     5049         |
|    8000     |     5083         |
|    9000     |     4930         |
|    10000    |    4985          |


![read_fixed_len_pages_plot](https://github.com/ZhuaMei/Data-Layout/blob/master/read_fixed_len_pages_plot.png)

- Reason page based format is superior to storing records using a CSV file.

The reason is that page based storing can make future records read/write more consistent. OS can take advantage of the page directory to retrieve data in DBMS.
Page based format is really important and inevitable for DBMS for storing records.

- the short comings of the way we organize pages.

One of the shortcomings may be that page will have extra size at its end but insufficient for record storage. The other short coming may differ from one type of DBMS another. Heap file generally does not have advantage since during scanning, it requires looping the page entry in the page directories to find the target.

## Part 4     
### csv2heapfile
Write from CSV file to Heapfile: (csv_size: 10000 records)

| page_size | ms  |
|-----------|-----|
| 1000      | 552 |
| 2000      | 855 |
| 3000      | 576 |
| 4000      | 605 |
| 5000      | 506 |
| 6000      | 546 |
| 7000      | 712 |
| 8000      | 732 |
| 9000      | 704 |
| 10000     | 487 |

Analysis: 
- When the page size is a multiple of five times the record size, the time it takes to load the csv file into heapfile files decreases. (e.g. 1000, 5000, 10000)
- The larger the page size is within, the shorter time it requires to finish the writing.

### Select

|page_size|ms |
|-------|-----|
| 1000  | 129 |
| 2000  | 118 |
| 3000  | 125 |
| 4000  | 125 |
| 5000  | 126 |
| 6000  | 117 |
| 7000  | 130 |
| 8000  | 118 |
| 9000  | 130 |
| 10000 | 139 |

## Part 5
### heap2colstore
Write from CSV file to Column-Oriented files: (csv_size: 10000 records)

|pageSize     |  time   |
|-------------|:-------:|
|    1000     | 204692  |
|    2000     | 94989   |
|    3000     | 77432   |
|    4000     |    90116|
|    5000     |   69877 |
|    6000     |  53681  |
|    7000     | 64412   |
|    8000     |71893    |
|    9000     |    54107|
|    10000    |   55531 |

Analysis: The page size has a great negative relationship to the time it takes to write the csv file into 100 column oriented files. The larger the page size is, the shorter time it requires to finish the writing. This implies that disk I/O is a significant cost of time(even for SSD) since page size directly impacts how many disk I/O will perform. Thus, generally speaking, larger page size is good and efficient for performing I/O-related tasks.

Comparison: This takes much longer time than the csv2heapfile(row oriented). There can be a couple of reasons. It could be the page size in col-oriented DBMS is even much greater than the actual size of file that is used to store all the data of one attrbute(i.e., the page has many blank area with no meaningful data).Although theorectically, the I/O operations required for these two file systems are the same, under the conditions that both the size of total records and size of page are consistent.

### select2
Select2: (csv_size: 10000 records)

| pageSize|    time |
|---------|---------|
| 1000    |     5   |
| 2000    |     2   |
| 3000    |     1   |
| 4000    |     1   |
| 5000    |     1   |
| 6000    |     1   |
| 7000    |     1   |
| 8000    |     0   |
| 9000    |     0   |
| 10000   |     0   |

Analysis: Applying select on just one single attribute from a column-oriented DBMS is pretty fast.
Comparison: Compared to the select from row-oritented DBMS. Since the I/O time needed for this operation is just 1/100(where 100 is the number of attributes of a row) of the one required for row-oriented DBMS(theorectically for the same query), under the condition that they are both heap file based, i.e., no index.

### select3
Select3: (csv_size: 10000 records)

| pageSize|    time  |
|---------|----------|
| 1000    |     8    |
| 2000    |     6    |
| 3000    |     3    |
| 4000    |     5    |
| 5000    |     1    |
| 6000    |     1    |
| 7000    |     1    |
| 8000    |     1    |
| 9000    |     1    |
| 10000   |     1    |
    
Analysis: Applying select on just one single attribute(depending on the condition of another one) from a column-oriented DBMS is pretty fast.

Comparison: It takes almost the same time as select on the attribute that is not dependent on the value of another attribute, for column-oriented files. Theorectically, the I/O of select3 will be the double of select2. The experiment time is not double though, implying that I/O time may not be a significant component of the total run time(for this experiment). Thus select values from one single attribute from column-oriented DBMS is generally much faster than row-oriented DBMS, again, under the condition that the queries are the same and the size of total records are the same.
It takes much less time than the select query performed on the row-oriented DBMS(heapfile, no index). This is because, theorectically, the I/O time of select3 is only 1/50 of the select. Since select need to retrive records in the heapfile while select3 only need to retrieve two files, where each is about 1/100 file size of the row-oriented's heapfile. 