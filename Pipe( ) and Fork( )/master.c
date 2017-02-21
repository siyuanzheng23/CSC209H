#include <stdio.h>
#include <string.h>
#include <strings.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/wait.h>
#include <fcntl.h>
#include <getopt.h>
#include "mapreduce.h"
#include "linkedlist.h"

int main(int argc, char *argv[]) {

	/* Initialization */
	int numMapWorker = -1;
	int numReduceWorker = -1;
    int option;
	int i, k, n, j, count, status;
	int fd[2],anotherfd[2];
	char dirname[MAX_FILENAME];
	char filename[MAX_FILENAME];
	/* file descriptors */
	int fdMap[numMapWorker][2];
	int fdReduce[numReduceWorker][2]; 
	pid_t pid;
	Pair newPair;
	k = 0;
	count = 0;
	strncpy(dirname, "", MAX_FILENAME);

	/* Process arguments */
	while ((option = getopt (argc, argv, "d:m:r:")) != -1) {
		switch (option) {
			case 'd':
				strncpy(dirname, optarg, MAX_FILENAME);
				break;
			case 'm':
				numMapWorker = atoi(optarg);
				break;
			case 'r':
				numReduceWorker = atoi(optarg);
				break;
			default:
				fprintf(stderr, "Usage: %s -d dirname [-r numprocs] [-m numprocs]\n", argv[0]);
            	exit(-1);
		}
	}

	if (dirname[0] == '\0') {
		/* If dirname is not provided: Exit with -1 */
		fprintf(stderr, "-d is not provided.\n");
		exit(-1);
	}
	if (dirname[strlen(dirname)] == '/') {
		/* If dirname does not end with /: Add / to end */
		strncat(dirname, "/", sizeof(dirname) - strlen(dirname) - 1);
		dirname[strlen(dirname)] = '\0';
	}
	if (numMapWorker == -1) {
		/* if -m is not provided: Set to default: 2 */
		numMapWorker = 2;
	}
	if (numReduceWorker == -1) {
		/* if -r is not provided: Set to default: 2 */
		numReduceWorker = 2;
	}

  	/* Obtain the output from ls process */
	if (pipe(fd) == -1) {
		perror("pipe");
		exit(-1);
	}
	n = fork();
	if (n > 0) { 			
		/* only parent gets here */
		if (close(fd[1]) == -1) {
			perror("close");
		}
		
		dup2(fd[0], STDIN_FILENO); 
		if (close(fd[0]) == -1) {
			perror("close");
		}
	    
	} else if (n == 0) {		
		/* only child gets here */
		if (close(fd[0]) == -1) {
			perror("close");
		};
		dup2(fd[1], STDOUT_FILENO);
		execl("/bin/ls", "ls", dirname, (char *)0);
		if (close(fd[1]) == -1) {
			perror("close");
		}
		exit(0);
	} else {
		/* error */
		 perror("fork()");
	}

	
	/* Map worker */
	pipe(anotherfd);
	for (i = 0; i < numMapWorker; i ++) {
	    pipe(fdMap[i]);
	    n = fork();
	    if (n < 0) {
	    	perror("fork");
	    	exit(-1);
	    } else if (n == 0) {
	    	if (close(fdMap[i][1]) == -1) {
	    		perror("close");
	    	}
	    	if (close(anotherfd[0]) == -1) {
	    		perror("close");
	    	}
	    	for (j = 0; j < i; j++) {
	    		if (close(fdMap[j][1]) == -1) {
	    			perror("close");
	    		}
	    	}
	    	map_worker(anotherfd[1], fdMap[i][0]);
	    	exit(0);
	    } else if (n > 0) {
	    	if (close(fdMap[i][0]) == -1) {
	    		perror("close");
	    	}
		}
	}    
	if (close(anotherfd[1]) == -1) {
		perror("close");
	}



	/* parent process's writing to mapworkers */
	if (n > 0) {  
		while (scanf("%s\n", filename) != EOF) {
			count ++;
			if (write(fdMap[count%numMapWorker][1],filename, 
				MAX_FILENAME) == -1) {
			    perror("write to pipe");
				exit(-1);
			}
		}
		
	    for(i = 0; i < numMapWorker; i++) {
	    	if (close(fdMap[i][1]) == -1) {
	    		perror("close the write end");
	    	}
	    }

	}



	/* Start the fork process here */
	if (n > 0) { 
		for(i = 0; i < numReduceWorker; i++) {
			if (pipe(fdReduce[i]) == -1) {
				perror("pipe");
			}
			n = fork();
			if (n < 0) {
				perror("fork");
			} else if (n == 0) {
				if (close(fdReduce[i][1]) == -1) {
					perror("close");
				}
				for(j = 0; j < i; j ++) {
					if (close(fdReduce[j][1]) == -1) {
						perror("close");
					}
				}
				reduce_worker(STDOUT_FILENO,fdReduce[i][0]);
				exit(0);
			} else if (n > 0) {
				if (close(fdReduce[i][0]) == -1) {
					perror("close");
					exit(-1);
				}
			}
        }
    }
	if (n > 0) {
		for (i = 0; i < numMapWorker; i++) {
			while (read(anotherfd[0], &newPair, sizeof(Pair)) 
				== sizeof(Pair)) {
				k = (newPair.key[0])%(numReduceWorker);
				write(fdReduce[k][1], &newPair, sizeof(Pair));
			}
		}
		
		if (close(anotherfd[0]) == -1) {
			perror("close");
		}

		/* Close all pipes connecting master and reduce workers */
		for (i = 0; i < numReduceWorker; i++) {
			if (close(fdReduce[i][1]) == -1) {
				perror("close");
			}
		}

		/* Wait for children to terminate */ 
		for (i = 0; i < numReduceWorker + numMapWorker + 1; i++) {
			if ((pid = wait(&status)) == -1) {
				perror("wait");
			}
		}
	}

	return 0;

}
