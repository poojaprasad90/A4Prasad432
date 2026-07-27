/*
************************************************************
* COMPILERS COURSE - Algonquin College
* Code version: Summer, 2026
* Author: Pooja Prasad
* Professors: Paulo Sousa
************************************************************
* File name: Step4Parser.h
* Course: CST 8152  Compilers
* Assignment: A4 (Parser / Syntax Analyzer)
* Language: Scribe
* Purpose: Header for the Scribe Parser (top-down LL(1) predictive parser).
************************************************************
*/

#ifndef PARSER_H_
#define PARSER_H_

/* Inclusion section */
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

/* Global vars */
static Token           lookahead;
extern BufferPointer   stringLiteralTable;
extern scribe_intg     line;
extern Token           tokenizer(scribe_void);
extern scribe_strg     keywordTable[KWT_SIZE];
extern scribe_intg     syntaxErrorNumber;

/* Scribe method-identifier names (end with '$') */
#define LANG_WRTE      "print$"
#define LANG_READ      "input$"
#define LANG_MAIN      "main$"

/* Scribe keyword attribute codes - MUST follow the order in keywordTable
   (Step3Scanner.h): vault, logic, whole, deci, text, check, so, alt,
   during, run, giveback. */
enum KEYWORDS {
    NO_ATTR = -1,
    KW_vault,      /* 0  - declarations block (was "data")   */
    KW_logic,      /* 1  - statements block   (was "code")   */
    KW_whole,      /* 2  - integer type       (was "int")    */
    KW_deci,       /* 3  - real type          (was "real")   */
    KW_text,       /* 4  - string type        (was "string") */
    KW_check,      /* 5  - if                                */
    KW_so,         /* 6  - then                              */
    KW_alt,        /* 7  - else                              */
    KW_during,     /* 8  - while                             */
    KW_run,        /* 9  - do                                */
    KW_giveback    /* 10 - return                            */
};

/* Number of BNF (non-terminal) rules tracked in the histogram */
#define NUM_BNF_RULES 24

/* Parser statistics */
typedef struct parserData {
    scribe_intg parsHistogram[NUM_BNF_RULES];   /* count per BNF rule */
} ParserData, * pParsData;

/* Number of errors */
scribe_intg numParserErrors;

/* Parser data */
ParserData psData;

/* Basic function definitions */
scribe_void startParser();
scribe_void matchToken(scribe_intg, scribe_intg);
scribe_void syncErrorHandler(scribe_intg);
scribe_void printError();
scribe_void printBNFData(ParserData psData);

/* List of BNF statements (non-terminals) */
enum BNF_RULES {
    BNF_error,                  /*  0: Error */
    BNF_program,                /*  1 */
    BNF_comment,                /*  2 */
    BNF_dataSession,            /*  3 */
    BNF_optVarDeclarations,     /*  4 */
    BNF_varDeclaration,         /*  5 */
    BNF_codeSession,            /*  6 */
    BNF_optStatements,          /*  7 */
    BNF_statement,              /*  8 */
    BNF_assignment,             /*  9 */
    BNF_arithExpr,              /* 10 */
    BNF_arithExprPrime,         /* 11 */
    BNF_term,                   /* 12 */
    BNF_termPrime,              /* 13 */
    BNF_factor,                 /* 14 */
    BNF_outputStatement,        /* 15 */
    BNF_outputList,             /* 16 */
    BNF_inputStatement,         /* 17 */
    BNF_returnStatement,        /* 18 */
    BNF_optReturnValue,         /* 19 */
    BNF_selectionStatement,     /* 20 */
    BNF_optElse,                /* 21 */
    BNF_iterationStatement,     /* 22 */
    BNF_conditional             /* 23 */
};

/* Names for the statistics table (same order as BNF_RULES) */
static scribe_strg BNFStrTable[NUM_BNF_RULES] = {
    "BNF_error",
    "BNF_program",
    "BNF_comment",
    "BNF_dataSession",
    "BNF_optVarDeclarations",
    "BNF_varDeclaration",
    "BNF_codeSession",
    "BNF_optStatements",
    "BNF_statement",
    "BNF_assignment",
    "BNF_arithExpr",
    "BNF_arithExprPrime",
    "BNF_term",
    "BNF_termPrime",
    "BNF_factor",
    "BNF_outputStatement",
    "BNF_outputList",
    "BNF_inputStatement",
    "BNF_returnStatement",
    "BNF_optReturnValue",
    "BNF_selectionStatement",
    "BNF_optElse",
    "BNF_iterationStatement",
    "BNF_conditional"
};

/* Non-terminal function declarations */
scribe_void program();
scribe_void comment();
scribe_void dataSession();
scribe_void optVarDeclarations();
scribe_void varDeclaration();
scribe_void codeSession();
scribe_void optStatements();
scribe_void statement();
scribe_void assignment();
scribe_void arithExpr();
scribe_void arithExprPrime();
scribe_void term();
scribe_void termPrime();
scribe_void factor();
scribe_void outputStatement();
scribe_void outputList();
scribe_void inputStatement();
scribe_void returnStatement();
scribe_void optReturnValue();
scribe_void selectionStatement();
scribe_void optElse();
scribe_void iterationStatement();
scribe_void conditional();

#endif
