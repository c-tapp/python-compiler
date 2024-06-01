﻿/*
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
* File name: MainParser.c
* Compiler: MS Visual Studio 2022
* Date: 05/05/24
* Purpose: This file contains all functionalities from Parser.
* Function list:
*	startParser(), matchToken(), syncErrorHandler(),
*	printError(), printBNFData(), program(),
*	mainBlock(), functionDefinition(), comment(),
*	expression(),
*	TO_DO: Add more as implimented
************************************************************
*/

#ifndef COMPILERS_H_
#include "Compilers.h"
#endif

#ifndef PARSER_H_
#include "Parser.h"
#endif

/* Parser data */
extern ParserData psData; /* BNF statistics */

/*
***********************************************************
* Function name: startParser
* Purpose: Function to start the parser
* Called functions: tokenizer(), program(),
*	matchToken(), printf()
* Algorithm: Initialize Parser data
*************************************************************
*/
mouse_None startParser() {
	mouse_int i = 0;
	for (i = 0; i < NUM_BNF_RULES; i++) {
		psData.parsHistogram[i] = 0;
	}
	/* Proceed parser */
	lookahead = tokenizer();
	if (lookahead.code != SEOF_T) {
		program();
	}
	matchToken(SEOF_T, NO_ATTR);
	printf("%s%s\n", STR_LANGNAME, ": Source file parsed");
}

 /*
 ***********************************************************
 * Function name: matchToken
 * Purpose: Main code for match
 * Called functions: tokenizer(), printError(), syncErrorHandler()
 * Parameters:
 *	tokenCode = Code to compare to
 *	tokenAttribute = Attribute to compare to
 * Algorithm: checks if the tokenCode or tokenAttribute is the 
 *	same as the one given in the parameters
 *************************************************************
 */
mouse_None matchToken(mouse_int tokenCode, mouse_int tokenAttribute) {
	mouse_int matchFlag = 1;
	switch (lookahead.code) {
	case KW_T:
		if (lookahead.attribute.codeType != tokenAttribute)
			matchFlag = 0;
	default:
		if (lookahead.code != tokenCode)
			matchFlag = 0;
	}
	if (matchFlag && lookahead.code == SEOF_T)
		return;
	if (matchFlag) {
		lookahead = tokenizer();
		if (lookahead.code == ERR_T) {
			printError();
			lookahead = tokenizer();
			syntaxErrorNumber++;
		}
	}
	else
		syncErrorHandler(tokenCode);
}

/*
***********************************************************
* Function name: syncErrorHandler
* Purpose: Function to handler error
* Called functions: printError(), exit(), tokenizer()
* Parameters:
*	syncTokenCode = Compares to the lookahead.code
* Algorithm: TO_DO how does this work?
*************************************************************
*/
mouse_None syncErrorHandler(mouse_int syncTokenCode) {
	printError();
	syntaxErrorNumber++;
	while (lookahead.code != syncTokenCode) {
		if (lookahead.code == SEOF_T)
			exit(syntaxErrorNumber);
		lookahead = tokenizer();
	}
	if (lookahead.code != SEOF_T)
		lookahead = tokenizer();
}

/*
***********************************************************
* Function name: printError
* Purpose: function for error printing
* Called functions: printf()
* Algorithm: Prints the lookahead's code
*************************************************************
*/
mouse_None printError() {
	extern numParserErrors;			/* link to number of errors (defined in Parser.h) */
	Token t = lookahead;
	printf("%s%s%3d\n", STR_LANGNAME, ": Syntax error:  Line:", line);
	printf("*****  Token code:%3d Attribute: ", t.code);
	switch (t.code) {
	case ERR_T:
		printf("ERR_T\t\t%s\n", t.attribute.errLexeme);
		break;
	case MNID_T:
		printf("MNID_T\t\t%s\n", t.attribute.idLexeme);
		break;
	case INL_T:
		printf("INL_T\t\t%d\n", t.attribute.intValue);
		break;
	case STR_T:
		printf("STR_T\t\t%d\t ", (mouse_int)t.attribute.codeType);
		printf("%s\n", readerGetContent(stringLiteralTable, (mouse_int)t.attribute.codeType));
		break;
	case FLT_T:
		printf("FLT_T\t\t%f\n", t.attribute.floatValue);
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
	case COM_T:
		printf("COM_T\n");
		break;
	case KW_T:
		printf("KW_T\t\t%s\n", keywordTable[t.attribute.codeType]);
		break;
	case VAR_T:
		printf("VAR_T\t\t\n");
		break;
	case TAB_T:
		printf("TAB_T\t\t%d\t\n", t.attribute.indentationCurrentPos);
		break;
	case RTE_T:
		printf("RTE_T\t\t%s\n", t.attribute.errLexeme);
		break;
	case SEOF_T:
		printf("SEOF_T\t\t%d\t\n", t.attribute.seofType);
		break;
	case CMT_T:
		printf("CMT_T\n");
		break;
	case OP_T:
		/* Checking Arithmetic Operators */
		switch (t.attribute.arithmeticOperator) {
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
		default:
			/* Checking Relational Operators */
			switch (t.attribute.relationalOperator) {
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
			default:
				/* Checking Logical Operators */
				switch (t.attribute.logicalOperator) {
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

				} // Logical check end
			} // Relational check end
		}// Arithmetic check end
		break;
	default:
		printf("Scanner error: invalid token code: %d\n", t.code);
		numParserErrors++; // Updated parser error
	}
}

/*
***********************************************************
* Function name: printBNFData
* Purpose: Print Parser statistics
* Called functions: printf()
* Parameters:
*	psData = Parser Data
* Algorithm: For loop itterates through the data printing it all
*************************************************************
*/
mouse_None printBNFData(ParserData psData) {
	/* Print Parser statistics */
	printf("Statistics:\n");
	printf("----------------------------------\n");
	int cont = 0;
	for (cont = 0; cont < NUM_BNF_RULES; cont++) {
		if (psData.parsHistogram[cont] > 0)
			printf("%s%s%s%d%s", "Token[", BNFStrTable[cont], "]=", psData.parsHistogram[cont], "\n");
	}
	printf("----------------------------------\n");
}

/*
 ***********************************************************
 * Function name: program()
 * Purpose: Disern between the function definitions and the main code while also accounting for comments
 * Called functions: comment(), functionDefinition(), printError(), 
		mainBlock(), matchToken(), printf()
 * Algorithm: Switch case that checks the next token
 *************************************************************
*/
mouse_None program() {
	/* Update program statistics */
	psData.parsHistogram[BNF_program]++;
	
	/* Program code */
	while(lookahead.code != SEOF_T) {
		switch(lookahead.code) {
		case CMT_T:
			comment(); // consume token, skip to next lookahead
			break;
		case KW_T:
			// Check if keyword == 'def'
			if (lookahead.attribute.codeType == 12) {
				psData.parsHistogram[BNF_functionDefinitions]++;	
				functionDefinition();
				printf("%s%s\n", STR_LANGNAME, ": Function Definitions parsed");
			}
			break;
		case ERR_T:
			printError();
			break;
		default:
			mainBlock(); 
			break;
		}
	}
	
	/* Match End of File token */
	matchToken(SEOF_T, NO_ATTR);
	printf("%s%s\n", STR_LANGNAME, ": Program parsed");
}

/*
***********************************************************
* Function name: mainBlock
* Purpose: Repeadedly call statements() until end of file is reached
* Called functions: statements(), printf()
* Algorithm: while loop that waits for SEOF_T
*************************************************************
*/
mouse_None mainBlock(){
	psData.parsHistogram[BNF_mainBlock]++;
	while (lookahead.code != SEOF_T) {
		statements();
	}
	printf("%s%s\n", STR_LANGNAME, ": Main Block parsed");
}

/*
 ************************************************************
 * Function name: statements
 * Purpose: Repeadedly call statement() until an apropriate token isnt found
 * Called functions: statement(), printf()
 * Algorithm: while loop that waits for certain tokens
 ***********************************************************
 */
mouse_None statements() {
	psData.parsHistogram[BNF_statements]++;

	while (lookahead.attribute.codeType != 13 && lookahead.code != CMT_T && lookahead.code != SEOF_T){
		statement();
	}
	printf("%s%s\n", STR_LANGNAME, ": Statements parsed");
}

/*
 ************************************************************
 * Function name: statement
 * Purpose: Parse the correct order for certain functions
 * Called functions: matchToken(), outputStatement(), printf(),
 *	printError(), expression()
 * Algorithm: Nested switch and if statements
 ***********************************************************
 */
mouse_None statement() {
	if (lookahead.code == TAB_T) {
		matchToken(TAB_T, NO_ATTR); // TO_DO Is checking for proper indentation is out of the scope of this assignment?
	}
	else {
		psData.parsHistogram[BNF_statement]++;
		switch (lookahead.code) {
		case KW_T:
			switch (lookahead.attribute.codeType) {
			case 7: // If
				psData.parsHistogram[BNF_logicalExpression]++;
				matchToken(KW_T, 7);
				matchToken(LPR_T, NO_ATTR);
				expression();
				matchToken(RPR_T, NO_ATTR);
				matchToken(COL_T, NO_ATTR);
				matchToken(TAB_T, NO_ATTR);
				if (lookahead.code == CMT_T) {
					// Satement explanation
					matchToken(CMT_T, NO_ATTR);
				}
				statements();
				printf("%s%s\n", STR_LANGNAME, ": Logical Expression parsed");
				break;
			case 8: // elif
				psData.parsHistogram[BNF_optionalElifStatement]++;
				matchToken(KW_T, 8);
				matchToken(LPR_T, NO_ATTR);
				expression();
				matchToken(RPR_T, NO_ATTR);
				matchToken(COL_T, NO_ATTR);
				matchToken(TAB_T, NO_ATTR);
				if (lookahead.code == CMT_T) {
					// Satement explanation
					matchToken(CMT_T, NO_ATTR);
				}
				statements();
				printf("%s%s\n", STR_LANGNAME, ": Optional Elif Statement parsed");
				break;
			case 9: // else
				psData.parsHistogram[BNF_optionalElseStatement]++;
				matchToken(KW_T, 9);
				matchToken(COL_T, NO_ATTR);
				matchToken(TAB_T, NO_ATTR);
				if (lookahead.code == CMT_T) {
					// Satement explanation
					matchToken(CMT_T, NO_ATTR);
				}
				statements();
				printf("%s%s\n", STR_LANGNAME, ": Optional Else Statement parsed");
				break;
			case 10: // While Loop
				psData.parsHistogram[BNF_iterationStatement]++;
				matchToken(KW_T, 10);
				matchToken(LPR_T, NO_ATTR);
				expression();
				matchToken(RPR_T, NO_ATTR);
				matchToken(COL_T, NO_ATTR);
				matchToken(TAB_T, NO_ATTR);
				statements();
				printf("%s%s\n", STR_LANGNAME, ": Itteration Statement parsed");
				break;
			case 11: // for loop
				matchToken(KW_T, 11);
				matchToken(LPR_T, NO_ATTR);
				matchToken(VAR_T, NO_ATTR);

				psData.parsHistogram[BNF_variableInitialization]++;
				psData.parsHistogram[BNF_assignmentStatement]++;
				if (lookahead.attribute.relationalOperator == OP_EQ && lookahead.code == OP_T) {
					psData.parsHistogram[BNF_relationalOperator]++;
					matchToken(OP_T, NO_ATTR);
					printf("%s%s\n", STR_LANGNAME, ": Relational Operator parsed");
					psData.parsHistogram[BNF_relationalExpression]++;
					psData.parsHistogram[BNF_assignmentStatement]++;
					printf("%s%s\n", STR_LANGNAME, ": Assignment Statement parsed");
					printf("%s%s\n", STR_LANGNAME, ": Relational Expression parsed");
					printf("%s%s\n", STR_LANGNAME, ": Variable Initialization parsed");
				}
				if (lookahead.code == INL_T) {
					matchToken(INL_T, NO_ATTR);
					psData.parsHistogram[BNF_variableDeclaration]++;
					printf("%s%s\n", STR_LANGNAME, ": Variable Declaration parsed");
				}else if (lookahead.code == VAR_T) {
					matchToken(VAR_T, NO_ATTR);
					psData.parsHistogram[BNF_variableDeclaration]++;
					printf("%s%s\n", STR_LANGNAME, ": Variable Declaration parsed");
				}
				printf("%s%s\n", STR_LANGNAME, ": Assignment Statement parsed");

				matchToken(COM_T, NO_ATTR);
				matchToken(VAR_T, NO_ATTR);

				if (lookahead.attribute.relationalOperator == OP_EQ || lookahead.attribute.relationalOperator == OP_GT || lookahead.attribute.relationalOperator == OP_LT && lookahead.code == OP_T) {
					psData.parsHistogram[BNF_relationalOperator]++;
					matchToken(OP_T, NO_ATTR);
					printf("%s%s\n", STR_LANGNAME, ": Relational Operator parsed");
				}
				if (lookahead.code == INL_T) {
					matchToken(INL_T, NO_ATTR);
				}
				else if (lookahead.code == VAR_T) {
					matchToken(VAR_T, NO_ATTR);
				}

				matchToken(COM_T, NO_ATTR);
				matchToken(VAR_T, NO_ATTR);

				if (lookahead.attribute.arithmeticOperator == OP_ADD && lookahead.code == OP_T) {
					matchToken(OP_T, NO_ATTR);
					matchToken(OP_T, NO_ATTR);
				}
				else if (lookahead.attribute.arithmeticOperator == OP_SUB && lookahead.code == OP_T) {
					matchToken(OP_T, NO_ATTR);
					matchToken(OP_T, NO_ATTR);
				}

				matchToken(RPR_T, NO_ATTR);
				matchToken(TAB_T, NO_ATTR);
				expression();		
				break;
			case 14:
				matchToken(KW_T,14);
				break;
			default:
				printError();
			}
			break;

		case MNID_T:
			if (strncmp(lookahead.attribute.idLexeme, LANG_WRTE, 6) == 0) {
				outputStatement();
			}
			else {
				psData.parsHistogram[BNF_functionCallStatement]++;
				psData.parsHistogram[BNF_funcName]++;
				matchToken(MNID_T, NO_ATTR);
				printf("%s%s\n", STR_LANGNAME, ": Function Name parsed");
				matchToken(LPR_T, NO_ATTR);
				psData.parsHistogram[BNF_expressionList]++;
				expression();
				printf("%s%s\n", STR_LANGNAME, ": Expression List parsed");
				matchToken(RPR_T, NO_ATTR);
				printf("%s%s\n", STR_LANGNAME, ": Function Call Statement parsed");
				if (lookahead.code == TAB_T) {
					matchToken(TAB_T, NO_ATTR);
				}
			}
			break;
		case VAR_T:
			matchToken(VAR_T, NO_ATTR);
			if (lookahead.code == COL_T) {
				psData.parsHistogram[BNF_variableInitialization]++;
				matchToken(COL_T, NO_ATTR);
				printf("%s%s\n", STR_LANGNAME, ": Variable Initialization parsed");
				if (lookahead.code == KW_T) {
					if (lookahead.attribute.codeType == 0 || lookahead.attribute.codeType == 1 || lookahead.attribute.codeType == 2 || lookahead.attribute.codeType == 3) {
						psData.parsHistogram[BNF_typeAnnotation]++;
						psData.parsHistogram[BNF_variableDeclaration]++;
						matchToken(KW_T, lookahead.attribute.codeType);
						printf("%s%s\n", STR_LANGNAME, ": Type Annotation parsed");
						printf("%s%s\n", STR_LANGNAME, ": Variable Declaration parsed");
					}
					else {
						printError();
					}
				}
			}

			if (lookahead.attribute.relationalOperator == OP_EQ) {
				psData.parsHistogram[BNF_assignmentStatement]++;
				// Assignment
				expression();		
				printf("%s%s\n", STR_LANGNAME, ": Assignment Statement parsed");
			}
			break;
		case CMT_T:
			comment(); // consume token, skip to next lookahead
			break;
		default:
			printError();
		}
		printf("%s%s\n", STR_LANGNAME, ": Statement parsed");
	}
}

/*
 ***********************************************************
 * Function name: functionDefinition
 * Purpose: Verify pattern for defining a function
 * Called functions: matchToken(), statements(), printf()
 * Algorithm: Several matchToken calls followed by statements()
 *************************************************************
 */
mouse_None functionDefinition() {
	psData.parsHistogram[BNF_functionDefinition]++;
	matchToken(KW_T, 12);
	psData.parsHistogram[BNF_funcName]++;
	matchToken(MNID_T, NO_ATTR);
	printf("%s%s\n", STR_LANGNAME, ": Function Name parsed");
	matchToken(LPR_T, NO_ATTR);
	// Optional Parameters
	matchToken(RPR_T, NO_ATTR);
	matchToken(COL_T, NO_ATTR);
	matchToken(TAB_T, NO_ATTR);
	// All functionDefinition components matched
	statements();
	if (lookahead.code == KW_T && lookahead.attribute.codeType == 13) {
		// Keword return termination
		matchToken(KW_T, 13);
		matchToken(VAR_T, NO_ATTR);
	}

	printf("%s%s\n", STR_LANGNAME, ": Function Definition parsed");
	if (lookahead.code == CMT_T) {
		matchToken(CMT_T, NO_ATTR);
	}
	if (lookahead.attribute.codeType == 12) {
		functionDefinition();
	}
}

/*
***********************************************************
* Function name: expression()
* Purpose: Verifys the patterns of expressions
* Called functions: matchToken(), expression(), printf(),
*	printError()
* Algorithm: Nested switch and if statements
*************************************************************
*/
mouse_None expression(){
	psData.parsHistogram[BNF_expression]++;
	switch (lookahead.code) {
	case OP_T:
		if (lookahead.attribute.relationalOperator == OP_EQ) {
			psData.parsHistogram[BNF_relationalOperator]++;
			matchToken(OP_T, NO_ATTR);
			printf("%s%s\n", STR_LANGNAME, ": Relational Operator parsed");
			// Conducting the other side of the equation
			psData.parsHistogram[BNF_relationalExpression]++;
			expression();
			printf("%s%s\n", STR_LANGNAME, ": Relational Expression parsed");
		}
		break;
	case INL_T:
		psData.parsHistogram[BNF_primaryExpression]++;
		matchToken(INL_T, NO_ATTR);
		if (lookahead.code == OP_T) {
			psData.parsHistogram[BNF_arithmeticExpression]++;
			if (lookahead.attribute.arithmeticOperator == OP_ADD || lookahead.attribute.arithmeticOperator == OP_SUB && lookahead.code == OP_T) {
				matchToken(OP_T, NO_ATTR);
				psData.parsHistogram[BNF_additiveArithmeticExpression]++;
				expression();
				printf("%s%s\n", STR_LANGNAME, ": Additive Arithmetic parsed");
			}
			else if (lookahead.attribute.arithmeticOperator == OP_MUL || lookahead.attribute.arithmeticOperator == OP_DIV || lookahead.attribute.arithmeticOperator == OP_MOD && lookahead.code == OP_T) {
				matchToken(OP_T, NO_ATTR);
				psData.parsHistogram[BNF_multiplicativeArithmeticExpression]++;
				expression();
				printf("%s%s\n", STR_LANGNAME, ": Multiplicative Arithmetic parsed");
			}
			else if (lookahead.attribute.arithmeticOperator == OP_EXP && lookahead.code == OP_T) {
				matchToken(OP_T, NO_ATTR);
				psData.parsHistogram[BNF_exponentialArithmeticExpression]++;
				expression();
				printf("%s%s\n", STR_LANGNAME, ": Exponential Arithmetic parsed");
			}
			printf("%s%s\n", STR_LANGNAME, ": Arithmetic parsed");
		}
		printf("%s%s\n", STR_LANGNAME, ": Primary Expression parsed");
		break;
	case FLT_T:
		psData.parsHistogram[BNF_primaryExpression]++;
		matchToken(FLT_T, NO_ATTR);
		if (lookahead.code == OP_T) {
			psData.parsHistogram[BNF_arithmeticExpression]++;
			if (lookahead.attribute.arithmeticOperator == OP_ADD || lookahead.attribute.arithmeticOperator == OP_SUB && lookahead.code == OP_T) {
				matchToken(OP_T, NO_ATTR);
				psData.parsHistogram[BNF_additiveArithmeticExpression]++;
				expression();
				printf("%s%s\n", STR_LANGNAME, ": Additive Arithmetic parsed");
			}
			else if (lookahead.attribute.arithmeticOperator == OP_MUL || lookahead.attribute.arithmeticOperator == OP_DIV || lookahead.attribute.arithmeticOperator == OP_MOD && lookahead.code == OP_T) {
				matchToken(OP_T, NO_ATTR);
				psData.parsHistogram[BNF_multiplicativeArithmeticExpression]++;
				expression();
				printf("%s%s\n", STR_LANGNAME, ": Multiplicative Arithmetic parsed");
			}
			else if (lookahead.attribute.arithmeticOperator == OP_EXP && lookahead.code == OP_T) {
				matchToken(OP_T, NO_ATTR);
				psData.parsHistogram[BNF_exponentialArithmeticExpression]++;
				expression();
				printf("%s%s\n", STR_LANGNAME, ": Exponential Arithmetic parsed");
			}
			printf("%s%s\n", STR_LANGNAME, ": Arithmetic parsed");
		}
		printf("%s%s\n", STR_LANGNAME, ": Primary Expression parsed");
		break;
	case VAR_T:
		psData.parsHistogram[BNF_variableExpression]++;
		psData.parsHistogram[BNF_primaryExpression]++;
		matchToken(VAR_T, NO_ATTR);
		if (lookahead.code == OP_T) {
			psData.parsHistogram[BNF_arithmeticExpression]++;
			if (lookahead.attribute.arithmeticOperator == OP_ADD || lookahead.attribute.arithmeticOperator == OP_SUB && lookahead.code == OP_T) {
				matchToken(OP_T, NO_ATTR);
				psData.parsHistogram[BNF_additiveArithmeticExpression]++;
				expression();
				printf("%s%s\n", STR_LANGNAME, ": Additive Arithmetic parsed");
			}
			else if (lookahead.attribute.arithmeticOperator == OP_MUL || lookahead.attribute.arithmeticOperator == OP_DIV || lookahead.attribute.arithmeticOperator == OP_MOD && lookahead.code == OP_T) {
				matchToken(OP_T, NO_ATTR);
				psData.parsHistogram[BNF_multiplicativeArithmeticExpression]++;
				expression();
				printf("%s%s\n", STR_LANGNAME, ": Multiplicative Arithmetic parsed");
			}
			else if (lookahead.attribute.arithmeticOperator == OP_EXP && lookahead.code == OP_T) {
				matchToken(OP_T, NO_ATTR);
				psData.parsHistogram[BNF_exponentialArithmeticExpression]++;
				expression();
				printf("%s%s\n", STR_LANGNAME, ": Exponential Arithmetic parsed");
			}
			else if (lookahead.attribute.relationalOperator == OP_EQ && lookahead.code == OP_T) {
				psData.parsHistogram[BNF_relationalOperator]++;
				matchToken(OP_T, NO_ATTR);
				printf("%s%s\n", STR_LANGNAME, ": Relational Operator parsed");
				// Conducting the other side of the equation
				psData.parsHistogram[BNF_relationalExpression]++;
				psData.parsHistogram[BNF_assignmentStatement]++;
				expression();
				printf("%s%s\n", STR_LANGNAME, ": Assignment Statement parsed");
				printf("%s%s\n", STR_LANGNAME, ": Relational Expression parsed");
			}
			else if (lookahead.attribute.logicalOperator == OP_IS || lookahead.attribute.logicalOperator == OP_OR && lookahead.code == OP_T) {
				psData.parsHistogram[BNF_logicalExpression]++;
				if (lookahead.attribute.logicalOperator == OP_OR) {
					psData.parsHistogram[BNF_logicalOrExpression]++;
					matchToken(OP_T, NO_ATTR);
					expression();
					printf("%s%s\n", STR_LANGNAME, ": Logical OR Expression parsed");
				}
				else if (lookahead.attribute.logicalOperator == OP_IS) {
					psData.parsHistogram[BNF_logicalIsExpression]++;
					matchToken(OP_T, NO_ATTR);
					expression();
					printf("%s%s\n", STR_LANGNAME, ": Logical IS Expression parsed");;
				}
				printf("%s%s\n", STR_LANGNAME, ": Logical Expression parsed");
			}
			printf("%s%s\n", STR_LANGNAME, ": Arithmetic parsed");
		}
		printf("%s%s\n", STR_LANGNAME, ": Variable Expression parsed");
		printf("%s%s\n", STR_LANGNAME, ": Primary Expression parsed");
		break;
	case STR_T:
		psData.parsHistogram[BNF_stringExpression]++;
		matchToken(STR_T, NO_ATTR);
		if (lookahead.code == OP_T) {
			psData.parsHistogram[BNF_arithmeticExpression]++;
			if (lookahead.attribute.arithmeticOperator == OP_ADD || lookahead.attribute.arithmeticOperator == OP_SUB && lookahead.code == OP_T) {
				matchToken(OP_T, NO_ATTR);
				psData.parsHistogram[BNF_additiveArithmeticExpression]++;
				expression();
				printf("%s%s\n", STR_LANGNAME, ": Additive Arithmetic parsed");
			}
			else if (lookahead.attribute.arithmeticOperator == OP_MUL || lookahead.attribute.arithmeticOperator == OP_DIV || lookahead.attribute.arithmeticOperator == OP_MOD && lookahead.code == OP_T) {
				matchToken(OP_T, NO_ATTR);
				psData.parsHistogram[BNF_multiplicativeArithmeticExpression]++;
				expression();
				printf("%s%s\n", STR_LANGNAME, ": Multiplicative Arithmetic parsed");
			}
			else if (lookahead.attribute.arithmeticOperator == OP_EXP && lookahead.code == OP_T) {
				matchToken(OP_T, NO_ATTR);
				psData.parsHistogram[BNF_exponentialArithmeticExpression]++;
				expression();
				printf("%s%s\n", STR_LANGNAME, ": Exponential Arithmetic parsed");
			}
			else if (lookahead.attribute.relationalOperator == OP_EQ && lookahead.code == OP_T) {
				psData.parsHistogram[BNF_relationalOperator]++;
				matchToken(OP_T, NO_ATTR);
				printf("%s%s\n", STR_LANGNAME, ": Relational Operator parsed");
				// Conducting the other side of the equation
				psData.parsHistogram[BNF_relationalExpression]++;
				psData.parsHistogram[BNF_assignmentStatement]++;
				expression();
				printf("%s%s\n", STR_LANGNAME, ": Assignment Statement parsed");
				printf("%s%s\n", STR_LANGNAME, ": Relational Expression parsed");
			}
			else if (lookahead.attribute.logicalOperator == OP_IS || lookahead.attribute.logicalOperator == OP_OR && lookahead.code == OP_T) {
				psData.parsHistogram[BNF_logicalExpression]++;
				if (lookahead.attribute.logicalOperator == OP_OR) {
					psData.parsHistogram[BNF_logicalOrExpression]++;
					matchToken(OP_T, NO_ATTR);
					expression();
					printf("%s%s\n", STR_LANGNAME, ": Logical OR Expression parsed");
				}
				else if (lookahead.attribute.logicalOperator == OP_IS) {
					psData.parsHistogram[BNF_logicalIsExpression]++;
					matchToken(OP_T, NO_ATTR);
					expression();
					printf("%s%s\n", STR_LANGNAME, ": Logical IS Expression parsed");;
				}
				printf("%s%s\n", STR_LANGNAME, ": Logical Expression parsed");
			}
			printf("%s%s\n", STR_LANGNAME, ": Arithmetic parsed");
		}
		printf("%s%s\n", STR_LANGNAME, ": String Expression parsed");
		break;
	case LPR_T:
		matchToken(LPR_T, NO_ATTR);
		expression();
		matchToken(RPR_T, NO_ATTR);
		if (lookahead.code == OP_T) {
			psData.parsHistogram[BNF_arithmeticExpression]++;
			if (lookahead.attribute.arithmeticOperator == OP_ADD || lookahead.attribute.arithmeticOperator == OP_SUB && lookahead.code == OP_T) {
				matchToken(OP_T, NO_ATTR);
				psData.parsHistogram[BNF_additiveArithmeticExpression]++;
				expression();
				printf("%s%s\n", STR_LANGNAME, ": Additive Arithmetic parsed");
			}
			else if (lookahead.attribute.arithmeticOperator == OP_MUL || lookahead.attribute.arithmeticOperator == OP_DIV || lookahead.attribute.arithmeticOperator == OP_MOD && lookahead.code == OP_T) {
				matchToken(OP_T, NO_ATTR);
				psData.parsHistogram[BNF_multiplicativeArithmeticExpression]++;
				expression();
				printf("%s%s\n", STR_LANGNAME, ": Multiplicative Arithmetic parsed");
			}
			else if (lookahead.attribute.arithmeticOperator == OP_EXP && lookahead.code == OP_T) {
				matchToken(OP_T, NO_ATTR);
				psData.parsHistogram[BNF_exponentialArithmeticExpression]++;
				expression();
				printf("%s%s\n", STR_LANGNAME, ": Exponential Arithmetic parsed");
			}
			printf("%s%s\n", STR_LANGNAME, ": Arithmetic parsed");
		}
		printf("%s%s\n", STR_LANGNAME, ": Primary Expression parsed");
		break;
	case MNID_T:
		psData.parsHistogram[BNF_functionCallStatement]++;
		psData.parsHistogram[BNF_funcName]++;
		matchToken(MNID_T, NO_ATTR); 
		printf("%s%s\n", STR_LANGNAME, ": Function Name parsed"); 
		matchToken(LPR_T, NO_ATTR);
		if (lookahead.code == MNID_T || lookahead.code == STR_T) {
			expression();
		}
		matchToken(RPR_T, NO_ATTR);
		printf("%s%s\n", STR_LANGNAME, ": Function Call Statement parsed"); 
		break;
	case BLN_T:
		psData.parsHistogram[BNF_conditionalExpression]++;
		matchToken(BLN_T, 1);
		printf("%s%s\n", STR_LANGNAME, ": Conditional Expression parsed");
		break;
	default:
		// No expression
		break;
	}
	printf("%s%s\n", STR_LANGNAME, ": Expression parsed");
}

/*
 ************************************************************
 * Function name: outputStatement
 * Purpose: Verifys the patterns of print 
 * Called functions: matchToken(), outputVariableList(), printf()
 * Algorithm: Several matchToken calls with a outputVariableList call
 ***********************************************************
 */
mouse_None outputStatement() {
	psData.parsHistogram[BNF_outputStatement]++;
	psData.parsHistogram[BNF_functionCallStatement]++;
	psData.parsHistogram[BNF_funcName]++;
	matchToken(MNID_T, NO_ATTR);
	printf("%s%s\n", STR_LANGNAME, ": Function Name parsed");
	matchToken(LPR_T, NO_ATTR);
	psData.parsHistogram[BNF_expressionList]++;
	outputVariableList();
	printf("%s%s\n", STR_LANGNAME, ": Expression List parsed");
	matchToken(RPR_T, NO_ATTR);
	printf("%s%s\n", STR_LANGNAME, ": Function Call Statement parsed");
	printf("%s%s\n", STR_LANGNAME, ": Output statement parsed");
}

/*
 ************************************************************
 * Function name: outputVariableList
 * Purpose: Verifys the patterns of print's input variables
 * Called functions: matchToken()
 * Algorithm: Nested switch and while statements
 ***********************************************************
 */
mouse_None outputVariableList() {
	psData.parsHistogram[BNF_outputVariable]++;
	switch (lookahead.code) {
	case STR_T:
		matchToken(STR_T, NO_ATTR);
		while (lookahead.attribute.arithmeticOperator == OP_ADD && lookahead.code == OP_T){
			matchToken(OP_T, NO_ATTR);
			outputVariableList();
		}
		break;
	case VAR_T:
		matchToken(VAR_T, NO_ATTR);
		while (lookahead.attribute.arithmeticOperator == OP_ADD && lookahead.code == OP_T) {
			matchToken(OP_T, NO_ATTR);
			outputVariableList();
		}
		break;
	default:
		break;
	}
	printf("%s%s\n", STR_LANGNAME, ": Output variable parsed");
}

 /*
 ***********************************************************
 * Function name: comment()
 * Purpose: Checks for a comment
 * Called functions: matchToken(), printf()
 * Algorithm: matchToken for comment
 *************************************************************
 */
mouse_None comment() {
	psData.parsHistogram[BNF_comments]++;
	matchToken(CMT_T, NO_ATTR);
	printf("%s%s\n", STR_LANGNAME, ": Comment parsed");
}