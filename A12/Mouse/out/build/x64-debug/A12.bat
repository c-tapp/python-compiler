:: ---------------------------------------------------------------------
:: COMPILERS COURSE - SCRIPT                                           -
:: SCRIPT A12 - CST8152 - Winter 2024                                  -
:: ---------------------------------------------------------------------

CLS
SET COMPILER=Mouse.exe

SET FILE1=INPUT0_Empty
SET FILE2=INPUT1_Hello
SET FILE3=INPUT2_Volume
SET FILE4=INPUT3_Factorial
SET FILE5=INPUT4_Menu
SET FILE6=INPUT5_Big

SET ASSIGNMENT=A12
SET EXTENSION=mse
SET OUTPUT=out
SET ERROR=err

SET PARAM=r

:: ---------------------------------------------------------------------
:: Begin of Tests (A12 - W24) ------------------------------------------
:: ---------------------------------------------------------------------

@echo off

ECHO "#_____________________________________________________#"
ECHO "|         COMPILERS - ALGONQUIN COLLEGE (W24)         |"
ECHO "#_____________________________________________________#"
ECHO "|                  .@@@@=                             |"
ECHO "|               %*#.    -% +-                         |"
ECHO "|         -.-@%+        .%.   .#.                     |"
ECHO "|                      #         *                    |"
ECHO "|                    :.           :+                  |"
ECHO "|                    -             -.                 |"
ECHO "|                   @               *                 |"
ECHO "|                   =               -                 |"
ECHO "|                  .                .+                |"
ECHO "|                  *#-              .#                |"
ECHO "|                +    ::          +-   @              |"
ECHO "|                %     #         +.    .=             |"
ECHO "|                +                     -              |"
ECHO "|                 -=                  +               |"
ECHO "|                    #             #                  |"
ECHO "|                     #           +                   |"
ECHO "|                        @#   %@ :-                   |"
ECHO "|                      +         +                    |"
ECHO "|                       #       +                     |"
ECHO "|                        =:   --                      |"
ECHO "|                          =@+                        |"
ECHO "|                                                     |"
ECHO "|/$$$$$$/$$$$   /$$$$$$  /$$   /$$  /$$$$$$$  /$$$$$$ |"
ECHO "| $$_  $$_  $$ /$$__  $$| $$  | $$ /$$_____/ /$$__  $$|"
ECHO "| $$ \ $$ \ $$| $$  \ $$| $$  | $$|  $$$$$$ | $$$$$$$$|"
ECHO "| $$ | $$ | $$| $$  | $$| $$  | $$ \____  $$| $$_____/|"
ECHO "| $$ | $$ | $$|  $$$$$$/|  $$$$$$/ /$$$$$$$/|  $$$$$$$|"
ECHO "|__/_|__/_|__/_\______/__\______/_|_______/__\_______/|"
ECHO "                                   		     "
ECHO "[READER SCRIPT .......................................]"
ECHO "                                                       "

ren *.exe %COMPILER%

::
:: BASIC TESTS  ----------------------------------------------------------
::
:: Basic Tests (A12 - W24) - - - - - - - - - - - - - - - - - - - - - -

%COMPILER% %PARAM% %FILE1%.%EXTENSION%	> %FILE1%-%ASSIGNMENT%.%OUTPUT%	2> %FILE1%-%ASSIGNMENT%.%ERROR%
%COMPILER% %PARAM% %FILE2%.%EXTENSION%	> %FILE2%-%ASSIGNMENT%.%OUTPUT%	2> %FILE2%-%ASSIGNMENT%.%ERROR%
%COMPILER% %PARAM% %FILE3%.%EXTENSION%	a 250 10	> %FILE3%-%ASSIGNMENT%.%OUTPUT%	2> %FILE3%-%ASSIGNMENT%.%ERROR%
%COMPILER% %PARAM% %FILE4%.%EXTENSION%	> %FILE4%-%ASSIGNMENT%.%OUTPUT%	2> %FILE4%-%ASSIGNMENT%.%ERROR%
%COMPILER% %PARAM% %FILE5%.%EXTENSION%	a 250 10	> %FILE5%-%ASSIGNMENT%.%OUTPUT%	2> %FILE5%-%ASSIGNMENT%.%ERROR%

::
:: ADVANCED TESTS  -------------------------------------------------------
::
:: Advanced Tests (A12 - W24) - - - - - - - - -- - - - - - - - - - - -

%COMPILER% %PARAM% %FILE6%.%EXTENSION%	f 100 10	> %FILE6%-%ASSIGNMENT%-f-100-10.%OUTPUT%	2> %FILE6%-%ASSIGNMENT%-f-100-10.%ERROR%
%COMPILER% %PARAM% %FILE6%.%EXTENSION%	a 100 10	> %FILE6%-%ASSIGNMENT%-a-100-10.%OUTPUT%	2> %FILE6%-%ASSIGNMENT%-a-100-10.%ERROR%
%COMPILER% %PARAM% %FILE6%.%EXTENSION%	m 100 10	> %FILE6%-%ASSIGNMENT%-m-100-10.%OUTPUT%	2> %FILE6%-%ASSIGNMENT%-m-100-10.%ERROR%

:: SHOW OUTPUTS - - - - - - - - - - - - - - - - - - - - - - - - - - -
DIR *.OUT
DIR *.ERR

:: ---------------------------------------------------------------------
:: End of Tests (A12 - W24) --------------------------------------------
:: ---------------------------------------------------------------------
