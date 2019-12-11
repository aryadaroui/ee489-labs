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
					  
complex X[N];		// Declare input array  
complex W[EXP];		// Twiddle e^(-j2pi/N) table 
complex W1[EXP];	// Twiddle e^(-j2pi/N) table 
complex temp;
Int16   spectrum[N/2];

#define FFT_FLAG        	1	// FFT when 1 and IFFT when 0
#define HALF_SCALE_FLAG		1	// scales by 0.5 at butterfly computation
#define RECIP_SCALE_FLAG	0	// scale input samples by 1/N at beginning of FFT

int main()
{
	Uint16 i, j, L, LE, LE1, k, n; // generic index varabiels. L, LE, and LE1 relate to the bitdepth
	FILE* fp; // file pointer
	// float scale;


	// fprintf(fp, "Bin (at each 128-FFT frame)\tFFT spectrum\n"); //for debug
	n = 0;
	printf("Exp --- started\n");
	
	if(FFT_FLAG == 1)
	{
		// fp = fopen("..\\data\\FFT_spectrum.xls","wt");  // *** for windows
		fp = fopen("./Output/FFT_spectrum.xls","wt");//for mac
		for(L = 1; L <= EXP; L++)				// Create twiddle factor table
		{
			LE = 1 << L;						// LE=2^L=points of sub DFT
			LE1 = LE >> 1;						// number of butterflies in sub-DFT 
			W[L - 1].re = cos(pi / LE1);
			W[L - 1].im = sin(pi / LE1);
		}
	}
	else
	{
		// fp = fopen("..\\data\\IFFT_signal.xls","wt");  // *** for windows
		fp = fopen("./data/IFFT_signal.xls","wt");//for mac
		for(L = 1; L <= EXP; L++)				// Create twiddle factor table
		{
			LE = 1 << L;						// LE=2^L=points of sub DFT
			LE1 = LE >> 1;						// number of butterflies in sub-DFT 
			W[L - 1].re = cos(-pi / LE1);		// Twiddle Phase is negative for IDFT
			W[L - 1].im = sin(-pi / LE1);		// Twiddle Phase is negative for IDFT
		}		
	}

	for(k = 1, j = 0; j < (13 * N); j++)		// data file has 1664 = 13*128 data 
	{
		for(i = 0; i < N - 1; i++) 
		{
			X[i].re = input7_f[j++];	// construct input samples 
			X[i].im = 0.0;        
		} 	

		// Start FFT 
		fft(X, EXP, W, FFT_FLAG, HALF_SCALE_FLAG, RECIP_SCALE_FLAG); // perform FFT with scale 
		bit_rev(X, EXP);

		for(i = 0; i < N; i++)			  // verify FFT result 
		{
			temp.re = X[i].re * X[i].re;
			temp.im = X[i].im * X[i].im;        
			spectrum[i] = (Int16)((temp.re + temp.im) * 32767); // this is not actual magnitude; This is magnitude squared
			fprintf(fp, "%d\t%d\n", n++, spectrum[i]);
		}
		// printf("------ WINDOW %i COMPLETE\n", j/13); // for debug
	}

	fclose(fp);
	printf("Exp --- completed\n");
	return 0;
}

