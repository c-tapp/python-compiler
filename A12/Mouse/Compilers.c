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
* File name: Compilers.c
* Compiler: MS Visual Studio 2022
* Course: CST 8152 – Compilers, Lab Section: 011
* Assignment: A12
* Date: 01/29/24
* Professor: Paulo Sousa
* Purpose: This file is the main header for the functions called by main function (.c)
* Function list:
*    main, printLogo
************************************************************
*/


#include <stdio.h>
#include <stdlib.h>

#ifndef COMPILERS_H_
#include "Compilers.h"
#endif

/*
***********************************************************
* Function name: main
* Purpose: Main function
* Author: Paulo Sousa
* History/Versions: Ver F22
* Called functions: mainReader(), mainScanner(), mainParser()
* Parameters: Command line arguments - argc, argv
* Return value: Status
* Algorithm: -
*************************************************************
*/

mouse_int main(int argc, char** argv) {
	mouse_int i;
	printLogo();
	if (DEBUG) {
		for (i = 0; i < argc; ++i)
			printf("argv[%d] = %s\n", i, argv[i]);
	}
	if (argc < 2) {
		printf("%s%c%s%c%s%c%s", "OPTIONS:\n* [",
			PGM_READER, "] - Reader\n* [",
			PGM_SCANNER, "] - Scanner\n* [",
			PGM_PARSER, "] - Parser\n");
		return EXIT_FAILURE;
	}
	mouse_char option = argv[1][0];
	switch (option) {
	case PGM_READER:
		printf("%s%c%s", "\n[Option '", PGM_READER, "': Starting READER .....]\n\n");
		mainReader(argc, argv);
		break;
	/* TO_DO: Include later SCANNER (A22) and PARSER (A32) */
	default:
		printf("%s%c%s%c%s%c%s", "* OPTIONS:\n- [",
			PGM_READER, "] - Reader\n- [",
			PGM_SCANNER, "] - Scanner\n- [",
			PGM_PARSER, "] - Parser\n");
		break;
	}
	return EXIT_SUCCESS;
}

/*
***********************************************************
* Function name: printLogo
* Purpose: Print Logo
* Author: Paulo Sousa
* History/Versions: Ver S23
* Called functions: -
* Parameters: -
* Return value: (Null)
* Algorithm: -
*************************************************************
*/

mouse_None printLogo() {
	printf("%s%s%s%s%s%s%s%s%s%s%s%s%s%s%s%s%s%s%s%s%s%s%s%s%s%s%s%s%s%s%s",
		"\t#-----------------------------------------------------#\n",
		"\t|         COMPILERS - ALGONQUIN COLLEGE (W24)         |\n",
		"\t#_____________________________________________________#\n",
		"\t|                     .@@@@=                          |\n",
		"\t|                  %*#.    -% +-                      |\n",
		"\t|            -.-@%+        .%.   .#.                  |\n",
		"\t|                         #         *                 |\n",
		"\t|                       :.           :+               |\n",
		"\t|                       -             -.              |\n",
		"\t|                      @               *              |\n",
		"\t|                      =               -              |\n",
		"\t|                     .                .+             |\n",
		"\t|                     *#-              .#             |\n",
		"\t|                   +    ::          +-   @           |\n",
		"\t|                   %     #         +.    .=          |\n",
		"\t|                   +                     -           |\n",
		"\t|                    -=                  +            |\n",
		"\t|                       #             #               |\n",
		"\t|                        #           +                |\n",
		"\t|                           @#   %@ :-                |\n",
		"\t|                         +         +                 |\n",
		"\t|                          #       +                  |\n",
		"\t|                           =:   --                   |\n",
		"\t|                             =@+                     |\n",
		"\t|                                                     |\n",
		"\t|/$$$$$$/$$$$   /$$$$$$  /$$   /$$  /$$$$$$$  /$$$$$$ |\n",
		"\t| $$_  $$_  $$ /$$__  $$| $$  | $$ /$$_____/ /$$__  $$|\n",
		"\t| $$ \ $$ \ $$| $$  \ $$| $$  | $$|  $$$$$$ | $$$$$$$$|\n",
		"\t| $$ | $$ | $$| $$  | $$| $$  | $$ \____  $$| $$_____/|\n",
		"\t| $$ | $$ | $$|  $$$$$$/|  $$$$$$/ /$$$$$$$/|  $$$$$$$|\n",
		"\t|__/_|__/_|__/_\______/__\______/_|_______/__\_______/|\n"
	);
}
