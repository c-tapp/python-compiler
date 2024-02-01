/*
************************************************************
* COMPILERS COURSE - Algonquin College
* Code version: Winter, 2024
* Author: Colin Tapp 040774946 / Henry Forget 041023812
* Professors: Paulo Sousa
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
* File name: Reader.c
* Compiler: MS Visual Studio 2022
* Course: CST 8152 – Compilers, Lab Section: 011
* Assignment: A12
* Date: 01/02/24
* Professor: Paulo Sousa
* Purpose: This file is the main code for Reader (.c)
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
#include "Reader.h"
#endif

/*
***********************************************************
* Function name: readerCreate
* Purpose: Creates the buffer reader according to capacity, increment
	factor and operational mode ('f', 'a', 'm')
* Called functions: calloc(), malloc()
* Parameters:
*   size = initial capacity
*   increment = increment factor
*   mode = operational mode
* Return value: bPointer (pointer to reader)
* Algorithm: Allocation of memory according to initial (default) values.
*************************************************************
*/
BufferPointer readerCreate(mouse_int size, mouse_int increment, mouse_int mode) {
	BufferPointer readerPointer;
	// Defensive programming
	if (size <= 0) {
		size = READER_DEFAULT_SIZE;
		increment = READER_DEFAULT_INCREMENT;
	}
	if (increment <= 0)
		mode = MODE_FIXED;
	if ((mode != MODE_FIXED) && (mode != MODE_ADDIT) && (mode != MODE_MULTI)) // Checking if mode is one of the valid chars (f = 102, a = 97, m = 109)
		return NULL;

	// Adjust the values according to parameters
	readerPointer = (BufferPointer)calloc(1, sizeof(Buffer));
	if (!readerPointer)
		return NULL;
	readerPointer->content = (mouse_str)malloc(size);

	// Initialize histogram
	for (int i = 0; i < NCHAR; i++)
		readerPointer->histogram[i] = 0;

	readerPointer->size = size;
	readerPointer->increment = increment;
	readerPointer->mode = mode;

	// Initialize flags as EMP
	readerPointer->flags = READER_DEFAULT_FLAG | READER_EMP_FLAG;

	if (readerPointer->content)
		readerPointer->content[0] = READER_TERMINATOR;
	readerPointer->position.wrte = 0;
	readerPointer->position.mark = 0;
	readerPointer->position.read = 0;
	return readerPointer;
}


/*
***********************************************************
* Function name: readerAddChar
* Purpose: Adds a char to buffer reader
* Called functions: sizeof(), realloc()
* Parameters:
*   readerPointer = pointer to Buffer Reader
*   ch = char to be added
* Return value:
*	readerPointer (pointer to Buffer Reader)
* Algorithm: include a char in the reader and
*	increase size if needed
*************************************************************
*/
BufferPointer readerAddChar(BufferPointer const readerPointer, mouse_char ch) {
	mouse_str tempReader = NULL;
	mouse_int newSize = 0;

	// Defensive programming
	if (!readerPointer)
		return NULL;
	if ((ch < 0) || (ch > 128))
		return NULL;

	readerPointer->flags = readerPointer->flags & (!READER_REL_FLAG); // Reset REL
	if (readerPointer->position.wrte * (mouse_int)sizeof(mouse_char) < readerPointer->size) {
		// Included at the bottom
	}
	else {
		readerPointer->flags = readerPointer->flags & (!READER_FUL_FLAG); // Reset FULL

		switch (readerPointer->mode) {
		case MODE_FIXED:
			readerPointer->flags = readerPointer->flags | (READER_FUL_FLAG); // Reader is full
			return NULL;
		case MODE_ADDIT:
			newSize = readerPointer->size + readerPointer->increment;
			// Defensive programming below
			break;
		case MODE_MULTI:
			newSize = readerPointer->size * readerPointer->increment;
			// Defensive programming below
			break;
		default:
			return NULL;
		}

		// Defensive Programming
		if ((newSize < 0) || (newSize > READER_MAX_SIZE)) { // newSize is invalid
			readerPointer->flags = readerPointer->flags | (READER_FUL_FLAG); // Reader is full
			return NULL;
		}

		// New reader allocation
		tempReader = (mouse_str)realloc(readerPointer->content, newSize);

		// Defensive programming
		if (!tempReader)
			return NULL;

		// Check Relocation
		if (tempReader != readerPointer->content) { 
			readerPointer->flags = readerPointer->flags | (!READER_REL_FLAG);
		}

		// Updating ReaderPointer
		readerPointer->content = tempReader;
		readerPointer->size = newSize;
	}
	readerPointer->content[readerPointer->position.wrte++] = ch; // Add the char
	readerPointer->flags = readerPointer->flags & (!READER_EMP_FLAG); // Reset EMP
	readerPointer->histogram[ch]++; // Update Histogram
	return readerPointer;
}

/*
***********************************************************
* Function name: readerClear
* Purpose: Clears the buffer reader
* Parameters:
*   readerPointer = pointer to Buffer Reader
* Return value:
*	Boolean value about operation success
* Algorithm: Reset offsets and flags
*************************************************************
*/
mouse_boln readerClear(BufferPointer const readerPointer) {
	// Defensive programming
	if (!readerPointer) 
		return mouse_False; 
	// Reset offsets
	readerPointer->position.wrte = readerPointer->position.mark = readerPointer->position.read = 0;
	// Reset flags
	readerPointer->flags = READER_DEFAULT_FLAG | READER_EMP_FLAG;
	
	return mouse_True;
}

/*
***********************************************************
* Function name: readerFree
* Purpose: Releases the buffer address
* Called functions: free()
* Parameters:
*   readerPointer = pointer to Buffer Reader
* Return value:
*	Boolean value about operation success
* Algorithm: free the memory taken by readerPointer
*************************************************************
*/
mouse_boln readerFree(BufferPointer const readerPointer) {
	// Defensive programming
	if (!readerPointer) 
		return mouse_False; 
	// Free pointers
	if (readerPointer->content) 
		free(readerPointer->content);
	free(readerPointer);
	
	return mouse_True;
}

/*
***********************************************************
* Function name: readerIsFull
* Purpose: Checks if buffer reader is full
* Parameters:
*   readerPointer = pointer to Buffer Reader
* Return value:
*	Boolean value about operation success
* Algorithm: Checks full flag
*************************************************************
*/
mouse_boln readerIsFull(BufferPointer const readerPointer) {
	// Defensive programming
	if (!readerPointer)
		return mouse_False;
	// Check flag if buffer is full
	if (readerPointer->flags & READER_FUL_FLAG)
		return mouse_True;
	return mouse_False;
}


/*
***********************************************************
* Function name: readerIsEmpty
* Purpose: Checks if buffer reader is empty.
* Parameters:
*   readerPointer = pointer to Buffer Reader
* Return value:
*	Boolean value about operation success
* Algorithm: Checks empty flag
*************************************************************
*/
mouse_boln readerIsEmpty(BufferPointer const readerPointer) {
	// Defensive programming
	if (!readerPointer)
		return mouse_False;
	// Check flag if buffer is empty
	if (readerPointer->flags & READER_EMP_FLAG)
		return mouse_True;
	return mouse_False;
}

/*
***********************************************************
* Function name: readerSetMark
* Purpose: Adjust the position of mark in the buffer
* Parameters:
*   readerPointer = pointer to Buffer Reader
*   mark = mark position for char
* Return value:
*	Boolean value about operation success
* Algorithm: Updates mark value
*************************************************************
*/
mouse_boln readerSetMark(BufferPointer const readerPointer, mouse_int mark) {
	// Defensive programming
	if (!readerPointer) 
		return mouse_False;
	if (mark < 0 || mark > readerPointer->position.wrte)
		return mouse_False;
	// Update the mark position offset with the new value
	readerPointer->position.mark = mark;
	return mouse_True;
}


/*
***********************************************************
* Function name: readerPrint
* Purpose: Prints the string in the buffer.
* Called functions: readerGetChar()
* Parameters:
*   readerPointer = pointer to Buffer Reader
* Return value:
*	Number of chars printed.
* TO_DO:
*   - Use defensive programming
*	- Check boundary conditions
*	- Adjust for your LANGUAGE.
* Algorithm: print the content of readerPointer->content
*************************************************************
*/
mouse_int readerPrint(BufferPointer const readerPointer) {
	mouse_int count = 0;
	mouse_char c;
	/* TO_DO: Defensive programming (including invalid chars) */
	c = readerGetChar(readerPointer);
	/* TO_DO: Check flag if buffer EOB has achieved */
	while (count < readerPointer->position.wrte) {
		count++;
		printf("%c", c);
		c = readerGetChar(readerPointer);
	}
	return count;
}

/*
***********************************************************
* Function name: readerLoad
* Purpose: Loads the string in the buffer with the content of
	an specific file.
* Called functions: fgetc(), readerAddChar(), ungetc()
* Parameters:
*   readerPointer = pointer to Buffer Reader
*   fileDescriptor = pointer to file descriptor
* Return value:
*	Number of chars read and put in buffer.
* Algorithm: reads an input file and loads the content 
*	into readerPointer
*************************************************************
*/
mouse_int readerLoad(BufferPointer const readerPointer, FILE* const fileDescriptor) {
	mouse_int size = 0;
	mouse_char c;
	// Defensive Programming
	if (!readerPointer) 
		return CHARSEOF;			 
	if (!fileDescriptor) 
		return CHARSEOF;

	c = (mouse_char)fgetc(fileDescriptor);
	while (!feof(fileDescriptor)) {
		if (!readerAddChar(readerPointer, c)) {
			ungetc(c, fileDescriptor);
			return READER_ERROR;
		}
		c = (char)fgetc(fileDescriptor);
		size++;
	}
	return size;
}


/*
***********************************************************
* Function name: readerRecover
* Purpose: Rewinds the buffer.
* Parameters:
*   readerPointer = pointer to Buffer Reader
* Return value
*	Boolean value about operation success
* Algorithm: resets read and mark position
*************************************************************
*/
mouse_boln readerRecover(BufferPointer const readerPointer) {
	// Defensive programming
	if (!readerPointer)
		return mouse_False;
	// Recover positions
	readerPointer->position.read = 0;
	readerPointer->position.mark = 0;
	return mouse_True;
}


/*
***********************************************************
* Function name: readerRetract
* Purpose: Retracts the buffer to put back the char in buffer.
* Parameters:
*   readerPointer = pointer to Buffer Reader
* Return value:
*	Boolean value about operation success
* Algorithm: unread one char from the reader
*************************************************************
*/
mouse_boln readerRetract(BufferPointer const readerPointer) {
	// Defensive programming
    if (!readerPointer)
        return mouse_False;
	// If read is positive, decrement 
    if (readerPointer->position.read > 0)
        readerPointer->position.read--;
	return mouse_True;
}


/*
***********************************************************
* Function name: readerRestore
* Purpose: Resets the buffer.
* Parameters:
*   readerPointer = pointer to Buffer Reader
* Return value:
*	Boolean value about operation success
* Algorithm: sets read to mark's offset
*************************************************************
*/
mouse_boln readerRestore(BufferPointer const readerPointer) {
	 // Defensive programming
    if (!readerPointer)
        return mouse_False;
    // Restore read offset to the value of the current mark offset
	readerPointer->position.read = readerPointer->position.mark;
	return mouse_True;
}


/*
***********************************************************
* Function name: readerGetChar
* Purpose: Returns the char in the getC position.
* Parameters:
*   readerPointer = pointer to Buffer Reader
* Return value:
*	Char in the getC position.
* Algorithm: reads the reader
* TO_DO:
*   - Use defensive programming
*	- Check boundary conditions
*	- Adjust for your LANGUAGE.
*************************************************************
*/
mouse_char readerGetChar(BufferPointer const readerPointer) {
	/* TO_DO: Defensive programming */
	/* TO_DO: Check condition to read/wrte */
	/* TO_DO: Set EOB flag */
	/* TO_DO: Reset EOB flag */
	if (readerPointer->position.wrte>0)
		return readerPointer->content[readerPointer->position.read++];
	return READER_TERMINATOR;
}


/*
***********************************************************
* Function name: readerGetContent
* Purpose: Returns the pointer to String.
* Parameters:
*   readerPointer = pointer to Buffer Reader
*   pos = position to get the pointer
* Return value:
*	Position of string char.
* Algorithm: returns a pointer to the location of the character reader
*************************************************************
*/
mouse_str readerGetContent(BufferPointer const readerPointer, mouse_int pos) {
	// Defensive programming
	if (!readerPointer) 
		return NULL;
	if ((pos < 0) || (pos > readerPointer->position.wrte)) 
		return NULL;
	return readerPointer->content + pos;;
}



/*
***********************************************************
* Function name: readerGetPosRead
* Purpose: Returns the value of getCPosition.
* Parameters:
*   readerPointer = pointer to Buffer Reader
* Return value:
*	The read position offset.
* Algorithm: gets read
*************************************************************
*/
mouse_int readerGetPosRead(BufferPointer const readerPointer) {
	// Defensive programming
	if (!readerPointer)
		return CHARSEOF;			
	return readerPointer->position.read;
}


/*
***********************************************************
* Function name: readerGetPosWrte
* Purpose: Returns the position of char to be added
* Parameters:
*   readerPointer = pointer to Buffer Reader
* Return value:
*	Write position
* Algorithm: gets write
*************************************************************
*/
mouse_int readerGetPosWrte(BufferPointer const readerPointer) {
	// Defensive programming
	if (!readerPointer)
		return CHARSEOF;
	return readerPointer->position.wrte;
}


/*
***********************************************************
* Function name: readerGetPosMark
* Purpose: Returns the position of mark in the buffer
* Parameters:
*   readerPointer = pointer to Buffer Reader
* Return value:
*	Mark position.
* Algorithm: gets mark
*************************************************************
*/
mouse_int readerGetPosMark(BufferPointer const readerPointer) {
	// Defensive programming
	if (!readerPointer)
		return CHARSEOF;
	return readerPointer->position.mark;
}


/*
***********************************************************
* Function name: readerGetSize
* Purpose: Returns the current buffer capacity
* Parameters:
*   readerPointer = pointer to Buffer Reader
* Return value:
*	Size of buffer.
* Algorithm: gets size
*************************************************************
*/
mouse_int readerGetSize(BufferPointer const readerPointer) {
	// Defensive programming
	if (!readerPointer)
		return CHARSEOF;
	return readerPointer->size;
}

/*
***********************************************************
* Function name: readerGetInc
* Purpose: Returns the buffer increment.
* Parameters:
*   readerPointer = pointer to Buffer Reader
* Return value:
*	The Buffer increment
* Algorithm: gets increment
*************************************************************
*/
mouse_int readerGetInc(BufferPointer const readerPointer) {
	// Defensive programming
	if (!readerPointer)
		return CHARSEOF;
	return readerPointer->increment;
}

/*
***********************************************************
* Function name: readerGetMode
* Purpose: Returns the operational mode
* Parameters:
*   readerPointer = pointer to Buffer Reader
* Return value:
*	Operational mode.
* Algorithm: gets mode
*************************************************************
*/
mouse_int readerGetMode(BufferPointer const readerPointer) {
	// Defensive programming
	if (!readerPointer)
		return CHARSEOF;
	return readerPointer->mode;
}


/*
***********************************************************
* Function name: readerGetFlags
* Purpose: Returns the entire flags of Buffer.
* Parameters:
*   readerPointer = pointer to Buffer Reader
* Return value:
*	Flags from Buffer.
* Algorithm: gets flags
*************************************************************
*/
mouse_byte readerGetFlags(BufferPointer const readerPointer) {
	// Defensive programming
	if (!readerPointer)
		return CHARSEOF;
	return readerPointer->flags;
}



/*
***********************************************************
* Function name: readerPrintStat
* Purpose: Shows the char statistic.
* Called functions: printf()
* Parameters:
*   readerPointer = pointer to Buffer Reader
* Return value: (Void)
* Algorithm: prints the histogram
*************************************************************
*/
mouse_None readerPrintStat(BufferPointer const readerPointer) {
	// Defensive programming
	if (readerPointer) 
		// Printing data
		for (mouse_int i = 0; i < NCHAR; i++) 
			// Ensuring there are more then 0 of the character
			if (readerPointer->histogram[i] > 0) 
				if (i=='\n')
					printf("[\\n]=%d\n", readerPointer->histogram[i]); // Printing the newline character
				else
					printf("[%c]=%d\n", i, readerPointer->histogram[i]); // Printing every other character
}

/*
***********************************************************
* Function name: readerNumErrors
* Purpose: Returns the number of errors found.
* Parameters:
*   readerPointer = pointer to Buffer Reader
* Return value:
*	Number of errors.
* Algorithm: gets number of errors
*************************************************************
*/
mouse_int readerNumErrors(BufferPointer const readerPointer) {
	// Defensive programming
	if (!readerPointer)
		return CHARSEOF;
	return readerPointer->numReaderErrors;
}
