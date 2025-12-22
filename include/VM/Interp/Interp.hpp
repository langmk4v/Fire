#pragma once

#include "VCPU.hpp"

namespace fire::vm::interp {

  class Interp {

    VCPU cpu;
    std::vector<Instruction>& prg;

  public:
    Interp(std::vector<Instruction>& prg) : prg(prg) { }

    void run();

  };

}