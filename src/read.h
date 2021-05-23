#ifndef READ_H
#define READ_H

#include "poly.h"

/** Arguments:
 * -char **verse -> pointer to a string (which is being read in this function)
 * -int *len -> length of [verse]
 * -int *alloctedVerseMemory -> allocated verse memory
 *Reading one verse/line from the input file.
 *The function returns whether the verse/line from the input exists of if it is EOF.
*/
int readLine(char **verse, int *len, int *allocedVerseMemory);

/**
 * Arguments:
 * -char *verse -> a string
 * Replacing all the whitespaces except ' ' in the [verse] with '@'
*/
void replaceSpaces(char **verse);

bool isWhitespace (char c);

/**
 * Arguments:
 * -char *verse -> a string
 * -int size -> length of [verse]
 * This function tells whether [verse] is a comment or not.
 */
int isComment(char *verse, int size);
/**
 * Arguments:
 * -char *verse -> a string
 * -int size -> length of [verse]
 * This function tells whether [verse] is a correct input line.
 */
int isCorrectLine(char *verse, int size);
/**
 * Arguments:
 * -char *verse -> a string
 * -int size -> length of [verse]
 * This function tells whether [verse] is empty.
 */
int isEmptyLine(char *verse, int size);

#endif