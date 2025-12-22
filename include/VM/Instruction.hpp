#pragma once

#include "Parser/Node_fwd.hpp"
#include "VM/Asm.hpp"

namespace fire::vm {

  enum Operations : uint8_t {
    OP_Nop,
    OP_Do,
    OP_Jmpz,
  };

  struct Instruction {
    Operations op;

    Node* expr = nullptr;

    Instruction* addr = nullptr; // for jump

    Instruction(Operations op) : op(op) { }
  };

}