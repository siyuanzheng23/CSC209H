printf "\033c" 
echo "standard output re-directed to 'create_result.txt' under the same directory"

# make sure dir_1 and dir_2 are consistent with the ones in get_histogram.sh
dir_1=./written_files
dir_2=./another_written_files
# dir_2=/Volumes/CLARENCE/written_files # mobile drive 

# remove the previous created directory
rm -rf dir_1
rm -rf dir_2

printf "Cleaning previous generated files or executables .... \n" >>  create_result.txt 
make clean >>  create_result.txt 
printf "\n\n" >>  create_result.txt 
printf "Creating executables \n\n" >>  create_result.txt 
make >> create_result.txt 
printf "\n\n" >>  create_result.txt 

mkdir $dir_1  >>  create_result.txt # directory used to store written files, will be deleted while calling make clean
printf "\n\n" >>  create_result.txt 
printf "\nWriting random files to $dir_1 using different block size\n" >>  create_result.txt 
./create_random_file $dir_1/file_1 2000000 301 >>  create_result.txt 
./create_random_file $dir_1/file_2 20000000 402 >>  create_result.txt 
./create_random_file $dir_1/file_3 20000000 503 >>  create_result.txt 
./create_random_file $dir_1/file_4 20000000 904 >>  create_result.txt 
./create_random_file $dir_1/file_5 20000000 1011 >>  create_result.txt 
./create_random_file $dir_1/file_6 20000000 10111 >>  create_result.txt 
./create_random_file $dir_1/file_7 20000000 111111 >>  create_result.txt 
./create_random_file $dir_1/file_8 20000000 1111111 >>  create_result.txt 
./create_random_file $dir_1/file_9 30000000 2234561 >>  create_result.txt 
./create_random_file $dir_1/file_10 30000000 3000000 >>  create_result.txt 


printf "\n--------------------------------------------------------------------------------\n" >>  create_result.txt 

mkdir $dir_2 >>  create_result.txt 
printf "\nWriting files to $dir_2 using different block size \n" >> create_result.txt 
./create_random_file $dir_2/file_1 2000000 301 >>  create_result.txt 
./create_random_file $dir_2/file_2 20000000 402 >>  create_result.txt 
./create_random_file $dir_2/file_3 20000000 503 >>  create_result.txt 
./create_random_file $dir_2/file_4 20000000 904 >>  create_result.txt 
./create_random_file $dir_2/file_5 20000000 1011 >>  create_result.txt 
./create_random_file $dir_2/file_6 20000000 10111 >>  create_result.txt 
./create_random_file $dir_2/file_7 20000000 111111 >>  create_result.txt 
./create_random_file $dir_2/file_8 20000000 1111111 >>  create_result.txt 
./create_random_file $dir_2/file_9 30000000 2234561 >>  create_result.txt 
./create_random_file $dir_2/file_10 30000000 3000000 >>  create_result.txt 


