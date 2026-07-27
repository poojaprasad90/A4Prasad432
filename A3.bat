:: ============================================================
:: A3 SCRIPT - Scanner (Scribe Language) - Windows
:: ============================================================
@echo off
echo --------------------------------------------
echo -      ALGONQUIN COLLEGE - COM - 26S       -
echo --------------------------------------------
echo -  [A3: Scanner - Scribe Language]         -
echo --------------------------------------------

:: Optional argument: encrypted input file (default: input\hello.scr)
set "arg=%1"
if "%arg%"=="" set "arg=input\hello.scr"

:: ---- Step 1: Compile (source files live in the code\ folder) ----
echo.
echo [1] Compiling...
gcc code\Compilers.c code\Step1Coder.c code\Step2Reader.c code\Step3Scanner.c ^
    code\Main1Coder.c code\Main2Reader.c code\Main3Scanner.c ^
    -Icode -o code\Compilers.exe
if errorlevel 1 (
    echo COMPILATION FAILED
    pause
    exit /b 1
)
timeout /t 1 /nobreak >nul

:: ---- Step 2: Encrypt the plain Scribe source (option 1 = Coder) ----
echo.
echo [2] Encrypting input\hello.txt -^> input\hello.scr ...
code\Compilers.exe 1 1 input\hello.txt input\hello.scr
timeout /t 1 /nobreak >nul

:: ---- Step 3: Scan the encrypted file (option 3 = Scanner) ----
echo.
echo [3] Scanning %arg% ...
code\Compilers.exe 3 %arg% > out.txt 2> err.txt
timeout /t 1 /nobreak >nul

:: ---- Step 4: Show results ----
echo.
echo ============ SCANNER OUTPUT (out.txt) ============
type out.txt
echo.
echo ============ ERRORS (err.txt) ============
type err.txt
echo.
echo ============ DONE ============
pause
