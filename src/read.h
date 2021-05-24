/** @file
  Wczytywanie.

  @author Anna Stawiska <as429600@students.mimuw.edu.pl>
  @date 24/05/2021
*/

#ifndef READ_H
#define READ_H

#include "poly.h"

/**
 * Wczytuje wiersz z wejścia.
 * @param[in] verse : wczytywany wiersz
 * @param[in] len : długość wiersza
 * @param[in] alloctedVerseMemory : allokowana pamięć na wiersz
 * @return Informuje czy wczytany wiersz isnieje czy jest EOF-em.
*/
int readLine(char **verse, int *len, int *allocedVerseMemory);

/**
 * Sprawdza czy znak jest białym znakiem.
 * @param[in] c : znak
 * @return Zwraca czy znak jest białym znakiem.
*/
bool isWhitespace (char c);

/**
 * Sprawdza czy wiersz zawiera komentarz.
 * @param[in] verse : wiersz
 * @param[in] size : rozmiar wiersza
 * @return Zwraca czy wiersz jest komentarzem.
*/
int isComment(char *verse, int size);

/**
 * Sprawdza czy wiersz jest pusty.
 * @param[in] verse : wiersz
 * @param[in] size : rozmiar wiersza
 * @return Zwraca czy wiersz jest pusty.
*/
int isEmptyLine(char *verse);

#endif