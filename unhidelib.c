#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "steganographylib.h"

char* unhideMessage(char *inFile) {
	int size, dec, buffer, a;
	FILE *fp;
	char *str;
	
	size = getMaxSize(inFile);		//get maximum size
	str = (char *)allocMem(1, size);
	
	fp = fopen(inFile, "r");
	if (!fp) {
		fprintf(stderr, "File open failed! (%s)\n", inFile);
		exit(2);
	}

	FILE* neglect = fopen("/dev/null", "w");		//use this file to throw the output
	if (!neglect) {
		fprintf(stderr, "File open failed! (/dev/null)\n");
		exit(2);
	}
	
	copyHeader(fp, neglect);	//pass through the PPM header
	
	//decode message
	dec = -1;
	buffer = 0;
	while (dec != 3 && dec != 0) {		//exits the loop if end of text or end of file is character read
		dec = 0;
		a = 1 << (BIT_SIZE - 1);	//a = 2**7
		while (a > 0) {
			if (fgetc(fp) % 2 == 1)	//adds the value if binary digit is 1
				dec += a;
			a >>= 1;	// a = a / 2
		}
		str[buffer++] = (char) dec;	//add character to the string
	}
	if (dec == 3)
		str[buffer-1] = 0;	//adds \0 at the end of the string to signify termination
	
	//close files
	fclose(fp);
	fclose(neglect);
	
	return str;	//return decoded message
}

int countFiles(char *basename) {
	char temp[128];
	int counter = 0;
	bool flag = true;
	FILE *fp;
	
	while (flag) {
		sprintf(temp, "%s-%03d.ppm", basename, counter);	//format string
		fp = fopen(temp, "r");
		counter++;
		//if the file doesn't exist, return counter
		if (!fp) {	
			counter--;
			flag = false;
		} else {
			fclose(fp);
		}
	}
	return counter;
}