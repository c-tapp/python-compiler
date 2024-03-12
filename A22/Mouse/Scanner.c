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
* File name: Scanner.c
* Compiler: MS Visual Studio 2022
* Course: CST 8152 – Compilers, Lab Section: 011
* Assignment: A22
* Date: 03/11/24
* Professor: Paulo Sousa
* Purpose: This file contains all functionalities from Scanner.
* Function list: startScanner(), nextClass(), nextState(),
*	printScannerData(), tokenizer(), funcSL(), funcIL(),
*	funcID(), funcCMT(), funcKEY(), funcErr(), TO_DO Add more if needed
************************************************************
*/

#define _CRT_SECURE_NO_WARNINGS

#include <stdio.h>   /* standard input / output */
#include <ctype.h>   /* conversion functions */
#include <stdlib.h>  /* standard library functions and constants */
#include <string.h>  /* string functions */
#include <limits.h>  /* integer types constants */
#include <float.h>   /* floating-point types constants */

/* #define NDEBUG to suppress assert() call */
#include <assert.h>  /* assert() prototype */

/* project header files */

#ifndef COMPILERS_H_
#include "Compilers.h"
#endif

#ifndef BUFFER_H_
#include "Reader.h"
#endif

#ifndef SCANNER_H_
#include "Scanner.h"
#endif

/* Global objects - variables */
/* This buffer is used as a repository for string literals. */
extern BufferPointer stringLiteralTable;	/* String literal table */
mouse_int line;								/* Current line number of the source code */
extern mouse_int errorNumber;				/* Defined in platy_st.c - run-time error number */

extern mouse_int stateType[NUM_STATES];
extern mouse_str keywordTable[KWT_SIZE];
extern PTR_ACCFUN finalStateTable[NUM_STATES];
extern mouse_int transitionTable[NUM_STATES][CHAR_CLASSES];

/* Local(file) global objects - variables */
static BufferPointer lexemeBuffer;			/* Pointer to temporary lexeme buffer */
static BufferPointer sourceBuffer;			/* Pointer to input source buffer */

/*
***********************************************************
* Function name: startScanner
* Purpose: This function initializes the scanner 
* Called functions: readerRecover(), readerClear()
* Parameters:
*   psc_buf = Pointer to the buffer
* Return value: EXIT_SUCCESS (Let calling function know it's done)
* Algorithm: Initilases the histogram and reader
*************************************************************
*/
mouse_int startScanner(BufferPointer psc_buf) {
	/* Start histogram */
	for (mouse_int i=0; i<NUM_TOKENS;i++)
		scData.scanHistogram[i] = 0;
	/* Basic scanner initialization */
	/* in case the buffer has been read previously  */
	readerRecover(psc_buf);
	readerClear(stringLiteralTable);
	line = 1;
	sourceBuffer = psc_buf;
	return EXIT_SUCCESS; 
}

/*
***********************************************************
* Function name: tokenizer
* Purpose: responsible to classify a char (or sequence of chars)
* Called functions: readerGetChar(), nextState(), readerGetPosRead(), 
*	readerSetMark(), readerRetract(), readerCreate(), fprintf(),
*	exit(), readerRestore(), readerAddChar(), readerGetContent(),
*	TO_DO: Add any more as needed
* Return value: currentToken (The token being worked on with updated parameters)
* Algorithm: detects a specific sequence and recognises any patterns then calls 
*	the appropriate function
*************************************************************
*/
Token tokenizer(mouse_None) {

	Token currentToken = { 0 }; /* token to return after pattern recognition. Set all structure members to 0 */
	mouse_char c;				/* input symbol */
	mouse_int state = 0;		/* initial state of the FSM */
	mouse_int lexStart;			/* start offset of a lexeme in the input char buffer (array) */
	mouse_int lexEnd;			/* end offset of a lexeme in the input char buffer (array)*/

	mouse_int lexLength;		/* token length */
	mouse_int i;				/* counter */
	/*
	mouse_char newc;			// new char
	*/

	while (1) { /* endless loop broken by token returns it will generate a warning */
		c = readerGetChar(sourceBuffer);

		/* ------------------------------------------------------------------------
			Part 1: Implementation of token driven scanner.
			Every token is possessed by its own dedicated code
			-----------------------------------------------------------------------
		*/
		switch (c) {

		/* Cases for spaces */
		case ' ':
		case '\f':
			break;

		/* Cases for format */
		case '\n':
			line++;
			break;
		case '\t': // TO_DO Tracking tab depth
			currentToken.code = TAB_T;
			currentToken.attribute.indentationCurrentPos++;
			scData.scanHistogram[currentToken.code]++;
			return currentToken;
		/* Cases for symbols */
		case '(':
			currentToken.code = LPR_T;
			scData.scanHistogram[currentToken.code]++;
			return currentToken;
		case ')':
			currentToken.code = RPR_T;
			scData.scanHistogram[currentToken.code]++;
			return currentToken;
		case ':':
			currentToken.code = COL_T;
			scData.scanHistogram[currentToken.code]++;
			return currentToken;
		
		/* Cases for Arithmetic Operators */
		case '+':
			currentToken.code = OP_ADD;
			scData.scanHistogram[currentToken.code]++;
			return currentToken;
		case '-':
			currentToken.code = OP_SUB;
			scData.scanHistogram[currentToken.code]++;
			return currentToken;
		case '*':
			currentToken.code = OP_MUL;
			scData.scanHistogram[currentToken.code]++;
			return currentToken;
		case '/':
			currentToken.code = OP_DIV;
			scData.scanHistogram[currentToken.code]++;
			return currentToken;
		case '%':
			currentToken.code = OP_MOD;
			scData.scanHistogram[currentToken.code]++;
			return currentToken;
		case '^': 
			currentToken.code = OP_EXP;
			scData.scanHistogram[currentToken.code]++;
			return currentToken;
		/* Cases for Relational Operators */
		case '=':
			currentToken.code = OP_EQ;
			scData.scanHistogram[currentToken.code]++;
			return currentToken;
		case '!=': // TO_DO this is two charaters
			currentToken.code = OP_NE;
			scData.scanHistogram[currentToken.code]++;
			return currentToken;
		case '>':
			currentToken.code = OP_GT;
			scData.scanHistogram[currentToken.code]++;
			return currentToken;
		case '<':
			currentToken.code = OP_LT;
			scData.scanHistogram[currentToken.code]++;
			return currentToken;
		/* Cases for Logical Operators */
		case '&&': // TO_DO this is two charaters
			currentToken.code = OP_AND;
			scData.scanHistogram[currentToken.code]++;
			return currentToken;
		case '||': // TO_DO this is two charaters
			currentToken.code = OP_OR;
			scData.scanHistogram[currentToken.code]++;
			return currentToken;
		case '!':
			currentToken.code = OP_NOT;
			scData.scanHistogram[currentToken.code]++;
			return currentToken;
		case '==': // TO_DO this is two charaters
			currentToken.code = OP_IS;
			scData.scanHistogram[currentToken.code]++;
			return currentToken;
		/* Cases for END OF FILE */
		case CHARSEOF0:
			currentToken.code = SEOF_T;
			scData.scanHistogram[currentToken.code]++;
			currentToken.attribute.seofType = SEOF_0;
			return currentToken;
		case CHARSEOF255:
			currentToken.code = SEOF_T;
			scData.scanHistogram[currentToken.code]++;
			currentToken.attribute.seofType = SEOF_255;
			return currentToken;

		/* ------------------------------------------------------------------------
			Part 2: Implementation of Finite State Machine (DFA) or Transition Table driven Scanner
			Note: Part 2 must follow Part 1 to catch the illegal symbols
			-----------------------------------------------------------------------
		*/

		/* TO_DO: Adjust / check the logic for your language */

		default: // general case
			state = nextState(state, c);
			lexStart = readerGetPosRead(sourceBuffer) - 1;
			readerSetMark(sourceBuffer, lexStart);
			int pos = 0;
			while (stateType[state] == NOFS) {
				c = readerGetChar(sourceBuffer);
				state = nextState(state, c);
				pos++;
			}
			if (stateType[state] == FSWR)
				readerRetract(sourceBuffer);
			lexEnd = readerGetPosRead(sourceBuffer);
			lexLength = lexEnd - lexStart;
			lexemeBuffer = readerCreate((mouse_int)lexLength + 2, 0, MODE_FIXED);
			if (!lexemeBuffer) {
				fprintf(stderr, "Scanner error: Can not create buffer\n");
				exit(1);
			}
			readerRestore(sourceBuffer);
			for (i = 0; i < lexLength; i++)
				readerAddChar(lexemeBuffer, readerGetChar(sourceBuffer));
			readerAddChar(lexemeBuffer, READER_TERMINATOR);
			currentToken = (*finalStateTable[state])(readerGetContent(lexemeBuffer, 0));
			readerRestore(lexemeBuffer); //xxx
			return currentToken;
		} // switch

	} //while

} // tokenizer

/*
***********************************************************
* Function name: nextState
* Purpose: Reads a token char by char to find its class
* Called functions: nextClass(), printf(), exit()
* Parameters:
*   state = The current state
*	c = the current char
* Return value: 
* Algorithm: Reads a token char by char and uses the 
*	transitionTable matrix to validate its class
*************************************************************
*/
mouse_int nextState(mouse_int state, mouse_char c) {
	mouse_int col;
	mouse_int next;
	col = nextClass(c);
	next = transitionTable[state][col];
	if (DEBUG)
		printf("Input symbol: %c Row: %d Column: %d Next: %d \n", c, state, col, next);
	assert(next != FS);
	if (DEBUG)
		if (next == FS) {
			printf("Scanner Error: Illegal state:\n");
			printf("Input symbol: %c Row: %d Column: %d\n", c, state, col);
			exit(1);
		}
	return next;
	/*
	************************************************************
	* Get Next State
		The assert(int test) macro can be used to add run-time diagnostic to programs
		and to "defend" from producing unexpected results.
		- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
		(*) assert() is a macro that expands to an if statement;
		if test evaluates to false (zero) , assert aborts the program
		(by calling abort()) and sends the following message on stderr:
		(*) Assertion failed: test, file filename, line linenum.
		The filename and linenum listed in the message are the source file name
		and line number where the assert macro appears.
		- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
		If you place the #define NDEBUG directive ("no debugging")
		in the source code before the #include <assert.h> directive,
		the effect is to comment out the assert statement.
		- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
		The other way to include diagnostics in a program is to use
		conditional preprocessing as shown bellow. It allows the programmer
		to send more details describing the run-time problem.
		Once the program is tested thoroughly #define DEBUG is commented out
		or #undef DEBUG is used - see the top of the file.
	***********************************************************
	*/
}

/*
***********************************************************
* Function name: nextClass()
* Purpose: return next column in TT
* Called functions: isalpha(), isdigit()
* Parameters:
*	c = The char being tested
* Return value: val (which column of the TT the char is from)
* Algorithm: Checks against each CHARCOL entry 
*************************************************************
*/
mouse_int nextClass(mouse_char c) {
	mouse_int val = -1;
	switch (c) {
	case CHRCOL0:
		val = 0;
		break;
	case CHRCOL1:
		val = 1;
		break;
	case CHRCOL3:
		val = 3;
		break;
	case CHRCOL5:
		val = 5;
		break;
	case CHRCOL6:
		val = 6;
		break;
	case CHRCOL7:
		val = 7;
		break;
	case CHRCOL8:
		val = 8;
		break;
	case CHRCOL9:
		val = 9;
		break;
	case CHRCOL10:
		val = 10;
		break;
	case CHRCOL11:
		val = 11;
		break;
	case CHARSEOF0:
	case CHARSEOF255:
		val = 13;
		break;
	default:
		if (isalpha(c))
			val = 4;
		else if (isdigit(c))
			val = 2;
		else
			val = 12;
	}
	return val;
}

/*
***********************************************************
* Function name: funcCMT
* Purpose: Responsible to identify comments
* Called functions: strlen(), readerGetPosWrte()
* Parameters:
*   lexeme = The current lexeme being worked on
* Return value: currentToken (The token with it's updated data)
* Algorithm: Once the TT detects the comment this fucntion double
*	checks that it ends in newline and adds the CMT_T code to the
*	lexeme
*************************************************************
*/
Token funcCMT(mouse_str lexeme) {
	Token currentToken = { 0 };
	mouse_int i = 0, len = (mouse_int)strlen(lexeme);
	currentToken.attribute.contentString = readerGetPosWrte(stringLiteralTable);
	for (i = 1; i < len - 1; i++) {
		if (lexeme[i] == '\n')
			line++;
	}
	currentToken.code = CMT_T;
	scData.scanHistogram[currentToken.code]++;
	return currentToken;
}

/*
***********************************************************
* Function name: funcIL
* Purpose: Responsible to identify integer literals
* Called functions: strlen(), atol()
* Parameters:
*   lexeme = The current lexeme being worked on
* Return value: currentToken (The token with it's updated data)
* Algorithm: TO_DO
*************************************************************
*/
Token funcIL(mouse_str lexeme) {
	/* TO_DO: Adjust the function for IL */
	Token currentToken = { 0 };
	mouse_long tlong;
	if (lexeme[0] != '\0' && strlen(lexeme) > NUM_LEN) {
		currentToken = (*finalStateTable[ESNR])(lexeme);
	}
	else {
		tlong = atol(lexeme);
		if (tlong >= 0 && tlong <= SHRT_MAX) {
			currentToken.code = INL_T;
			scData.scanHistogram[currentToken.code]++;
			currentToken.attribute.intValue = (mouse_int)tlong;
		}
		else {
			currentToken = (*finalStateTable[ESNR])(lexeme);
		}
	}
	return currentToken;
/*
 ************************************************************
 * - It is necessary respect the limit (ex: 2-byte integer in C).
 * - In the case of larger lexemes, error shoul be returned.
 * - Only first ERR_LEN characters are accepted and eventually,
 *   additional three dots (...) should be put in the output.
 ***********************************************************
 */
}

/*
***********************************************************
* Function name: funcID
* Purpose: Responsible to identify the pattern for IDs
* Called functions: strlen(), funcKEY(), strncpy()
* Parameters:
*   lexeme = The current lexeme being worked on
* Return value: currentToken (The token with it's updated data)
* Algorithm: Checks for either ( or : to determine if it's a 
*	method or variable respectively, else check if it's a 
*	keyword
*************************************************************
*/
Token funcID(mouse_str lexeme) {
	Token currentToken = { 0 };
	size_t length = strlen(lexeme);
	mouse_char lastch = lexeme[length - 1];
	mouse_int isID = mouse_False;
	switch (lastch) {
		case MNID_SUF:
			readerRetract(sourceBuffer);
			lexeme[length - 1] = '\0';
			currentToken.code = MNID_T;
			scData.scanHistogram[currentToken.code]++;
			isID = mouse_True;
			break;
		case VRID_SUF:
			readerRetract(sourceBuffer);
			lexeme[length - 1] = '\0';
			currentToken.code = VAR_T;
			scData.scanHistogram[currentToken.code]++;
			isID = mouse_True;
			break;
		default:
			// Test Keyword
			//lexeme[length - 1] = '\0';
			currentToken = funcKEY(lexeme);
			break;
	}
	if (isID == mouse_True) {
		strncpy(currentToken.attribute.idLexeme, lexeme, VID_LEN);
		currentToken.attribute.idLexeme[VID_LEN] = CHARSEOF0;
	}
	return currentToken;
/*
 ************************************************************
 *		Since keywords obey the same pattern, is required to test if
 *		the current lexeme matches with KW from language.
 *	- Remember to respect the limit defined for lexemes (VID_LEN) and
 *	  set the lexeme to the corresponding attribute (vidLexeme).
 *    Remember to end each token with the \0.
 *  - Suggestion: Use "strncpy" function.
 ***********************************************************
 */
}

/*
***********************************************************
* Function name: funcSL
* Purpose: Responsible to identify string literals
* Called functions: strlen(), readerGetPosWrte(),
*	readerAddChar(), strcpy()
* Parameters:
*   lexeme = The current lexeme being worked on
* Return value: currentToken (The token with it's updated data)
* Algorithm:  Once the TT detects the string this fucntion double
*	checks that it has no RTE and adds the STR_T code to the
*	lexeme
*************************************************************
*/
Token funcSL(mouse_str lexeme) {
	Token currentToken = { 0 };
	mouse_int i = 0, len = (mouse_int)strlen(lexeme);
	currentToken.attribute.contentString = readerGetPosWrte(stringLiteralTable);
	for (i = 1; i < len - 1; i++) {
		if (lexeme[i] == '\n')
			line++;
		if (!readerAddChar(stringLiteralTable, lexeme[i])) {
			currentToken.code = RTE_T;
			scData.scanHistogram[currentToken.code]++;
			strcpy(currentToken.attribute.errLexeme, "Run Time Error:");
			errorNumber = RTE_CODE;
			return currentToken;
		}
	}
	if (!readerAddChar(stringLiteralTable, CHARSEOF0)) {
		currentToken.code = RTE_T;
		scData.scanHistogram[currentToken.code]++;
		strcpy(currentToken.attribute.errLexeme, "Run Time Error:");
		errorNumber = RTE_CODE;
		return currentToken;
	}
	currentToken.code = STR_T;
	scData.scanHistogram[currentToken.code]++;
	return currentToken;
/*
************************************************************
 * - The lexeme must be stored in the String Literal Table
 *   (stringLiteralTable). You need to include the literals in
 *   this structure, using offsets. Remember to include \0 to
 *   separate the lexemes. Remember also to incremente the line.
 ***********************************************************
 */
}

/*
***********************************************************
* Function name: funcKEY
* Purpose: Responsible to identify keywords
* Called functions: strcmp(), funcErr()
* Parameters:
*   lexeme = The current lexeme being worked on
* Return value: currentToken (The token with it's updated data)
* Algorithm: Once the funcID detects it's not a method this 
*	fucntion double checks if its a keyword and adds the KW_T 
*	code to the lexeme
*************************************************************
*/
Token funcKEY(mouse_str lexeme) {
	Token currentToken = { 0 };
	mouse_int kwindex = -1, j = 0;
	mouse_int len = (mouse_int)strlen(lexeme);
	//lexeme[len - 1] = '\0';
	for (j = 0; j < KWT_SIZE; j++)
		if (!strcmp(lexeme, &keywordTable[j][0]))
			kwindex = j;
	if (kwindex != -1) {
		if (kwindex == 4) { // True
			currentToken.code = BLN_T;
			scData.scanHistogram[currentToken.code]++;
			currentToken.attribute.codeType = kwindex;
			currentToken.attribute.boolValue = 1;
		}
		else if (kwindex == 5) { // False
			currentToken.code = BLN_T;
			scData.scanHistogram[currentToken.code]++;
			currentToken.attribute.codeType = kwindex;
			currentToken.attribute.boolValue = 0;
		}
		else { // Other Keywords
			currentToken.code = KW_T;
			scData.scanHistogram[currentToken.code]++;
			currentToken.attribute.codeType = kwindex;
		}
	}
	else {
		currentToken = funcErr(lexeme);
	}
	return currentToken;
}

/*
***********************************************************
* Function name: funcErr
* Purpose: Responsible to identify errors
* Called functions: strlen(), strncpy(), strcat(), strcpy()
* Parameters:
*   lexeme = The current lexeme being worked on
* Return value: currentToken (The token with it's updated data)
* Algorithm: If an invalid location in reached in the TT
*	this fucnction is called to deal with that lexeme
*************************************************************
*/
Token funcErr(mouse_str lexeme) {
	Token currentToken = { 0 };
	mouse_int i = 0, len = (mouse_int)strlen(lexeme);
	if (len > ERR_LEN) {
		strncpy(currentToken.attribute.errLexeme, lexeme, ERR_LEN - 3);
		currentToken.attribute.errLexeme[ERR_LEN - 3] = CHARSEOF0;
		strcat(currentToken.attribute.errLexeme, "...");
	}
	else {
		strcpy(currentToken.attribute.errLexeme, lexeme);
	}
	for (i = 0; i < len; i++)
		if (lexeme[i] == '\n')
			line++;
	currentToken.code = ERR_T;
	scData.scanHistogram[currentToken.code]++;
	return currentToken;
/*
************************************************************
 * - This function uses the errLexeme, respecting the limit given
 *   by ERR_LEN. If necessary, use three dots (...) to use the
 *   limit defined. The error lexeme contains line terminators,
 *   so remember to increment line.
 ***********************************************************
 */
}

/*
***********************************************************
* Function name: printToken
* Purpose: prints the token returned by the scanner
* Called functions: printf(), exit(), readerGetContent()
*	TO_DO: Add any more as needed
* Parameters:
*   t = Token in question
* Return value: Prints the token and its info
* Algorithm: Checks the token's code parameter and uses a 
*	case function to determine how to print it
*************************************************************
*/
mouse_None printToken(Token t) {
	extern mouse_str keywordTable[]; /* link to keyword table in */
	switch (t.code) {
	case ERR_T:
		printf("ERR_T\t\t%s\n", t.attribute.errLexeme);
		break;
	case MNID_T:
		printf("MNID_T\t\t%s\n", t.attribute.idLexeme);
		break;
	case INL_T:
		printf("INL_T\t\tTO_DO\n");
		break;
	case STR_T:
		printf("STR_T\t\t%d\t ", (mouse_int)t.attribute.codeType);
		printf("%s\n", readerGetContent(stringLiteralTable, (mouse_int)t.attribute.codeType));
		break;
	case FLT_T:
		printf("FLT_T\t\tTO_DO\n");
		break;
	case BLN_T:
		printf("BLN_T\t\t%d\n", t.attribute.boolValue);
		break;
	case LPR_T:
		printf("LPR_T\n");
		break;
	case RPR_T:
		printf("RPR_T\n");
		break;
	case COL_T:
		printf("COL_T\n");
		break;
	case KW_T:
		printf("KW_T\t\t%s\n", keywordTable[t.attribute.codeType]);
		break;
	case VAR_T:
		printf("VAR_T\t\t%s\n", t.attribute.idLexeme);
		break;
	case TAB_T:
		printf("TAB_T\t\t%d\t\n", t.attribute.indentationCurrentPos);
		break;
	case RTE_T:
		printf("RTE_T\t\t%s", t.attribute.errLexeme);
		/* Call here run-time error handling component */
		if (errorNumber) {
			printf("%d", errorNumber);
			exit(errorNumber);
		}
		printf("\n");
		break;
	case SEOF_T:
		printf("SEOF_T\t\t%d\t\n", t.attribute.seofType);
		break;
	case CMT_T:
		printf("CMT_T\n");
		break;
	case OP_ADD:			
		printf("OP_ADD\n");
		break;
	case OP_SUB:
		printf("OP_SUB\n");
		break;
	case OP_MUL:
		printf("OP_MUL\n");
		break;
	case OP_DIV:
		printf("OP_DIV\n");
		break;
	case OP_MOD:
		printf("OP_MOD\n");
		break;
	case OP_EXP:
		printf("OP_EXP\n");
		break;
	case OP_EQ:
		printf("OP_EQ\n");
		break;
	case OP_NE:
		printf("OP_NE\n");
		break;
	case OP_GT:
		printf("OP_GT\n");
		break;
	case OP_LT:
		printf("OP_LT\n");
		break;
	case OP_AND:
		printf("OP_AND\n");
		break;
	case OP_OR:
		printf("OP_OR\n");
		break;
	case OP_NOT:
		printf("OP_NOT\n");
		break;
	case OP_IS:
		printf("OP_IS\n");
		break;
	default:
		printf("Scanner error: invalid token code: %d\n", t.code);
	}
}

/*
***********************************************************
* Function name: printScannerData()
* Purpose: prints statistics of tokens
* Called functions: printf()
* Parameters: 
*   scData = The scanner data struct
* Return value: Prints the Statistics table
* Algorithm: Scans the Histogram for each token and it's type
*	and prints how many appear
*************************************************************
*/
mouse_None printScannerData(ScannerData scData) {
	/* Print Scanner statistics */
	printf("Statistics:\n");
	printf("----------------------------------\n");
	int cont = 0;
	for (cont = 0; cont < NUM_TOKENS; cont++) {
		if (scData.scanHistogram[cont] > 0)
			printf("%s%s%s%d%s", "Token[", tokenStrTable[cont], "]=", scData.scanHistogram[cont], "\n");
	}
	printf("----------------------------------\n");
}