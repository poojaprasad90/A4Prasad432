/*
************************************************************
* COMPILERS COURSE - Algonquin College
* Code version: Fall, 2025
* Author: Pooja Prasad
* Professors: Paulo Sousa
************************************************************
#
# ECHO "=---------------------------------------="
# ECHO "|  COMPILERS - ALGONQUIN COLLEGE (F25)  |"
# ECHO "=---------------------------------------="
# ECHO "    @@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@    �
# ECHO "    @@                             @@    �
# ECHO "    @@           %&@@@@@@@@@@@     @@    �
# ECHO "    @@       @%% (@@@@@@@@@  @     @@    �
# ECHO "    @@      @& @   @ @       @     @@    �
# ECHO "    @@     @ @ %  / /   @@@@@@     @@    �
# ECHO "    @@      & @ @  @@              @@    �
# ECHO "    @@       @/ @*@ @ @   @        @@    �
# ECHO "    @@           @@@@  @@ @ @      @@    �
# ECHO "    @@            /@@    @@@ @     @@    �
# ECHO "    @@     @      / /     @@ @     @@    �
# ECHO "    @@     @ @@   /@/   @@@ @      @@    �
# ECHO "    @@     @@@@@@@@@@@@@@@         @@    �
# ECHO "    @@                             @@    �
# ECHO "    @@         S C R I B E           @@    �
# ECHO "    @@                             @@    �
# ECHO "    @@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@    �
# ECHO "                                         "
# ECHO "[COMPILER SCRIPT .......................]"
# ECHO "                                         "
*/

/*
************************************************************
* File name: Compilers.h
* Compiler: MS Visual Studio 2022
* Course: CST 8152 � Compilers, Lab Section: [011, 012]
* Assignment: A1-A5.
* Date: Sep 01 2025
* Professor: Paulo Sousa
* Purpose: This file defines the functions called by main function.
* Function list: mainReader(), mainScanner(), mainParser().
*************************************************************/

#ifndef COMPILERS_H_
#define COMPILERS_H_

#define DEBUG 0

/*
 * ............................................................................
 *
 * ADVICE 2: This file must be updated according to each assignment
 * (see the progression: reader > scanner > parser).
 * ............................................................................
 */

/* Language name */
#define STR_LANGNAME	"Scribe"

/* Logical constants - adapt for your language */
#define SCRIBE_TRUE  1
#define SCRIBE_FALSE 0

#define INVALID NULL
#define EOF_CHAR '\0'

/*
------------------------------------------------------------
Data types definitions
NOTE: Some types may not be directly used by your language,
		but they can be necessary for conversions.
------------------------------------------------------------
*/

/* Define your typedefs */
typedef char			scribe_char;
typedef char*			scribe_strg;
typedef int				scribe_intg;
typedef float			scribe_real;
typedef void			scribe_void;

typedef unsigned char	scribe_boln;
typedef unsigned char	scribe_byte;

typedef long			scribe_long;
typedef double			scribe_doub;

/*
------------------------------------------------------------
Programs:
1: Coder - invokes Main1Coder code
2: Reader - invokes Main2Reader code
3: Scanner - invokes Main3Scanner code
4: Parser - invokes Main4Parser code
5: Writer - invokes Main5Writer code
------------------------------------------------------------
*/
enum PROGRAMS {
	PGM_CDR = '1', // Coder:	05pt
	PGM_RDR = '2', // Reader:	05pt
	PGM_SCN = '3', // Scanner:	15pt
	PGM_PSR = '4', // Parser:	15pt
	PGM_WRT = '5', // Writer:	10pt
};

/*
------------------------------------------------------------
Main functions signatures
(Code will be updated during next assignments)
------------------------------------------------------------
*/
scribe_intg main1Coder(scribe_intg argc, scribe_strg* argv);
scribe_intg main2Reader(scribe_intg argc, scribe_strg* argv);
scribe_intg main3Scanner(scribe_intg argc, scribe_strg* argv);
scribe_intg main4Parser(scribe_intg argc, scribe_strg* argv);

scribe_void printLogo();
scribe_void errorPrint(scribe_strg fmt, ...);

#endif

/*
	  "args": [
		1,
		1,
		"README.txt",
		"CODED.txt"
	  ]
	  "args": [
		4,
		"CODED.txt"
	  ]
*/
