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

#include "composite_op.h"
#include "composite_op_cmp.h"

#include <iostream>

// Test of composite_op.h and composite_op_cmp.h

COMPOSITE_OP_CLASS(A)
  {
  public:

    COMPOSITE_OP_MBR_T(char) c;
    COMPOSITE_OP_MBR_T(int) i;

    A() : c('c'), i(666) { }
  };

COMPOSITE_OP(A, Composite_op_cmp_eq)
COMPOSITE_OP(A, Composite_op_cmp_greater)

COMPOSITE_OP_CLASS(B) : public COMPOSITE_OP_SUB(A)
  {
  public:

    COMPOSITE_OP_SUB(A) mbr_a;

    COMPOSITE_OP_MBR_T(A<>) mbr_a2;

    COMPOSITE_OP_MBR_T(unsigned) u;

    B() : u(27614) { }
  };

COMPOSITE_OP(B, Composite_op_cmp_eq)
COMPOSITE_OP(B, Composite_op_cmp_greater)

unsigned pad_impl_size_total;

template <typename T> struct Pad_impl
  {
    void operator () (const T &, const T &)
      { pad_impl_size_total += sizeof(T); }
  };

COMPOSITE_OP(A, Pad_impl)

COMPOSITE_OP(B, Pad_impl)

// Returns total number of "pad" bytes in a class defined with
// COMPOSITE_OP_CLASS.
//
template <class C>
inline unsigned pad_bytes(const C &c)
  {
    COMPOSITE_OP_CHECK(C, Pad_impl)

    pad_impl_size_total = 0;

    Pad_impl<C>()(c, c);

    return(sizeof(C) - pad_impl_size_total);
  }

using std::cout;

int main()
  {
    B<> b1, b2;

    if (0 == 1)
      cout << "FAIL\n";

    if (!(b1 == b2))
      cout << "FAIL\n";
    if (b1 < b2)
      cout << "FAIL\n";
    if (!(b1 <= b2))
      cout << "FAIL\n";
    if (b1 > b2)
      cout << "FAIL\n";
    if (!(b1 >= b2))
      cout << "FAIL\n";

    ++b1.c;

    if (b1 == b2)
      cout << "FAIL\n";
    if (!(b1 > b2))
      cout << "FAIL\n";
    if (!(b1 >= b2))
      cout << "FAIL\n";
    if (b1 < b2)
      cout << "FAIL\n";
    if (b1 <= b2)
      cout << "FAIL\n";

    ++b2.mbr_a.i;

    if (b1 == b2)
      cout << "FAIL\n";
    if (!(b1 > b2))
      cout << "FAIL\n";
    if (!(b1 >= b2))
      cout << "FAIL\n";
    if (b1 < b2)
      cout << "FAIL\n";
    if (b1 <= b2)
      cout << "FAIL\n";

    --b1.c;

    if (b1 == b2)
      cout << "FAIL\n";
    if (b1 > b2)
      cout << "FAIL\n";
    if (b1 >= b2)
      cout << "FAIL\n";
    if (!(b1 < b2))
      cout << "FAIL\n";
    if (!(b1 <= b2))
      cout << "FAIL\n";

    --b2.mbr_a.i;
    ++b1.mbr_a2.c;

    if (b1 == b2)
      cout << "FAIL\n";
    if (!(b1 > b2))
      cout << "FAIL\n";
    if (!(b1 >= b2))
      cout << "FAIL\n";
    if (b1 < b2)
      cout << "FAIL\n";
    if (b1 <= b2)
      cout << "FAIL\n";

    --b1.mbr_a2.c;
    ++b2.u;

    if (b1 == b2)
      cout << "FAIL\n";
    if (b1 > b2)
      cout << "FAIL\n";
    if (b1 >= b2)
      cout << "FAIL\n";
    if (!(b1 < b2))
      cout << "FAIL\n";
    if (!(b1 <= b2))
      cout << "FAIL\n";

    --b2.u;

    if (!(b1 == b2))
      cout << "FAIL\n";
    if (b1 < b2)
      cout << "FAIL\n";
    if (!(b1 <= b2))
      cout << "FAIL\n";
    if (b1 > b2)
      cout << "FAIL\n";
    if (!(b1 >= b2))
      cout << "FAIL\n";

    cout << "A pad bytes = " << pad_bytes(A<>()) << '\n';

    cout << "B pad bytes = " << pad_bytes(B<>()) << '\n';

    return(0);
  }

#include "composite_op_cmp.cpp"
