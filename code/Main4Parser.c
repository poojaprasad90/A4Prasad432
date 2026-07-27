/*
************************************************************
* COMPILERS COURSE - Algonquin College
* Code version: Summer, 2026
* Author: Pooja Prasad
* Professors: Paulo Sousa
************************************************************
* File name: Main4Parser.c
* Course: CST 8152  Compilers
* Assignment: A4 (Parser / Syntax Analyzer)
* Language: Scribe
* Purpose: Driver for the Scribe Parser. Loads + decrypts the source
*          (Coder A1), fills the buffer (Reader A2), starts the Scanner
*          (A3), then runs the top-down Parser (A4).
************************************************************
*/

#define _CRT_SECURE_NO_WARNINGS

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>

#ifndef COMPILERS_H_
#include "Compilers.h"
#endif
#ifndef READER_H_
#include "Step2Reader.h"
#endif
#ifndef SCANNER_H_
#include "Step3Scanner.h"
#endif
#ifndef PARSER_H_
#include "Step4Parser.h"
#endif

/* Check for ANSI C compliancy */
#define ANSI_C 0
#if defined(__STDC__)
#undef ANSI_C
#define ANSI_C 1
#endif

/* Global objects */
BufferPointer stringLiteralTable;   /* String Literal Table */
scribe_intg   errorNumber;          /* Run-time error number */

/* External objects */
extern scribe_intg syntaxErrorNumber;   /* number of syntax errors (Parser) */
extern scribe_intg line;                /* source line number (Scanner) */
extern Token tokenizer(scribe_void);
extern scribe_intg startScanner(BufferPointer psc_buf);
extern ParserData psData;

/* Local function declarations */
scribe_void printParserError(scribe_strg fmt, ...);
scribe_void displayParser(BufferPointer ptrBuffer);
scribe_long getParserFilesize(scribe_strg fname);

/*
************************************************************
*  Parser Main function
*  Params: argc / argv from the command prompt
*  Return: EXIT_SUCCESS
***********************************************************
*/
scribe_intg main4Parser(scribe_intg argc, scribe_strg* argv) {

    BufferPointer sourceBuffer;     /* Pointer to input (source) buffer */
    FILE* fileHandler;              /* Input file handle */
    scribe_intg loadSize = 0;       /* Size of the file loaded in the buffer */

    numParserErrors = 0;            /* Initialize parser errors */

    /* Check for correct arguments - source file name */
    if (argc <= 2) {
        printParserError("Date: %s  Time: %s", __DATE__, __TIME__);
        printParserError("Runtime error at line %d in file %s", __LINE__, __FILE__);
        printParserError("%s%s", argv[0], ": Missing source file name.");
        printParserError("%s", "Usage: <Option=4> <SourceFile>");
        exit(EXIT_FAILURE);
    }

    /* Create a source code input buffer - multiplicative mode */
    sourceBuffer = readerCreate(READER_DEFAULT_SIZE, READER_DEFAULT_FACTOR);
    if (sourceBuffer == NULL) {
        printParserError("%s%s", argv[1], ": Could not create source buffer");
        exit(EXIT_FAILURE);
    }

    /* Open source file */
    scribe_strg sourceFile = argv[2];
    if ((fileHandler = fopen(sourceFile, "r")) == NULL) {
        printParserError("%s%s%s", argv[0], ": Cannot open file: ", argv[2]);
        exit(EXIT_FAILURE);
    }

    /* Load source file into input buffer (readerLoad also decrypts) */
    printf("Reading file %s ....Please wait\n", sourceFile);
    loadSize = readerLoad(sourceBuffer, sourceFile);
    if (loadSize == READER_ERROR)
        printParserError("%s%s", argv[0], ": Error in loading buffer.");
    fclose(fileHandler);

    if (loadSize == READER_ERROR) {
        printf("The input file %s %s\n", argv[2], "is not completely loaded.");
        printf("Input file size: %ld\n", getParserFilesize(argv[2]));
    }

    /* Add SEOF to input buffer and display it */
    if ((loadSize != READER_ERROR) && (loadSize != 0)) {
        if (readerAddChar(sourceBuffer, READER_TERMINATOR)) {
            displayParser(sourceBuffer);
        }
    }

    /* Create the String Literal Table */
    stringLiteralTable = readerCreate(READER_DEFAULT_SIZE, READER_DEFAULT_FACTOR);
    if (stringLiteralTable == NULL) {
        printParserError("%s%s", argv[0], ": Could not create string literals buffer");
        exit(EXIT_FAILURE);
    }

    /* Initialize the scanner with the source buffer */
    if (startScanner(sourceBuffer)) {
        printParserError("%s%s", argv[0], ": Empty program buffer - parsing canceled");
        exit(EXIT_FAILURE);
    }

    /* Start parsing */
    printf("\nParsing the source file...\n\n");
    startParser();

    printf("\nNumber of Parser errors: %d\n", syntaxErrorNumber);

    /* Print the BNF statistics */
    printBNFData(psData);

    return (EXIT_SUCCESS);
}

/*
************************************************************
*  Error printing with variable arguments (to stderr)
***********************************************************
*/
scribe_void printParserError(scribe_strg fmt, ...) {
    va_list ap;
    va_start(ap, fmt);
    (void)vfprintf(stderr, fmt, ap);
    va_end(ap);
    if (strchr(fmt, '\n') == NULL)
        fprintf(stderr, "\n");
}

/*
************************************************************
* Displays the source buffer parameters and contents
***********************************************************
*/
scribe_void displayParser(BufferPointer ptrBuffer) {
    printf("\nPrinting input buffer parameters:\n\n");
    printf("The capacity of the buffer is:  %d\n", readerGetSize(ptrBuffer));
    printf("The current size of the buffer is:  %d\n", readerGetPosWrte(ptrBuffer));
    printf("\nPrinting input buffer contents:\n\n");
    readerRecover(ptrBuffer);
    readerPrint(ptrBuffer);
}

/*
************************************************************
* Returns the size of a file
***********************************************************
*/
scribe_long getParserFilesize(scribe_strg fname) {
    FILE* fileInput;
    scribe_long fileLength;
    fileInput = fopen(fname, "r");
    if (fileInput == NULL) {
        printParserError("%s%s", "Cannot open file: ", fname);
        return 0L;
    }
    fseek(fileInput, 0L, SEEK_END);
    fileLength = ftell(fileInput);
    fclose(fileInput);
    return fileLength;
}
