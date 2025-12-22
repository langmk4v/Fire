#pragma once

#include <vector>

namespace fire::parser {
  struct Node;
}

namespace fire::vm {

  struct Instruction;

  namespace compiler {

    class Compiler {

    public:
      Compiler(std::vector<Instruction>& out);

      void compile(Node* node);

    private:

    };

  }
}