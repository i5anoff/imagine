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

#ifndef IG_ENVI_LIBRARY_H
#define IG_ENVI_LIBRARY_H

#include "imagine/ig.h"

#include <memory>

namespace ig   {
namespace impl { class library_native; }

class IG_API library {
public:
  using funcptr_type = void (*)();

  library();
  explicit library(const std::string& path);
  virtual ~library();

  auto resolve(const char* symbol) -> funcptr_type;
  bool open(const std::string& path);
  void close();

  bool loaded() const;

  library(const library&) = delete;
  library& operator=(const library&) = delete;

private:
  std::unique_ptr<impl::library_native> native_;
};

} // namespace ig

#endif // IG_ENVI_LIBRARY_H
