#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <unistd.h>
#include <sys/wait.h>
#include "steganographylib.h"
#include "hidelib.h"

int main(int argc, char *argv[]) {
	//exit if arguments are not 3
	if (argc < 3) {
		fprintf(stderr, "At least 2 Arguments are required!\n");
		exit(2);
	}

	int maxSize, opt;
	char* message;
	bool mFlag, pFlag, sFlag;
	int mArg = 0;
	
	//parse flags
	mFlag = pFlag = sFlag = false;
	while ((opt = getopt(argc, argv, "m:ps")) != -1) {
		switch (opt) {
			case 'm':
				mFlag = true;
				mArg = atoi(optarg);
				break; 
			case 'p':
				pFlag = true;
				break;
			case 's':
				sFlag = true;
				break;
			default:
				break;
		}
	}
	
	if ((mFlag && pFlag) || (mFlag && sFlag) || (pFlag && sFlag)) {	//if there are more than 1 flags, raise an error
		fprintf(stderr, "More than 1 flag is used!\n");
		exit(2);
	}
	
	if (mFlag) {	//if -m is on
		int i, *sizeArray;
		char temp1[128], temp2[128], *sub_msg;
		
		sizeArray = getAllSize(argv[optind], mArg);
		message = getMsg(getSumArray(sizeArray, mArg));
		sub_msg = message;
		
		for (i=0; i<mArg; i++) {
			if (strlen(message) < sizeArray[i]) {	//if the message fits entirely in this file
				sprintf(temp1, "%s-%03d.ppm", argv[optind], i);
				sprintf(temp2, "%s-%03d.ppm", argv[optind+1], i);
				hideMsg(temp1, message, strlen(message), temp2);
				break; 
			} else {	//if the message doesn't entirely fit in this file
				sprintf(temp1, "%s-%03d.ppm", argv[optind], i);
				sprintf(temp2, "%s-%03d.ppm", argv[optind+1], i);
				hideMsg(temp1, sub_msg, sizeArray[i], temp2);
				sub_msg = sub_msg + sizeArray[i];
			}
		}
		//free memories
		free(message);
		free(sizeArray);
	}else if (pFlag) {	//if -p is on
		if (argc != 3) {	//only accepts 2 arguments
			fprintf(stderr, "1 Argument is required! for -p\n");
			exit(2);
		}
		FILE *fp;
		char *msg, *inFile, *outFile;
		pid_t pid, pidW;
		
		fp = fopen(argv[2], "r");
		if (!fp) {
			fprintf(stderr, "Unable to open file: %s\n", argv[2]);
			exit(2);
		}
		
		//forks the process for each lines in the input text
		char *params[5] = {"./hide", NULL, NULL, NULL, 0};
		msg = (char *)allocMem(1, 128);
		inFile = (char *)allocMem(1, 128);
		outFile = (char *)allocMem(1, 128);
		while (fscanf(fp, "%s %s %s", msg,  inFile, outFile) != EOF) {
			params[1] = inFile;
			params[2] = outFile;
			params[3] = msg;
			pid = fork();
			
			if (pid == 0) {
				execvp("./hide", params);	//make child process run this program without any flag
			}
		}
		while ((pidW = wait(NULL)) > 0);	//wait for all child processes to finish
		
		//free memory
		free(msg);
		free(inFile);
		free(outFile);
	}else {		//if no flag  or -s flag is used
		maxSize = getMaxSize(argv[optind]);
		if ((argc == 4 && !sFlag) || (argc == 5 && sFlag)) 		//option to supply the message through a text file
			message = readMsg(argv[optind+2], maxSize);	
		else			//if no extra text file is supplied, get the message from stdin
			message = getMsg(maxSize);
		hideMsg(argv[optind], message, strlen(message), argv[optind+1]);	//hide message
		free(message);
		
		if (sFlag) {
			int *dimen;
			pid_t pid;
			
			dimen = getDimension(argv[optind]);	//get width and height to position window
			pid = fork();		//forks the process to display both images at once
			if (pid == 0)
				drawPPM(argv[optind], 0, 0);		//display the original image
			else
				drawPPM(argv[optind+1], dimen[1], 0);	//display the modified image
			
			wait(NULL);	//wait for child process to finish
			
			//free memory
			free(dimen);
		}
	}
}
