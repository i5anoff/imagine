/*
 Copyright (c) 2017
        Hugo "hrkz" Frezat <hugo.frezat@gmail.com>

 Permission is hereby granted, free of charge, to any person obtaining a copy
 of this software and associated documentation files (the "Software"), to deal
 in the Software without restriction, including without limitation the rights
 to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 copies of the Software, and to permit persons to whom the Software is
 furnished to do so, subject to the following conditions:

 The above copyright notice and this permission notice shall be included in
 all copies or substantial portions of the Software.

 THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 THE SOFTWARE.
*/

#ifndef IG_MATH_OPERATION_H
#define IG_MATH_OPERATION_H

#include "imagine/math/theory/matrix.h"

namespace ig  {
namespace lin {

template <typename Lhs, typename Rhs>
constexpr auto dot(const matrix_base<Lhs>& lhs, const matrix_base<Rhs>& rhs) {
  return (lhs % rhs).sum();
}

template <typename Lhs, typename Rhs>
constexpr auto cross(const matrix_base<Lhs>& lhs, const matrix_base<Rhs>& rhs) {
  using vec_type = colvec<std::common_type_t< mat_t<Lhs>, mat_t<Rhs> >, 3>;
  assert(lhs.vector() && lhs.vecsize() == 3 && "Cross exists only in three-dimensional space");
  assert(rhs.vector() && rhs.vecsize() == 3 && "Cross exists only in three-dimensional space");

  return vec_type { lhs[1] * rhs[2] - lhs[2] * rhs[1],
                    lhs[2] * rhs[0] - lhs[0] * rhs[2],
                    lhs[0] * rhs[1] - lhs[1] * rhs[0] };
}

} // namespace lin
} // namespace ig

#endif // IG_MATH_OPERATION_H
