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
# ECHO "    @@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@    ”
# ECHO "    @@                             @@    ”
# ECHO "    @@           %&@@@@@@@@@@@     @@    ”
# ECHO "    @@       @%% (@@@@@@@@@  @     @@    ”
# ECHO "    @@      @& @   @ @       @     @@    ”
# ECHO "    @@     @ @ %  / /   @@@@@@     @@    ”
# ECHO "    @@      & @ @  @@              @@    ”
# ECHO "    @@       @/ @*@ @ @   @        @@    ”
# ECHO "    @@           @@@@  @@ @ @      @@    ”
# ECHO "    @@            /@@    @@@ @     @@    ”
# ECHO "    @@     @      / /     @@ @     @@    ”
# ECHO "    @@     @ @@   /@/   @@@ @      @@    ”
# ECHO "    @@     @@@@@@@@@@@@@@@         @@    ”
# ECHO "    @@                             @@    ”
# ECHO "    @@         S C R I B E           @@    ”
# ECHO "    @@                             @@    ”
# ECHO "    @@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@    ”
# ECHO "                                         "
# ECHO "[READER SCRIPT .........................]"
# ECHO "                                         "
*/

/*
************************************************************
* File name: Reader.h
* Compiler: MS Visual Studio 2022
* Course: CST 8152 – Compilers, Lab Section: [011, 012]
* Assignment: A12.
* Date: Jan 01 2025
* Professor: Paulo Sousa
* Purpose: This file is the main header for Reader (.h)
************************************************************
*/

/*
 *.............................................................................
 * MAIN ADVICE:
 *.............................................................................
 */

#ifndef COMPILERS_H_
#include "Compilers.h"
#endif

#ifndef READER_H_
#define READER_H_

 /* TIP: Do not change pragmas, unless necessary .......................................*/
 /*#pragma warning(1:4001) *//*to enforce C89 type comments  - to make //comments an warning */
 /*#pragma warning(error:4001)*//* to enforce C89 comments - to make // comments an error */

 /* standard header files */
#include <stdio.h>  /* standard input/output */
#include <stdlib.h> /* for dynamic memory allocation*/
#include <limits.h> /* implementation-defined data type ranges and limits */

/* CONSTANTS DEFINITION: GENERAL (NOT LANGUAGE DEPENDENT) .................................. */

/* Constants about controls (not need to change) */
#define READER_ERROR		(-1)		/* General error message */
#define READER_TERMINATOR	'\0'		/* General EOF */

/* CONSTANTS DEFINITION: PREFIXED BY LANGUAGE NAME .................................. */

/* You should add your own constant definitions here */
#define READER_MAX_SIZE		INT_MAX-1	/* maximum capacity */ 

#define READER_DEFAULT_SIZE		100			/* default initial buffer reader capacity */
#define READER_DEFAULT_FACTOR	0.5f		/* default factor */

/* Add your bit-masks constant definitions here */
/*
// BITS                             (    76543210)
#define READER_DEFAULT_FLAG 0x00 	// (0b00000000) = (0x00)_16 = (000)_10
// BIT 3: END = End of buffer flag
#define READER_SET_FLAG_END 0x08	// (0b00001000) = (0x08)_16 = (008)_10
// BIT 2: REL = Rellocation memory flag
#define READER_SET_FLAG_REL 0x04	// (0b00000100) = (0x04)_16 = (004)_10
// BIT 1: EMP = Buffer empty flag
#define READER_SET_FLAG_FUL 0x02	// (0b00000010) = (0x02)_16 = (002)_10
// BIT 0: FUL = Buffer full flag
#define READER_SET_FLAG_EMP 0x01	// (0b00000001) = (0x01)_16 = (001)_10
*/

#define NCHAR				128			/* Chars from 0 to 127 */

#define CHARSEOF			(-1)		/* EOF Code for Reader */

/* STRUCTURES DEFINITION: SUFIXED BY LANGUAGE NAME .................................. */

/* Adjust datatypes */

/* Offset declaration */
typedef struct position {
	scribe_intg wrte;					/* the offset to the add chars (in chars) */
	scribe_intg read;					/* the offset to the get a char position (in chars) */
	scribe_intg mark;					/* the offset to the mark position (in chars) */
} Position;

/* Flags declaration */
typedef struct flag {
	scribe_boln isEmpty;					/* checks if there is no content */
	scribe_boln isFull;					/* the content is using all size */
	scribe_boln isRead;					/* all content was read */
	scribe_boln isMoved;					/* the content was moved in reallocation */
} Flag;

/* Buffer structure */
typedef struct bufferReader {
	scribe_strg		content;			/* pointer to the beginning of character array (character buffer) */
	scribe_intg		size;				/* current dynamic memory size (in bytes) allocated to character buffer */
	scribe_real		factor;				/* factor for increase the buffer */
	Flag			flags;				/* contains character array reallocation flag and end-of-buffer flag */
	Position		position;			/* Offset / position field */
	scribe_intg		histogram[NCHAR];	/* Statistics of chars */
	scribe_intg		numReaderErrors;	/* Number of errors from Reader */
	scribe_intg		checkSum;			/* Sum of bytes(chars) */
} Buffer, * BufferPointer;

/* FUNCTIONS DECLARATION:  .................................. */

/* General Operations */
BufferPointer	readerCreate(scribe_intg, scribe_real);
BufferPointer	readerAddChar(BufferPointer const, scribe_char);
scribe_boln		readerClear(BufferPointer const);
scribe_boln		readerFree(BufferPointer const);
scribe_boln		readerIsFull(BufferPointer const);
scribe_boln		readerIsEmpty(BufferPointer const);
scribe_boln		readerSetMark(BufferPointer const, scribe_intg);
scribe_intg		readerPrint(BufferPointer const);
scribe_intg		readerLoad(BufferPointer const, scribe_strg);
scribe_boln		readerRecover(BufferPointer const);
scribe_boln		readerRetract(BufferPointer const);
scribe_boln		readerRestore(BufferPointer const);
scribe_intg		readerChecksum(BufferPointer const);
/* Getters */
scribe_char		readerGetChar(BufferPointer const);
scribe_strg		readerGetContent(BufferPointer const, scribe_intg);
scribe_intg		readerGetPosRead(BufferPointer const);
scribe_intg		readerGetPosWrte(BufferPointer const);
scribe_intg		readerGetPosMark(BufferPointer const);
scribe_intg		readerGetSize(BufferPointer const);
scribe_void		readerPrintFlags(BufferPointer const);
scribe_void		readerPrintStat(BufferPointer const);
scribe_intg		readerNumErrors(BufferPointer const);

#endif
