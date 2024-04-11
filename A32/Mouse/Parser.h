/*
************************************************************
* COMPILERS COURSE - Algonquin College
* Code version: Winter, 2024
* Author: Colin Tapp 040774946 / Henry Forget 041023812
* Professors: Anurag Das
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
* File name: MainParser.c
* Compiler: MS Visual Studio 2022
* Course: CST 8152 – Compilers, Lab Section: 011
* Assignment: A32
* Date: 05/05/24
* Professor: Anurag Das
* Purpose: This file is the main header for Parser (.h)
* Function list:
*	startParser(), matchToken(), syncErrorHandler(),
*	printError(), printBNFData(), program(), 
*	mainBlock(), functionDefinition(), comment(), 
*	expression(),
*	TO_DO: Add more as implimented
************************************************************
*/

#ifndef PARSER_H_
#define PARSER_H_

/* Inclusion section */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>

#ifndef COMPILERS_H_
#include "Compilers.h"
#endif
#ifndef READER_H_
#include "Reader.h"
#endif
#ifndef SCANNER_H_
#include "Scanner.h"
#endif

/* Global vars */
static Token			lookahead;
extern BufferPointer	stringLiteralTable;
extern mouse_int		line;
extern Token			tokenizer(mouse_None);
extern mouse_str		keywordTable[KWT_SIZE];
static mouse_int		syntaxErrorNumber = 0;

#define LANG_WRTE		"print"
#define LANG_READ		"input"

/* Constants */
enum KEYWORDS {
	NO_ATTR = -1, 	/* NO_ATTR = epsilon = -1 */
	KW_int,			/* 00 */
	KW_float,		/* 01 */
	KW_str,			/* 02 */
	KW_bool,		/* 03 */
	KW_True,		/* 04 */
	KW_False,		/* 05 */
	KW_None,		/* 06 */
	KW_if,			/* 07 */
	KW_elif,		/* 08 */
	KW_else,		/* 09 */
	KW_while,		/* 10 */
	KW_for,			/* 11 */
	KW_def,			/* 12 */
	KW_return,		/* 13 */
	KW_break,		/* 14 */
	KW_continue,	/* 15 */
	KW_pass,		/* 16 */
	KW_and,			/* 17 */
	KW_or,			/* 18 */
};

/* Define the number of BNF rules */
#define NUM_BNF_RULES 38

/* Parser */
typedef struct parserData {
	mouse_int parsHistogram[NUM_BNF_RULES];	/* Number of BNF Statements */
} ParserData, * pParsData;

/* Number of errors */
mouse_int numParserErrors;

/* Scanner data */
ParserData psData;

/* Function definitions */
mouse_None startParser();
mouse_None matchToken(mouse_int, mouse_int);
mouse_None syncErrorHandler(mouse_int);
mouse_None printError();
mouse_None printBNFData(ParserData psData);

/* List of BNF statements */
enum BNF_RULES {
	BNF_error,										/* 00: Error token */
	BNF_program, 									/* 01 */
	BNF_functionDefinitions, 						/* 02 */
	BNF_functionDefinition, 						/* 03 */
	BNF_mainBlock, 									/* 04 */
	BNF_funcName, 									/* 05 */
	BNF_comments, 									/* 06 */
	BNF_statements, 								/* 07 */
	BNF_statement, 									/* 08 */
	BNF_variableDeclaration, 						/* 09 */
	BNF_typeAnnotation, 							/* 10 */
	BNF_variableInitialization, 					/* 11 */
	BNF_assignmentStatement, 						/* 12 */
	BNF_inputStatement, 							/* 13 */
	BNF_outputStatement, 							/* 14 */
	BNF_variableExpression, 						/* 15 */
	BNF_expression, 								/* 16 */
	BNF_arithmeticExpression, 						/* 17 */
	BNF_additiveArithmeticExpression, 				/* 18 */
	BNF_multiplicativeArithmeticExpression, 		/* 19 */
	BNF_exponentialArithmeticExpression, 			/* 20 */
	BNF_primaryExpression, 							/* 21 */
	BNF_stringExpression, 							/* 22 */
		BNF_conditionalExpression, 						/* 23 */
		BNF_logicalExpression, 							/* 24 */
		BNF_logicalOrExpression, 						/* 25 */
		BNF_logicalAndExpression, 						/* 26 */
		BNF_logicalNotExpression, 						/* 27 */
	BNF_relationalExpression, 						/* 28 */
	BNF_relationalOperator, 						/* 29 */
		BNF_selectionStatement, 						/* 30 */
		BNF_optionalElifStatement, 						/* 31 */
		BNF_elifStatement, 								/* 32 */
		BNF_optionalElseStatement, 						/* 33 */
		BNF_iterationStatement, 						/* 34 */
	BNF_functionCallStatement, 						/* 35 */
	BNF_expressionList,	 							/* 36 */
	BNF_outputVariable								/* 37 */
};


static mouse_str BNFStrTable[NUM_BNF_RULES] = {
	"BNF_error",									/* 00: Error token */
	"BNF_program", 									/* 01 */
	"BNF_functionDefinitions", 						/* 02 */
	"BNF_functionDefinition", 						/* 03 */
	"BNF_mainBlock", 								/* 04 */
	"BNF_funcName", 								/* 05 */
	"BNF_comments", 								/* 06 */
	"BNF_statements", 								/* 07 */
	"BNF_statement", 								/* 08 */
	"BNF_variableDeclaration", 						/* 09 */
	"BNF_typeAnnotation", 							/* 10 */
	"BNF_variableInitialization", 					/* 11 */
	"BNF_assignmentStatement", 						/* 12 */
	"BNF_inputStatement", 							/* 13 */
	"BNF_outputStatement", 							/* 14 */
	"BNF_variableExpression", 						/* 15 */
	"BNF_expression", 								/* 16 */
	"BNF_arithmeticExpression", 					/* 17 */
	"BNF_additiveArithmeticExpression", 			/* 18 */
	"BNF_multiplicativeArithmeticExpression", 		/* 19 */
	"BNF_exponentialArithmeticExpression", 			/* 20 */
	"BNF_primaryExpression", 						/* 21 */
	"BNF_stringExpression", 						/* 22 */
	"BNF_conditionalExpression", 					/* 23 */
	"BNF_logicalExpression", 						/* 24 */
	"BNF_logicalOrExpression", 						/* 25 */
	"BNF_logicalAndExpression", 					/* 26 */
	"BNF_logicalNotExpression", 					/* 27 */
	"BNF_relationalExpression", 					/* 28 */
	"BNF_relationalOperator", 						/* 29 */
	"BNF_selectionStatement", 						/* 30 */
	"BNF_optionalElifStatement", 					/* 31 */
	"BNF_elifStatement", 							/* 32 */
	"BNF_optionalElseStatement", 					/* 33 */
	"BNF_iterationStatement", 						/* 34 */
	"BNF_functionCallStatement", 					/* 35 */
	"BNF_expressionList",	 						/* 36 */
	"BNF_outputVariable"							/* 37 */
};

/* TO_DO: Place ALL non-terminal function declarations */
mouse_None program();
mouse_None mainBlock();
mouse_None functionDefinition();
mouse_None comment();
mouse_None statements();
mouse_None statement();
mouse_None outputStatement();
mouse_None outputVariableList();
mouse_None expression();

#endif
