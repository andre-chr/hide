#include <stdio.h>

/**
	Name: getMsg
	Description: gets message from stdin and checks whether the message length is allowed
	Parameters: maxSize, the maximum length of message
	Returns: pointer to the message string
*/
char *getMsg(int maxSize);

/**
	Name: readMsg
	Description: gets message from a text file and checks whether the message length is allowed
	Parameters: fileName, pointer to the filename of the file containing the message
				maxSize, the maximum length of message
	Returns: pointer to the message string
*/
char *readMsg(char *fileName, int maxSize);

/**
	Name: charToBinary
	Description: converts a char to a binary string
	Parameters: c, character to be converted
	Returns: pointer to the binary string
*/
char *charToBinary(char c);

/**
	Name: encodeMsg
	Description: encode message and write it to output file
	Parameters: ifp, pointer to the input file
				msg, message to be encoded
				msgLen, the length of the message
				ofp, pointer to the output file
	Returns: null
*/
void encodeMsg(FILE* ifp, char* msg, int msgLen, FILE* ofp);

/**
	Name: hideMsg
	Description: hides message in the output file
	Parameters: inFile, filename of the input file
				msg, message to be encoded
				msgLen, the length of the message
				outFile, filename of the output file
	Returns: null
*/

void hideMsg(char *inFile, char *msg, int msgLen, char *outFile);

/**
	Name: drawPPM
	Description: Displays a PPM image for 5 seconds
	Parameters: fileName, filename of the PPM image
				x, x position of the image window
				y, y position of the image window
	Returns: null
*/
void drawPPM(char *fileName, int x, int y);