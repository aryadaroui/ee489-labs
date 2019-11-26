#include <stdio.h>
#include <stdlib.h>
#include "tistdtypes.h" // this is so dumb.


// .WAV file header
Uint8 waveHeader[44]={                          						// 44 bytes for WAV file header
		0x52,   0x49,   0x46,   0x46,   0x00,   0x00,   0x00,   0x00, 	// 0:7
		0x57,   0x41,   0x56,   0x45,   0x66,   0x6D,   0x74,   0x20, 	// 8:15
		0x10,   0x00,   0x00,   0x00,   0x01,   0x00,   0x01,   0x00, 	// 16-23
		0x40,   0x1F,   0x00,   0x00,   0x80,   0x3E,   0x00,   0x00, 	// 24:31
		0x02,   0x00,   0x10,   0x00,   0x64,   0x61,   0x74,   0x61, 	// 32:39 bitsPerSample is at 34 and 35. bitPerSample = 16 bit
		0x00,   0x00,   0x00,   0x00};                                	// 40-43

#define SIZE    1024		// Used for original pcm to wav func
Uint8 chunk[SIZE];          // Declare a char[1024] array for experiment

void main()
{
	FILE *fp1,*fp2, *fp3;   // file pointers
	Uint32 i;           	// unsigned long integer used as a counter
	Uint8 nugget[2];		// two element arrary of 8 bytes each for easier byte swapping
	int temp;				// the int converted from string before shrunk to uint16 buffer
	Uint16 buffer[1];		// the new int value before swapped back to pcm orientation
	char str[10];			// the read string from the csv file

	printf("START\n");

	fp1 = fopen("..\\data\\C55DSPUSBStickAudioTest.pcm", "rb"); // Open pcm file to read
	fp2 = fopen("..\\data\\test.wav", "wb");					// Open wav file to write
	fp3 = fopen("..\\data\\test.csv", "wb");					// Open csv file to write

	if (fp1 == NULL)	// Check if the input file exists
	{
		printf("Failed to open input file 'C55DSPUSBStickAudioTest.pcm'\n");
		exit(0);
	}
	else
	{
		printf("pcm file opened to read.\n");
	}

	// OUTPUT pcm to csv
	while (fread(nugget, sizeof(Uint8), 2, fp1) == 2) // reads from pcm two bytes at a time
	{
		fprintf(fp3, "%d\n", (short)((nugget[1]<<8)|nugget[0])); // swaps the two bytes, and then writes them
	}
	fclose(fp3); // CLOSE csv

	fp3 = fopen("..\\data\\test.csv", "r"); 	// OPEN csv file to read
	fseek(fp2, 44, 0); // navigate 44 bytes forward in the wav file, past the header
	// OUTPUT csv to wav
	// i = 0; // reinitialize
	while (fgets(str, 10, fp3) != NULL) 		// read characters into str from csv one line at a time
	{
		temp = atoi(str);						// convert read string to int
		buffer[0] = (Uint16)(temp);				// convert int to uint16
		nugget[0] = (Uint8)(buffer[0]&0xFF);	// mask the second byte of buffer into the first element of nugget
		nugget[1] = (Uint8)(buffer[0]>>8&0xFF);	// shift and mask the first byte of buffer into the second of nugget
		fwrite(nugget, sizeof(Uint8), 2, fp2);	// write nugget into wave two bytes at a time
		// i += 16;								// count how many data bytes are written
	}

	// // ORIGINAL pcm to wav
	//     while (fread(chunk, sizeof(Uint8), SIZE, fp1) == SIZE)	// Read in SIZE of input data bytes
	// {

	//     fwrite(chunk, sizeof(Uint8), SIZE, fp2);  				// Write SIZE of data bytes to output file
	//     i += SIZE;
	//     printf("%ld bytes processed\n", i);    					// Show the number of data is processed
	//     // printf("data: %u \n",  chunk[i]);   					// Print what we have in the channel
	// }

	i = 59392; // cheat knowing how many data bytes were processed
	
	waveHeader[40] = (Uint8)(i&0xff);          // Update the size parameter into WAV header
	waveHeader[41] = (Uint8)(i>>8)&0xff;       // Masking only the portion of i needed
	waveHeader[42] = (Uint8)(i>>16)&0xff;
	waveHeader[43] = (Uint8)(i>>24)&0xff;
	waveHeader[4] = waveHeader[40];
	waveHeader[5] = waveHeader[41];
	waveHeader[6] = waveHeader[42];
	waveHeader[7] = waveHeader[43];

	rewind(fp2);                                // return to start of wav file, where the header is
	fwrite(waveHeader, sizeof(Uint8), 44, fp2); // write 44 bytes of WAV header to output file

	fclose(fp1);                                // close pcm file
	fclose(fp2);                                // close wav file

	printf("\nCOMPLETE\n");

	// return 0;
}

