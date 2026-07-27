/*
************************************************************
* COMPILERS COURSE - Algonquin College
* Code version: Summer, 2026
* Author: Pooja Prasad
* Professors: Paulo Sousa
************************************************************
* File name: Step4Parser.c
* Course: CST 8152  Compilers
* Assignment: A4 (Parser / Syntax Analyzer)
* Language: Scribe
* Purpose: Top-down LL(1) predictive parser for the Scribe language.
*
* SCRIBE GRAMMAR (BNF)
* --------------------------------------------------------------------
* <program>            -> [<comment>] main$ ( ) { <dataSession> <codeSession> }
* <comment>            -> CMT_T
* <dataSession>        -> [<comment>] vault { <optVarDeclarations> }
* <optVarDeclarations> -> <varDeclaration> <optVarDeclarations> | e
* <varDeclaration>     -> (whole | deci | text) AVID_T ;
* <codeSession>        -> [<comment>] logic { <optStatements> }
* <optStatements>      -> <statement> <optStatements> | e
* <statement>          -> <assignment> | <outputStatement> | <inputStatement>
*                         | <returnStatement> | <selectionStatement>
*                         | <iterationStatement> | <comment>
* <selectionStatement> -> check ( <conditional> ) so { <optStatements> } [alt { <optStatements> }]
* <iterationStatement> -> during ( <conditional> ) run { <optStatements> }
* <conditional>        -> <arithExpr> (< | >) <arithExpr>
* <assignment>         -> AVID_T = ( STR_T | <arithExpr> ) ;
* <arithExpr>          -> <term> <arithExprPrime>
* <arithExprPrime>     -> (+ | -) <term> <arithExprPrime> | e
* <term>               -> <factor> <termPrime>
* <termPrime>          -> (* | /) <factor> <termPrime> | e
* <factor>             -> AVID_T | INL_T | FPL_T | ( <arithExpr> )
* <outputStatement>    -> print$ ( <outputList> ) ;
* <outputList>         -> STR_T | AVID_T | e
* <inputStatement>     -> input$ ( AVID_T ) ;
* <returnStatement>    -> giveback <optReturnValue> ;
* <optReturnValue>     -> INL_T | FPL_T | AVID_T | e
* --------------------------------------------------------------------
************************************************************
*/

#ifndef COMPILERS_H_
#include "Compilers.h"
#endif
#ifndef SCANNER_H_
#include "Step3Scanner.h"
#endif
#ifndef PARSER_H_
#include "Step4Parser.h"
#endif

/* Parser data (BNF statistics) */
extern ParserData psData;

/* Single shared syntax-error counter (declared extern in Step4Parser.h) */
scribe_intg syntaxErrorNumber = 0;

/*
 * ============================================================
 * BASIC FUNCTIONS
 * ============================================================
 */

/* Start the parser: read the first token and launch <program>. */
scribe_void startParser() {
    scribe_intg i = 0;
    for (i = 0; i < NUM_BNF_RULES; i++)
        psData.parsHistogram[i] = 0;

    lookahead = tokenizer();
    if (lookahead.code != SEOF_T)
        program();
    matchToken(SEOF_T, NO_ATTR);
    printf("%s%s\n", STR_LANGNAME, ": Source file parsed");
}

/* Match the lookahead against an expected token (and keyword attribute). */
scribe_void matchToken(scribe_intg tokenCode, scribe_intg tokenAttribute) {
    scribe_intg matchFlag = SCRIBE_TRUE;
    switch (lookahead.code) {
    case KW_T:
        if (lookahead.attribute.codeType != tokenAttribute)
            matchFlag = SCRIBE_FALSE;
    default:
        if (lookahead.code != tokenCode)
            matchFlag = SCRIBE_FALSE;
    }
    if (matchFlag && lookahead.code == SEOF_T)
        return;
    if (matchFlag) {
        lookahead = tokenizer();
        if (lookahead.code == ERR_T) {
            printError();
            lookahead = tokenizer();
        }
    }
    else
        syncErrorHandler(tokenCode);
}

/* Panic-mode error recovery: skip tokens until the sync token or EOF. */
scribe_void syncErrorHandler(scribe_intg syncTokenCode) {
    printError();
    while (lookahead.code != syncTokenCode) {
        if (lookahead.code == SEOF_T)
            exit(syntaxErrorNumber);
        lookahead = tokenizer();
    }
    if (lookahead.code != SEOF_T)
        lookahead = tokenizer();
}

/* Print a syntax error message describing the offending token. */
scribe_void printError() {
    Token t = lookahead;
    syntaxErrorNumber++;
    printf("%s%s%3d\n", STR_LANGNAME, ": Syntax error:  Line:", line);
    printf("*****  Token code:%3d Attribute: ", t.code);
    switch (t.code) {
    case ERR_T:
        printf("*ERROR*: %s\n", t.attribute.errLexeme);
        break;
    case SEOF_T:
        printf("SEOF_T\t\t%d\t\n", t.attribute.seofType);
        break;
    case MNID_T:
        printf("MNID_T:\t\t%s\t\n", t.attribute.idLexeme);
        break;
    case AVID_T:
        printf("AVID_T:\t\t%s\t\n", t.attribute.idLexeme);
        break;
    case STR_T:
        printf("STR_T: %s\n", readerGetContent(stringLiteralTable, t.attribute.contentString));
        break;
    case KW_T:
        printf("KW_T: %s\n", keywordTable[t.attribute.codeType]);
        break;
    case INL_T:
        printf("INL_T: %d\n", t.attribute.intValue);
        break;
    case FPL_T:
        printf("FPL_T: %f\n", t.attribute.floatValue);
        break;
    case ASS_T:
        printf("ASS_T\n");
        break;
    case ART_T:
        printf("ART_T\n");
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
    case EOS_T:
        printf("NA\n");
        break;
    default:
        printf("%s%s%d\n", STR_LANGNAME, ": Parser error: invalid token code: ", t.code);
        numParserErrors++;
    }
}

/*
 * ============================================================
 * HELPERS
 * ============================================================
 */

/* True if lookahead is an arithmetic operator + or - */
static scribe_intg isAddOp() {
    return (lookahead.code == ART_T &&
        (lookahead.attribute.arithmeticOperator == OP_ADD ||
         lookahead.attribute.arithmeticOperator == OP_SUB));
}
/* True if lookahead is an arithmetic operator * or / */
static scribe_intg isMulOp() {
    return (lookahead.code == ART_T &&
        (lookahead.attribute.arithmeticOperator == OP_MUL ||
         lookahead.attribute.arithmeticOperator == OP_DIV));
}
/* True if lookahead is a method identifier equal to name */
static scribe_intg isMethod(scribe_strg name) {
    return (lookahead.code == MNID_T &&
        strcmp(lookahead.attribute.idLexeme, name) == 0);
}
/* True if lookahead is a type keyword (whole/deci/text) */
static scribe_intg isTypeKW() {
    return (lookahead.code == KW_T &&
        (lookahead.attribute.codeType == KW_whole ||
         lookahead.attribute.codeType == KW_deci  ||
         lookahead.attribute.codeType == KW_text));
}

/*
 * ============================================================
 * GRAMMAR (NON-TERMINAL) FUNCTIONS
 * ============================================================
 */

/*
 * <program> -> [<comment>] main$ ( ) { <dataSession> <codeSession> }
 * FIRST = { CMT_T, MNID_T(main$), SEOF_T }
 */
scribe_void program() {
    psData.parsHistogram[BNF_program]++;
    switch (lookahead.code) {
    case CMT_T:
        comment();
        /* fall through to main$ */
    case MNID_T:
        if (isMethod(LANG_MAIN)) {
            matchToken(MNID_T, NO_ATTR);
            matchToken(LPR_T, NO_ATTR);
            matchToken(RPR_T, NO_ATTR);
            matchToken(LBR_T, NO_ATTR);
            dataSession();
            codeSession();
            matchToken(RBR_T, NO_ATTR);
        }
        else {
            printError();
        }
        break;
    case SEOF_T:
        ; /* empty program */
        break;
    default:
        printError();
    }
    printf("%s%s\n", STR_LANGNAME, ": Program parsed");
}

/* <comment> -> CMT_T */
scribe_void comment() {
    psData.parsHistogram[BNF_comment]++;
    matchToken(CMT_T, NO_ATTR);
    printf("%s%s\n", STR_LANGNAME, ": Comment parsed");
}

/*
 * <dataSession> -> [<comment>] vault { <optVarDeclarations> }
 * FIRST = { CMT_T, KW_T(vault) }
 */
scribe_void dataSession() {
    psData.parsHistogram[BNF_dataSession]++;
    if (lookahead.code == CMT_T)
        comment();
    matchToken(KW_T, KW_vault);
    matchToken(LBR_T, NO_ATTR);
    optVarDeclarations();
    matchToken(RBR_T, NO_ATTR);
    printf("%s%s\n", STR_LANGNAME, ": Data Session parsed");
}

/*
 * <optVarDeclarations> -> <varDeclaration> <optVarDeclarations> | e
 * FIRST = { KW_T(whole|deci|text), e }
 */
scribe_void optVarDeclarations() {
    psData.parsHistogram[BNF_optVarDeclarations]++;
    while (isTypeKW()) {
        varDeclaration();
    }
    printf("%s%s\n", STR_LANGNAME, ": Optional Variable Declarations parsed");
}

/*
 * <varDeclaration> -> (whole | deci | text) AVID_T ;
 * FIRST = { KW_T(whole|deci|text) }
 */
scribe_void varDeclaration() {
    psData.parsHistogram[BNF_varDeclaration]++;
    switch (lookahead.attribute.codeType) {
    case KW_whole:
        matchToken(KW_T, KW_whole);
        break;
    case KW_deci:
        matchToken(KW_T, KW_deci);
        break;
    case KW_text:
        matchToken(KW_T, KW_text);
        break;
    default:
        printError();
    }
    matchToken(AVID_T, NO_ATTR);
    matchToken(EOS_T, NO_ATTR);
    printf("%s%s\n", STR_LANGNAME, ": Variable Declaration parsed");
}

/*
 * <codeSession> -> [<comment>] logic { <optStatements> }
 * FIRST = { CMT_T, KW_T(logic) }
 */
scribe_void codeSession() {
    psData.parsHistogram[BNF_codeSession]++;
    if (lookahead.code == CMT_T)
        comment();
    matchToken(KW_T, KW_logic);
    matchToken(LBR_T, NO_ATTR);
    optStatements();
    matchToken(RBR_T, NO_ATTR);
    printf("%s%s\n", STR_LANGNAME, ": Code Session parsed");
}

/*
 * <optStatements> -> <statement> <optStatements> | e
 * FIRST = { AVID_T, MNID_T(print$/input$), KW_T(giveback), CMT_T, e }
 */
scribe_void optStatements() {
    psData.parsHistogram[BNF_optStatements]++;
    while (lookahead.code == AVID_T
        || lookahead.code == CMT_T
        || isMethod(LANG_WRTE)
        || isMethod(LANG_READ)
        || (lookahead.code == KW_T && (lookahead.attribute.codeType == KW_giveback
             || lookahead.attribute.codeType == KW_check
             || lookahead.attribute.codeType == KW_during))) {
        statement();
    }
    printf("%s%s\n", STR_LANGNAME, ": Optional Statements parsed");
}

/*
 * <statement> -> <assignment> | <outputStatement> | <inputStatement>
 *                | <returnStatement> | <comment>
 */
scribe_void statement() {
    psData.parsHistogram[BNF_statement]++;
    switch (lookahead.code) {
    case CMT_T:
        comment();
        break;
    case AVID_T:
        assignment();
        break;
    case MNID_T:
        if (isMethod(LANG_WRTE))
            outputStatement();
        else if (isMethod(LANG_READ))
            inputStatement();
        else
            printError();
        break;
    case KW_T:
        switch (lookahead.attribute.codeType) {
        case KW_giveback:
            returnStatement();
            break;
        case KW_check:
            selectionStatement();
            break;
        case KW_during:
            iterationStatement();
            break;
        default:
            printError();
        }
        break;
    default:
        printError();
    }
    printf("%s%s\n", STR_LANGNAME, ": Statement parsed");
}

/*
 * <assignment> -> AVID_T = ( STR_T | <arithExpr> ) ;
 * FIRST = { AVID_T }
 */
scribe_void assignment() {
    psData.parsHistogram[BNF_assignment]++;
    matchToken(AVID_T, NO_ATTR);
    matchToken(ASS_T, NO_ATTR);
    if (lookahead.code == STR_T)
        matchToken(STR_T, NO_ATTR);
    else
        arithExpr();
    matchToken(EOS_T, NO_ATTR);
    printf("%s%s\n", STR_LANGNAME, ": Assignment parsed");
}

/*
 * <arithExpr> -> <term> <arithExprPrime>
 * FIRST = { AVID_T, INL_T, FPL_T, LPR_T }
 */
scribe_void arithExpr() {
    psData.parsHistogram[BNF_arithExpr]++;
    term();
    arithExprPrime();
    printf("%s%s\n", STR_LANGNAME, ": Arithmetic Expression parsed");
}

/*
 * <arithExprPrime> -> (+ | -) <term> <arithExprPrime> | e
 */
scribe_void arithExprPrime() {
    psData.parsHistogram[BNF_arithExprPrime]++;
    if (isAddOp()) {
        matchToken(ART_T, NO_ATTR);
        term();
        arithExprPrime();
    }
    /* else epsilon */
}

/*
 * <term> -> <factor> <termPrime>
 */
scribe_void term() {
    psData.parsHistogram[BNF_term]++;
    factor();
    termPrime();
}

/*
 * <termPrime> -> (* | /) <factor> <termPrime> | e
 */
scribe_void termPrime() {
    psData.parsHistogram[BNF_termPrime]++;
    if (isMulOp()) {
        matchToken(ART_T, NO_ATTR);
        factor();
        termPrime();
    }
    /* else epsilon */
}

/*
 * <factor> -> AVID_T | INL_T | FPL_T | ( <arithExpr> )
 */
scribe_void factor() {
    psData.parsHistogram[BNF_factor]++;
    switch (lookahead.code) {
    case AVID_T:
        matchToken(AVID_T, NO_ATTR);
        break;
    case INL_T:
        matchToken(INL_T, NO_ATTR);
        break;
    case FPL_T:
        matchToken(FPL_T, NO_ATTR);
        break;
    case LPR_T:
        matchToken(LPR_T, NO_ATTR);
        arithExpr();
        matchToken(RPR_T, NO_ATTR);
        break;
    default:
        printError();
    }
    printf("%s%s\n", STR_LANGNAME, ": Factor parsed");
}

/*
 * <outputStatement> -> print$ ( <outputList> ) ;
 * FIRST = { MNID_T(print$) }
 */
scribe_void outputStatement() {
    psData.parsHistogram[BNF_outputStatement]++;
    matchToken(MNID_T, NO_ATTR);
    matchToken(LPR_T, NO_ATTR);
    outputList();
    matchToken(RPR_T, NO_ATTR);
    matchToken(EOS_T, NO_ATTR);
    printf("%s%s\n", STR_LANGNAME, ": Output Statement parsed");
}

/*
 * <outputList> -> STR_T | AVID_T | e
 */
scribe_void outputList() {
    psData.parsHistogram[BNF_outputList]++;
    switch (lookahead.code) {
    case STR_T:
        matchToken(STR_T, NO_ATTR);
        break;
    case AVID_T:
        matchToken(AVID_T, NO_ATTR);
        break;
    default:
        ; /* empty */
    }
    printf("%s%s\n", STR_LANGNAME, ": Output List parsed");
}

/*
 * <inputStatement> -> input$ ( AVID_T ) ;
 * FIRST = { MNID_T(input$) }
 */
scribe_void inputStatement() {
    psData.parsHistogram[BNF_inputStatement]++;
    matchToken(MNID_T, NO_ATTR);
    matchToken(LPR_T, NO_ATTR);
    matchToken(AVID_T, NO_ATTR);
    matchToken(RPR_T, NO_ATTR);
    matchToken(EOS_T, NO_ATTR);
    printf("%s%s\n", STR_LANGNAME, ": Input Statement parsed");
}

/*
 * <returnStatement> -> giveback <optReturnValue> ;
 * FIRST = { KW_T(giveback) }
 */
scribe_void returnStatement() {
    psData.parsHistogram[BNF_returnStatement]++;
    matchToken(KW_T, KW_giveback);
    optReturnValue();
    matchToken(EOS_T, NO_ATTR);
    printf("%s%s\n", STR_LANGNAME, ": Return Statement parsed");
}

/*
 * <optReturnValue> -> INL_T | FPL_T | AVID_T | e
 */
scribe_void optReturnValue() {
    psData.parsHistogram[BNF_optReturnValue]++;
    switch (lookahead.code) {
    case INL_T:
        matchToken(INL_T, NO_ATTR);
        break;
    case FPL_T:
        matchToken(FPL_T, NO_ATTR);
        break;
    case AVID_T:
        matchToken(AVID_T, NO_ATTR);
        break;
    default:
        ; /* empty */
    }
}

/*
 * <selectionStatement> -> check ( <conditional> ) so { <optStatements> } <optElse>
 * FIRST = { KW_T(check) }
 */
scribe_void selectionStatement() {
    psData.parsHistogram[BNF_selectionStatement]++;
    matchToken(KW_T, KW_check);
    matchToken(LPR_T, NO_ATTR);
    conditional();
    matchToken(RPR_T, NO_ATTR);
    matchToken(KW_T, KW_so);
    matchToken(LBR_T, NO_ATTR);
    optStatements();
    matchToken(RBR_T, NO_ATTR);
    optElse();
    printf("%s%s\n", STR_LANGNAME, ": Selection Statement parsed");
}

/*
 * <optElse> -> alt { <optStatements> } | e
 */
scribe_void optElse() {
    psData.parsHistogram[BNF_optElse]++;
    if (lookahead.code == KW_T && lookahead.attribute.codeType == KW_alt) {
        matchToken(KW_T, KW_alt);
        matchToken(LBR_T, NO_ATTR);
        optStatements();
        matchToken(RBR_T, NO_ATTR);
    }
    /* else epsilon */
}

/*
 * <iterationStatement> -> during ( <conditional> ) run { <optStatements> }
 * FIRST = { KW_T(during) }
 */
scribe_void iterationStatement() {
    psData.parsHistogram[BNF_iterationStatement]++;
    matchToken(KW_T, KW_during);
    matchToken(LPR_T, NO_ATTR);
    conditional();
    matchToken(RPR_T, NO_ATTR);
    matchToken(KW_T, KW_run);
    matchToken(LBR_T, NO_ATTR);
    optStatements();
    matchToken(RBR_T, NO_ATTR);
    printf("%s%s\n", STR_LANGNAME, ": Iteration Statement parsed");
}

/*
 * <conditional> -> <arithExpr> (< | >) <arithExpr>
 * FIRST = { AVID_T, INL_T, FPL_T, LPR_T }
 */
scribe_void conditional() {
    psData.parsHistogram[BNF_conditional]++;
    arithExpr();
    if (lookahead.code == REL_T)
        matchToken(REL_T, NO_ATTR);
    else
        printError();
    arithExpr();
    printf("%s%s\n", STR_LANGNAME, ": Conditional Expression parsed");
}

/*
 * ============================================================
 * STATISTICS
 * ============================================================
 */
scribe_void printBNFData(ParserData psData) {
    printf("Statistics:\n");
    printf("----------------------------------\n");
    int cont = 0;
    for (cont = 0; cont < NUM_BNF_RULES; cont++) {
        if (psData.parsHistogram[cont] > 0)
            printf("%s%s%s%d%s", "Token[", BNFStrTable[cont], "]=",
                   psData.parsHistogram[cont], "\n");
    }
    printf("----------------------------------\n");
}
