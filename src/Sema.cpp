#include <iostream>

#include "Utils.hpp"
#include "Sema.hpp"
#include "Error.hpp"

namespace fire {

  Sema::Sema() {
  }

  Scope* Sema::create_scope(Node* node, Scope* parent) {
    switch (node->kind) {
      case NodeKind::Module: {
        auto mod = node->as<NdModule>();
        auto mod_scope = new SCModule(mod, parent);

        mod_scope->symbol = {
            .name = mod->name,
            .kind = SymbolKind::Module,
            .node = mod,
        };

        for (auto& item : mod->items) {
          auto scope = create_scope(item, mod_scope);

          mod_scope->symtable.append(&scope->symbol);

          mod_scope->get_table(item->kind)->append(&scope->symbol);
        }

        return mod_scope;
      }

      case NodeKind::Namespace: {
        auto ns = node->as<NdNamespace>();
        auto ns_scope = new SCNamespace(ns, parent);

        ns_scope->symbol = {
            .name = ns->name,
            .kind = SymbolKind::Namespace,
            .node = ns,
        };

        for (auto& item : ns->items) {
          auto scope = create_scope(item, ns_scope);

          ns_scope->symtable.append(&scope->symbol);

          ns_scope->get_table(item->kind)->append(&scope->symbol);
        }

        return ns_scope;
      }

      case NodeKind::Class: {
        auto cla = node->as<NdClass>();
        auto cla_scope = new SCClass(cla, parent);

        cla_scope->symbol = {
            .name = cla->name.text,
            .kind = SymbolKind::Class,
            .node = cla,
        };

        for (auto& field : cla->fields) {
          auto scope = create_scope(field, cla_scope);
          cla_scope->symtable.append(&scope->symbol);
          cla_scope->fields.append(&scope->symbol);
        }

        for (auto& method : cla->methods) {
          auto scope = create_scope(method, cla_scope);
          cla_scope->symtable.append(&scope->symbol);
          cla_scope->methods.append(&scope->symbol);
        }

        return cla_scope;
      }

      case NodeKind::Enum: {
        auto enu = node->as<NdEnum>();
        auto enu_scope = new SCEnum(enu, parent);

        enu_scope->symbol = {
            .name = enu->name.text,
            .kind = SymbolKind::Enum,
            .node = enu,
        };

        for (auto& enumerator : enu->enumerators) {
          auto scope = create_scope(enumerator, enu_scope);
          enu_scope->symtable.append(&scope->symbol);
          enu_scope->enumerators.append(&scope->symbol);
        }

        return enu_scope;
      }
    }

    todo;
  }

} // namespace fire