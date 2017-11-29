
Hide/Unhide


This folder contains 2 programs, hide and unhide. The functionalities are as follows:
	- hide.c: Encodes a message in a picture in ppm format from the input file to the specified output file. Hide also has extra 
			functionalities that can be used as an option, which are:
				-m: Hiding a message in multiple files for longer messages. The program will take more than one PPM files
					with the same base name and will hide a message in multiple output files with the same output base name.
				-p: Hiding multiple message simultaneously. The program will take an input text file which contains lines of
					input PPM, output PPM, and the message to be hidden. It will then run each lines in a separate process
					so we can hide messages faster.
				-s: Displaying the input and output picture side by side for comparison. This option will hide the message normally
					and it will display the input and output images side by side for 5 seconds.
	- unhide.c Decodes a message from a picture from the input file and prints it to the console. Unhide also has an -m option 
			to decode a message that was hidden using -m option of hide.

Required library:
	1. SDL2 library
	
How to run the program:
	1. Open the file directory in the command line
	2. Run "make" at the command line to compile the program and  build the executables
	3. To hide a message without any option, enter "./hide [input-image] [output-image]", then the program will prompt you 
		for the message. Enter your desired message to the command line. 
		
		These are the instructions to run hide with options:
			-m: To use this option, we need to specify the number of images that we can use to hide the message. In
				addition to that, we need to supply the specified images with the naming scheme: [basename]-###.ppm,
				where "###" is the file number, e.g. image-000.ppm. As such, we need to name the images with numbers
				from 0 to number_of_images. After the images are prepared, run 
				"./hide -m [no_of_images] [basename] [output-basename]" in the command line.
			-p: To use this option, we need to create a text file containing line(s) of: "[message-textfile] [input-image] [output-image]"
				where the number of lines will be the number of messages to be encoded in the image. Then, run 
				"./hide -p [processes-textfile]" in the command line.
			-s: To run this option, enter: "./hide -s [input-image] [output-image]" in the command line.
	4. To unhide a message, enter "./unhide [image]". The hidden message will be shown in the command line. To unhide a message
		located in multiple images using hide -m, enter: "./unhide -m [basename]" in the command line.
		
	*note: Input and output file must be of .ppm format & only one flag can be used at a time

There are no known bugs in the programs.
	
