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

#ifndef IG_NC_MATRIX_H
#define IG_NC_MATRIX_H

#include "imagine/nc/linalg/base/alg.h"
#include "imagine/nc/linalg/base/alg_block.h"
#include "imagine/nc/linalg/base/alg_col.h"
#include "imagine/nc/linalg/base/alg_row.h"
#include "imagine/nc/linalg/base/alg_diag.h"
#include "imagine/nc/linalg/base/alg_trans.h"
#include "imagine/nc/linalg/base/relational.h"

#include "imagine/nc/linalg/expr/product_expr.h"

#include "imagine/nc/linalg/expr/unary_expr.h"
#include "imagine/nc/linalg/expr/scalar_expr.h"
#include "imagine/nc/linalg/expr/binary_expr.h"

#include <algorithm>
#include <numeric>

namespace ig {

template <typename T_, int M_, int N_>
struct alg_traits< matrix<T_, M_, N_> > {

  using T = T_;
  static constexpr auto M = M_;
  static constexpr auto N = N_;
};

template <typename T, int M = dynamic_sized, int N = M>
class matrix : public alg< matrix<T, M, N> > {
public:
  using base = alg< matrix<T, M, N> >;
  using make = std::initializer_list<T>;
  static constexpr auto dynamics_rows = (M < 0);
  static constexpr auto dynamics_cols = (N < 0);
  static constexpr auto dynamics = (dynamics_rows && dynamics_cols);
  static constexpr auto hybrids  = (dynamics_rows || dynamics_cols);
  static constexpr auto statics  = !hybrids;

  template < typename = std::enable_if_t<statics> >
  constexpr matrix() : rows_{M}, cols_{N}, data_{} {}

  template < typename = std::enable_if_t<statics> >
  constexpr matrix(std::initializer_list<T> args)
    : rows_{M}, cols_{N}, data_{} {
    std::copy(args.begin(), args.end(), data_.begin());
    if (args.size() != size()) std::uninitialized_fill(data_.begin() + args.size(), data_.end(),
                                                      (*args.begin()));
  }

  template < typename = std::enable_if_t<hybrids> >
  constexpr matrix(size_t n) 
    : rows_{dynamics_rows ? n : M}, cols_{dynamics_cols ? n : N}, data_(rows_ * cols_) {}

  template < typename = std::enable_if_t<dynamics> >
  constexpr matrix(size_t m, size_t n) 
    : rows_{m}, cols_{n}, data_(rows_ * cols_) {}

  template <typename Alg>
  constexpr matrix(const alg<Alg>& o);

  matrix(const base& o) { *this = o.derived(); }

  constexpr auto rows() const { return rows_; }
  constexpr auto cols() const { return cols_; }

  auto ptr() const { return data_.data(); }
  auto ptr()       { return data_.data(); }

  auto operator()(size_t row, size_t col) const -> const T&;
  auto operator()(size_t row, size_t col) -> T&;

  auto operator[](size_t n) const -> const T&;
  auto operator[](size_t n) -> T&;

  auto make_eye() -> matrix&;

  template < typename = std::enable_if_t<statics> >
  static auto eye()         { matrix eye{}; return eye.make_eye(); }

  template < typename = std::enable_if_t<hybrids> >
  static auto eye(size_t n) { matrix eye{n}; return eye.make_eye(); }

protected:
  size_t rows_, cols_;
  std::conditional_t< hybrids, std::vector<T>, std::array<T, M * N> > data_;
};

template <typename T, int M, int N>
template <typename Alg>
constexpr matrix<T, M, N>::matrix(const alg<Alg>& o)
  : rows_{statics ? M : (dynamics_rows ? o.rows() : M)},
    cols_{statics ? N : (dynamics_cols ? o.cols() : N)} {
  eval(*this, o, data_);
}

template <typename T, int M, int N>
auto matrix<T, M, N>::operator()(size_t row, size_t col) const -> const T& {
  assert(row < rows_ && col < cols_ && "Invalid matrix subscript");
  return data_[col*rows_ + row];
}

template <typename T, int M, int N>
auto matrix<T, M, N>::operator()(size_t row, size_t col) -> T& {
  assert(row < rows_ && col < cols_ && "Invalid matrix subscript");
  return data_[col*rows_ + row];
}

template <typename T, int M, int N>
auto matrix<T, M, N>::operator[](size_t n) const -> const T& {
  assert(n < rows_ * cols_ && "Invalid matrix subscript");
  return data_[n];
}

template <typename T, int M, int N>
auto matrix<T, M, N>::operator[](size_t n) -> T& {
  assert(n < rows_ * cols_ && "Invalid matrix subscript");
  return data_[n];
}

// Eye (Identity)
template <typename T, int M, int N>
auto matrix<T, M, N>::make_eye() -> matrix& {
  size_t i = 0, stride = diagsize() + 1;
  std::generate(begin(), end(), [&i, &stride, this]() { return !(i++ % stride); });
  return *this;
}

#define IG_ALG_USINGS(Type, TypeSuffix, Size) \
using matrix##Size##TypeSuffix = matrix<Type, Size, Size>; \
using vector##Size##TypeSuffix = matrix<Type, Size, 1>;    \
using row##Size##TypeSuffix    = matrix<Type, 1, Size>;

#define IG_MATRIX_USINGS_SIZES(Type, TypeSuffix) \
using matrix##TypeSuffix = matrix<Type, dynamic_sized, dynamic_sized>; \
using vector##TypeSuffix = matrix<Type, dynamic_sized, 1>;             \
using row##TypeSuffix    = matrix<Type, 1, dynamic_sized>;             \
IG_ALG_USINGS(Type, TypeSuffix, 2) \
IG_ALG_USINGS(Type, TypeSuffix, 3) \
IG_ALG_USINGS(Type, TypeSuffix, 4)

IG_MATRIX_USINGS_SIZES(int, i)
IG_MATRIX_USINGS_SIZES(float, f)
IG_MATRIX_USINGS_SIZES(double, d)
IG_MATRIX_USINGS_SIZES(std::complex<float>, cf)
IG_MATRIX_USINGS_SIZES(std::complex<double>, cd)

#undef IG_MATRIX_USINGS_SIZES
#undef IG_ALG_USINGS

} // namespace ig

#endif // IG_NC_MATRIX_H