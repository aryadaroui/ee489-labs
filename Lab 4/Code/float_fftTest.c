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
					  


// #define isHalfScale		1	// scales by 0.5 at butterfly computation
// #define isRecipScale	0	// scale input samples by 1/N at beginning of FFT
// #define isOneSided			1	// output one-sided spectrum instead of 2 sided

int main()
{	
	complex X[N];		// Declare input array  
	complex W[EXP];		// Twiddle e^(-j2pi/N) table 
	complex temp;
	int16   spectrum[N];
	complex	allSpectra[N*13];
	double   signal[N];
	bool isHalfScale;		// scales by 0.5 at butterfly computation
	bool isRecipScale;	// scale input samples by 1/N at beginning of FFT
	bool isOneSided;		// output one-sided spectrum instead of 2 sided
	uint16 i, j, L, LE, LE1, k, n; // generic index varabiels. L, LE, and LE1 relate to the bitdepth
	FILE* fpFFT;  // file pointer
	FILE* fpIFFT; // file pointer
	uint16 side = 1;

	// fprintf(fp, "Bin (at each 128-FFT frame)\tFFT spectrum\n"); //for debug
	n = 0;
	printf("Exp --- started\n");
	
	// fpFFT  = fopen("..\\Output\\FFT_spectrum.xls","wt"); // *** for windows
	// fpIFFT = fopen("..\\Output\\IFFT_signal.xls","wt");  // *** for windows
	fpFFT  = fopen("./Output/FFT_spectrum.xls","wt");	//for mac
	fpIFFT = fopen("./Output/IFFT_signal.xls","wt");	//for mac

	isHalfScale = TRUE;
	isRecipScale = FALSE;
	isOneSided = TRUE;

	// PROCESS and WRITE FFT 
	for(j = 0; j < (13 * N); j++)		// data file has 1664 = 13*128 data 
	{
		for(i = 0; i < N; i++) 
		{
			X[i].re = input7_f[j++];	// construct input samples 
			X[i].im = 0.0;        
		}
		j--; // this is needed otherwise j will be one more than it's supposed to

		// PROCESS FFT
		fft(X, EXP, isHalfScale, isRecipScale); // perform FFT with scale. this should not return void and alter array like this. would be better to return array ptr
		bit_rev(X, EXP);

		for (i = 0; i < N; i++)
		{
			allSpectra[j - N + i + 1].re = X[i].re;
			allSpectra[j - N + i + 1].im = X[i].im;
		}
		
		// SET sided-ness
		if(isOneSided)
		{
			side = 2;
		}
		else
		{
			side = 1;
		}
		
		// WRITE FFT spectrum to file
		for(i = 0; i < N / side; i++)			  // verify FFT result. was N/2 to only get one-sided spectrum
		{
			temp.re = X[i].re * X[i].re;
			temp.im = X[i].im * X[i].im;        
			spectrum[i] = (int16)((temp.re + temp.im) * 32767); // this is not actual magnitude; This is magnitude squared
			fprintf(fpFFT, "%d\t%d\n", n++, spectrum[i]);
		}
	}

	n = 0;
	isHalfScale = FALSE;
	isRecipScale = FALSE;

	// PROCESS and WRITE IFFT
	for(j = 0; j < 13; j++)
	{
		for(i = 0; i < N; i++) 
		{
			X[i].re = allSpectra[i + (N * j)].re;
			X[i].im = allSpectra[i + (N * j)].im;
		} 	

		// PROCESS IFFT
		ifft(X, EXP, isHalfScale, isRecipScale); // perform FFT with scale
		bit_rev(X, EXP);

		// WRITE IFFT signal to file
		for(i = 0; i < N; i++)			  // verify FFT result.
		{
			// temp.re = X[i].re * X[i].re;
			// temp.im = X[i].im * X[i].im;        
			// signal[i] = (float)((temp.re + temp.im)); // this is not actual magnitude; This is magnitude squared
			// fprintf(fpIFFT, "%d\t%f\n", n++, signal[i]);
			fprintf(fpIFFT,"%d\t%f\n", n++, X[i].re);
		}
	}

	fclose(fpFFT);
	fclose(fpIFFT);
	printf("Exp --- completed\n");
	return 0;
}

