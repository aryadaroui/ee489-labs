/*
 * c5505.cmd
 *
 *  Created on: Dec 16, 2012
 *      Author: BLEE
 *
 *  For the book "Real Time Digital Signal Processing:
 *                Fundamentals, Implementation and Application, 3rd Ed"
 *                By Sen M. Kuo, Bob H. Lee, and Wenshun Tian
 *                Publisher: John Wiley and Sons, Ltd
 */

-stack    0x2000      /* Primary stack size   */
-sysstack 0x1000      /* Secondary stack size */
-heap     0x2000      /* Heap area size       */

-c                    /* Use C linking conventions: auto-init vars at runtime */
-u _Reset             /* Force load of reset interrupt handler                */

MEMORY
{
    MMR     (RW)  : origin = 0000000h length = 0000c0h  /* MMRs */
    DARAM   (RW)  : origin = 00000c0h length = 00ff40h  /* On-chip DARAM */
    SARAM   (RW)  : origin = 0030000h length = 01e000h  /* On-chip SARAM */

    SAROM_0 (RX)  : origin = 0fe0000h length = 008000h 	/* On-chip ROM 0 */
    SAROM_1 (RX)  : origin = 0fe8000h length = 008000h 	/* On-chip ROM 1 */
    SAROM_2 (RX)  : origin = 0ff0000h length = 008000h 	/* On-chip ROM 2 */
    SAROM_3 (RX)  : origin = 0ff8000h length = 008000h 	/* On-chip ROM 3 */
}


SECTIONS
{
    vectors (NOLOAD)
    .bss        : > DARAM   /* Fill = 0 */
    vector      : > DARAM   ALIGN = 256 
    .stack      : > DARAM  
    .sysstack   : > DARAM  
    .sysmem     : > DARAM 
    .text       : > SARAM  
    .data       : > DARAM
    .cinit      : > DARAM
    .const      : > DARAM
    .cio        : > DARAM
    .usect      : > DARAM
    .switch     : > DARAM 
}
