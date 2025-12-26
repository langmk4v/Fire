#include "Utils.hpp"
#include "Error.hpp"
#include "Node.hpp"
#include "Sema/Sema.hpp"

namespace fire {

  TypeInfo TypeChecker::eval_expr_ty(Node* node, NdVisitorContext ctx) {
    (void)node;
    (void)ctx;

    switch (node->kind) {
      case NodeKind::Value:
        return node->as<NdValue>()->obj->type;

      case NodeKind::Symbol: {
        auto sym = node->as<NdSymbol>();

        if (!sym->symbol_ptr) { todo; }

        switch (sym->symbol_ptr->kind) {
          case SymbolKind::Var:
            if (!sym->symbol_ptr->var_info->is_type_deducted) { todo; }
            return sym->symbol_ptr->var_info->type;
        }

        todo;
      }

      case NodeKind::KeyValuePair: {
        todo;
      }

      case NodeKind::Self: {
        if (!ctx.in_method) { throw err::semantics::cannot_use_self_here(node->token); }
        return make_class_type(ctx.cur_class->get_node());
      }

      case NodeKind::Array: {
        auto arr = node->as<NdArray>();

        std::vector<TypeInfo> elems;

        for (auto& elem : arr->data) {
          elems.push_back(eval_expr_ty(elem, ctx));
        }

        if (elems.empty()) {
          if (!ctx.empty_array_element_type) {
            todo; // cannot deduce element type of empty array
          }

          return TypeInfo(TypeKind::Vector, {*ctx.empty_array_element_type}, false, false);
        }

        auto ty = TypeInfo(TypeKind::Vector, {elems[0]}, false, false);

        for (size_t i = 1; i < elems.size(); i++) {
          if (!ty.equals(elems[i])) {
            todo; // type mismatch
          }
        }

        return ty;
      }

      case NodeKind::Tuple: {
        todo;
      }

      case NodeKind::Slice: {
        todo;
      }

      case NodeKind::Subscript: {
        todo;
      }

      case NodeKind::MemberAccess: {
        todo;
      }

      case NodeKind::CallFunc: {
        todo;
      }

      case NodeKind::GetTupleElement: {
        todo;
      }

      case NodeKind::Inclement: {
        todo;
      }

      case NodeKind::Declement: {
        todo;
      }

      case NodeKind::BitNot: {
        todo;
      }

      case NodeKind::Not: {
        todo;
      }

      case NodeKind::Ref: {
        todo;
      }

      case NodeKind::Deref: {
        todo;
      }

      case NodeKind::Assign: {
        todo;
      }

      case NodeKind::AssignWithOp: {
        todo;
      }
    }

    assert(node->is_expr());

    auto ex = node->as<NdExpr>();

    auto lhs_ty = eval_expr_ty(ex->lhs, ctx);
    auto rhs_ty = eval_expr_ty(ex->rhs, ctx);

    // todo: check operators ...

    return lhs_ty;
  }

  TypeInfo TypeChecker::eval_typename_ty(NdSymbol* node, NdVisitorContext ctx) {
    (void)node;
    (void)ctx;
    todo;
  }

  TypeInfo TypeChecker::make_class_type(NdClass* node) {
    auto type = TypeInfo(TypeKind::Class);

    type.class_node = node;

    return type;
  }

  void TypeChecker::check_expr(Node* node, NdVisitorContext ctx) {
  }

  void TypeChecker::check_stmt(Node* node, NdVisitorContext ctx) {
  }

  void TypeChecker::check_scope(NdScope* node, NdVisitorContext ctx) {
  }

  void TypeChecker::check_function(NdFunction* node, NdVisitorContext ctx) {
  }

  void TypeChecker::check_class(NdClass* node, NdVisitorContext ctx) {
  }

  void TypeChecker::check_enum(NdEnum* node, NdVisitorContext ctx) {
  }

  void TypeChecker::check_namespace(NdNamespace* node, NdVisitorContext ctx) {
  }

  void TypeChecker::check_module(NdModule* node, NdVisitorContext ctx) {
  }

} // namespace fire