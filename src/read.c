/** @file
  Implementacja wczytywania.

  @author Anna Stawiska <as429600@students.mimuw.edu.pl>
  @date 24/05/2021
*/

#include <stdio.h>
#include "memory.h"
#include "read.h"

/**
 * Dynamicznie rozszerza tablicę (jeśli trzeba) i dodaje do niej znak.
 * @param[in] verse : wczytywany wiersz
 * @param[in] free : ilość wolnego miejsca w wierszu (tablicy charów)
 * @param[in] id : indeks
 * @param[in] alloctedVerseMemory : allokowana pamięć na wiersz
 * @param[in] c : wczytywany znak
 * @param[in] ifAdd : czy dodawać znak bo jest ostatnim znakiem (dodatkowym dodawanym przez "nas" - '\0')
 * @return
*/
static void dynamicAddToVerse(char **verse, int *free, int *id, int *allocedVerseMemory, char c, int ifAdd) {
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

bool isWhitespace (char c) {
    char *signs = " \0\t\n\v\f\r";

    for (int i = 0; i < 7; i++) {
        if (c == signs[i])
            return 1;
    }

    return 0;
}


int isComment(char *verse, int size) {
    return (size > 0 && verse[0] == '#');
}

int isEmptyLine(char *verse) {
    return verse[0] == '\0';
}
