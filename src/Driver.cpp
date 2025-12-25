#include <iostream>
#include <string>

#include "Utils.hpp"

#include "Lexer.hpp"
#include "Token.hpp"
#include "Parser.hpp"
#include "Sema.hpp"
#include "VM.hpp"
#include "Driver.hpp"
#include "Error.hpp"

#include "IR.hpp"

namespace fire {

  int Driver::main(int argc, char** argv) {
    for (int i = 1; i < argc; i++)
      this->inputs.emplace_back(new SourceCode(argv[i]));

    if (inputs.empty()) {
      std::cout << "no input files." << std::endl;
      return -1;
    }

    for (auto source : this->inputs) {
      std::vector<Token> tokens;

      try {
        auto lexer = Lexer(*source);
        tokens = lexer.lex();

        auto parser = Parser(*source, tokens);

        auto mod = parser.parse();

        mod->name = "__main__";

        if (!mod->main_fn) {
          printf("fatal error: function 'main' not defined.\n");
          return -1;
        } else if (mod->main_fn->args.size() > 0) {
          printf("fatal error: function 'main' cannot have arguments.\n");
          return -1;
        } else if (mod->main_fn->result_type) {
          printf("fatal error: result type of 'main' cannot be specified.\n");
          return -1;
        }

        auto se = Sema(mod);
        se.analyze_full();

        Compiler::compile_full(IR::from_node(mod));

        return 0;
      } catch (int n) {
        printf("%d\n", n);
      } catch (err::e e) {
        e.print();
      }
    }

    return 1;
  }

} // namespace fire