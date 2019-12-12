/*
* float_fftTest.c
*
*  Created on: May 29, 2012
*      Author: BLEE
*
*  Description: This is the 128-point floating-point FFT experiment test program
*
*  For the book "Real Time Digital Signal Processing:
*                Fundamentals, Implementation and Application, 3rd Ed"
*                By Sen M. Kuo, Bob H. Lee, and Wenshun Tian
*                Publisher: John Wiley and Sons, Ltd
*/


// I changed the fopen call to be in the if statement based on FFT flag. Still need to open spectrum data and use this as input instead of signal. Should be done then. Also I did twiddle factor fix.

#include <stdio.h>
#include "tistdtypes.h"
#include <math.h>   
// #include "fcomplex.h"	// *** Floating-point complex.h header file  // arya commented this line out
#include "float_fft.h"
#include "input_f.dat"	// Test data file 
					  


// #define HALF_SCALE_FLAG		1	// scales by 0.5 at butterfly computation
// #define RECIP_SCALE_FLAG	0	// scale input samples by 1/N at beginning of FFT
// #define ONE_SIDED			1	// output one-sided spectrum instead of 2 sided

int main()
{	
	complex X[N];		// Declare input array  
	complex W[EXP];		// Twiddle e^(-j2pi/N) table 
	complex temp;
	int16   spectrum[N];
	int16   signal[N];
	bool HALF_SCALE_FLAG	= TRUE;	// scales by 0.5 at butterfly computation
	bool RECIP_SCALE_FLAG	= FALSE;	// scale input samples by 1/N at beginning of FFT
	bool ONE_SIDED			= TRUE;	// output one-sided spectrum instead of 2 sided
	uint16 i, j, L, LE, LE1, k, n; // generic index varabiels. L, LE, and LE1 relate to the bitdepth
	FILE* fpFFT;  // file pointer
	FILE* fpIFFT; // file pointer
	uint16 side;

	
	// float scale;


	// fprintf(fp, "Bin (at each 128-FFT frame)\tFFT spectrum\n"); //for debug
	n = 0;
	printf("Exp --- started\n");
	
		// fpFFT  = fopen("..\\Output\\FFT_spectrum.xls","wt"); // *** for windows
		// fpIFFT = fopen("..\\Output\\IFFT_signal.xls","wt");  // *** for windows
		fpFFT  = fopen("./Output/FFT_spectrum.xls","wt");	//for mac
		fpIFFT = fopen("./Output/IFFT_signal.xls","wt");	//for mac

	// PROCESS and WRITE FFT 
	for(k = 1, j = 0; j < (13 * N); j++)		// data file has 1664 = 13*128 data 
	{
		for(i = 0; i < N - 1; i++) 
		{
			X[i].re = input7_f[j++];	// construct input samples 
			X[i].im = 0.0;        
		} 	

		// PROCESS FFT 
		fft(X, EXP, HALF_SCALE_FLAG, RECIP_SCALE_FLAG); // perform FFT with scale 
		bit_rev(X, EXP);

		// WRITE FFT spectrum to file
		for(i = 0; i < N / 2; i++)			  // verify FFT result. was N/2 to only get one-sided spectrum
		{
			temp.re = X[i].re * X[i].re;
			temp.im = X[i].im * X[i].im;        
			spectrum[i] = (int16)((temp.re + temp.im) * 32767); // this is not actual magnitude; This is magnitude squared
			fprintf(fpFFT, "%d\t%d\n", n++, spectrum[i]);
		}
	}

	n = 0;
	// PROCESS and WRITE IFFT 
	for(k = 1, j = 0; j < 13; j++)		// data file has 1664 = 13*128 data 
	{
		HALF_SCALE_FLAG = FALSE;
		RECIP_SCALE_FLAG = TRUE;

		// PROCESS IFFT 
		bit_rev(X, EXP);
		ifft(X, EXP, HALF_SCALE_FLAG, RECIP_SCALE_FLAG); // perform FFT with scale 

		// WRITE IFFT signal to file
		for(i = 0; i < N / 2; i++)			  // verify FFT result. was N/2 to only get one-sided spectrum
		{
			temp.re = X[i].re * X[i].re;
			temp.im = X[i].im * X[i].im;        
			signal[i] = (int16)((temp.re + temp.im) * 32767); // this is not actual magnitude; This is magnitude squared
			fprintf(fpIFFT, "%d\t%d\n", n++, signal[i]);
		}
		// fclose(fpIFFT);

	}

	fclose(fpFFT);
	fclose(fpIFFT);
	printf("Exp --- completed\n");
	return 0;
}

