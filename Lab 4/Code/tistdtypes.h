/*
* tistdtypes.h
*
*  Created on: Mar 17, 2012
*      Author: BLEE, modified
*
*  Description: This C header file defines the data types used
*
*  For the book "Real Time Digital Signal Processing:
*                Fundamentals, Implementation and Application, 3rd Ed"
*                By Sen M. Kuo, Bob H. Lee, and Wenshun Tian
*                Publisher: John Wiley and Sons, Ltd
*
*/

/*****************************************************\
 *  Copyright 2003, Texas Instruments Incorporated.  *
 *  All rights reserved.                             *
 *  Restricted rights to use, duplicate or disclose  *
 *  this   code   are  granted   through  contract.  *
\*****************************************************/

/* Standard Types File: [last revised: 2004-01-14] */

#ifndef _TISTDTYPES_H_
#define _TISTDTYPES_H_

/*
    The purpose of this header file is to consolidate all the primitive "C"
    data types into one file. This file is expected to be included in the
    basic types file exported by other software components, for example CSL.
 */

#ifndef _TI_STD_TYPES
#define _TI_STD_TYPES

#ifndef TRUE

typedef int		bool;
#define TRUE		((bool) 1)
#define FALSE		((bool) 0)

#endif

// typedef int             int;
typedef unsigned int    uint;
// typedef char            Char;
// typedef char *          String;
// typedef void *          Ptr;
// typedef unsigned short	bool;

typedef unsigned long	uint32;
typedef unsigned short	uint16;
typedef unsigned char	uint8;

 /* Signed integer definitions (32bit, 16bit, 8bit) follow... */
typedef long		int32;
typedef short		int16;
typedef char		int8;

#endif /* _TI_STD_TYPES */


#endif /* _TISTDTYPES_H_ */

