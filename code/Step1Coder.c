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
# ECHO "    @@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@    
# ECHO "    @@                             @@    
# ECHO "    @@           %&@@@@@@@@@@@     @@    
# ECHO "    @@       @%% (@@@@@@@@@  @     @@    
# ECHO "    @@      @& @   @ @       @     @@    
# ECHO "    @@     @ @ %  / /   @@@@@@     @@    
# ECHO "    @@      & @ @  @@              @@    
# ECHO "    @@       @/ @*@ @ @   @        @@    
# ECHO "    @@           @@@@  @@ @ @      @@    
# ECHO "    @@            /@@    @@@ @     @@    
# ECHO "    @@     @      / /     @@ @     @@    
# ECHO "    @@     @ @@   /@/   @@@ @      @@    
# ECHO "    @@     @@@@@@@@@@@@@@@         @@    
# ECHO "    @@                             @@    
# ECHO "    @@         S C R I B E           @@    
# ECHO "    @@                             @@    
# ECHO "    @@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@    
# ECHO "                                         "
# ECHO "[CODER SCRIPT ..........................]"
# ECHO "                                         "
*/

/*
***********************************************************
* File name: Reader.c
* Compiler: MS Visual Studio 2022
* Course: CST 8152  Compilers, Lab Section: 300
* Assignment: A1
* Date: Sep 01 2025
* Professor: Paulo Sousa
* Purpose: This file is the main code for Buffer/Reader (A12)
************************************************************
*/

/*
 *.............................................................................
 * MAIN ADVICE:
 * - Review the functions to use "Defensive Programming".
 *.............................................................................
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#ifndef COMPILERS_H_
#include "Compilers.h"
#endif

#ifndef CODER_H_
#include "Step1Coder.h"
#endif

// Function to perform the Vigenere cipher (encoding or decoding)
void vigenereFile(const scribe_strg inputFileName, const scribe_strg outputFileName, const scribe_strg key, scribe_intg encode) {
	FILE* inputFile = NULL;
	FILE* outputFile = NULL;
	scribe_intg ch;
	scribe_intg keyLen;
	scribe_intg keyIndex = 0;
	scribe_intg shift;

	if (!inputFileName || !outputFileName || !key)
		return;

	inputFile = fopen(inputFileName, "r");
	if (!inputFile)
		return;

	outputFile = fopen(outputFileName, "w");
	if (!outputFile) {
		fclose(inputFile);
		return;
	}

	keyLen = (scribe_intg)strlen(key);

	while ((ch = fgetc(inputFile)) != EOF) {
		if (ch >= ASCII_START && ch <= ASCII_END) {
			shift = key[keyIndex % keyLen] - ASCII_START;
			if (encode == CYPHER)
				ch = (ch - ASCII_START + shift) % ASCII_RANGE + ASCII_START;
			else
				ch = (ch - ASCII_START - shift + ASCII_RANGE) % ASCII_RANGE + ASCII_START;
			keyIndex++;
		}
		fputc(ch, outputFile);
	}

	fclose(inputFile);
	fclose(outputFile);
}

// Function to perform the Vigenere cipher in memory
scribe_strg vigenereMem(const scribe_strg inputFileName, const scribe_strg key, scribe_intg encode) {
	scribe_strg output = NULL;
	FILE* inputFile = NULL;
	scribe_intg fileSize;
	scribe_intg keyLen;
	scribe_intg keyIndex = 0;
	scribe_intg shift;
	scribe_intg i;

	if (!inputFileName || !key)
		return NULL;

	fileSize = getSizeOfFile(inputFileName);
	if (fileSize <= 0)
		return NULL;

	inputFile = fopen(inputFileName, "r");
	if (!inputFile)
		return NULL;

	output = (scribe_strg)malloc(fileSize + 1);
	if (!output) {
		fclose(inputFile);
		return NULL;
	}

	keyLen = (scribe_intg)strlen(key);

	for (i = 0; i < fileSize; i++) {
		scribe_intg ch = fgetc(inputFile);
		if (ch == EOF)
			break;
		if (ch >= ASCII_START && ch <= ASCII_END) {
			shift = key[keyIndex % keyLen] - ASCII_START;
			if (encode == CYPHER)
				ch = (ch - ASCII_START + shift) % ASCII_RANGE + ASCII_START;
			else
				ch = (ch - ASCII_START - shift + ASCII_RANGE) % ASCII_RANGE + ASCII_START;
			keyIndex++;
		}
		output[i] = (scribe_char)ch;
	}
	output[i] = '\0';

	fclose(inputFile);
	return output;
}

// Function to encode (cypher)
void cypher(const scribe_strg inputFileName, const scribe_strg outputFileName, const scribe_strg key) {
    vigenereFile(inputFileName, outputFileName, key, CYPHER);
}

// Function to decode (decypher)
void decypher(const scribe_strg inputFileName, const scribe_strg outputFileName, const scribe_strg key) {
    vigenereFile(inputFileName, outputFileName, key, DECYPHER);
}

// Get file size (util method)
scribe_intg getSizeOfFile(const scribe_strg filename) {
	scribe_intg size = 0;
	FILE* fp = fopen(filename, "r");
	if (!fp)
		return 0;
	fseek(fp, 0, SEEK_END);
	size = (scribe_intg)ftell(fp);
	fclose(fp);
	return size;
}
