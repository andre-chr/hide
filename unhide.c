#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <unistd.h>
#include "steganographylib.h"
#include "unhidelib.h"

int main(int argc, char *argv[]) {
	if (argc < 2) {
		fprintf(stderr, "More than 1 argument required! Found %d argument(s)\n", argc);
		exit(2);
	}
	char *message;
	int opt;
	bool mFlag = false;
	
	//handles -m flag
	while ((opt = getopt(argc, argv, "m")) != -1) {
		switch(opt) {
			case 'm':
				mFlag = true;
				break;
		}
	}
	
	if (mFlag) {
		int i, *sizeArray;
		char *subMsg;
		char *fileName;
		
		int filesCount = countFiles(argv[optind]);	//get the number of files to unhide
		sizeArray = getAllSize(argv[optind], filesCount);	//get the maximum string length of message
		message = (char *)calloc(getSumArray(sizeArray, filesCount), 1);	//allocate memory for message
		if (!message) {
			fprintf(stderr, "Unable to allocate memory!\n");
			exit(2);
		}
		
		fileName = (char *)allocMem(1, 128);
		for (i=0; i < filesCount; i++) {
			sprintf(fileName, "%s-%03d.ppm", argv[optind], i);	//format string
			subMsg = unhideMessage(fileName);	//decodes a part of the message in the file
			strcat(message, subMsg);	//add the acquired segment of message to the whole message
			free(subMsg);		//free memory after it is copied
		}
		//free memory
		free(fileName);
		free(sizeArray);
	} else {
		message = unhideMessage(argv[1]);	//get the hidden message from the image
	}
	printf("The hidden message is:\n%s\n", message);	//print hidden message
	
	//free memory
	free(message);
}
