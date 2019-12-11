/*
 * float_fft.c
 *
 *  Created on: May 29, 2012
 *      Author: BLEE
 *
 *  Description: Floating-point complex radix-2 decimation-in-time FFT
 *               Perform in place FFT the output overwrite the input array
 *
 *  For the book "Real Time Digital Signal Processing:
 *                Fundamentals, Implementation and Application, 3rd Ed"
 *                By Sen M. Kuo, Bob H. Lee, and Wenshun Tian
 *                Publisher: John Wiley and Sons, Ltd
 */

#include "tistdtypes.h"
#include "fcomplex.h"       // Floating-point complex.h header file 

void fft(complex*, Uint16, complex*, Uint16, Uint16, Uint16);

void fft(complex* X, Uint16 EXP, complex* W, Uint16 iFlag, Uint16 hFlag, Uint16 rFlag)
{
	complex  temp1;	// Temporary storage of complex variable 
	complex	 temp2;	// Temporary storage of complex variable 
	complex watch1, watch2, watch3; // for debug
	complex  U;     // Twiddle factor W^k 
	Uint16 a, j, i;	// a is index for higher point in butterfly. j and i are generic loop indices
	Uint16 b;		// Index for lower point in butterfly 
	Uint16 level;	// level is where the stages lie.
	Uint16 stageSize;	// Number of points in sub DFT at stage level and offset to next DFT in stage 
	Uint16 reach;		// Number of butterflies in one DFT a stage level.  Also is offset to lower point in butterfly at stage level 
	Uint16 bits = EXP;	// reassigned to a less confusing name
	float hScale, rScale;		// scale to be applied
	Uint16 N = 1 << bits;// Number of points for FFT 
	Uint16 stage = 0;	// stage is where the butterfly computations actaully criss cross. It is different from the level

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
	
	// if(iFlag == 1)
	// {
	// 	scale = 0.5;
	// }


	for(i = 0; i < N; i++)
	{
		watch1.re = X[i].re;
		X[i].re = X[i].re * rScale;
		watch1.re = X[i].re;
		X[i].im = X[i].im * rScale;
	}

	
   	/* ORIGINAL, GIVEN FFT, DIT
		for (level=1; level<=bits; level++) 	// FFT of length 2^bits 
		{
			stageSize=1<<level;        	// stageSize=2^level=points of sub DFT : Will be 2, 4, 8, 16, 32, 64, 128 
			reach=stageSize>>1;      	// Number of butterflies in sub-DFT 
			U.re = 1.0;
			U.im = 0.0;
	
			for (j=0; j<reach;j++)
			{
				for(a=j; a<N; a+=stageSize) // Butterfly computations 
				{
					b=a+reach;
					temp1.re = (X[b].re*U.re - X[b].im*U.im)*scale;
					temp1.im = (X[b].im*U.re + X[b].re*U.im)*scale;
	
					X[b].re = X[a].re*scale - temp1.re;
					X[b].im = X[a].im*scale - temp1.im;
	
					X[a].re = X[a].re*scale + temp1.re;
					X[a].im = X[a].im*scale + temp1.im;
	
					printf("%d -> %d\n", a,b);
				}
				
				// Recursive compute W^k as U*W^(k-1) 
				temp1.re = U.re*W[level-1].re - U.im*W[level-1].im;
				U.im = U.re*W[level-1].im + U.im*W[level-1].re;
				U.re = temp1.re;
			}
		}
	*/

	// OUR FFT, DIF :-)
	for(level = bits; level < 8; level--) 	// FFT of length 2^bits 
	{
		stageSize = 1 << level;        	// stageSize=2^level=points of sub DFT : Will be 128, 64, 32, 16, ... 
		reach = stageSize >> 1;      	// Number of butterflies in sub-DFT 
		U.re  = 1.0;
		U.im  = 0.0;
		stage = -1;					// immediately gets incremented to 0

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

				X[b].re = (temp2.re * U.re - temp2.im * U.im) * hScale; // test?
				X[b].im = (temp2.im * U.re + temp2.re * U.im) * hScale;

				// watch2.re = X[b].re;							// for debug
				// watch2.im = X[b].im;							// for debug
				// watch3 = (Int16)((temp1.re+temp1.im)*32767);	// for debug

				// printf("%d -> %d\n", a,b);
				// printf("reach: #%d\n", reach);
				// printf("b: #%d\n", b);
			}
			
			// Recursive compute W^k as U*W^(k-1) 
			temp1.re = U.re * W[level - 1].re - U.im * W[level - 1].im;
			U.im	 = U.re * W[level - 1].im + U.im * W[level - 1].re;
			U.re	 = temp1.re;
			// printf("-- stage complete ---\n");	// for debug
		}
		// printf("\n--- level complete ---\n");	// for debug
	}
}