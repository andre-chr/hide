#include <stdio.h>
#define BIT_SIZE 8

/**
	Name: allocMem
	Description: allocate memory and checks if the memory is allocated
	Parameters: size, size of the structure in bytes
				nMem, number of members to allocate
	Returns: void pointer to the allocated memory location
*/
void *allocMem(int size, int nMem);

/**
	Name: copyHeader
	Description: Copies header of ifp to ofp, returns an array containing width & height
	Parameters: ifp, the pointer to the input file
				ofp, the pointer to the output file
	Returns: an array of size 2 containing the width and height of the PPM image, {width, height}
*/
int* copyHeader(FILE *ifp, FILE *ofp);

/**
	Name: copyNextChar
	Description: copies the next character from ifp to ofp
	Parameters: ifp, the pointer to the input file
				ofp, the pointer to the output file
	Returns: null
*/
void copyNextChar(FILE *ifp, FILE *ofp);

/**
	Name: getDimension
	Description: get the width and height of a PPM file
	Parameters: fileName, the name of the PPM file
	Returns: a pointer to an integer array of size 2, where array[0] = width and array[1] = height
*/
int *getDimension(char *fileName);

/**
	Name: getMaxSzie
	Description: get the maximum length of message a PPM file can hide
	Parameters: file, the filename of the PPM file
	Returns: maximum length of message a PPM file can hide
*/
int getMaxSize(char *file);

/**
	Name: passComments
	Description: pass through comments in PPM header and copies it to output file
	Parameters: ifp, the pointer to the input file
				ofp, the pointer to the output file
	Returns: null
*/
void passComments(FILE *ifp, FILE *ofp);

/**
	Name: getAllSize
	Description: get all maximum length of message PPM files with format "basename-xxx.ppm" can hide
	Parameters: inFile, the basename
	Returns: an array of integers containing the maximum length of message each file can hide
*/
int *getAllSize(char *inFile, int numOfFiles);

/**
	Name: getSumArray
	Description: get the sum of elements of an array
	Parameters: array, the array to get the sum from
				numOfElements, the length of the array
	Returns: sum of elements of the array
*/
int getSumArray(int *array, int numElements);
