:: ============================================================
:: A4 SCRIPT - Parser (Scribe Language) - Windows
:: Compiles the full front-end (Coder+Reader+Scanner+Parser),
:: encrypts the 3 test inputs, and parses each one.
:: ============================================================
@echo off
echo --------------------------------------------
echo -      ALGONQUIN COLLEGE - COM - 26S       -
echo --------------------------------------------
echo -  [A4: Parser - Scribe Language]          -
echo --------------------------------------------

:: ---- Step 1: Compile (all source files in code\) ----
echo.
echo [1] Compiling...
gcc code\Compilers.c code\Step1Coder.c code\Step2Reader.c code\Step3Scanner.c ^
    code\Step4Parser.c code\Main1Coder.c code\Main2Reader.c code\Main3Scanner.c ^
    code\Main4Parser.c -Icode -o code\Compilers.exe
if errorlevel 1 (
    echo COMPILATION FAILED
    pause
    exit /b 1
)
timeout /t 1 /nobreak >nul

:: ---- Step 2: Encrypt the 3 plain Scribe sources (option 1 = Coder) ----
echo.
echo [2] Encrypting test inputs -^> .scr ...
code\Compilers.exe 1 1 input\empty.txt  input\empty.scr
code\Compilers.exe 1 1 input\phello.txt input\phello.scr
code\Compilers.exe 1 1 input\arith.txt  input\arith.scr
code\Compilers.exe 1 1 input\io.txt     input\io.scr
code\Compilers.exe 1 1 input\control.txt input\control.scr
code\Compilers.exe 1 1 input\err_semicolon.txt input\err_semicolon.scr
code\Compilers.exe 1 1 input\err_expr.txt      input\err_expr.scr
timeout /t 1 /nobreak >nul

:: ---- Step 3: Parse each encrypted file (option 4 = Parser) ----
echo.
echo ============ CASE 1: EMPTY ============
code\Compilers.exe 4 input\empty.scr
echo.
echo ============ CASE 2: HELLO ============
code\Compilers.exe 4 input\phello.scr
echo.
echo ============ CASE 3: ARITHMETIC ============
code\Compilers.exe 4 input\arith.scr
echo. 
echo ============ CASE 4: INPUT / OUTPUT ============
code\Compilers.exe 4 input\io.scr
echo.
echo ============ CASE 5: CONTROL FLOW (if-else + while-do) ============
code\Compilers.exe 4 input\control.scr
echo.
echo ============ CASE 6: ERROR - missing semicolon ============
code\Compilers.exe 4 input\err_semicolon.scr
echo.
echo ============ CASE 7: ERROR - incomplete expression ============
code\Compilers.exe 4 input\err_expr.scr

echo.
echo ============ DONE ============
pause
