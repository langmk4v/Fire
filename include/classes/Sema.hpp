#pragma once

#include "Utils.hpp"
#include "Node.hpp"

/*

1. シンボル定義情報を収集

2. 名前の参照の解決

3. 型チェック
   - 型の一致確認
   - 型推論

4. 文をチェック
   - return, break, continue が使えるかどうか

5. 式の意味チェック
   - 型と演算子が一致するかどうか
   - 型に対して演算子が使用可能か

6. 制御フローの確認


*/

namespace fire {

  enum class SymbolKind {
    Unknown,
    Var,
    Func,
    Enum,
    Class,
    Namespace,
    Module,
  };

  struct Symbol {
    std::string name = "";
    SymbolKind kind = SymbolKind::Unknown;
    TypeInfo type = {};
    Node* node = nullptr;
  };

  struct SymbolTable {
    SymbolTable* parent = nullptr;

    std::vector<Symbol*> symbols;

    SymbolTable(SymbolTable* parent = nullptr) : parent(parent) {
    }

    Symbol*& append(Symbol* s) {
      return symbols.emplace_back(s);
    }
  };

  struct Scope {
    Node* node = nullptr;
    Scope* parent = nullptr;

    Symbol symbol;
    SymbolTable symtable;

    template <typename T>
    T* as() {
      return static_cast<T*>(this);
    }

    virtual ~Scope() = default;

  protected:
    Scope(Node* node, Scope* parent) : node(node), parent(parent) {
    }
  };

  struct SCScope : Scope {
    SymbolTable variables;

    SCScope(NdScope* node, Scope* parent) : Scope(node, parent) {
    }
  };

  struct SCFunction : Scope {
    SymbolTable arguments;
    SCScope* body = nullptr;

    SCFunction(NdFunction* node, Scope* parent) : Scope(node, parent) {
    }
  };

  struct SCEnum : Scope {
    SymbolTable enumerators;
    SCEnum(NdEnum* node, Scope* parent) : Scope(node, parent) {
    }
  };

  struct SCClass : Scope {
    SymbolTable fields;
    SymbolTable methods;

    SCClass(NdClass* node, Scope* parent) : Scope(node, parent) {
    }
  };

  struct SCNamespace : Scope {
    SymbolTable variables;
    SymbolTable functions;
    SymbolTable enums;
    SymbolTable classes;
    SymbolTable namespaces;

    SymbolTable* get_table(NodeKind kind) {
      switch (kind) {
        case NodeKind::Let: return &variables;
        case NodeKind::Function: return &functions;
        case NodeKind::Enum: return &enums;
        case NodeKind::Class: return &classes;
        case NodeKind::Namespace: return &namespaces;
      }
      todo;
    }

    SCNamespace(NdNamespace* node, Scope* parent) : Scope(node, parent) {
    }
  };

  struct SCModule : Scope {
    SymbolTable variables;
    SymbolTable functions;
    SymbolTable enums;
    SymbolTable classes;
    SymbolTable namespaces;

    SymbolTable* get_table(NodeKind kind) {
      switch (kind) {
        case NodeKind::Let: return &variables;
        case NodeKind::Function: return &functions;
        case NodeKind::Enum: return &enums;
        case NodeKind::Class: return &classes;
        case NodeKind::Namespace: return &namespaces;
      }
      todo;
    }

    SCModule(NdModule* node, Scope* parent) : Scope(node, parent) {
    }
  };

  class Sema {

    SCModule* root_scope = nullptr;

    Scope* cur_scope = nullptr;

  public:
    Sema();

    void analyze_all(NdModule* mod) {
      root_scope = create_scope(mod, nullptr)->as<SCModule>();

      resolve_names(mod);

      infer_types(mod);

      check_semantics(mod);
    }

    Scope* create_scope(Node* node, Scope* parent);

    void resolve_names(Node* node);

    void infer_types(Node* node);

    void check_semantics(Node* node);
  };

} // namespace fire