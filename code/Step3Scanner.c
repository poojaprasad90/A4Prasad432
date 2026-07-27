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
# ECHO "[READER SCRIPT .........................]"
# ECHO "                                         "
*/

/*
************************************************************
* File name: Scanner.c
* Compiler: MS Visual Studio 2026
* Course: CST 8152 � Compilers, Lab Section: [301]
* Assignment: A1, A2.
* Date:	Jul 01 2026
* Purpose: This file contains all functionalities from Scanner.
* Function list: (...).
************************************************************
*/

/* Adjust the function header */

 /* The #define _CRT_SECURE_NO_WARNINGS should be used in MS Visual Studio projects
  * to suppress the warnings about using "unsafe" functions like fopen()
  * and standard sting library functions defined in string.h.
  * The define does not have any effect in Borland compiler projects.
  */
#define _CRT_SECURE_NO_WARNINGS

#include <stdio.h>   /* standard input / output */
#include <ctype.h>   /* conversion functions */
#include <stdlib.h>  /* standard library functions and constants */
#include <string.h>  /* string functions */
#include <limits.h>  /* integer types constants */
#include <float.h>   /* floating-point types constants */

/* #define NDEBUG to suppress assert() call */
#include <assert.h>  /* assert() prototype */

/* project header files */

#ifndef COMPILERS_H_
#include "Compilers.h"
#endif

#ifndef BUFFER_H_
#include "Step2Reader.h"
#endif

#ifndef SCANNER_H_
#include "Step3Scanner.h"
#endif

/*
----------------------------------------------------------------
Global vars definitions
----------------------------------------------------------------
*/

/* Global objects - variables */
/* This buffer is used as a repository for string literals. */
extern BufferPointer stringLiteralTable;	/* String literal table */
scribe_intg line;								/* Current line number of the source code */
extern scribe_intg errorNumber;				/* Defined in platy_st.c - run-time error number */

extern scribe_intg stateType[NUM_STATES];
extern scribe_strg keywordTable[KWT_SIZE];

extern PTR_ACCFUN finalStateTable[NUM_STATES];
extern scribe_intg transitionTable[NUM_STATES][CHAR_CLASSES];

/* Local(file) global objects - variables */
static BufferPointer lexemeBuffer;			/* Pointer to temporary lexeme buffer */
static BufferPointer sourceBuffer;			/* Pointer to input source buffer */

/*
 ************************************************************
 * Intitializes scanner
 *		This function initializes the scanner using defensive programming.
 ***********************************************************
 */
 /* Follow the standard and adjust datatypes */

scribe_intg startScanner(BufferPointer psc_buf) {
	/* Start histogram */
	for (scribe_intg i=0; i<NUM_TOKENS;i++)
		scData.scanHistogram[i] = 0;
	/* Basic scanner initialization */
	/* in case the buffer has been read previously  */
	readerRecover(psc_buf);
	readerClear(stringLiteralTable);
	line = 1;
	sourceBuffer = psc_buf;
	return EXIT_SUCCESS; /*0*/
}

/*
 ************************************************************
 * Process Token
 *		Main function of buffer, responsible to classify a char (or sequence
 *		of chars). In the first part, a specific sequence is detected (reading
 *		from buffer). In the second part, a pattern (defined by Regular Expression)
 *		is recognized and the appropriate function is called (related to final states 
 *		in the Transition Diagram).
 ***********************************************************
 */

Token tokenizer(scribe_void) {

	/* Follow the standard and adjust datatypes */

	Token currentToken = { 0 }; /* token to return after pattern recognition. Set all structure members to 0 */
	scribe_char c;			/* input symbol */
	scribe_intg state = 0;	/* initial state of the FSM */
	scribe_intg lexStart;	/* start offset of a lexeme in the input char buffer (array) */
	scribe_intg lexEnd;		/* end offset of a lexeme in the input char buffer (array)*/

	scribe_intg lexLength;	/* token length */
	scribe_intg i;			/* counter */
	///scribe_char newc;		// new char

	/* Starting lexeme */
	scribe_strg lexeme;	/* lexeme (to check the function) */
	lexeme = (scribe_strg)malloc(VID_LEN * sizeof(scribe_char));
	if (!lexeme)
		return currentToken;
	lexeme[0] = EOS_CHR;

	while (1) { /* endless loop broken by token returns it will generate a warning */
		c = readerGetChar(sourceBuffer);

		// Defensive programming
		if (c < 0 || c >= NCHAR)
			return currentToken;

		/* ------------------------------------------------------------------------
			Part 1: Implementation of token driven scanner.
			Every token is possessed by its own dedicated code
			-----------------------------------------------------------------------
		*/

		/* All patterns that do not require accepting functions */
		switch (c) {

		/* Cases for spaces */
		case SPC_CHR:
		case TAB_CHR:
			break;
		case NWL_CHR:
			line++;
			break;

		/* Cases for symbols */
		case SCL_CHR:
			currentToken.code = EOS_T;
			scData.scanHistogram[currentToken.code]++;
			return currentToken;
		case LPR_CHR:
			currentToken.code = LPR_T;
			scData.scanHistogram[currentToken.code]++;
			return currentToken;
		case RPR_CHR:
			currentToken.code = RPR_T;
			scData.scanHistogram[currentToken.code]++;
			return currentToken;
		case LBR_CHR:
			currentToken.code = LBR_T;
			scData.scanHistogram[currentToken.code]++;
			return currentToken;
		case RBR_CHR:
			currentToken.code = RBR_T;
			scData.scanHistogram[currentToken.code]++;
			return currentToken;
		/* Cases for operators (single-char) */
		case ASS_CHR:
			currentToken.code = ASS_T;
			scData.scanHistogram[currentToken.code]++;
			return currentToken;
		case ADD_CHR:
			currentToken.code = ART_T;
			currentToken.attribute.arithmeticOperator = OP_ADD;
			scData.scanHistogram[currentToken.code]++;
			return currentToken;
		case SUB_CHR:
			currentToken.code = ART_T;
			currentToken.attribute.arithmeticOperator = OP_SUB;
			scData.scanHistogram[currentToken.code]++;
			return currentToken;
		case MUL_CHR:
			currentToken.code = ART_T;
			currentToken.attribute.arithmeticOperator = OP_MUL;
			scData.scanHistogram[currentToken.code]++;
			return currentToken;
		case DIV_CHR:
			currentToken.code = ART_T;
			currentToken.attribute.arithmeticOperator = OP_DIV;
			scData.scanHistogram[currentToken.code]++;
			return currentToken;
		case LTH_CHR:
			currentToken.code = REL_T;
			currentToken.attribute.relationalOperator = OP_LT;
			scData.scanHistogram[currentToken.code]++;
			return currentToken;
		case GTH_CHR:
			currentToken.code = REL_T;
			currentToken.attribute.relationalOperator = OP_GT;
			scData.scanHistogram[currentToken.code]++;
			return currentToken;

		/* Cases for END OF FILE */
		case EOS_CHR:
			currentToken.code = SEOF_T;
			scData.scanHistogram[currentToken.code]++;
			currentToken.attribute.seofType = SEOF_0;
			return currentToken;
		case EOF_CHR:
			currentToken.code = SEOF_T;
			scData.scanHistogram[currentToken.code]++;
			currentToken.attribute.seofType = SEOF_255;
			return currentToken;

		/* ------------------------------------------------------------------------
			Part 2: Implementation of Finite State Machine (DFA) or Transition Table driven Scanner
			Note: Part 2 must follow Part 1 to catch the illegal symbols
			-----------------------------------------------------------------------
		*/

		/* Adjust / check the logic for your language */

		default: // general case
			state = nextState(state, c);
			lexStart = readerGetPosRead(sourceBuffer) - 1;
			readerSetMark(sourceBuffer, lexStart);
			int pos = 0;
			while (stateType[state] == NOFS) {
				c = readerGetChar(sourceBuffer);
				state = nextState(state, c);
				pos++;
			}
			if (stateType[state] == FSWR)
				readerRetract(sourceBuffer);
			lexEnd = readerGetPosRead(sourceBuffer);
			lexLength = lexEnd - lexStart;
			lexemeBuffer = readerCreate((scribe_intg)lexLength + 2, READER_DEFAULT_FACTOR);
			if (!lexemeBuffer) {
				fprintf(stderr, "Scanner error: Can not create buffer\n");
				exit(1);
			}
			readerRestore(sourceBuffer);
			for (i = 0; i < lexLength; i++)
				readerAddChar(lexemeBuffer, readerGetChar(sourceBuffer));
			readerAddChar(lexemeBuffer, READER_TERMINATOR);
			lexeme = readerGetContent(lexemeBuffer, 0);
			// Defensive programming
			if (!lexeme)
				return currentToken;
			currentToken = (*finalStateTable[state])(lexeme);
			readerRestore(lexemeBuffer);
			return currentToken;
		} // switch

	} //while

} // tokenizer


/*
 ************************************************************
 * Get Next State
	The assert(int test) macro can be used to add run-time diagnostic to programs
	and to "defend" from producing unexpected results.
	- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
	(*) assert() is a macro that expands to an if statement;
	if test evaluates to false (zero) , assert aborts the program
	(by calling abort()) and sends the following message on stderr:
	(*) Assertion failed: test, file filename, line linenum.
	The filename and linenum listed in the message are the source file name
	and line number where the assert macro appears.
	- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
	If you place the #define NDEBUG directive ("no debugging")
	in the source code before the #include <assert.h> directive,
	the effect is to comment out the assert statement.
	- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
	The other way to include diagnostics in a program is to use
	conditional preprocessing as shown bellow. It allows the programmer
	to send more details describing the run-time problem.
	Once the program is tested thoroughly #define DEBUG is commented out
	or #undef DEBUG is used - see the top of the file.
 ***********************************************************
 */
 /* Just change the datatypes */

scribe_intg nextState(scribe_intg state, scribe_char c) {
	scribe_intg col;
	scribe_intg next;
	col = nextClass(c);
	next = transitionTable[state][col];
	if (DEBUG)
		printf("Input symbol: %c Row: %d Column: %d Next: %d \n", c, state, col, next);
	assert(next != FS);
	if (DEBUG)
		if (next == FS) {
			printf("Scanner Error: Illegal state:\n");
			printf("Input symbol: %c Row: %d Column: %d\n", c, state, col);
			exit(1);
		}
	return next;
}

/*
 ************************************************************
 * Get Next Token Class
	* Create a function to return the column number in the transition table:
	* Considering an input char c, you can identify the "class".
	* For instance, a letter should return the column for letters, etc.
 ***********************************************************
 */
/* Use your column configuration */

/* Adjust the logic to return next column in TT */
/*    [A-z],[0-9],    _,    &,   \', SEOF,    #, other
	   L(0), D(1), U(2), M(3), Q(4), E(5), C(6),  O(7) */

scribe_intg nextClass(scribe_char c) {
	scribe_intg val = -1;
	switch (c) {
	case UND_CHR:
		val = 2;
		break;
	case AMP_CHR:
		val = 3;
		break;
	case QUT_CHR:
		val = 4;
		break;
	case HST_CHR:
		val = 6;
		break;
	case PRD_CHR:
		val = 7;
		break;
	case EOS_CHR:
	case EOF_CHR:
		val = 5;
		break;
	default:
		if (isalpha(c))
			val = 0;
		else if (isdigit(c))
			val = 1;
		else
			val = 8;
	}
	return val;
}

/*
 ************************************************************
 * Acceptance State Function COM
 *		Function responsible to identify COM (comments).
 ***********************************************************
 */
 /* Adjust the function for IL */

Token funcCMT(scribe_strg lexeme) {
	Token currentToken = { 0 };
	scribe_intg i = 0, len = (scribe_intg)strlen(lexeme);
	currentToken.attribute.contentString = readerGetPosWrte(stringLiteralTable);
	for (i = 1; i < len - 1; i++) {
		if (lexeme[i] == NWL_CHR)
			line++;
	}
	currentToken.code = CMT_T;
	scData.scanHistogram[currentToken.code]++;
	return currentToken;
}


 /*
  ************************************************************
  * Acceptance State Function IL
  *		Function responsible to identify IL (integer literals).
  * - It is necessary respect the limit (ex: 2-byte integer in C).
  * - In the case of larger lexemes, error shoul be returned.
  * - Only first ERR_LEN characters are accepted and eventually,
  *   additional three dots (...) should be put in the output.
  ***********************************************************
  */
  /* Adjust the function for IL */

Token funcIL(scribe_strg lexeme) {
	Token currentToken = { 0 };
	scribe_long tlong;
	if (lexeme[0] != EOS_CHR && strlen(lexeme) > NUM_LEN) {
		currentToken = (*finalStateTable[ESNR])(lexeme);
	}
	else {
		tlong = atol(lexeme);
		if (tlong >= 0 && tlong <= SHRT_MAX) {
			currentToken.code = INL_T;
			scData.scanHistogram[currentToken.code]++;
			currentToken.attribute.intValue = (scribe_intg)tlong;
		}
		else {
			currentToken = (*finalStateTable[ESNR])(lexeme);
		}
	}
	return currentToken;
}

/*
 ************************************************************
 * Acceptance State Function FPL
 *		Function responsible to identify FPL (floating-point literals).
 * - Respects the float range; on overflow an error token is produced.
 ***********************************************************
 */
Token funcFPL(scribe_strg lexeme) {
	Token currentToken = { 0 };
	double tdouble;
	if (lexeme[0] != EOS_CHR && strlen(lexeme) > (VID_LEN)) {
		currentToken = (*finalStateTable[ESNR])(lexeme);
		return currentToken;
	}
	tdouble = atof(lexeme);
	if (tdouble >= 0.0 && tdouble <= FLT_MAX) {
		currentToken.code = FPL_T;
		scData.scanHistogram[currentToken.code]++;
		currentToken.attribute.floatValue = (scribe_real)tdouble;
	}
	else {
		currentToken = (*finalStateTable[ESNR])(lexeme);
	}
	return currentToken;
}


/*
 ************************************************************
 * Acceptance State Function ID
 *		In this function, the pattern for IDs must be recognized.
 *		Since keywords obey the same pattern, is required to test if
 *		the current lexeme matches with KW from language.
 *	- Remember to respect the limit defined for lexemes (VID_LEN) and
 *	  set the lexeme to the corresponding attribute (vidLexeme).
 *    Remember to end each token with the \0.
 *  - Suggestion: Use "strncpy" function.
 ***********************************************************
 */
 /* Adjust the function for ID */

Token funcID(scribe_strg lexeme) {
	Token currentToken = { 0 };
	size_t length = strlen(lexeme);
	scribe_char lastch = lexeme[length - 1];
	scribe_intg j, kwindex = -1;
	/* Defensive: variable identifiers must respect VID_LEN */
	if (length > VID_LEN)
		return funcErr(lexeme);
	/* Method identifiers end with '&' */
	if (lastch == AMP_CHR) {
		currentToken.code = MNID_T;
		scData.scanHistogram[currentToken.code]++;
		strncpy(currentToken.attribute.idLexeme, lexeme, VID_LEN);
		currentToken.attribute.idLexeme[VID_LEN] = EOS_CHR;
		return currentToken;
	}
	/* Otherwise it is a keyword or a variable identifier (AVID) */
	for (j = 0; j < KWT_SIZE; j++)
		if (!strcmp(lexeme, &keywordTable[j][0])) {
			kwindex = j;
			break;
		}
	if (kwindex != -1) {
		currentToken.code = KW_T;
		scData.scanHistogram[currentToken.code]++;
		currentToken.attribute.codeType = kwindex;
		return currentToken;
	}
	currentToken.code = AVID_T;
	scData.scanHistogram[currentToken.code]++;
	strncpy(currentToken.attribute.idLexeme, lexeme, VID_LEN);
	currentToken.attribute.idLexeme[VID_LEN] = EOS_CHR;
	return currentToken;
}


/*
************************************************************
 * Acceptance State Function SL
 *		Function responsible to identify SL (string literals).
 * - The lexeme must be stored in the String Literal Table 
 *   (stringLiteralTable). You need to include the literals in 
 *   this structure, using offsets. Remember to include \0 to
 *   separate the lexemes. Remember also to incremente the line.
 ***********************************************************
 */
/* Adjust the function for SL */

Token funcSL(scribe_strg lexeme) {
	Token currentToken = { 0 };
	scribe_intg i = 0, len = (scribe_intg)strlen(lexeme);
	currentToken.attribute.contentString = readerGetPosWrte(stringLiteralTable);
	for (i = 1; i < len - 1; i++) {
		if (lexeme[i] == NWL_CHR)
			line++;
		if (!readerAddChar(stringLiteralTable, lexeme[i])) {
			currentToken.code = RTE_T;
			scData.scanHistogram[currentToken.code]++;
			strcpy(currentToken.attribute.errLexeme, "Run Time Error:");
			errorNumber = RTE_CODE;
			return currentToken;
		}
	}
	if (!readerAddChar(stringLiteralTable, EOS_CHR)) {
		currentToken.code = RTE_T;
		scData.scanHistogram[currentToken.code]++;
		strcpy(currentToken.attribute.errLexeme, "Run Time Error:");
		errorNumber = RTE_CODE;
		return currentToken;
	}
	currentToken.code = STR_T;
	scData.scanHistogram[currentToken.code]++;
	return currentToken;
}


/*
************************************************************
 * This function checks if one specific lexeme is a keyword.
 * - Tip: Remember to use the keywordTable to check the keywords.
 ***********************************************************
 */
 /* Adjust the function for Keywords */

Token funcKEY(scribe_strg lexeme) {
	Token currentToken = { 0 };
	scribe_intg kwindex = -1, j = 0;
	scribe_intg len = (scribe_intg)strlen(lexeme);
	///lexeme[len - 1] = EOS_CHR;
	for (j = 0; j < KWT_SIZE; j++)
		if (!strcmp(lexeme, &keywordTable[j][0]))
			kwindex = j;
	if (kwindex != -1) {
		currentToken.code = KW_T;
		scData.scanHistogram[currentToken.code]++;
		currentToken.attribute.codeType = kwindex;
	}
	else {
		currentToken = funcErr(lexeme);
	}
	return currentToken;
}


/*
************************************************************
 * Acceptance State Function Error
 *		Function responsible to deal with ERR token.
 * - This function uses the errLexeme, respecting the limit given
 *   by ERR_LEN. If necessary, use three dots (...) to use the
 *   limit defined. The error lexeme contains line terminators,
 *   so remember to increment line.
 ***********************************************************
 */
 /* Adjust the function for Errors */

Token funcErr(scribe_strg lexeme) {
	Token currentToken = { 0 };
	scribe_intg i = 0, len = (scribe_intg)strlen(lexeme);
	if (len > ERR_LEN) {
		strncpy(currentToken.attribute.errLexeme, lexeme, ERR_LEN - 3);
		currentToken.attribute.errLexeme[ERR_LEN - 3] = EOS_CHR;
		strcat(currentToken.attribute.errLexeme, "...");
	}
	else {
		strcpy(currentToken.attribute.errLexeme, lexeme);
	}
	for (i = 0; i < len; i++)
		if (lexeme[i] == NWL_CHR)
			line++;
	currentToken.code = ERR_T;
	scData.scanHistogram[currentToken.code]++;
	return currentToken;
}


/*
 ************************************************************
 * The function prints the token returned by the scanner
 ***********************************************************
 */

scribe_void printToken(Token t) {
	extern scribe_strg keywordTable[]; /* link to keyword table in */
	switch (t.code) {
	case RTE_T:
		printf("RTE_T\t\t%s", t.attribute.errLexeme);
		/* Call here run-time error handling component */
		if (errorNumber) {
			printf("%d", errorNumber);
			exit(errorNumber);
		}
		printf("\n");
		break;
	case ERR_T:
		printf("ERR_T\t\t%s\n", t.attribute.errLexeme);
		break;
	case SEOF_T:
		printf("SEOF_T\t\t%d\t\n", t.attribute.seofType);
		break;
	case MNID_T:
		printf("MNID_T\t\t%s\n", t.attribute.idLexeme);
		break;
	case STR_T:
		printf("STR_T\t\t%d\t ", (scribe_intg)t.attribute.codeType);
		printf("%s\n", readerGetContent(stringLiteralTable, (scribe_intg)t.attribute.codeType));
		break;
	case LPR_T:
		printf("LPR_T\n");
		break;
	case RPR_T:
		printf("RPR_T\n");
		break;
	case LBR_T:
		printf("LBR_T\n");
		break;
	case RBR_T:
		printf("RBR_T\n");
		break;
	case KW_T:
		printf("KW_T\t\t%s\n", keywordTable[t.attribute.codeType]);
		break;
	case CMT_T:
		printf("CMT_T\n");
		break;
	case EOS_T:
		printf("EOS_T\n");
		break;
	case INL_T:
		printf("INL_T\t\t%d\n", (scribe_intg)t.attribute.intValue);
		break;
	case FPL_T:
		printf("FPL_T\t\t%f\n", t.attribute.floatValue);
		break;
	case AVID_T:
		printf("AVID_T\t\t%s\n", t.attribute.idLexeme);
		break;
	case ASS_T:
		printf("ASS_T\n");
		break;
	case ART_T:
		printf("ART_T\t\t%d\n", (scribe_intg)t.attribute.arithmeticOperator);
		break;
	case REL_T:
		printf("REL_T\t\t%d\n", (scribe_intg)t.attribute.relationalOperator);
		break;
		default:
		printf("Scanner error: invalid token code: %d\n", t.code);
	}
}

/*
 ************************************************************
 * The function prints statistics of tokens
 * Param:
 *	- Scanner data
 * Return:
 *	- Void (procedure)
 ***********************************************************
 */
scribe_void printScannerData(ScannerData scData) {
	/* Print Scanner statistics */
	printf("Statistics:\n");
	printf("----------------------------------\n");
	int cont = 0;
	for (cont = 0; cont < NUM_TOKENS; cont++) {
		if (scData.scanHistogram[cont] > 0)
			printf("%s%s%s%d%s", "Token[", tokenStrTable[cont], "]=", scData.scanHistogram[cont], "\n");
	}
	printf("----------------------------------\n");
}

/*
(If necessary): HERE YOU WRITE YOUR ADDITIONAL FUNCTIONS (IF ANY).
*/
