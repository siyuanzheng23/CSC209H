echo "standard output re-directed to 'histogram.txt' under the same directory"
printf "Running get_histogram to get the distribution and speed of reading from disk \n\n" >>  histogram.txt 

# make sure dir_1 and dir_2 are consistent with the ones in create_random_file.sh
dir_1=./written_files
dir_2=./another_written_files
# dir_2=/Volumes/CLARENCE/written_files # mobile drive 

printf "Reading the file from $dir_1 using different block size\n" >>  histogram.txt 
./get_histogram $dir_1/file_10 300  >>  histogram.txt 
./get_histogram $dir_1/file_10 400  >>  histogram.txt 
./get_histogram $dir_1/file_10 500  >>  histogram.txt 
./get_histogram $dir_1/file_10 1001  >>  histogram.txt 
./get_histogram $dir_1/file_10 5079  >>  histogram.txt 
./get_histogram $dir_1/file_10 10000  >>  histogram.txt 
./get_histogram $dir_1/file_10 100001  >>  histogram.txt 
./get_histogram $dir_1/file_10 1000001  >>  histogram.txt 
./get_histogram $dir_1/file_10 2012345 >>  histogram.txt 
./get_histogram $dir_1/file_10 3000000  >>  histogram.txt 



printf "\n--------------------------------------------------------------------------------\n" >>  histogram.txt 

printf "Reading the file from $dir_2 using different block size\n" >>  histogram.txt 
./get_histogram $dir_2/file_10 300  >>  histogram.txt 
./get_histogram $dir_2/file_10 400  >>  histogram.txt 
./get_histogram $dir_2/file_10 500  >>  histogram.txt 
./get_histogram $dir_2/file_10 1001  >>  histogram.txt 
./get_histogram $dir_2/file_10 5079  >>  histogram.txt 
./get_histogram $dir_2/file_10 10000  >>  histogram.txt 
./get_histogram $dir_2/file_10 100001  >>  histogram.txt 
./get_histogram $dir_2/file_10 1000001  >>  histogram.txt 
./get_histogram $dir_2/file_10 2012345 >>  histogram.txt 
./get_histogram $dir_2/file_10 3000000  >>  histogram.txt 
