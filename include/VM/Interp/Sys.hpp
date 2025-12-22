#pragma once

namespace fire::vm::interp {
  enum class Sys { // for built-in funcs
    None,
    Print,
    Println,
  };

  static constexpr bool is_var_args_fn(Sys s) {
    switch (s) {
      case Sys::Print:
      case Sys::Println:
        return true;
    }

    return false;
  }
}