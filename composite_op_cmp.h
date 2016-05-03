/*
Copyright (c) 2016 Walter William Karas

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.
*/

#ifndef COMPOSITE_OP_CMP_H_20160919_
#define COMPOSITE_OP_CMP_H_20160919_

#include "composite_op.h"

// Warning:  These comparison operators are not thread-safe/reentrant.

template <typename T> struct Composite_op_cmp
  {
    int operator () (const T &op1, const T &op2)
      {
        if (op1 < op2)
          return(-1);
        if (op2 < op1)
          return(1);
        return(0);
      }
  };

namespace Composite_op_cmp_ns
{

namespace Impl
{

enum Fuzzy { No = 0, Maybe, Yes };

extern Fuzzy accum;

} // end namespace Impl

} // end namespace Composite_op_cmp_ns

template <typename T> struct Composite_op_cmp_eq
  {

    void operator () (const T &op1, const T &op2)
      {
        using namespace Composite_op_cmp_ns;

        if ((Impl::accum != Impl::No) && !(op1 == op2))
          Impl::accum = Impl::No;
      }
  };

template <typename T> struct Composite_op_cmp_greater
  {
    void operator () (const T &op1, const T &op2)
      {
        using namespace Composite_op_cmp_ns;

        if (Impl::accum == Impl::Maybe)
          {
            int c = Composite_op_cmp<T>()(op1, op2);
            if (c > 0)
              Impl::accum = Impl::Yes;
            else if (c < 0)
              Impl::accum = Impl::No;
          }
      }
  };

template <class C>
inline bool operator == (const C &op1, const C &op2)
  {
    COMPOSITE_OP_CHECK(C, Composite_op_cmp_eq)

    using namespace Composite_op_cmp_ns;

    Impl::accum = Impl::Yes;

    Composite_op_cmp_eq<C>()(op1, op2);

    return(Impl::accum == Impl::Yes);
  }

template <typename T>
inline bool operator != (const T &op1, const T &op2)
  { return(!(op1 == op2)); }

template <class C>
inline bool operator > (const C &op1, const C &op2)
  {
    COMPOSITE_OP_CHECK(C, Composite_op_cmp_eq)

    using namespace Composite_op_cmp_ns;

    Impl::accum = Impl::Maybe;

    Composite_op_cmp_greater<C>()(op1, op2);

    return(Impl::accum == Impl::Yes);
  }

template <class C>
inline bool operator >= (const C &op1, const C &op2)
  {
    COMPOSITE_OP_CHECK(C, Composite_op_cmp_eq)

    using namespace Composite_op_cmp_ns;

    Impl::accum = Impl::Maybe;

    Composite_op_cmp_greater<C>()(op1, op2);

    return(Impl::accum != Impl::No);
  }

template <typename T>
inline bool operator < (const T &op1, const T &op2)
  { return(!(op1 >= op2)); }

template <typename T>
inline bool operator <= (const T &op1, const T &op2)
  { return(!(op1 > op2)); }

#endif // Include once.
