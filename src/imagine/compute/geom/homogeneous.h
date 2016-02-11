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

#ifndef COMPUTE_HOMOGENEOUS_H
#define COMPUTE_HOMOGENEOUS_H

#include "imagine/compute/linalg/matrix.h"
#include "imagine/compute/linalg/analysis.h"
#include "imagine/compute/linalg/quaternion.h"

namespace ig
{

using vec3 = vector<float, 3>;
using vec2 = vector<float, 2>;

using quat = quaternion<float>;

class IG_API mat4 : public matrix<float, 4>
{
public:
  using base = matrix<float, 4>;

  constexpr mat4() = default;
  constexpr mat4(std::initializer_list<float> args)
    : base{args} {}

  template <typename TAlg>
  constexpr mat4(const alg<TAlg>& o) : base{o} {}

  vec3 transform(const vec3& v, bool unit = false) const;

  static mat4 translating(const vec3& t);
  static mat4 rotating(const quat& r);
  static mat4 scaling(const vec3& s);
  
  static mat4 orthographic(std::size_t w, std::size_t h, float zn, float zf);
  static mat4 perspective(float fovy, float asp, float zn, float zf);
  static mat4 look(const vec3& eye, const vec3& focus, const vec3& up);

  static const mat4 eye;
};

} // namespace ig

#endif // COMPUTE_HOMOGENEOUS_H