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

#include <iostream>

// Test of compostie_op.h.  Will generate operator == for a class if all its
// components (data members, base classes) have operator ==.

// Goodbye thread safety.
bool eq_impl_are_equal;

template <typename T> struct Eq
  {
    void operator () (const T &op1, const T &op2)
      { eq_impl_are_equal = eq_impl_are_equal && (op1 == op2); }
  };

template <class C, COMPOSITE_OP_CHECK(C, Eq)>
inline bool operator == (const C &op1, const C &op2)
  {
    eq_impl_are_equal = true;

    Eq<C>()(op1, op2);

    return(eq_impl_are_equal);
  }

COMPOSITE_OP_CLASS(A)
  {
  public:

    COMPOSITE_OP_MBR_T(char) c;
    COMPOSITE_OP_MBR_T(int) i;

    A() : c('c'), i(666) { }
  };

COMPOSITE_OP_2(A, Eq)

COMPOSITE_OP_CLASS(B) : public COMPOSITE_OP_SUB(A)
  {
  public:

    COMPOSITE_OP_SUB(A) mbr_a;

    COMPOSITE_OP_MBR_T(A<>) mbr_a2;

    COMPOSITE_OP_MBR_T(unsigned) u;

    B() : u(27614) { }
  };

COMPOSITE_OP_2(B, Eq)

using std::cout;

int main()
  {
    B<> b1, b2;

    if (!(b1 == b2))
      cout << "FAIL\n";

    ++b1.c;

    if (b1 == b2)
      cout << "FAIL\n";

    --b1.c;
    ++b2.mbr_a.i;

    if (b1 == b2)
      cout << "FAIL\n";

    --b2.mbr_a.i;
    ++b1.mbr_a2.c;

    if (b1 == b2)
      cout << "FAIL\n";

    --b1.mbr_a2.c;
    ++b2.u;

    if (b1 == b2)
      cout << "FAIL\n";

    --b2.u;

    if (!(b1 == b2))
      cout << "FAIL\n";

    return(0);
  }
