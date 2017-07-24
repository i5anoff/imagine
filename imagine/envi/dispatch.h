/*
 Copyright (c) 2017
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

#ifndef IG_ENVI_DISPATCH_H
#define IG_ENVI_DISPATCH_H

#include "imagine/ig.h"

namespace ig   {
namespace impl { class dispatch_native; }

class ig_api dispatch {
public:
  friend class window;

  dispatch();
  virtual ~dispatch();

  virtual int32_t run();
  virtual void exit(int32_t return_code);

  virtual void process_events();
  template <typename Callable> void tick(Callable&& fn) { tick_ = fn; }

  dispatch(const dispatch&) = delete;
  dispatch& operator=(const dispatch&) = delete;

private:
  std::unique_ptr<impl::dispatch_native> native_;
  std::function
  < void() > tick_ = [] {};
};

} // namespace ig

#endif // IG_ENVI_DISPATCH_H
