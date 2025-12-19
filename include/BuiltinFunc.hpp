#pragma once

#include <vector>

#include "TypeInfo.hpp"
#include "Object.hpp"

namespace superman::builtins {
  struct Function {
    using FuncPointer = Object* (*)(std::vector<Object*>& args);

    char const* name;
    std::vector<TypeInfo> arg_types;

    bool is_variable_args;

    FuncPointer impl;

    Function(char const* name, std::vector<TypeInfo> arg_types, bool is_var_arg, FuncPointer impl)
        : name(name), arg_types(std::move(arg_types)), is_variable_args(is_var_arg), impl(impl) {
    }
  };

  extern Function const fn_print;
  extern Function const fn_println;

} // namespace superman::builtins