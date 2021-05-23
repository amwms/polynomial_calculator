/** @file
  Implementacja stosu.

  @author Anna Stawiska <as429600@students.mimuw.edu.pl>
  @date 02/05/2021
*/
#include <stdio.h>
#include "poly.h"
#include "memory.h"
#include "read.h"
#include "parse.h"
#include "calculator.h"


int main() {
    int count = 1;
    int allocedVerseMemory = 4;
    char *verse = (char*) safeMalloc((sizeof(char) * allocedVerseMemory));
    int len = 0;
    Stack *stack = createStack();

    while (readLine(&verse, &len, &allocedVerseMemory)) {
        char *str = verse;
//        replaceSpaces(&str);
        size_t size = strlen(str);

        for (size_t i = 0; i < size; i++) {
            if (isWhitespace(str[i]) && str[i] != ' ')
                str[i] = '@';
        }


//        printf("%s\n", str);
//TODO-COS NIE DZIAŁA
//        printf("%d, %d\n", isComment(verse, len), isEmptyLine(verse, len));
//        if (!isComment(verse, len) && !isEmptyLine(verse, len)) {
//            if (verse[0] == '(' || isDigit(verse[0]) || verse[0] == '-') {
//                parseVerse(&str, stack, count);
//            }
//            else {
//                parseAndDoOperation(stack, &str, count);
//            }
//        }

//correction - always check for poly if firts sign in verse is not a letter
//        if (count == 66) {
//            printf("SIZE: %d, VERSE:\"%s\"\n", len, verse);
//            printf("is empty: %d\n", isEmptyLine(verse, len));
//        }
        if (!isComment(verse, len) && !isEmptyLine(verse, len)) {
            if ((verse[0] >= 'A' && verse[0] <= 'Z') || (verse[0] >= 'a' && verse[0] <= 'z')) {
                parseAndDoOperation(stack, &str, count);
            }
            else {
                parseVerse(&str, stack, count);
            }
        }

        count++;
    }
//    while (readLine(&verse, &len, &allocedVerseMemory)) {
//        char *str = verse;
////        printf("IS PARSED :%d \n", isParseNumberLL(&str, &count));
////        printf("NUMBER: %lld \n", count);
////        printf("HOW MUCH READ: %ld \n", str - verse);
//
//        printf("IS PARSED POLY:%d \n", isParseVerse(&str, &poly));
//        printf("POLY: "); printPoly(&poly); printf("\n");
//        printf("HOW MUCH READ: %ld \n", str - verse);
//    }
//        if(!(isComment(verse, len) || isCorrectLine(verse, len))) {
//            if (verse[0] == '(')
//                p = parsePoly(verse);
//            else if (isNumber(verse))
//                p= parseCoeff(verse);
//            else
//                printf("BŁAD");
//        }
//        printf("DHDGFHDGF");
//
//        count++;
//    }

    for(int i = 0; i < stack->sizeUsed; i++) {
        PolyDestroy(&stack->polys[i]);
    }
    free(verse);
    free(stack->polys);
    free(stack);

}
