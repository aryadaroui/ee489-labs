/*
 * ifft.c
 *
 *  Created on: May 29, 2012
 *      Author: BLEE and Brian & Arya & Inessa
 *
 *  Description: Floating-point complex radix-2 decimation-in-freq IFFT
 *               Perform in place FFT the output overwrite the input array
 *
 *  Adapted from the book "Real Time Digital Signal Processing:
 *                Fundamentals, Implementation and Application, 3rd Ed"
 *                By Sen M. Kuo, Bob H. Lee, and Wenshun Tian
 *                Publisher: John Wiley and Sons, Ltd
 * 
 */



#include "tistdtypes.h"
// #include "fcomplex.h"       // Floating-point complex.h header file 
#include <math.h>   


#define pi 3.1415926535897  

void ifft(complex*, uint16, uint16, uint16);

void ifft(complex* X, uint16 EXP, uint16 hFlag, uint16 rFlag)
{
	complex  temp1;	// Temporary storage of complex variable 
	complex	 temp2;	// Temporary storage of complex variable 
	// complex watch1, watch2, watch3; // for debug
	complex W[7];		// Twiddle e^(-j2pi/N) table 
	complex  U;     // Twiddle factor W^k 
	uint16 a, j, i;	// a is index for higher point in butterfly. j and i are generic loop indices
	uint16 b;		// Index for lower point in butterfly 
	uint16 level;	// level is where the stages lie.
	uint16 stageSize;	// Number of points in sub DFT at stage level and offset to next DFT in stage 
	uint16 reach;		// Number of butterflies in one DFT a stage level.  Also is offset to lower point in butterfly at stage level 
	uint16 bits = EXP;	// reassigned to a less confusing name
	float hScale, rScale;		// scale to be applied
	uint16 N = 1 << bits;		// Number of points for FFT 
	uint16 stage = 0;			// stage is where the butterfly computations actaully criss cross. It is different from the level

	// CALC Twiddle Factor
	for(level = 1; level <= EXP; level++)				
	{
		stageSize = 1 << level;						// stageSize=2^level=points of sub DFT
		reach = stageSize >> 1;						// number of butterflies in sub-DFT 
		W[level - 1].re = cos(pi / reach);			// We chose to use the complex conjugate method instead of negative twiddle factor
		W[level - 1].im = sin(pi / reach);			
	}

	// PROC the conjugate
	for (i = 0;	 i<N; i++)
	{
		X[i].im = -X[i].im;
	}

	// SET Scaling Factor
	if(hFlag == 1)
	{
		hScale = 0.5;
	}
	else   
	{
		hScale = 1.0; 
	}


	if(rFlag == 1)
	{
		rScale = 1.0/N;
	}
	else   
	{
		rScale = 1.0; 
	}           
	
	// OUR FFT, DIF :-)
	for(level = bits; level < 8; level--) 	// FFT of length 2^bits 
	{
		stageSize = 1 << level;        	// stageSize=2^level=points of sub DFT : Will be 128, 64, 32, 16, ... 
		reach = stageSize >> 1;      	// Number of butterflies in sub-DFT 
		U.re  = 1.0;
		U.im  = 0.0;
		stage = -1;					// gets incremented to 0 before use

		// for (j=0; j<N; j+=stageSize)		// alternate option for smarter butterfly
		for(j = 0; j < reach; j++)
		{
			stage++;
			// for(a=j; a<reach+stageSize*stage; a++) // alternate option for smarter butterfly
			for(a = j; a < N; a += stageSize)
			{
				b = a + reach;

				temp1.re = X[a].re + X[b].re;
				temp1.im = X[a].im + X[b].im;

				temp2.re = X[a].re - X[b].re;
				temp2.im = X[a].im - X[b].im;

				X[a].re = temp1.re * hScale;
				X[a].im = temp1.im * hScale;

				X[b].re = (temp2.re * U.re - temp2.im * U.im) * hScale;
				X[b].im = (temp2.im * U.re + temp2.re * U.im) * hScale;
			}
			
			// Recursive compute W^k as U*W^(k-1) 
			temp1.re = U.re * W[level - 1].re - U.im * W[level - 1].im;
			U.im	 = U.re * W[level - 1].im + U.im * W[level - 1].re;
			U.re	 = temp1.re;
		}
	}
}