/*
************************************************************
* Python-like Compiler
* Code version: Winter, 2024
* Author: Colin Tapp / Henry Forget 
************************************************************
_______________________________________________________
|                     .@@@@=                          |
|                  %*#.    -% +-                      |
|            -.-@%+        .%.   .#.                  |
|                         #         *                 |
|                       :.           :+               |
|                       -             -.              |
|                      @               *              |
|                      =               -              |
|                     .                .+             |
|                     *#-              .#             |
|                   +    ::          +-   @           |
|                   %     #         +.    .=          |
|                   +                     -           |
|                    -=                  +            |
|                       #             #               |
|                        #           +                |
|                           @#   %@ :-                |
|                         +         +                 |
|                          #       +                  |
|                           =:   --                   |
|                             =@+                     |
|                                                     |
|/$$$$$$/$$$$   /$$$$$$  /$$   /$$  /$$$$$$$  /$$$$$$ |
| $$_  $$_  $$ /$$__  $$| $$  | $$ /$$_____/ /$$__  $$|
| $$ \ $$ \ $$| $$  \ $$| $$  | $$|  $$$$$$ | $$$$$$$$|
| $$ | $$ | $$| $$  | $$| $$  | $$ \____  $$| $$_____/|
| $$ | $$ | $$|  $$$$$$/|  $$$$$$/ /$$$$$$$/|  $$$$$$$|
|__/_|__/_|__/_\______/__\______/_|_______/__\_______/|
*/

/*
************************************************************
* File name: Reader.h
* Compiler: MS Visual Studio 2022
* Date: 01/02/24
* Purpose: This file is the main header for Reader (.h)
* Function list:
*    readerCreate, readerAddChar, readerClear, readerFree,
*    readerIsFull, readerIsEmpty, readerSetMark, readerPrint,
*    readerLoad, readerRecover, readerRetract, readerRestore,
*    readerGetChar, readerGetContent, readerGetPosRead,
*    readerGetPosWrte, readerGetPosMark, readerGetSize,
*    readerGetInc, readerGetMode, readerGetFlags,
*    readerPrintStat, readerNumErrors
************************************************************
*/

#ifndef COMPILERS_H_
#include "Compilers.h"
#endif

#ifndef READER_H_
#define READER_H_

/* TIP: Do not change pragmas, unless necessary .......................................*/
/*#pragma warning(1:4001) *//*to enforce C89 type comments  - to make //comments an warning */
/*#pragma warning(error:4001)*//* to enforce C89 comments - to make // comments an error */

/* standard header files */
#include <stdio.h>  /* standard input/output */
#include <malloc.h> /* for dynamic memory allocation*/
#include <limits.h> /* implementation-defined data type ranges and limits */

/* CONSTANTS DEFINITION: GENERAL (NOT LANGUAGE DEPENDENT) .................................. */

/* Modes (used to create buffer reader) */
enum READER_MODE {
	MODE_FIXED = 'f', /* Fixed mode (constant size) (f = 102)*/
	MODE_ADDIT = 'a', /* Additive mode (constant increment to be added) (a = 97)*/
	MODE_MULTI = 'm'  /* Multiplicative mode (constant increment to be multiplied) (m = 109)*/
};

/* Constants about controls (not need to change) */
#define READER_ERROR		(-1)						/* General error message */
#define READER_TERMINATOR	'\0'						/* General EOF */

/* CONSTANTS DEFINITION: PREFIXED BY LANGUAGE NAME (MOUSE) .................................. */

/* You should add your own constant definitions here */
#define READER_MAX_SIZE	INT_MAX-1	/* maximum capacity */ 

#define READER_DEFAULT_SIZE			250		/* default initial buffer reader capacity */
#define READER_DEFAULT_INCREMENT	10		/* default increment factor */

/* Add your bit-masks constant definitions here - Defined for MOUSE */
/* BITS                                (7654.3210) */
#define READER_DEFAULT_FLAG 0x00	/* (0000.0000)_2 = (000)_10 */
#define READER_END_FLAG 0x08  		/* (0000.1000)_2 = (008)_10 */
#define READER_REL_FLAG 0x04  		/* (0000.0100)_2 = (004)_10 */
#define READER_EMP_FLAG 0x02  		/* (0000.0010)_2 = (002)_10 */
#define READER_FUL_FLAG 0x01		/* (0000.0001)_2 = (001)_10 */

#define NCHAR				128			/* Chars from 0 to 127 */

#define CHARSEOF			(-1)		/* EOF Code for Reader */

/* STRUCTURES DEFINITION: SUFIXED BY LANGUAGE NAME (MOUSE) .................................. */

/* Offset declaration */
typedef struct position {
	mouse_int mark;			/* the offset to the mark position (in chars) */
	mouse_int read;			/* the offset to the get a char position (in chars) */
	mouse_int wrte;			/* the offset to the add chars (in chars) */
} Position;

/* Buffer structure */
typedef struct bufferReader {
	mouse_str		content;			/* pointer to the beginning of character array (character buffer) */
	mouse_int		size;				/* current dynamic memory size (in bytes) allocated to character buffer */
	mouse_int		increment;			/* character array increment factor */
	mouse_int		mode;				/* operational mode indicator */
	mouse_byte		flags;				/* contains character array reallocation flag and end-of-buffer flag */
	Position		position;			/* Offset / position field */
	mouse_int		histogram[NCHAR];	/* Statistics of chars */
	mouse_int		numReaderErrors;	/* Number of errors from Reader */
} Buffer, *BufferPointer;

/* FUNCTIONS DECLARATION:  .................................. */
/* General Operations */
BufferPointer	readerCreate		(mouse_int, mouse_int, mouse_int);
BufferPointer	readerAddChar		(BufferPointer const, mouse_char);
mouse_boln		readerClear		    (BufferPointer const);
mouse_boln		readerFree		    (BufferPointer const);
mouse_boln		readerIsFull		(BufferPointer const);
mouse_boln		readerIsEmpty		(BufferPointer const);
mouse_boln		readerSetMark		(BufferPointer const, mouse_int);
mouse_int		readerPrint		    (BufferPointer const);
mouse_int		readerLoad			(BufferPointer const, FILE* const);
mouse_boln		readerRecover		(BufferPointer const);
mouse_boln		readerRetract		(BufferPointer const);
mouse_boln		readerRestore		(BufferPointer const);
/* Getters */
mouse_char		readerGetChar		(BufferPointer const);
mouse_str		readerGetContent	(BufferPointer const, mouse_int);
mouse_int		readerGetPosRead	(BufferPointer const);
mouse_int		readerGetPosWrte	(BufferPointer const);
mouse_int		readerGetPosMark	(BufferPointer const);
mouse_int		readerGetSize		(BufferPointer const);
mouse_int		readerGetInc		(BufferPointer const);
mouse_int		readerGetMode		(BufferPointer const);
mouse_byte		readerGetFlags		(BufferPointer const);
mouse_None		readerPrintStat		(BufferPointer const);
mouse_int		readerNumErrors		(BufferPointer const);

#endif
