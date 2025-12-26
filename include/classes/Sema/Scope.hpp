#pragma once

#include "Utils.hpp"
#include "Symbols.hpp"

namespace fire {

  enum class ScopeKind {
    Scope,
    Try,
    Catch,
    If,
    For,
    Func,
    Enum,
    Class,
    Namespace,
    Module,
  };

  struct Scope {
    ScopeKind kind;

    Node* node = nullptr;
    Scope* parent = nullptr;

    Symbol symbol;
    SymbolTable symtable;

    template <typename T>
    T* as() {
      return static_cast<T*>(this);
    }

    virtual ~Scope() = default;

    Scope* from_node(Node* node, Scope* parent);

  protected:
    Scope(ScopeKind kind, Node* node, Scope* parent);
  };

  struct SCScope : Scope {
    SymbolTable variables;
    std::vector<Scope*> subscopes;

    SCScope(NdScope* node, Scope* parent);
  };

  struct SCIf : Scope {
    Symbol* var = nullptr;
    SCScope* then_scope = nullptr;
    SCScope* else_scope = nullptr;

    SCIf(NdIf* node, Scope* parent);
  };

  struct SCFor : Scope {
    Symbol* iter_name = nullptr;
    SCScope* body = nullptr;

    SCFor(NdFor* node, Scope* parent);
  };

  struct SCCatch : Scope {
    Symbol* holder_name = nullptr;
    SCScope* body = nullptr;

    SCCatch(NdCatch* node, Scope* parent);
  };

  struct SCTry : Scope {
    SCScope* body = nullptr;
    std::vector<SCCatch*> catches;
    SCScope* finally_scope = nullptr;

    SCTry(NdTry* node, Scope* parent);
  };

  struct SCFunction : Scope {
    SymbolTable arguments;
    SCScope* body = nullptr;

    SCFunction(NdFunction* node, Scope* parent);
  };

  struct SCEnum : Scope {
    SymbolTable enumerators;
    SCEnum(NdEnum* node, Scope* parent);
  };

  struct SCClass : Scope {
    SymbolTable fields;
    SymbolTable methods;

    NdClass* get_node() {
      return node->as<NdClass>();
    }

    SCClass(NdClass* node, Scope* parent);
  };

  struct SCNamespace : Scope {
    SymbolTable variables;
    SymbolTable functions;
    SymbolTable enums;
    SymbolTable classes;
    SymbolTable namespaces;

    SymbolTable* get_table(NodeKind kind) {
      switch (kind) {
        case NodeKind::Let:
          return &variables;
        case NodeKind::Function:
          return &functions;
        case NodeKind::Enum:
          return &enums;
        case NodeKind::Class:
          return &classes;
        case NodeKind::Namespace:
          return &namespaces;
      }
      todo;
    }

    SCNamespace(NdNamespace* node, Scope* parent);
  };

  struct SCModule : Scope {
    SymbolTable variables;
    SymbolTable functions;
    SymbolTable enums;
    SymbolTable classes;
    SymbolTable namespaces;

    SymbolTable* get_table(NodeKind kind) {
      switch (kind) {
        case NodeKind::Let:
          return &variables;
        case NodeKind::Function:
          return &functions;
        case NodeKind::Enum:
          return &enums;
        case NodeKind::Class:
          return &classes;
        case NodeKind::Namespace:
          return &namespaces;
      }
      todo;
    }

    SCModule(NdModule* node, Scope* parent);
  };

} // namespace fire