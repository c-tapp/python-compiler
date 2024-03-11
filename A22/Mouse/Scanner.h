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
* File name: Scanner.h
* Compiler: MS Visual Studio 2022
* Course: CST 8152 – Compilers, Lab Section: 011
* Assignment: A22
* Date: 03/05/24
* Professor: Paulo Sousa
* Purpose: This file is the main header for Scanner (.h)
* Function list: startScanner(), nextClass(), nextState(),
*	printScannerData(), tokenizer(), funcSL(), funcIL(),
*	funcID(), funcCMT(), funcKEY(), funcErr(), TO_DO Add more if needed
************************************************************
*/

#ifndef COMPILERS_H_
#include "Compilers.h"
#endif

#ifndef READER_H_
#include "Reader.h"
#endif

#ifndef SCANNER_H_
#define SCANNER_H_

#ifndef NULL
#include <_null.h> /* NULL pointer constant is defined there */
#endif

/*#pragma warning(1:4001) */	/*to enforce C89 type comments  - to make //comments an warning */

/*#pragma warning(error:4001)*/	/* to enforce C89 comments - to make // comments an error */

/* Constants */
#define VID_LEN 20  /* variable identifier length */
#define ERR_LEN 40  /* error message length */
#define NUM_LEN 5   /* maximum number of digits for IL */

#define RTE_CODE 1  /* Value for run-time error */

/* Define the number of tokens */
#define NUM_TOKENS 12

/* Define Token codes - Create your token classes */
enum TOKENS {
	ERR_T,		/*  0: Error token */
	MNID_T,		/*  1: Method name identifier token */
	INL_T,		/*  2: Integer literal token */
	STR_T,		/*  3: String literal token */
	FLT_T,		/*  4  Float literal token */
	BLN_T,		/*  5. Boolean literal token */
	LPR_T,		/*  6: Left parenthesis token */
	RPR_T,		/*  7: Right parenthesis token */
	KW_T,		/*	8: Keyword token */
	RTE_T,		/*  9: Run-time error token */
	SEOF_T,		/* 10: Source end-of-file token */
	CMT_T		/* 11: Comment token */
};

/* Define the list of keywords */
static mouse_str tokenStrTable[NUM_TOKENS] = {
	"ERR_T",		/*  0: Error token */
	"MNID_T",		/*  1: Method name identifier token */
	"INL_T",		/*  2: Integer literal token */
	"STR_T",		/*  3: String literal token */
	"FLT_T",		/*  4  Float literal token */
	"BLN_T",		/*  5. Boolean literal token */
	"LPR_T",		/*  6: Left parenthesis token */
	"RPR_T",		/*  7: Right parenthesis token */
	"KW_T",			/*	8: Keyword token */
	"RTE_T",		/*  9: Run-time error token */
	"SEOF_T",		/* 10: Source end-of-file token */
	"CMT_T"			/* 11: Comment token */
};

/* Operators token attributes */
typedef enum ArithmeticOperators { OP_ADD, OP_SUB, OP_MUL, OP_DIV, OP_MOD} AriOperator;
typedef enum RelationalOperators { OP_EQ, OP_NE, OP_GT, OP_LT } RelOperator;
typedef enum LogicalOperators { OP_AND, OP_OR, OP_NOT } LogOperator;
typedef enum SourceEndOfFile { SEOF_0, SEOF_255 } EofOperator;

/* Data structures for declaring the token and its attributes */
typedef union TokenAttribute {
	mouse_char errLexeme[ERR_LEN + 1];	/* error token attribite */
	mouse_char idLexeme[VID_LEN + 1];	/* variable identifier token attribute */
	mouse_int intValue;					/* integer literal attribute (value) */
	mouse_int contentString;			/* string literal offset from the beginning of the string literal buffer (stringLiteralTable->content) */
	mouse_float floatValue;				/* floating-point literal attribute (value) */
	mouse_boln boolValue;				/* Boolean literal attribute */
	mouse_int keywordIndex;				/* keyword index in the keyword table */
	AriOperator arithmeticOperator;		/* arithmetic operator attribute code */
	RelOperator relationalOperator;		/* relational operator attribute code */
	LogOperator logicalOperator;		/* logical operator attribute code */
	EofOperator seofType;				/* source-end-of-file attribute code */
	mouse_int codeType;					/* integer attributes accessor */
} TokenAttribute;

/* Should be used if no symbol table is implemented */
typedef struct idAttibutes {
	mouse_byte flags;			/* Flags information */
	union {
		mouse_int intValue;				/* Integer value */
		mouse_float floatValue;			/* Float value */
		mouse_str stringContent;		/* String value */
		mouse_boln boolValue;			/* Boolean value */
	} values;
} IdAttibutes;

/* Token declaration */
typedef struct Token {
	mouse_int code;				/* token code */
	TokenAttribute attribute;	/* token attribute */
	IdAttibutes   idAttribute;	/* not used in this scanner implementation - for further use */
} Token;

/* Scanner */
typedef struct scannerData {
	mouse_int scanHistogram[NUM_TOKENS];	/* Statistics of chars */
} ScannerData, * pScanData;

///////////////////////////////////////////////////////////////////////////////////////////////////////

/* EOF definitions */
#define CHARSEOF0 '\0'
#define CHARSEOF255 0xFF

/*  Special case tokens processed separately one by one in the token-driven part of the scanner:
 *  LPR_T, RPR_T, LBR_T, RBR_T, EOS_T, SEOF_T and special chars used for tokenis include _, & and ' */

/* TO_DO: Define lexeme FIXED classes */
/* These constants will be used on nextClass */
#define CHRCOL2 '_'
#define CHRCOL3 '&'
#define CHRCOL4 '\"'
#define CHRCOL5 '|'
//#define CHRCOL6 '#'

/* These constants will be used on VID / MID function */
#define MNID_SUF '&'
#define COMM_SYM '#'

/* TO_DO: Error states and illegal state */
#define ESNR	8		/* Error state with no retract */
#define ESWR	9		/* Error state with retract */
#define FS		10		/* Illegal state */

 /* TO_DO: State transition table definition */
#define NUM_STATES		10
#define CHAR_CLASSES	8

/* TO_DO: Transition table - type of states defined in separate table */
static mouse_int transitionTable[NUM_STATES][CHAR_CLASSES] = {
/*    [A-z],[0-9],    _,    &,   \', SEOF,    #, other
	   L(0), D(1), U(2), M(3), Q(4), E(5), C(6),  O(7) */
	{     1, ESNR, ESNR, ESNR,    4, ESWR,	  6, ESNR},	// S0: NOAS
	{     1,    1,    1,    2,	  3,    3,   3,    3},	// S1: NOAS
	{    FS,   FS,   FS,   FS,   FS,   FS,	 FS,   FS},	// S2: ASNR (MVID)
	{    FS,   FS,   FS,   FS,   FS,   FS,	 FS,   FS},	// S3: ASWR (KEY)
	{     4,    4,    4,    4,    5, ESWR,	  4,    4},	// S4: NOAS
	{    FS,   FS,   FS,   FS,   FS,   FS,	 FS,   FS},	// S5: ASNR (SL)
	{     6,    6,    6,    6,    6, ESWR,	  7,    6},	// S6: NOAS
	{    FS,   FS,   FS,   FS,   FS,   FS,	 FS,   FS},	// S7: ASNR (COM)
	{    FS,   FS,   FS,   FS,   FS,   FS,	 FS,   FS},	// S8: ASNR (ES)
	{    FS,   FS,   FS,   FS,   FS,   FS,	 FS,   FS}  // S9: ASWR (ER)
};

/* Define accepting states types */
#define NOFS	0		/* not accepting state */
#define FSNR	1		/* accepting state with no retract */
#define FSWR	2		/* accepting state with retract */

/* TO_DO: Define list of acceptable states */
static mouse_int stateType[NUM_STATES] = {
	NOFS, /* 00 */
	NOFS, /* 01 */
	FSNR, /* 02 (MID) - Methods */
	FSWR, /* 03 (KEY) */
	NOFS, /* 04 */
	FSNR, /* 05 (SL) */
	NOFS, /* 06 */
	FSNR, /* 07 (COM) */
	FSNR, /* 08 (Err1 - no retract) */
	FSWR  /* 09 (Err2 - retract) */
};

/* Static (local) function  prototypes */
mouse_int			startScanner(BufferPointer psc_buf);
static mouse_int	nextClass(mouse_char c);					/* character class function */
static mouse_int	nextState(mouse_int, mouse_char);		/* state machine function */
mouse_None			printScannerData(ScannerData scData);
Token				tokenizer(mouse_None);

/*
-------------------------------------------------
Automata definitions
-------------------------------------------------
*/

/* Pointer to function (of one char * argument) returning Token */
typedef Token(*PTR_ACCFUN)(mouse_str lexeme);

/* Declare accepting states functions */
Token funcSL	(mouse_str lexeme);
Token funcIL	(mouse_str lexeme);
Token funcID	(mouse_str lexeme);
Token funcCMT   (mouse_str lexeme);
Token funcKEY	(mouse_str lexeme);
Token funcErr	(mouse_str lexeme);

/* 
 * Accepting function (action) callback table (array) definition 
 * If you do not want to use the typedef, the equvalent declaration is:
 */

/* TO_DO: Define final state table */
static PTR_ACCFUN finalStateTable[NUM_STATES] = {
	NULL,		/* -    [00] */
	NULL,		/* -    [01] */
	funcID,		/* MNID	[02] */
	funcKEY,	/* KEY  [03] */
	NULL,		/* -    [04] */
	funcSL,		/* SL   [05] */
	NULL,		/* -    [06] */
	funcCMT,	/* COM  [07] */
	funcErr,	/* ERR1 [06] */
	funcErr		/* ERR2 [07] */
};

/*
-------------------------------------------------
Language keywords
-------------------------------------------------
*/

/* Define the number of Keywords from the language */
#define KWT_SIZE 19

/* Define the list of keywords */
static mouse_str keywordTable[KWT_SIZE] = {
	"int",		/* KW00 */
	"float",	/* KW01 */
	"str",		/* KW02 */
	"bool",		/* KW03 */
	"True",		/* KW04 */
	"False",	/* KW05 */
	"None",		/* KW06 */
	"if",		/* KW07 */
	"elif",		/* KW08 */
	"else",		/* KW09 */
	"while",	/* KW10 */
	"for",		/* KW11 */
	"def",		/* KW12 */
	"return",	/* KW13 */
	"break",	/* KW14 */
	"continue",	/* KW15 */
	"pass",		/* KW16 */
	"and",		/* KW17 */
	"or"		/* KW18 */
};

/* NEW SECTION: About indentation */

/*
 * Scanner attributes to be used (ex: including: intendation data
 */

#define INDENT '\t'  /* Tabulation */

/* Should be used if no symbol table is implemented */
typedef struct languageAttributes {
	mouse_char indentationCharType;
	mouse_int indentationCurrentPos;
} LanguageAttributes;

/* Number of errors */
mouse_int numScannerErrors;

/* Scanner data */
ScannerData scData;

#endif
