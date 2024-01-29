# File name: CMakeList.txt
# Compiler: MS Visual Studio 2022
# Author(s): Colin Tapp 040774946 / Henry Forget 041023812 
# Course: CST 8152 – Compilers, Lab Section: 011
# Assignment: A12
# Date: 01/29/24
# Purpose: CMake project for Compilers, include source and define project specific logic here.

##_____________________________________________________#
#|         COMPILERS - ALGONQUIN COLLEGE (W24)         |
##_____________________________________________________#
#|                  .@@@@=                             |
#|               %*#.    -% +-                         |
#|         -.-@%+        .%.   .#.                     |
#|                      #         *                    |
#|                    :.           :+                  |
#|                    -             -.                 |
#|                   @               *                 |
#|                   =               -                 |
#|                  .                .+                |
#|                  *#-              .#                |
#|                +    ::          +-   @              |
#|                %     #         +.    .=             |
#|                +                     -              |
#|                 -=                  +               |
#|                    #             #                  |
#|                     #           +                   |
#|                        @#   %@ :-                   |
#|                      +         +                    |
#|                       #       +                     |
#|                        =:   --                      |
#|                          =@+                        |
#|                                                     |
#|/$$$$$$/$$$$   /$$$$$$  /$$   /$$  /$$$$$$$  /$$$$$$ |
#| $$_  $$_  $$ /$$__  $$| $$  | $$ /$$_____/ /$$__  $$|
#| $$ \ $$ \ $$| $$  \ $$| $$  | $$|  $$$$$$ | $$$$$$$$|
#| $$ | $$ | $$| $$  | $$| $$  | $$ \____  $$| $$_____/|
#| $$ | $$ | $$|  $$$$$$/|  $$$$$$/ /$$$$$$$/|  $$$$$$$|
#|__/_|__/_|__/_\______/__\______/_|_______/__\_______/|

cmake_minimum_required (VERSION 3.8)

project ("MouseCompiler")

add_executable (MouseCompiler
	"Compilers.h"	"Compilers.c"
	"Reader.h"		"Reader.c"		"MainReader.c"
	# TO_DO: Add A22 (Scanner) and A32 (Parser)
	#"Scanner.h"	"Scanner.c"		"MainScanner.c"
	#"Parser.h"		"Parser.c"		"MainParser.c"
)

# TO_DO: Add tests and install targets if needed.