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
* File name: Compilers.h
* Compiler: MS Visual Studio 2022
* Course: CST 8152 – Compilers, Lab Section: 011
* Assignment: A12
* Date: 01/29/24
* Professor: Paulo Sousa
* Purpose: This file is the main header for the functions called by main function (.h)
* Function list:
*    mainReader, mainScanner, mainParser, printLogo
************************************************************
*/

#ifndef COMPILERS_H_
#define COMPILERS_H_

#define DEBUG 0

/* Language name */
#define STR_LANGNAME	"Mouse"

/* Logical constants */
#define mouse_True  1
#define mouse_False 0

/*
------------------------------------------------------------
Data types definitions
NOTE: Some types may not be directly used by your language,
		but they can be necessary for conversions.
------------------------------------------------------------
*/

typedef char			mouse_char;
typedef char*			mouse_str;
typedef int				mouse_int;
typedef float			mouse_float;
typedef void			mouse_None;

typedef unsigned char	mouse_boln;
typedef unsigned char	mouse_byte;

typedef long			mouse_long;
typedef double			mouse_doub;

/*
------------------------------------------------------------
Programs:
1: Reader - invokes MainReader code
2: Scanner - invokes MainScanner code
3: Parser - invokes MainParser code
------------------------------------------------------------
*/
enum PROGRAMS {
	PGM_READER	= 'r',
	PGM_SCANNER = 's',
	PGM_PARSER	= 'p'
};

/*
------------------------------------------------------------
Main functions signatures
(Code will be updated during next assignments)
------------------------------------------------------------
*/
mouse_int mainReader(mouse_int argc, mouse_str* argv);

/* 
TO_DO: Include later mainScaner (A22) and mainParser (A32)
*/
mouse_None printLogo();

#endif
