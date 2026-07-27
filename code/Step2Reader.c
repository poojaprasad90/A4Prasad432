/*
************************************************************
* COMPILERS COURSE - Algonquin College
* Code version: Summer, 2026
* Author: Pooja Prasad
* Professors: Paulo Sousa
************************************************************
*/

#include <ctype.h>
#include <string.h>

#ifndef COMPILERS_H_
#include "Compilers.h"
#endif

#ifndef CODER_H_
#include "Step1Coder.h"
#endif

#ifndef READER_H_
#include "Step2Reader.h"
#endif

/*
***********************************************************
* Function name: readerCreate
***********************************************************
*/
BufferPointer readerCreate(scribe_intg size, scribe_real factor) {
	BufferPointer readerPointer = NULL;
	if (size <= 0)
		size = READER_DEFAULT_SIZE;
	if (factor <= 0.0f)
		factor = READER_DEFAULT_FACTOR;
	if (size > READER_MAX_SIZE)
		return NULL;
	/* Allocate reader structure */
	readerPointer = (BufferPointer)calloc(1, sizeof(Buffer));
	if (!readerPointer)
		return NULL;
	/* Allocate content */
	readerPointer->content = (scribe_strg)malloc(size);
	if (!readerPointer->content) {
		free(readerPointer);
		return NULL;
	}
	/* Initialize properties */
	readerPointer->size = size;
	readerPointer->factor = factor;
	/* Initialize positions */
	readerPointer->position.wrte = 0;
	readerPointer->position.read = 0;
	readerPointer->position.mark = 0;
	/* Initialize flags - empty by default */
	readerPointer->flags.isEmpty = SCRIBE_TRUE;
	readerPointer->flags.isFull = SCRIBE_FALSE;
	readerPointer->flags.isRead = SCRIBE_FALSE;
	readerPointer->flags.isMoved = SCRIBE_FALSE;
	/* Initialize histogram */
	memset(readerPointer->histogram, 0, NCHAR * sizeof(scribe_intg));
	/* Initialize errors */
	readerPointer->numReaderErrors = 0;
	readerPointer->checkSum = 0;
	return readerPointer;
}

/*
***********************************************************
* Function name: readerAddChar
***********************************************************
*/
BufferPointer readerAddChar(BufferPointer const readerPointer, scribe_char ch) {
	scribe_strg tempReader = NULL;
	scribe_intg newSize = 0;
	scribe_real ratio = 0.0f;
	/* Defensive programming */
	if (!readerPointer)
		return NULL;
	if (ch < 0 || ch > 127) {
		readerPointer->numReaderErrors++;
		return NULL;
	}
	/* Check if buffer is not full */
	if (readerPointer->position.wrte < readerPointer->size) {
		readerPointer->flags.isFull = SCRIBE_FALSE;
	} else {
		/* Buffer is full - try to resize */
		readerPointer->flags.isFull = SCRIBE_TRUE;
		/* Calculate new size */
		ratio = (scribe_real)(1.0f + readerPointer->factor);
		newSize = (scribe_intg)(readerPointer->size * ratio);
		/* Defensive programming: check overflow and max */
		if (newSize <= 0 || newSize <= readerPointer->size || newSize > READER_MAX_SIZE)
			return NULL;
		/* Reallocate */
		tempReader = (scribe_strg)realloc(readerPointer->content, newSize);
		if (!tempReader)
			return NULL;
		/* Check if memory moved */
		if (tempReader != readerPointer->content)
			readerPointer->flags.isMoved = SCRIBE_TRUE;
		readerPointer->content = tempReader;
		readerPointer->size = newSize;
		readerPointer->flags.isFull = SCRIBE_FALSE;
	}
	/* Add the char */
	readerPointer->content[readerPointer->position.wrte++] = ch;
	/* Update isEmpty flag */
	readerPointer->flags.isEmpty = SCRIBE_FALSE;
	/* Check if now full */
	if (readerPointer->position.wrte >= readerPointer->size)
		readerPointer->flags.isFull = SCRIBE_TRUE;
	/* Update histogram */
	readerPointer->histogram[(unsigned char)ch]++;
	return readerPointer;
}

/*
***********************************************************
* Function name: readerClear
***********************************************************
*/
scribe_boln readerClear(BufferPointer const readerPointer) {
	if (!readerPointer)
		return SCRIBE_FALSE;
	readerPointer->position.wrte = 0;
	readerPointer->position.read = 0;
	readerPointer->position.mark = 0;
	readerPointer->flags.isEmpty = SCRIBE_TRUE;
	readerPointer->flags.isFull = SCRIBE_FALSE;
	readerPointer->flags.isRead = SCRIBE_FALSE;
	readerPointer->flags.isMoved = SCRIBE_FALSE;
	readerPointer->numReaderErrors = 0;
	readerPointer->checkSum = 0;
	memset(readerPointer->histogram, 0, NCHAR * sizeof(scribe_intg));
	return SCRIBE_TRUE;
}

/*
***********************************************************
* Function name: readerFree
***********************************************************
*/
scribe_boln readerFree(BufferPointer const readerPointer) {
	if (!readerPointer)
		return SCRIBE_FALSE;
	if (readerPointer->content)
		free(readerPointer->content);
	free(readerPointer);
	return SCRIBE_TRUE;
}

/*
***********************************************************
* Function name: readerIsFull
***********************************************************
*/
scribe_boln readerIsFull(BufferPointer const readerPointer) {
	if (!readerPointer)
		return SCRIBE_FALSE;
	return readerPointer->flags.isFull;
}

/*
***********************************************************
* Function name: readerIsEmpty
***********************************************************
*/
scribe_boln readerIsEmpty(BufferPointer const readerPointer) {
	if (!readerPointer)
		return SCRIBE_FALSE;
	return readerPointer->flags.isEmpty;
}

/*
***********************************************************
* Function name: readerSetMark
***********************************************************
*/
scribe_boln readerSetMark(BufferPointer const readerPointer, scribe_intg mark) {
	if (!readerPointer)
		return SCRIBE_FALSE;
	if (mark < 0 || mark > readerPointer->position.wrte)
		return SCRIBE_FALSE;
	readerPointer->position.mark = mark;
	return SCRIBE_TRUE;
}

/*
***********************************************************
* Function name: readerPrint
***********************************************************
*/
scribe_intg readerPrint(BufferPointer const readerPointer) {
	scribe_intg count = 0;
	scribe_char c;
	if (!readerPointer)
		return READER_ERROR;
	/* Reset read position */
	readerPointer->position.read = 0;
	readerPointer->flags.isRead = SCRIBE_FALSE;
	while (!readerPointer->flags.isRead) {
		c = readerGetChar(readerPointer);
		if (readerPointer->flags.isRead)
			break;
		printf("%c", c);
		count++;
	}
	return count;
}

/*
***********************************************************
* Function name: readerLoad
***********************************************************
*/
scribe_intg readerLoad(BufferPointer const readerPointer, scribe_strg fileName) {
	scribe_intg count = 0;
	scribe_strg content = NULL;
	scribe_intg i = 0;
	if (!readerPointer || !fileName)
		return READER_ERROR;
	/* Decrypt file content using A1 coder */
	content = vigenereMem(fileName, STR_LANGNAME, DECYPHER);
	if (!content)
		return READER_ERROR;
	/* Add each char to reader */
	while (content[i] != '\0') {
		if (!readerAddChar(readerPointer, content[i])) {
			free(content);
			return READER_ERROR;
		}
		count++;
		i++;
	}
	free(content);
	return count;
}

/*
***********************************************************
* Function name: readerRecover
***********************************************************
*/
scribe_boln readerRecover(BufferPointer const readerPointer) {
	if (!readerPointer)
		return SCRIBE_FALSE;
	readerPointer->position.read = 0;
	readerPointer->position.mark = 0;
	readerPointer->flags.isRead = SCRIBE_FALSE;
	return SCRIBE_TRUE;
}

/*
***********************************************************
* Function name: readerRetract
***********************************************************
*/
scribe_boln readerRetract(BufferPointer const readerPointer) {
	if (!readerPointer)
		return SCRIBE_FALSE;
	if (readerPointer->position.read <= 0)
		return SCRIBE_FALSE;
	readerPointer->position.read--;
	readerPointer->flags.isRead = SCRIBE_FALSE;
	return SCRIBE_TRUE;
}

/*
***********************************************************
* Function name: readerRestore
***********************************************************
*/
scribe_boln readerRestore(BufferPointer const readerPointer) {
	if (!readerPointer)
		return SCRIBE_FALSE;
	readerPointer->position.read = readerPointer->position.mark;
	readerPointer->flags.isRead = SCRIBE_FALSE;
	return SCRIBE_TRUE;
}

/*
***********************************************************
* Function name: readerGetChar
***********************************************************
*/
scribe_char readerGetChar(BufferPointer const readerPointer) {
	if (!readerPointer)
		return READER_TERMINATOR;
	/* Check if end of buffer reached */
	if (readerPointer->position.read >= readerPointer->position.wrte) {
		readerPointer->flags.isRead = SCRIBE_TRUE;
		return READER_TERMINATOR;
	}
	readerPointer->flags.isRead = SCRIBE_FALSE;
	return readerPointer->content[readerPointer->position.read++];
}

/*
***********************************************************
* Function name: readerGetContent
***********************************************************
*/
scribe_strg readerGetContent(BufferPointer const readerPointer, scribe_intg pos) {
	if (!readerPointer)
		return NULL;
	if (pos < 0 || pos >= readerPointer->position.wrte)
		return NULL;
	return readerPointer->content + pos;
}

/*
***********************************************************
* Function name: readerGetPosRead
***********************************************************
*/
scribe_intg readerGetPosRead(BufferPointer const readerPointer) {
	if (!readerPointer)
		return READER_ERROR;
	return readerPointer->position.read;
}

/*
***********************************************************
* Function name: readerGetPosWrte
***********************************************************
*/
scribe_intg readerGetPosWrte(BufferPointer const readerPointer) {
	if (!readerPointer)
		return READER_ERROR;
	return readerPointer->position.wrte;
}

/*
***********************************************************
* Function name: readerGetPosMark
***********************************************************
*/
scribe_intg readerGetPosMark(BufferPointer const readerPointer) {
	if (!readerPointer)
		return READER_ERROR;
	return readerPointer->position.mark;
}

/*
***********************************************************
* Function name: readerGetSize
***********************************************************
*/
scribe_intg readerGetSize(BufferPointer const readerPointer) {
	if (!readerPointer)
		return READER_ERROR;
	return readerPointer->size;
}

/*
***********************************************************
* Function name: readerPrintFlags
***********************************************************
*/
scribe_void readerPrintFlags(BufferPointer const readerPointer) {
	if (!readerPointer)
		return;
	printf("Flag.isEmpty = %d\n", readerPointer->flags.isEmpty);
	printf("Flag.isFull = %d\n", readerPointer->flags.isFull);
	printf("Flag.isMoved = %d\n", readerPointer->flags.isMoved);
	printf("Flag.isRead = %d\n", readerPointer->flags.isRead);
}

/*
***********************************************************
* Function name: readerPrintStat
***********************************************************
*/
scribe_void readerPrintStat(BufferPointer const readerPointer) {
	scribe_intg i;
	if (!readerPointer)
		return;
	for (i = 0; i < NCHAR; i++) {
		if (readerPointer->histogram[i] > 0) {
			if (i >= 32 && i <= 126)
				printf("B[%c]=%d, ", (char)i, readerPointer->histogram[i]);
			else
				printf("B[%d]=%d, ", i, readerPointer->histogram[i]);
		}
	}
	printf("\n");
}

/*
***********************************************************
* Function name: readerNumErrors
***********************************************************
*/
scribe_intg readerNumErrors(BufferPointer const readerPointer) {
	if (!readerPointer)
		return READER_ERROR;
	return readerPointer->numReaderErrors;
}

/*
***********************************************************
* Function name: readerChecksum
***********************************************************
*/
scribe_intg readerChecksum(BufferPointer const readerPointer) {
	scribe_intg i;
	scribe_intg sum = 0;
	if (!readerPointer)
		return READER_ERROR;
	for (i = 0; i < readerPointer->position.wrte; i++) {
		sum += (unsigned char)readerPointer->content[i];
	}
	readerPointer->checkSum = sum & 0xFF;
	return readerPointer->checkSum;
}
