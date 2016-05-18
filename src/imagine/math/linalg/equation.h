/*
 Copyright (c) 2015, 2016
        Hugo "hrkz" Frezat <hugo.frezat@gmail.com>

 Permission is hereby granted, free of charge, to any person obtaining a copy
 of this software and associated documentation files (the "Software"), to deal
 in the Software without restriction, including without limitation the rights
 to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 copies of the Software, and to permit persons to whom the Software is
 furnished to do so, subject to the following conditions:

 The above copyright notice and this permission notice shall be included in all
 copies or substantial portions of the Software.

 THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 SOFTWARE.
*/

#ifndef IG_MATH_EQUATION_H
#define IG_MATH_EQUATION_H

#include "imagine/math/linalg/matrix.h"

namespace ig     {
namespace linalg {

template <typename Alg, typename Vec>
void forward_solve(const alg<Alg>& lhs, alg<Vec>& rhs, bool unit = false) {
  assert(lhs.square() && "Forward solver requires a square matrix");
  assert(rhs.vector() && rhs.rows() == lhs.rows()
         && "Invalid b vector to solve");

  if (!unit) rhs[0] /= lhs(0, 0);
  for (size_t i = 1; i < lhs.cols(); ++i) {
    auto s = rhs[i];
    for (size_t j = 0; j < i; ++j)
      s -= lhs(i, j) * rhs[j];

    if (!unit) rhs[i] = s / lhs(i, i);
    else       rhs[i] = s;
  }
}

template <typename Alg, typename Vec>
void backward_solve(const alg<Alg>& lhs, alg<Vec>& rhs, bool unit = false) {
  assert(lhs.square() && "Backward solver requires a square matrix");
  assert(rhs.vector() && rhs.rows() == lhs.rows()
         && "Invalid b vector to solve");

  auto N = lhs.cols();
  if (!unit) rhs[N - 1] /= lhs(N - 1, N - 1);
  for (size_t i = N - 1; i--> 0;) {
    auto s = rhs[i];
    for (size_t j = i + 1; j < N; ++j)
      s -= lhs(i, j) * rhs[j];

    if (!unit) rhs[i] = s / lhs(i, i);
    else       rhs[i] = s;
  }
}

} // namespace linalg
} // namespace ig

#endif // IG_MATH_EQUATION_H