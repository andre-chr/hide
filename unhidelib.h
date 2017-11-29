#include <stdio.h>

/**
	Name: unhideMessage
	Description: decodes a message in a PPM image
	Parameters: inFile, the file name of the PPM image
	Returns: pointer to a string of the decoded message
*/
char* unhideMessage(char *inFile);

/**
	Name: countFiles
	Description: count the number of files with the same basename in the directory
	Parameters: basename
	Returns: the number of files with the basename
*/
int countFiles(char *basename);