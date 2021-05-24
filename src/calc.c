/** @file
  Implementacja kalkulatora na wielomianach.

  @author Anna Stawiska <as429600@students.mimuw.edu.pl>
  @date 24/05/2021
*/
#include "poly.h"
#include "memory.h"
#include "read.h"
#include "parse.h"
#include "calculator.h"

/**
 * Działający kalkulator wielomianów, wykonujący komendy z treści zadania.
 */
int main() {
    int count = 1;
    int allocedVerseMemory = 4;
    char *verse = (char*) safeMalloc((sizeof(char) * allocedVerseMemory));
    int len = 0;
    Stack *stack = createStack();

    while (readLine(&verse, &len, &allocedVerseMemory)) {
        char *str = verse;

        // zamiana wszytkich błaych znaków poza spacjami na '@' i '\0' w środku wiersza na '$'
        for (int i = 0; i < len - 1; i++) {
            if (isWhitespace(str[i]) && str[i] != ' ')
                str[i] = '@';
            if (str[i] == '\0')
                str[i] = '$';
        }

        if (!isComment(verse, len) && !isEmptyLine(verse)) {
            if ((verse[0] >= 'A' && verse[0] <= 'Z') || (verse[0] >= 'a' && verse[0] <= 'z'))
                parseAndDoOperation(stack, &str, count);
            else
                parseVerse(&str, stack, count);
        }

        count++;
    }

    for(int i = 0; i < stack->sizeUsed; i++)
        PolyDestroy(&stack->polys[i]);
    free(stack->polys);
    free(stack);
    free(verse);
}
