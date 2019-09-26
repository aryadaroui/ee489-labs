#include <stdio.h>
#include <stdlib.h>
#include "tistdtypes.h" // this is so dumb

void BinPrint8(Uint8 v)
{
	Uint8 mask=1<<((sizeof(Uint8)<<3)-1);
	while(mask) {
		printf("%d", (v&mask ? 1 : 0));
		mask >>= 1;
	}
}

Uint8 waveHeader[44]={                          // 44 bytes for WAV file header
		0x52,   0x49,   0x46,   0x46,   0x00,   0x00,   0x00,   0x00, // 0:7
		0x57,   0x41,   0x56,   0x45,   0x66,   0x6D,   0x74,   0x20, // 8:15
		0x10,   0x00,   0x00,   0x00,   0x01,   0x00,   0x01,   0x00, // 16-23
		0x40,   0x1F,   0x00,   0x00,   0x80,   0x3E,   0x00,   0x00, // 24:31
		0x02,   0x00,   0x10,   0x00,   0x64,   0x61,   0x74,   0x61, // 32:39 bitsPerSample is at 34 and 35. bitPerSample = 16 bit
		0x00,   0x00,   0x00,   0x00};                                // 40-43
#define SIZE    1024
Uint8 chunk[SIZE];                                 // Declare a char[1024] array for experiment

int main()
{
	FILE *fp1,*fp2, *fp3;   // File pointers
	Uint32 i;           	// Unsigned long integer used as a counter
	Uint32 k;				// arbitrary index
	Uint8 nugget[2];
	int temp;
	Uint16 buffer[1];
	char str[10];

	printf("START\n");

	fp1 = fopen("./data/data.pcm", "rb"); // Open pcm file
	fp2 = fopen("./data/test.wav", "wb"); // Open wav file
	fp3 = fopen("./data/test.csv", "wb"); // Open wav file

	// OPEN pcm file
	if (fp1 == NULL)                            // Check if the input file exists
	{
		printf("Failed to open input file 'C55DSPUSBStickAudioTest.pcm'\n");
		exit(0);
	}
	else
	{
		printf("pcm file opened.\n");
	}
	// OUTPUT pcm to csv lol
	while (fread(nugget, sizeof(Uint8), 2, fp1) == 2)
	{
		fprintf(fp3, "%d\n", (short)((nugget[1]<<8)|nugget[0]));
	}

	fclose(fp3); // CLOSE csv

	fp3 = fopen("./data/test.csv", "r"); 	// OPEN csv

    fseek(fp2, 44, 0);       // inessa's fault

	// OUTPUT csv to wav lol
	while (fgets(str, 10, fp3) != NULL)
	{
		// printf(nugget);
		// printf("2 uint8: %d\n", nugget);
		// printf("str %d", nugget);
		// printf("int %d", atoi(nugget));
		// printf("strg: %s", str);
		temp = atoi(str);
		buffer[0] = (Uint16)(temp);
		// printf("ok: %d\n", buffer);
		// buffer = (Uint16)(((Uint8)(buffer<<8))|((Uint8)(buffer>>8)));
		nugget[0] = (Uint8)(buffer[0]&0xFF);
		nugget[1] = (Uint8)(buffer[0]>>8&0xFF);
		// printf("%u", nugget[0]);
		// printf("%u", nugget[1]);
		// buffer[0] = ((nugget[1]<<8)|nugget[0]);
		// printf("sw: %d\n", buffer);
		// nugget = buffer;
		// printf("sint: %d\n", temp);
		// printf("uint: %d\n", buffer[0]);
		fwrite(nugget, sizeof(Uint8), 2, fp2);
	}


	
	// ORIGINAL pcm to wav lol
	//     while (fread(chunk, sizeof(Uint8), SIZE, fp1) == SIZE) // Read in SIZE of input data bytes
	// {

	//     fwrite(chunk, sizeof(Uint8), SIZE, fp2);  // Write SIZE of data bytes to output file
	//     i += SIZE;
	//     printf("%ld bytes processed\n", i);    // Show the number of data is processed
	//     // printf("data: %u \n",  chunk[i]);   // Print what we have in the channel
	// }

	i = 59392;
	

	waveHeader[40] = (Uint8)(i&0xff);          // Update the size parameter into WAV header
	waveHeader[41] = (Uint8)(i>>8)&0xff;       // Masking only the portion of i needed
	waveHeader[42] = (Uint8)(i>>16)&0xff;
	waveHeader[43] = (Uint8)(i>>24)&0xff;
	waveHeader[4] = waveHeader[40];
	waveHeader[5] = waveHeader[41];
	waveHeader[6] = waveHeader[42];
	waveHeader[7] = waveHeader[43];

	rewind(fp2);                                // Adjust output file point to beginning to write header into output file
	fwrite(waveHeader, sizeof(Uint8), 44, fp2); // Write 44 bytes of WAV header to output file

	fclose(fp1);                                // Close input file
	fclose(fp2);                                // Close output file
	fclose(fp3); 

	printf("\nCOMPLETE\n");

	return 0;
}

