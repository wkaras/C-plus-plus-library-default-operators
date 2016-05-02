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

#ifndef COMPOSITE_OP_H_20160502_
#define COMPOSITE_OP_H_20160502_

namespace Composite_op_impl
{

enum Usage { Jekyll, Hyde };

template <typename T>
struct Dummy_op { };

template <typename T, template <typename T> class Op, Usage U>
struct J_and_h;

template <typename T>
struct J_and_h<T, Dummy_op, Jekyll>
  {
    typedef T Type;
  };

template <typename T, template <typename T> class Op>
struct J_and_h<T, Op, Hyde>
  {
    class Type
      {
      private:
        T v;

      public:

        void operator = (const Type &src) { Op<T>()(this->v, src.v); }
      };
  };

} // end namespace Composite_op_impl

// See tst.cpp for example of use.

// Define templeted structure named CLASS_NAME, this macro generates
// everyting before : or {.  The defined class must use the default
// assignement operator.
#define COMPOSITE_OP_CLASS(CLASS_NAME) \
  template < \
    template <typename T> class Composite_op_impl_op_tpl = \
      Composite_op_impl::Dummy_op, \
    Composite_op_impl::Usage Composite_op_impl_usage = \
      Composite_op_impl::Jekyll> \
  struct CLASS_NAME

// Expands to type name for base or class instance member, whose class
// must have been defined with COMPOSITE_OP_CLASS with same op template.
#define COMPOSITE_OP_SUB(CLASS_NAME) \
  CLASS_NAME<Composite_op_impl_op_tpl, Composite_op_impl_usage>

// Expands to type name for primitive swappable member.  Type TYPE_NAME
// must have an assignment operator.
#define COMPOSITE_OP_MBR_T(TYPE_NAME) \
  typename Composite_op_impl::J_and_h< \
    TYPE_NAME, Composite_op_impl_op_tpl, Composite_op_impl_usage>::Type

// Specialize the composite op functor template with binary op for a new
// type defined with COMPOSITE_OP_CLASS.  The functor template must be of
// the form template<typename T> OP_TEMPLATE_NAME.
#define COMPOSITE_OP_2(CLASS_NAME, OP_TEMPLATE_NAME) \
template <> \
struct OP_TEMPLATE_NAME<class CLASS_NAME<> > \
  { \
    static const int Composite_op_impl_sfinae = 0; \
    void operator () (const CLASS_NAME<> &dest, const CLASS_NAME<> &src) \
      { \
        typedef CLASS_NAME<OP_TEMPLATE_NAME, Composite_op_impl::Hyde> CH; \
        CH &d = reinterpret_cast<CH &>(const_cast<CLASS_NAME<> &>(dest)); \
        const CH &s = reinterpret_cast<const CH &>(src); \
        d = s; \
      } \
  };

// Specialize the composite op functor template with unary op for a new
// type defined with COMPOSITE_OP_CLASS.  The functor template must be of
// the form template<typename T> OP_TEMPLATE_NAME.
#define COMPOSITE_OP_1(CLASS_NAME, OP_TEMPLATE_NAME) \
template <> \
struct OP_TEMPLATE_NAME<class CLASS_NAME<> > \
  { \
    static const int Composite_op_impl_sfinae = 0; \
    void operator () (CLASS_NAME<> &x) \
      { \
        typedef CLASS_NAME<OP_TEMPLATE_NAME, Composite_op_impl::Hyde> CH; \
        CH &xh = reinterpret_cast<CH &>(x); \
        xh = xh; \
      } \
  };

// Invoke this to declare the last formal template parameter of any
// template you don't want instantiated unless CLASS_NAME has a specialization
// created with COMPOSITE_OP_[12].
#define COMPOSITE_OP_CHECK(CLASS_NAME, OP_TEMPLATE_NAME) \
  int composite_op_impl_dummy = \
    OP_TEMPLATE_NAME<CLASS_NAME>::Composite_op_impl_sfinae

// Hopefully, if efforts to add class introspection/reflection to the
// C++ standard succeed, there will be a cleaner way to do this.

#endif // Include once.
