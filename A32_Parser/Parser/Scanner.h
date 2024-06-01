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
* File name: Scanner.h
* Compiler: MS Visual Studio 2022
* Date: 03/16/24
* Purpose: This file is the main header for Scanner (.h)
* Function list: startScanner(), nextClass(), nextState(),
*	printScannerData(), tokenizer(), funcSL(), funcIL(),
*	funcID(), funcCMT(), funcKEY(), funcErr()
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
#define NUM_LEN 50   /* maximum number of digits for IL */

#define RTE_CODE 1  /* Value for run-time error */

/* Define the number of tokens */
#define NUM_TOKENS 17

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
	COL_T,		/*  8: Colon token */
	COM_T,		/*  9: Comma token */
	KW_T,		/* 10: Keyword token */
	VAR_T,		/* 11: Variable token */
	TAB_T,		/* 12: Tabulation token */
	RTE_T,		/* 13: Run-time error token */
	SEOF_T,		/* 14: Source end-of-file token */
	CMT_T,		/* 15: Comment token */
	OP_T,		/* 16: Operator token */
	EOL_T		/* 17: End of Line token (\n) */
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
	"COL_T",		/*  8: Colon token */
	"COM_T",		/*  9: Comma token */
	"KW_T",			/* 10: Keyword token */
	"VAR_T",		/* 11: Variable token */
	"TAB_T",		/* 12: Tabulation token */
	"RTE_T",		/* 13: Run-time error token */
	"SEOF_T",		/* 14: Source end-of-file token */
	"CMT_T",		/* 15: Comment token */
	"OP_T"			/* 16: Operator token */
};

/* Operators token attributes - Added values to differentiate in printToken()*/
typedef enum ArithmeticOperators { OP_ADD, OP_SUB, OP_MUL, OP_DIV, OP_MOD, OP_EXP} AriOperator;
typedef enum RelationalOperators { OP_EQ=6, OP_NE=7, OP_GT=8, OP_LT=9} RelOperator;
typedef enum LogicalOperators { OP_AND=10, OP_OR=11, OP_NOT=12, OP_IS=13} LogOperator;
typedef enum SourceEndOfFile { SEOF_0, SEOF_255} EofOperator;

/* Data structures for declaring the token and its attributes */
typedef union TokenAttribute {
	mouse_char errLexeme[ERR_LEN + 1];	/* error token attribite */
	mouse_char idLexeme[VID_LEN + 1];	/* variable identifier token attribute */
	mouse_int intValue;					/* integer literal attribute (value) */
	mouse_int contentString;			/* string literal offset from the beginning of the string literal buffer (stringLiteralTable->content) */
	mouse_float floatValue;				/* floating-point literal attribute (value) */
	mouse_boln boolValue;				/* Boolean literal attribute */
	mouse_int indentationCurrentPos;	/* Tabulation's current depth */
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
 *  LPR_T, RPR_T, LPR_T, RPR_T, EOS_T, SEOF_T and special chars used for tokenis include _, & and ' */

/* Define lexeme FIXED classes */
/* These constants will be used on nextClass */
#define CHRCOL0  '('
#define CHRCOL2  '#'
#define CHRCOL4  '\n'
#define CHRCOL5  '+'
#define CHRCOL6  '-'
#define CHRCOL7  '.'
#define CHRCOL8  ','
#define CHRCOL9 '\"'
#define CHRCOL10 '_'

/* These constants will be used on VID / MID function */
#define MNID_SUF '('
#define COMM_SYM '#'

/* Error states and illegal state */
#define ESNR	14		/* Error state with no retract */
#define ESWR	15		/* Error state with retract */
#define FS		16		/* Illegal state */

 /* State transition table definition */
#define NUM_STATES		17
#define CHAR_CLASSES	13

/* Transition table - type of states defined in separate table */
static mouse_int transitionTable[NUM_STATES][CHAR_CLASSES] = {
/*      (  , [0-9],  #  ,[A-Z], \n  ,  +  ,  -  , (.) , (,) ,  "  ,  _  ,other, EOF
	  B1(0), D(1), H(2), L(3), N(4),O1(5),O2(6),P1(7),P2(8), Q(9),U(10),Z(11), (12)*/
	{  ESNR,    5,    1,   11, ESNR,    7,    7, ESNR, ESNR,    3, ESNR, ESNR, ESWR},	// S0:  NOFS
	{     1,    1,    1,    1,    2,    1,    1,    1,    1,    1,    1,    1, ESWR},	// S1:  NOFS
	{	 FS,   FS,   FS,   FS,   FS,   FS,   FS,   FS,   FS,   FS,   FS,   FS,   FS},	// S2:  FSNR (COM)
	{     3,   3,    3,    3,    3,    3,    3,    3,    3,    4,    3,    3,    3},	// S3:  NOFS 
	{    FS,   FS,   FS,   FS,   FS,   FS,   FS,   FS,   FS,   FS,   FS,   FS,   FS},	// S4:  FSNR (SL)
	{     6,    5,    6,    6,    6,    6,    6,    8,    6,    6,    6,    6,    6},	// S5:  NOFS 
	{    FS,   FS,   FS,   FS,   FS,   FS,   FS,   FS,   FS,   FS,   FS,   FS,   FS},	// S6:  FSWR (IL)
	{  ESNR,    5, ESNR, ESNR, ESNR, ESNR, ESNR,    8, ESNR, ESNR, ESNR, ESNR, ESNR},	// S7:  NOFS
	{  ESNR,    9, ESNR, ESNR, ESNR, ESNR, ESNR, ESNR, ESNR, ESNR, ESNR, ESNR, ESNR},	// S8:  NOFS
	{    10,    9,   10,   10,   10,   10,   10,   10,   10,   10,   10,   10,   10},   // S9:  NOFS
	{    FS,   FS,   FS,   FS,   FS,   FS,   FS,   FS,   FS,   FS,   FS,   FS,   FS},   // S10: FSWR (FL)
	{    13,   11,   12,   11,   12,   12,   12,   12,   12,   12,   11,   12,   12},   // S11: NOFS
	{    FS,   FS,   FS,   FS,   FS,   FS,   FS,   FS,   FS,   FS,   FS,   FS,   FS},   // S12: FSWR (Key / ID)
	{    FS,   FS,   FS,   FS,   FS,   FS,   FS,   FS,   FS,   FS,   FS,   FS,   FS},   // S13: FSNR (MID) - No Retract due to retract implimented in FuncID
	{    FS,   FS,   FS,   FS,   FS,   FS,   FS,   FS,   FS,   FS,   FS,   FS,   FS},   // S14: FSNR (ER1)
	{    FS,   FS,   FS,   FS,   FS,   FS,   FS,   FS,   FS,   FS,   FS,   FS,   FS}    // S15: FSWR (ER2)
};

/* Define accepting states types */
#define NOFS	0		/* not accepting state */
#define FSNR	1		/* accepting state with no retract */
#define FSWR	2		/* accepting state with retract */

/* Define list of acceptable states */
static mouse_int stateType[NUM_STATES] = {
	NOFS,	// (00) No Final State
	NOFS,	// (01) No Final State
	FSNR,	// (02) Comments
	NOFS,	// (03) No Final State
	FSNR,	// (04) String Literal
	NOFS,	// (05) No Final State
	FSWR,	// (06) Integer Literal
	NOFS,	// (07) No Final State
	NOFS,	// (08) No Final State
	NOFS,	// (09) No Final State
	FSWR,	// (10) Float Literal
	NOFS,	// (11) No Final State
	FSWR,	// (12) Keywords / ID
	FSNR,	// (13) MID - Methods - No Retract due to retract implimented in FuncID
	FSNR,	// (14) Err1 - No Retract
	FSWR	// (15) Err2 - With Retract
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
Token funcNL	(mouse_str lexeme);
Token funcID	(mouse_str lexeme);
Token funcCMT   (mouse_str lexeme);
Token funcKEY	(mouse_str lexeme);
Token funcErr	(mouse_str lexeme);

/* 
 * Accepting function (action) callback table (array) definition 
 * If you do not want to use the typedef, the equvalent declaration is:
 */

/* Define final state table */
static PTR_ACCFUN finalStateTable[NUM_STATES] = {
	NULL,	// (00) No Final State
	NULL,	// (01) No Final State
	funcCMT,// (02) Comments
	NULL,	// (03) No Final State
	funcSL,	// (04) String Literal
	NULL,	// (05) No Final State
	funcNL,	// (06) Integer Literal
	NULL,	// (07) No Final State
	NULL,	// (08) No Final State
	NULL,	// (09) No Final State
	funcNL,	// (10) Float Literal
	NULL,	// (11) No Final State
	funcID,	// (12) Keywords
	funcID,	// (13) MID - Methods
	funcErr,// (14) Err1 - No Retract
	funcErr	// (15) Err2 - With Retract
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
