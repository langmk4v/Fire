#pragma once

#include <vector>

namespace fire {
  struct SourceCode;

  class Driver {
    std::vector<SourceCode> inputs;

  public:
    Driver();

    ~Driver();

    int main(int argc, char** argv);
  };
} // namespace fire