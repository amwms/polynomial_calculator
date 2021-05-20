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
        if (!isComment(verse, len) || !isEmptyLine(verse, len)) {
            if (verse[0] == '(' || isDigit(verse[0])) {
                parseVerse(&verse, stack, count);
            }
            else {
                parseAndDoOperation(stack, &verse, count);
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


}
