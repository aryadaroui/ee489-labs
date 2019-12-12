/*
 * fbit_rev.c
 *
 *  Created on: May 29, 2012
 *      Author: BLEE
 *
 *  Description: This is the FFT bit reversal function
 *               Arrange input samples in bit-reverse addressing order
 *               - the index j is the bit reverse of i
 *
 *  For the book "Real Time Digital Signal Processing:
 *                Fundamentals, Implementation and Application, 3rd Ed"
 *                By Sen M. Kuo, Bob H. Lee, and Wenshun Tian
 *                Publisher: John Wiley and Sons, Ltd
 */

#include "tistdtypes.h"
// #include "fcomplex.h"	// *** Floating-point complex.h header file 

void bit_rev(complex* X, int16 EXP)
{
    uint16 i, j, k;
    uint16 N = 1 << EXP;	// Number of points for FFT 
    uint16 N2 = N >> 1;
    complex temp;		// Temporary storage of the complex variable 
	
    for(j = 0, i = 1; i < N - 1; i++)
    {
        k = N2;
        while(k <= j)
        {
           	j -= k;
            k >>= 1;
        }
        
        j += k;

        if (i < j)
        {
          temp = X[j];
          X[j] = X[i];
          X[i] = temp;
        }
		// printf("%d -- %d -- %d\n", i, j, k); // for debug
    }  
}
