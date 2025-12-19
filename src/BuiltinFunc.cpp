#include <iostream>

#include "BuiltinFunc.hpp"

namespace superman::builtins {

  //
  // print
  //
  Object* impl_print(std::vector<Object*>& args) {
    int64_t len = 0;

    for (auto&& x : args) {
      auto s = x->to_string();
      len += s.length() + 1;
      std::cout << s << ' ';
    }

    return new ObjInt(len);
  }

  //
  // println
  //
  Object* impl_println(std::vector<Object*>& args) {
    int64_t len = 0;

    for (auto&& x : args) {
      auto s = x->to_string();
      len += s.length() + 1;
      std::cout << s << ' ';
    }

    std::cout << std::endl;
    len++;

    return new ObjInt(len);
  }

  Function const fn_print = Function("print", {}, TypeKind::Int, true, impl_print);
  Function const fn_println = Function("println", {}, TypeKind::Int, true, impl_println);

} // namespace superman::builtins