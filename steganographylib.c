#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define BIT_SIZE 8

void *allocMem(int size, int nMem) {
	void *res;
	
	//allocate memory and checks if it is successful
	if (!(res = malloc(size * nMem))) {	
		fprintf(stderr, "Unable to allocate memory!\n");
		exit(2);
	}
	return res;	//return pointer
}

void copyNextChar(FILE *ifp, FILE *ofp){
	char tmp;
	
	tmp = fgetc(ifp);
	fputc(tmp, ofp);
}

void passComments(FILE *ifp, FILE *ofp) {
	char tmp;
	
	tmp = fgetc(ifp);
	while (tmp == '#') {	
		fputc(tmp, ofp);		//write '#' to ofp
		tmp = fgetc(ifp);
		while ( tmp != '\n') {
				fputc(tmp, ofp);
				tmp = fgetc(ifp);
		}
		fputc(tmp, ofp);		//write '\n' to ofp
		tmp = fgetc(ifp);
	}
	ungetc(tmp, ifp);
}

int *getDimension(char *file) {
	int *arr;
	FILE *fp = fopen(file, "r");
	FILE *neglect = fopen("/dev/null", "w");		//use this file to throw the output
	
	arr = (int *)allocMem(sizeof(int), 2);		//allocate space to store width & height in an array
	
	if (!fp) {
		fprintf(stderr, "File open failed!: %s\n", file);
		exit(2);
	}
	
	if (!neglect) {
		fprintf(stderr, "File open failed!: %s\n", "/dev/null");
		exit(2);
	}
	
	//checks if magic number is P6
	if (fgetc(fp) != 'P' || fgetc(fp) != '6') {
		fprintf(stderr, "Magic number is not P6!\n");
		exit(2);
	}
	fgetc(fp);	//pass the next \n
	passComments(fp, neglect);	//pass the comments
	
	fscanf(fp, "%d %d\n", &arr[0], &arr[1]);	//get the width and height
	
	return arr;
}	

int getMaxSize(char *file) {
	int width, height;
	FILE *fp = fopen(file, "r");
	FILE *neglect = fopen("/dev/null", "w");		//use this file to throw the output
	
	if (!fp) {
		fprintf(stderr, "File open failed!: %s\n", file);
		exit(2);
	}
	
	if (!neglect) {
		fprintf(stderr, "File open failed!: %s\n", "/dev/null");
		exit(2);
	}
	
	if (fgetc(fp) != 'P' || fgetc(fp) != '6') {
		fprintf(stderr, "Magic number is not P6!\n");
		exit(2);
	}
	fgetc(fp);
	passComments(fp, neglect);
	
	fscanf(fp, "%d %d\n", &width, &height);
	
	fclose(fp);
	fclose(neglect);
	return width * height * 3 / 8;	
}


void copyHeader(FILE *ifp, FILE *ofp) {
	int width, height, maxColour;
	
	//Check if magic number is P6
	if (fgetc(ifp) != 'P' || fgetc(ifp) != '6') {
		fprintf(stderr, "Magic number is not P6!\n");
		exit(2);
	}
	fprintf(ofp, "P6");
	
	copyNextChar(ifp, ofp);
	passComments(ifp, ofp);
	
	//get width and height
	fscanf(ifp, "%d %d\n", &width, &height);
	
	//write width & height to output file
	fprintf(ofp, "%d %d\n", width, height);
	
	passComments(ifp, ofp);
	
	//check if max pixel color is 255
    	fscanf(ifp, "%d\n", &maxColour);
    	if (maxColour != 255) {
		fprintf(stderr, "Max colour value is not 255!\n");
		exit(2);
    	}
	fprintf(ofp, "%d\n", maxColour);
	
	passComments(ifp, ofp);
}

int *getAllSize(char *inFile, int numOfFiles) {
	int i;
	int *res;
	char *str;
	
	res = (int *)allocMem(sizeof(int), numOfFiles);
	
	str = (char *)allocMem(1, 1024);
	
	for (i=0; i<numOfFiles; i++){
		sprintf(str, "%s-%03d.ppm", inFile, i);
		res[i] = getMaxSize(str);
	}
	free(str);
	return res;
}

int getSumArray(int *array, int numElements) {
	if (numElements == 0)
		return 0;
	if (numElements == 1)
		return array[0];
	int i, sum=0;
	for (i=0; i < numElements; i++) {
		sum = sum + array[i];	
	}
	return sum;
}