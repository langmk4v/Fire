#pragma once

#include <vector>

#include "TypeInfo.hpp"
#include "Object.hpp"

namespace superman::builtins {

  struct Function;

  extern Function const fn_print;
  extern Function const fn_println;

  struct Function {
    using FuncPointer = Object* (*)(std::vector<Object*>& args);

    char const* name;
    std::vector<TypeInfo> arg_types;
    TypeInfo result_type;

    bool is_variable_args;

    FuncPointer impl;

    static inline builtins::Function const* all_builtin_funcs[]{
        &builtins::fn_print,
        &builtins::fn_println,
    };

    Function(char const* name, std::vector<TypeInfo> arg_types, TypeInfo result_type,
             bool is_var_arg, FuncPointer impl)
        : name(name), arg_types(std::move(arg_types)), result_type(std::move(result_type)),
          is_variable_args(is_var_arg), impl(impl) {
    }
  };

} // namespace superman::builtins