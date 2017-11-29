#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <SDL.h>
#include "steganographylib.h"

char* getMsg(int maxSize) {
	int buff;
	char c;
	char* msg = (char*)allocMem(1, maxSize);
	
	//get msg to hide
	buff = 0;
	printf("Enter message to hide:\n");
	while ((c = getchar()) != EOF) {
		msg[buff++] = c;		//add c to msg
	}
	msg[buff] = 3;		//signify end of text at the end of message
	printf("\n");
	
	//handle error when msg is too long
	if ((int)strlen(msg) >= maxSize) {
		fprintf(stderr, "Message is too long!\n");
		exit(2);
    	}
	
	return msg;
}

char *readMsg(char *fileName, int maxSize) {
	FILE *fp;
	char c;
	int buff;
	char *msg;
	
	if (!(fp = fopen(fileName, "r"))) {
		fprintf(stderr, "File open failed!: %s\n", fileName);
		exit(2);
	}
	
	msg = (char *)allocMem(1, maxSize);
	
	buff = 0;
	while ((c = fgetc(fp)) != EOF) {
		msg[buff++] = c;
	}
	msg[buff] = 3;		//signal end of text at the end of the message
	
	//handle error when msg is too long
	if ((int)strlen(msg) >= maxSize) {
		fprintf(stderr, "Message is too long!\n");
		exit(2);
    	}
	return msg;
}

char* charToBinary(char c) {
	int i;
	char* binary;
	
	//allocate space for binary
	binary = (char*)allocMem(1, BIT_SIZE);
	//loops for length of bitstring
	for(i = BIT_SIZE - 1; i >= 0; i--) {
	    binary[7-i] = ( c & (1 << i) ) ? '1' : '0';		//if binary digit of i and c is 1, assign 1 else 0
	}
	return binary;
}

void encodeMsg(FILE* ifp, char* msg, int msgLen, FILE* ofp) {
	//read ifp, modify the least significant bit and write to ofp
	int i, j, tmp;
	char *str;
	for (i = 0; i < msgLen; i++) {
		str = charToBinary(msg[i]);	//turn character to binary string
		for (j = 0; j < BIT_SIZE; j++) {
			tmp = fgetc(ifp);
			if (str[j] == '0' && tmp % 2 == 1)
				tmp -= 1;	//set least significant digit to 0
			else if (str[j] == '1' && tmp % 2 == 0)
				tmp += 1;	//set least significant digit to 1
			fputc(tmp, ofp);	//write to output file
		}
	}
}

void hideMsg(char *inFile, char *msg, int msgLen, char *outFile) {
	FILE *ifp, *ofp;
	char tmp;
	int *dimen;
	
	dimen = getDimension(inFile);		//get width and height
	
	//open input PPM file
   	ifp = fopen(inFile, "r");
   	if (!ifp) {
		fprintf(stderr, "File open failed!: %s\n", inFile);
		exit(2);
	}

	 //open/create output PPM file
	ofp = fopen(outFile, "w");
	if (!ofp) {
		fprintf(stderr, "File open failed!: %s\n", outFile);
		exit(2);
	}

	copyHeader(ifp, ofp);		//copyHeader
	encodeMsg(ifp, msg, msgLen, ofp);		//encode message and write to output file
	
	int i;
	for (i=msgLen*8; i<dimen[0]*dimen[1]*3; i++) {
		tmp = fgetc(ifp);
		fputc(tmp, ofp);
	}
	
	printf("Message successfully hidden in: %s!\n", outFile);		//if program reaches this point, encoding msg is successful
	
	//close files
	fclose(ifp);
	fclose(ofp);
	
	//free memory
	free(dimen);
}

void drawPPM(char *fileName, int x, int y) {
	SDL_Window *window = NULL;
	SDL_Surface *screenSurface = NULL;
	FILE *fp, *neglect;
	int i;
	int *dimen, *p;
	
	//get width and height of image
	dimen = getDimension(fileName);		//dimen[0] = width	dimen[1] = height
	
	//initialize SDL
	if (SDL_Init(SDL_INIT_VIDEO) < 0) {
		fprintf(stderr, "SDL cannot initialize! SDL_Error %s\n", SDL_GetError());
		return;
	}
	
	//initialize window
	window = SDL_CreateWindow(fileName, 
				  x, y, 
				  dimen[0], dimen[1], SDL_WINDOW_SHOWN);
	
	if (!window) {
		printf("Window cannot be created! SDL_Error: %s\n", SDL_GetError());
		return;
	}
	screenSurface = SDL_GetWindowSurface(window);		//get the surface to manipulate pixel
	
	fp = fopen(fileName, "r");
	if (!fp) {
		fprintf(stderr, "File open failed! (%s)\n", fileName);
		return;
	}
	
	neglect = fopen("/dev/null", "w");		//use this file to throw the output
	if (!neglect) {
		fprintf(stderr, "File open failed! (/dev/null)\n");
		return;
	}
	copyHeader(fp, neglect);	//pass through header and throw output to /dev/null
	
	int r, g, b;
	for ( i=0;  i< dimen[0]*dimen[1]; i++) {
		p = (int *)screenSurface->pixels + i;
		r = fgetc(fp);
		g = fgetc(fp);
		b = fgetc(fp);
		*p = SDL_MapRGB(screenSurface->format, r, g, b);	//set pixel at *p to r,g,b
	}
	SDL_UpdateWindowSurface(window);
	SDL_Delay(5000);	//suspend program for 5 seconds
	SDL_DestroyWindow(window);	
	SDL_Quit();	//closes the image
	
	//close files
	fclose(fp);
	fclose(neglect);
	
	//free memory
	free(dimen);
}


