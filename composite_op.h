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

// namespace for hidden implementation stuff.
//
namespace Composite_op_impl
{

template <typename T, template <typename> class Op>
struct Select
  {
    class Type
      {
      private:
        T v;

      public:

        void operator = (const Type &src) { Op<T>()(this->v, src.v); }
      };
  };

template <typename T>
struct No_op { };

// Use partial specialization for No_op to trigger normal usage of type T.
template <typename T>
struct Select<T, No_op>
  {
    typedef T Type;
  };

} // end namespace Composite_op_impl

// See tst.cpp for example of use.

// Begin the definition of templeted structure named CLASS_NAME.  This macro
// generates everyting before : or {.  The defined class must use the default
// assignement operator.  COMPOSITE_OP_SUB and COMPOSITE_OP_MBR_T must be
// used to define all subclases and data members of CLASS_NAME.
//
#define COMPOSITE_OP_CLASS(CLASS_NAME) \
  template < \
    template <typename T> class Composite_op_impl_op_tpl = \
      Composite_op_impl::No_op> \
  class CLASS_NAME

// Expands to type name for base or class instance member, whose class
// must have been defined with COMPOSITE_OP_CLASS with same op template.
//
#define COMPOSITE_OP_SUB(CLASS_NAME) CLASS_NAME<Composite_op_impl_op_tpl>

// Expands to type name for data member that the composite op should act
// upon.  Type TYPE_NAME must have an assignment operator.
//
#define COMPOSITE_OP_MBR_T(TYPE_NAME) \
  typename Composite_op_impl::Select< \
    TYPE_NAME, Composite_op_impl_op_tpl>::Type

// Specialize the composite op functor template for a class defined with
// COMPOSITE_OP_CLASS.  The functor template must be of the form
// template<typename T> OP_TEMPLATE_NAME.  The scope of the declaration of
// OP_TEMPLATE_NAME, as well as the scope of the invocation of this macro,
// must be the same as the scope of CLASS_NAME.
//
#define COMPOSITE_OP(CLASS_NAME, OP_TEMPLATE_NAME) \
inline void Composite_op_not_defined_for_this_class( \
  CLASS_NAME<> *, OP_TEMPLATE_NAME<CLASS_NAME<> > *) { } \
template <> \
struct OP_TEMPLATE_NAME<class CLASS_NAME<> > \
  { \
    void operator () (const CLASS_NAME<> &dest, const CLASS_NAME<> &src) \
      { \
        typedef CLASS_NAME<OP_TEMPLATE_NAME> X; \
        X &d = reinterpret_cast<X &>(const_cast<CLASS_NAME<> &>(dest)); \
        const X &s = reinterpret_cast<const X &>(src); \
        d = s; \
      } \
  };

// Invoke this macro in the body of a function to cause a compiler error,
// if the function assumes there is an invocation of
// COMPOSITE_OP(CLASS_NAME, OP_TEMPLATE_NAME) .  The compile error
// will indicate there is no necessary overload for
// "Composite_op_not_defined_for_this_class".
//
#define COMPOSITE_OP_CHECK(CLASS_NAME, OP_TEMPLATE_NAME) \
  Composite_op_not_defined_for_this_class( \
    static_cast<CLASS_NAME *>(0), \
    static_cast<OP_TEMPLATE_NAME<CLASS_NAME> *>(0));

// The implementation assumes that a "degenerate" class, whose only data
// member is one instance of a type, will have the same size and alignment
// as the type.  This implies that a "shadow" class, where the type of each
// data member and/or base is a degenerate class equivalent of the
// corresponding component of the "shadowed" class, will have the same
// memory layout as the shadowed class.

// Hopefully, if efforts to add class introspection/reflection to the
// C++ standard succeed, there will be a cleaner way to do this.

#endif // Include once.
