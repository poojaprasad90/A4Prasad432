/*
************************************************************
* COMPILERS COURSE - Algonquin College
* Code version: Summer, 2026
* Author: Pooja Prasad
* Professors: Paulo Sousa
************************************************************
#
# ECHO "=---------------------------------------="
# ECHO "|  COMPILERS - ALGONQUIN COLLEGE (S26)  |"
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
* File name: Scanner.h
* Compiler: MS Visual Studio 2026
* Course: CST 8152 – Compilers, Lab Section: [301]
* Assignment: A22, A32.
* Date: Jul 01 2026
* Purpose: This file is the main header for Scanner (.h)
* Function list: (...).
*************************************************************/

#ifndef COMPILERS_H_
#include "Compilers.h"
#endif

#ifndef READER_H_
#include "Step2Reader.h"
#endif

#ifndef SCANNER_H_
#define SCANNER_H_

#ifndef NULL
#include <stddef.h> /* NULL pointer constant is defined there */
#endif

/*#pragma warning(1:4001) */	/*to enforce C89 type comments  - to make //comments an warning */

/*#pragma warning(error:4001)*/	/* to enforce C89 comments - to make // comments an error */

/* Constants */
#define VID_LEN 20  /* variable identifier length */
#define ERR_LEN 40  /* error message length */
#define NUM_LEN 5   /* maximum number of digits for IL */

#define RTE_CODE 1  /* Value for run-time error */

/* Define the number of tokens */
#define NUM_TOKENS 18

/* Define Token codes - Create your token classes */
enum TOKENS {
	ERR_T,		/*  0: Error token */
	MNID_T,		/*  1: Method name identifier token (start: &) */
	INL_T,		/*  2: Integer literal token */
	STR_T,		/*  3: String literal token */
	LPR_T,		/*  4: Left parenthesis token */
	RPR_T,		/*  5: Right parenthesis token */
	LBR_T,		/*  6: Left brace token */
	RBR_T,		/*  7: Right brace token */
	KW_T,		/*  8: Keyword token */
	EOS_T,		/*  9: End of statement (semicolon) */
	RTE_T,		/* 10: Run-time error token */
	SEOF_T,		/* 11: Source end-of-file token */
	CMT_T		/* 12: Comment token */,
	/* Extended tokens (A3): identifiers, literals, operators */
	AVID_T,		/* 13: Variable (arithmetic) identifier token */
	FPL_T,		/* 14: Floating-point literal token */
	ASS_T,		/* 15: Assignment operator token */
	ART_T,		/* 16: Arithmetic operator token */
	REL_T		/* 17: Relational operator token */
};

/* Define the list of keywords */
static scribe_strg tokenStrTable[NUM_TOKENS] = {
	"ERR_T",
	"MNID_T",
	"INL_T",
	"STR_T",
	"LPR_T",
	"RPR_T",
	"LBR_T",
	"RBR_T",
	"KW_T",
	"EOS_T",
	"RTE_T",
	"SEOF_T",
	"CMT_T",
	"AVID_T",
	"FPL_T",
	"ASS_T",
	"ART_T",
	"REL_T"
};

/* Operators token attributes */
typedef enum ArithmeticOperators { OP_ADD, OP_SUB, OP_MUL, OP_DIV } AriOperator;
typedef enum RelationalOperators { OP_EQ, OP_NE, OP_GT, OP_LT } RelOperator;
typedef enum LogicalOperators { OP_AND, OP_OR, OP_NOT } LogOperator;
typedef enum SourceEndOfFile { SEOF_0, SEOF_255 } EofOperator;

/* Data structures for declaring the token and its attributes */
typedef union TokenAttribute {
	scribe_intg codeType;      /* integer attributes accessor */
	AriOperator arithmeticOperator;		/* arithmetic operator attribute code */
	RelOperator relationalOperator;		/* relational operator attribute code */
	LogOperator logicalOperator;		/* logical operator attribute code */
	EofOperator seofType;				/* source-end-of-file attribute code */
	scribe_intg intValue;				/* integer literal attribute (value) */
	scribe_intg keywordIndex;			/* keyword index in the keyword table */
	scribe_intg contentString;			/* string literal offset from the beginning of the string literal buffer (stringLiteralTable->content) */
	scribe_real floatValue;				/* floating-point literal attribute (value) */
	scribe_char idLexeme[VID_LEN + 1];	/* variable identifier token attribute */
	scribe_char errLexeme[ERR_LEN + 1];	/* error token attribite */
} TokenAttribute;

/* Should be used if no symbol table is implemented */
typedef struct idAttibutes {
	scribe_byte flags;			/* Flags information */
	union {
		scribe_intg intValue;				/* Integer value */
		scribe_real floatValue;			/* Float value */
		scribe_strg stringContent;		/* String value */
	} values;
} IdAttibutes;

/* Token declaration */
typedef struct Token {
	scribe_intg code;				/* token code */
	TokenAttribute attribute;	/* token attribute */
	IdAttibutes   idAttribute;	/* not used in this scanner implementation - for further use */
} Token;

/* Scanner */
typedef struct scannerData {
	scribe_intg scanHistogram[NUM_TOKENS];	/* Statistics of chars */
} ScannerData, * pScanData;

///////////////////////////////////////////////////////////////////////////////////////////////////////

/* Define lexeme FIXED classes */
/* EOF definitions */
#define EOS_CHR '\0'	// CH00
#define EOF_CHR 0xFF	// CH01
#define UND_CHR '_'		// CH02
#define AMP_CHR '$'		// CH03 (method marker)
#define QUT_CHR '"'	// CH04 (string delimiter)
#define HST_CHR '~'		// CH05 (comment delimiter)
#define TAB_CHR '\t'	// CH06
#define SPC_CHR ' '		// CH07
#define NWL_CHR '\n'	// CH08
#define SCL_CHR ';'		// CH09
#define LPR_CHR '('		// CH10
#define RPR_CHR ')'		// CH11
#define LBR_CHR '{'		// CH12
#define RBR_CHR '}'		// CH13
#define PRD_CHR '.'		// CH14 (period / decimal point)
#define ASS_CHR '='		// CH15 (assignment operator)
#define ADD_CHR '+'		// CH16 (arithmetic)
#define SUB_CHR '-'		// CH17 (arithmetic)
#define MUL_CHR '*'		// CH18 (arithmetic)
#define DIV_CHR '/'		// CH19 (arithmetic)
#define LTH_CHR '<'		// CH20 (relational)
#define GTH_CHR '>'		// CH21 (relational)

/*  Special case tokens processed separately one by one in the token-driven part of the scanner:
 *  LPR_T, RPR_T, LBR_T, RBR_T, EOS_T, SEOF_T and special chars used for tokenis include _, & and ' */


/* Error states and illegal state */
#define ESNR	8		/* Error state with no retract */
#define ESWR	9		/* Error state with retract */
#define FS		99		/* Illegal state */

 /* State transition table definition */
#define NUM_STATES		15
#define CHAR_CLASSES	9

/* Transition table - type of states defined in separate table */
static scribe_intg transitionTable[NUM_STATES][CHAR_CLASSES] = {
/*    L(0), D(1), U(2), M(3), Q(4), E(5), C(6), P(7), O(8)
      [A-z] [0-9]   _     &     '   SEOF   #     .    other */
	{     1,   10, ESNR, ESNR,    4, ESWR,    6, ESNR, ESNR},	// S00: NOAS (start)
	{     1,    1,    1,    2,    3,    3,    3,    3,    3},	// S01: NOAS (identifier body)
	{    FS,   FS,   FS,   FS,   FS,   FS,   FS,   FS,   FS},	// S02: ASNR (MVID - method id)
	{    FS,   FS,   FS,   FS,   FS,   FS,   FS,   FS,   FS},	// S03: ASWR (VID / keyword)
	{     4,    4,    4,    4,    5, ESWR,    4,    4,    4},	// S04: NOAS (string body)
	{    FS,   FS,   FS,   FS,   FS,   FS,   FS,   FS,   FS},	// S05: ASNR (string literal)
	{     6,    6,    6,    6,    6, ESWR,    7,    6,    6},	// S06: NOAS (comment body)
	{    FS,   FS,   FS,   FS,   FS,   FS,   FS,   FS,   FS},	// S07: ASNR (comment)
	{    FS,   FS,   FS,   FS,   FS,   FS,   FS,   FS,   FS},	// S08: ASNR (error, no retract)
	{    FS,   FS,   FS,   FS,   FS,   FS,   FS,   FS,   FS},	// S09: ASWR (error, retract)
	{    13,   10,   13,   13,   13,   13,   13,   11,   13},	// S10: NOAS (integer body)
	{  ESWR,   12, ESWR, ESWR, ESWR, ESWR, ESWR, ESWR, ESWR},	// S11: NOAS (seen '.', need digit)
	{    14,   12,   14,   14,   14,   14,   14,   14,   14},	// S12: NOAS (fraction body)
	{    FS,   FS,   FS,   FS,   FS,   FS,   FS,   FS,   FS},	// S13: ASWR (integer literal)
	{    FS,   FS,   FS,   FS,   FS,   FS,   FS,   FS,   FS}	// S14: ASWR (floating-point literal)
};

/* Define accepting states types */
#define NOFS	0		/* not accepting state */
#define FSNR	1		/* accepting state with no retract */
#define FSWR	2		/* accepting state with retract */

/* Define list of acceptable states */
static scribe_intg stateType[NUM_STATES] = {
	NOFS, /* 00 */
	NOFS, /* 01 */
	FSNR, /* 02 (MVID) - methods */
	FSWR, /* 03 (VID / keyword) */
	NOFS, /* 04 */
	FSNR, /* 05 (SL) */
	NOFS, /* 06 */
	FSNR, /* 07 (COM) */
	FSNR, /* 08 (Err1 - no retract) */
	FSWR, /* 09 (Err2 - retract) */
	NOFS, /* 10 (integer body) */
	NOFS, /* 11 (seen '.') */
	NOFS, /* 12 (fraction body) */
	FSWR, /* 13 (IL - integer literal) */
	FSWR  /* 14 (FPL - floating-point literal) */
};

/*
-------------------------------------------------
Adjust your functions'definitions
-------------------------------------------------
*/

/* Static (local) function  prototypes */
scribe_intg			startScanner(BufferPointer psc_buf);
static scribe_intg	nextClass(scribe_char c);					/* character class function */
static scribe_intg	nextState(scribe_intg, scribe_char);		/* state machine function */
scribe_void			printScannerData(ScannerData scData);
Token				tokenizer(scribe_void);

/*
-------------------------------------------------
Automata definitions
-------------------------------------------------
*/

/* Pointer to function (of one char * argument) returning Token */
typedef Token(*PTR_ACCFUN)(scribe_strg lexeme);

/* Declare accepting states functions */
Token funcSL	(scribe_strg lexeme);
Token funcIL	(scribe_strg lexeme);
Token funcID	(scribe_strg lexeme);
Token funcCMT   (scribe_strg lexeme);
Token funcKEY	(scribe_strg lexeme);
Token funcErr	(scribe_strg lexeme);
Token funcFPL	(scribe_strg lexeme);

/* 
 * Accepting function (action) callback table (array) definition 
 * If you do not want to use the typedef, the equvalent declaration is:
 */

/* Define final state table */
static PTR_ACCFUN finalStateTable[NUM_STATES] = {
	NULL,		/* -    [00] */
	NULL,		/* -    [01] */
	funcID,		/* MVID [02] */
	funcID,		/* VID/KEY [03] */
	NULL,		/* -    [04] */
	funcSL,		/* SL   [05] */
	NULL,		/* -    [06] */
	funcCMT,	/* COM  [07] */
	funcErr,	/* ERR1 [08] */
	funcErr,	/* ERR2 [09] */
	NULL,		/* -    [10] */
	NULL,		/* -    [11] */
	NULL,		/* -    [12] */
	funcIL,		/* IL   [13] */
	funcFPL		/* FPL  [14] */
};

/*
-------------------------------------------------
Language keywords
-------------------------------------------------
*/

/* Define the number of Keywords from the language */
#define KWT_SIZE 11

/* Define the list of keywords */
static scribe_strg keywordTable[KWT_SIZE] = {
	"vault",		/* KW00 - declarations block */
	"logic",	/* KW01 - statements block */
	"whole",	/* KW02 - integer type */
	"deci",		/* KW03 - real type */
	"text",		/* KW04 - string type */
	"check",	/* KW05 - if */
	"so",		/* KW06 - then */
	"alt",		/* KW07 - else */
	"during",	/* KW08 - while */
	"run",		/* KW09 - do */
	"giveback"	/* KW10 - return */
};

/* NEW SECTION: About indentation */

/*
 * Scanner attributes to be used (ex: including: intendation data
 */

#define INDENT TAB_CHR  /* Tabulation */

/* Should be used if no symbol table is implemented */
typedef struct languageAttributes {
	scribe_char indentationCharType;
	scribe_intg indentationCurrentPos;
	/* Include any extra attribute to be used in your scanner (OPTIONAL and FREE) */
} LanguageAttributes;

/* Number of errors */
scribe_intg numScannerErrors;

/* Scanner data */
ScannerData scData;

#endif
