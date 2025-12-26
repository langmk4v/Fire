#pragma once

#include "IR.hpp"

namespace fire {
  class HighIRCreator {
  public:
    static IR::High::Base* create_full_hir(Node* node);
  };

  class MiddleIRCreator {
  public:
    static IR::Middle::MIR* create_full_mir(Node* node);
  };

  class LowIRCreator {
  public:
    static IR::Low::LIR* create_full_lir(Node* node);
  };

  class NodeLower {
  public:
    static IR::Low::LIR* lower_full(Node* node);
  };
} // namespace fire