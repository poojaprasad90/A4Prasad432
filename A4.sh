#!/bin/bash
# ============================================================
# A4 SCRIPT - Parser (Scribe Language) - macOS / Linux
# Compiles the full front-end (Coder+Reader+Scanner+Parser),
# encrypts the 3 test inputs, and parses each one.
# ============================================================
echo "--------------------------------------------"
echo "-      ALGONQUIN COLLEGE - COM - 26S       -"
echo "--------------------------------------------"
echo "-  [A4: Parser - Scribe Language]          -"
echo "--------------------------------------------"

# ---- Step 1: Compile (all source files in code/) ----
echo ""
echo "[1] Compiling..."
gcc code/Compilers.c code/Step1Coder.c code/Step2Reader.c code/Step3Scanner.c \
    code/Step4Parser.c code/Main1Coder.c code/Main2Reader.c code/Main3Scanner.c \
    code/Main4Parser.c -Icode -o code/Compilers
if [ $? -ne 0 ]; then
    echo "COMPILATION FAILED"
    exit 1
fi

# ---- Step 2: Encrypt the 3 plain Scribe sources (option 1 = Coder) ----
echo ""
echo "[2] Encrypting test inputs -> .scr ..."
./code/Compilers 1 1 input/empty.txt  input/empty.scr  >/dev/null 2>&1
./code/Compilers 1 1 input/phello.txt input/phello.scr >/dev/null 2>&1
./code/Compilers 1 1 input/arith.txt  input/arith.scr  >/dev/null 2>&1
./code/Compilers 1 1 input/io.txt     input/io.scr     >/dev/null 2>&1
./code/Compilers 1 1 input/control.txt input/control.scr >/dev/null 2>&1
./code/Compilers 1 1 input/err_semicolon.txt input/err_semicolon.scr >/dev/null 2>&1
./code/Compilers 1 1 input/err_expr.txt      input/err_expr.scr      >/dev/null 2>&1

# ---- Step 3: Parse each encrypted file (option 4 = Parser) ----
echo ""
echo "============ CASE 1: EMPTY ============"
./code/Compilers 4 input/empty.scr
echo ""
echo "============ CASE 2: HELLO ============"
./code/Compilers 4 input/phello.scr
echo ""
echo "============ CASE 3: ARITHMETIC ============"
./code/Compilers 4 input/arith.scr
echo ""
echo "============ CASE 4: INPUT / OUTPUT ============"
./code/Compilers 4 input/io.scr
echo ""
echo "============ CASE 5: CONTROL FLOW (if-else + while-do) ============"
./code/Compilers 4 input/control.scr
echo ""
echo "============ CASE 6: ERROR - missing semicolon ============"
./code/Compilers 4 input/err_semicolon.scr
echo ""
echo "============ CASE 7: ERROR - incomplete expression ============"
./code/Compilers 4 input/err_expr.scr

echo ""
echo "============ DONE ============"
