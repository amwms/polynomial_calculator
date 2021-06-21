/** @file
  Biblioteka do składania wielomianów.

  @author Anna Stawiska <as429600@students.mimuw.edu.pl>
  @date 20/06/2021
*/

#ifndef __POLY_COMPOSING_H__
#define __POLY_COMPOSING_H__

/**
 * Funkcja składająca wielomiany.
 * @param[in] p : wielomian
 * @param[in] k : rozmiar tablicy wielomianów, które będą podstawiane do złożenia
 * @param[in] q : tablica wielomianów, które będą składane z wielomianem @p p
 * @return @f$p(q[0], q[1], q[2], \ldots)@f$
 */
Poly compose(const Poly *p, size_t k, Poly *q);

#endif //__POLY_COMPOSING_H__
