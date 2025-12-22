#pragma once

namespace fire::vm {

  struct Instruction;

  namespace interp {
    struct VCPU {
      size_t pc = 0;
      bool result = false;
      Instruction* lr = nullptr;
    };
  }
}