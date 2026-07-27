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
* File name: compilers.c
* Compiler: MS Visual Studio 2022
* Course: CST 8152 � Compilers, Lab Section: [011, 012]
* Assignment: A12, A22, A32.
* Date: sEP 01 2025
* Professor: Paulo Sousa
* Purpose: This file is the main program of Compilers Project
* Function list: main().
************************************************************
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>
#include <ctype.h>

#ifndef COMPILERS_H_
#include "Compilers.h"
#endif

/*
 * ............................................................................
 *
 * ADVICE 2: This file must be updated according to each assignment
 * (see the progression: reader > scanner > parser).
 * ............................................................................
 */

/*
***********************************************************
* Function name: main
* Purpose: Main function
* Author: Paulo Sousa
* History/Versions: Ver F25
* Called functions: mainReader(), mainScanner(), mainParser()
* Parameters: Command line arguments - argc, argv
* Return value: Status
* Algorithm: -
*************************************************************
*/

scribe_intg main(int argc, char** argv) {

	scribe_intg i;
	printLogo();
	if (DEBUG) {
		for (i = 0; i < argc; ++i)
			printf("argv[%d] = %s\n", i, argv[i]);
	}
	if (argc < 2) {
		printf("%s%c%s%c%s%c%s%c%s%c%s", "OPTIONS:\n* [",
			PGM_CDR, "] - Coder\n* [",
			PGM_RDR, "] - Reader\n* [",
			PGM_SCN, "] - Scanner\n* [",
			PGM_PSR, "] - Parser\n* [",
			PGM_WRT, "] - Writer\n");
		return EXIT_FAILURE;
	}
	scribe_char option = argv[1][0];
	switch (option) {
	case PGM_CDR:
		printf("%s%c%s", "\n[Option '", PGM_CDR, "': Starting CODER .....]\n\n");
		main1Coder(argc, argv);
		break;
	case PGM_RDR:
		printf("%s%c%s", "\n[Option '", PGM_RDR, "': Starting READER .....]\n\n");
		main2Reader(argc, argv);
		break;
	case PGM_SCN:
		printf("%s%c%s", "\n[Option '", PGM_SCN, "': Starting SCANNER .....]\n\n");
		main3Scanner(argc, argv);
		break;
	case PGM_PSR:
		printf("%s%c%s", "\n[Option '", PGM_PSR, "': Starting PARSER .....]\n\n");
		main4Parser(argc, argv);
		break;
	default:
		printf("%s%c%s%c%s%c%s%c%s%c%s", "OPTIONS:\n* [",
			PGM_CDR, "] - Coder\n* [",
			PGM_RDR, "] - Reader\n* [",
			PGM_SCN, "] - Scanner\n* [",
			PGM_PSR, "] - Parser\n* [",
			PGM_WRT, "] - Writer\n");
		break;
	}
	return EXIT_SUCCESS;
}

/*
***********************************************************
* Function name: printLogo
* Purpose: Print Logo
* Author: Paulo Sousa
* History/Versions: Ver F25
* Called functions: -
* Parameters: -
* Return value: (Null)
* Algorithm: -
*************************************************************
*/

scribe_void printLogo() {
	printf("%s%s%s%s%s%s%s%s%s%s%s%s%s%s%s%s%s%s%s%s%s%s%s",
		"\t=---------------------------------------=\n",
		"\t|  COMPILERS - ALGONQUIN COLLEGE (S25)  |\n",
		"\t=---------------------------------------=\n",
		"\t    @@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@    \n",
		"\t    @@                             @@    \n",
		"\t    @@           %&@@@@@@@@@@@     @@    \n",
		"\t    @@       @%% (@@@@@@@@@  @     @@    \n",
		"\t    @@      @& @   @ @       @     @@    \n",
		"\t    @@     @ @ %  / /   @@@@@@     @@    \n",
		"\t    @@      & @ @  @@              @@    \n",
		"\t    @@       @/ @*@ @ @   @        @@    \n",
		"\t    @@           @@@@  @@ @ @      @@    \n",
		"\t    @@            /@@    @@@ @     @@    \n",
		"\t    @@     @      / /     @@ @     @@    \n",
		"\t    @@     @ @@   /@/   @@@ @      @@    \n",
		"\t    @@     @@@@@@@@@@@@@@@         @@    \n",
		"\t    @@                             @@    \n",
		"\t    @@         S C R I B E           @@    \n",
		"\t    @@                             @@    \n",
		"\t    @@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@    \n",
		"\t                                         \n",
		"\t[COMPILER SCRIPT .......................]\n",
		"\t                                         \n"
	);
}

/*
************************************************************
* Error printing function with variable number of arguments
* Params: Variable arguments, using formats from C language.
*	- Internal vars use list of arguments and types from stdarg.h
*   - NOTE: The format is using signature from C Language
************************************************************
*/

scribe_void errorPrint(scribe_strg fmt, ...) {
	/* Initialize variable list */
	va_list ap;
	va_start(ap, fmt);

	(scribe_void)vfprintf(stderr, fmt, ap);
	va_end(ap);

	/* Move to new line */
	if (strchr(fmt, '\n') == NULL)
		fprintf(stderr, "\n");
}
