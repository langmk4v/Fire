#pragma once

#include <vector>

namespace fire::lexer {
  struct SourceCode;
}

namespace fire {
  class Driver {
    std::vector<lexer::SourceCode> inputs;

  public:
    Driver();

    ~Driver();

    int main(int argc, char** argv);
  };
} // namespace fire