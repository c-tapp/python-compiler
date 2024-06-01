# File name: CMakeList.txt
# Compiler: MS Visual Studio 2022
# Author(s): Colin Tapp / Henry Forget 
# Date: 03/05/24

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

# Enable Hot Reload for MSVC compilers if supported.
if (POLICY CMP0141)
  cmake_policy(SET CMP0141 NEW)
  set(CMAKE_MSVC_DEBUG_INFORMATION_FORMAT "$<IF:$<AND:$<C_COMPILER_ID:MSVC>,$<CXX_COMPILER_ID:MSVC>>,$<$<CONFIG:Debug,RelWithDebInfo>:EditAndContinue>,$<$<CONFIG:Debug,RelWithDebInfo>:ProgramDatabase>>")
endif()

project ("Mouse")

add_executable (Mouse
	"Compilers.h"	"Compilers.c"
	"Reader.h"	"Reader.c"	"MainReader.c"
	"Scanner.h"	"Scanner.c"	"MainScanner.c"
)

