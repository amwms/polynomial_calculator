#include <stdio.h>
#include <string.h>
#include "memory.h"
#include "parse.h"
#include "read.h"

/**
 * Adding [c] to dynamic array [verse] (array of chars)
*/
void dynamicAddToVerse(char **verse, int *free, int *id, int *allocedVerseMemory, char c, int ifAdd) {
    if (*free == 0) {
        (*verse) = safeRealloc(*verse, sizeof(char) * (*allocedVerseMemory) * 2);
        *free = *allocedVerseMemory;
        *allocedVerseMemory *= 2;
    }

    if (*id == 0 || (*verse)[0] != '#' || ifAdd) {
        (*verse)[*id] = c;
        (*id)++; (*free)--;
    }
}

int readLine(char **verse, int *len, int *allocedVerseMemory) {
    char c;
    int isEOF = 0, free = *allocedVerseMemory, id = 0;

    while (!(isEOF = (scanf("%c", &c) == -1)) && c != '\n') {
        dynamicAddToVerse(verse, &free, &id, allocedVerseMemory, c, 0);
    }

    //adding '\0' at the end of [verse] to show it is a string
    dynamicAddToVerse(verse, &free, &id, allocedVerseMemory, '\0', 1);

    *len = id;

    return (!isEOF || id > 1);
}

/** Checking if the character c is a whitespace.
* If the character c is a whitespace the function returns the value 1,
* if not it returns the value 0.
*/
bool isWhitespace (char c) {
    char *signs = " \0\t\n\v\f\r";

    for (int i = 0; i < 6; i++) {
        if (c == signs[i])
            return 1;
    }

    return 0;
}

/**
 * Arguments:
 * -char *verse -> a string
 * Replacing all the whitespaces except ' ' in the [verse] with '@'
*/
void replaceSpaces(char *verse) {
    int size = strlen(verse);

    for (int i = 0; i < size; i++) {
        if (isWhitespace(verse[i]) && verse[i] != ' ')
            verse[i] = '@';
    }
}

int isComment(char *verse, int size) {
    return (size > 0 && verse[0] == '#');
}

int isCorrectLine(char *verse, int size) {
    for (int i = 0; i < size - 1; i++) {
        char a = verse[i];
        if (!isWhitespace(a))
            return 0;
    }
    return 1;
}

int isEmptyLine(char *verse, int size) {
    for (int i = 0; i < size - 1; i++) {
        if (!(isWhitespace(verse[i])))
            return 0;
    }
    return 1;
}
