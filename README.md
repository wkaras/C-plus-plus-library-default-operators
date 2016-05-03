# C-plus-plus-library-default-operators
Helps generate a default operator / member function for classes whose
components (base classes, data members) have the operator / member function.
This is an ugly hack, and would only be worth using in rare circumstances.
But it does illustrate that there is a common, underlying introspection /
reflection mechanism implicit in the default constructors, destructor, and
assignement operators.  The ISO C++ Committee is considering adding a default
equality operator, that would also use this mechanism.  I think it's probably
a better course to add some explicit introspection / reflection mechanism that
would allow for library implementations of a default equality operator, other
comparison operators, hash, endiance swapping and perhaps other members.  This
is a discussion of one such explicit mechanism:

https://groups.google.com/a/isocpp.org/forum/?fromgroups#!topic/std-proposals/_Pl2_WdSV8k
