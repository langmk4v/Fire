#include "BuiltinFunc.hpp"
#include "Sema.hpp"
#include "Eval.hpp"

namespace superman {

  Evaluator::CallStack::CallStack(int vn) { variables.resize(vn); }

  Evaluator::CallStack::~CallStack() {}

  Evaluator::CallStack& Evaluator::push_stack(int var_count) {
    return *call_stack.emplace_back(new CallStack(var_count));
  }

  void Evaluator::pop_stack() {
    delete call_stack.back();
    call_stack.pop_back();
  }

  void Evaluator::eval_stmt(Node* node) {
    switch (node->kind) {

    case NodeKind::Module:
    case NodeKind::Function:
    case NodeKind::Class:
    case NodeKind::Enum:
      break;

    case NodeKind::Scope: {
      auto scope = node->as<NdScope>();

      for (auto s : scope->items) {
        eval_stmt(s);
      }

      break;
    }

    case NodeKind::Let: {
      auto let = node->as<NdLet>();

      if (let->init) cur_stack().variables[let->index] = eval_expr(let->init);

      break;
    }

    case NodeKind::Return: {
      auto ret = node->as<NdReturn>();

      if (ret->expr) cur_stack().result = eval_expr(ret->expr);

      break;
    }

    default:
      eval_expr(node);
      break;
    }
  }

  Object* Evaluator::eval_expr(Node* node) {

    assert(node != nullptr);

    switch (node->kind) {
    case NodeKind::Value:
      return node->as<NdValue>()->obj;

    case NodeKind::Symbol: {
      auto sym = node->as<NdSymbol>();

      switch (sym->type) {
      case NdSymbol::Var:
        if (sym->is_global_var) return globals[sym->var_offset];
        return cur_stack().variables[sym->var_offset];
      }

      todoimpl;
    }

    //
    // call-func expression
    //
    case NodeKind::CallFunc: {
      auto cf = node->as<NdCallFunc>();

      (void)cf;

      std::vector<Object*> args;

      for (auto&& a : cf->args)
        args.push_back(eval_expr(a));

      // built-in
      if (cf->blt_fn) {
        return cf->blt_fn->impl(args);
      }

      // user-defined
      if (cf->func_nd) {
        auto& stack =
            push_stack(cf->func_nd->scope_ptr->as<sema::FunctionScope>()->local_var_count);

        for (int i = 0; i < static_cast<int>(cf->func_nd->args.size()); i++) {
          stack.variables[i] = args[i];
        }

        eval_stmt(cf->func_nd->body);

        auto res = stack.result;

        pop_stack();

        return res;
      }

      todoimpl;
    }

      using Ty = TypeKind;

    case NodeKind::Add: {
      auto [x, l, r] = get_expr_tu(node);

      switch (l->type.kind) {
      case Ty::Int:
        l->as<ObjInt>()->val += r->as<ObjInt>()->val;
        break;
      case Ty::Float:
        l->as<ObjFloat>()->val += r->as<ObjFloat>()->val;
        break;
      case Ty::String:
        l->as<ObjString>()->val += r->as<ObjString>()->val;
        break;
      }

      return l;
    }

    case NodeKind::Sub: {
      auto [x, l, r] = get_expr_tu(node);

      switch (l->type.kind) {
      case Ty::Int:
        l->as<ObjInt>()->val -= r->as<ObjInt>()->val;
        break;
      case Ty::Float:
        l->as<ObjFloat>()->val -= r->as<ObjFloat>()->val;
        break;
      }

      return l;
    }

    case NodeKind::Mul: {
      auto [x, l, r] = get_expr_tu(node);

      // Handle string * number
      if (l->type.kind == Ty::String && (r->type.kind == Ty::Int || r->type.kind == Ty::Float)) {
        auto str = l->as<ObjString>();
        int count = (r->type.kind == Ty::Int) ? static_cast<int>(r->as<ObjInt>()->val)
                                              : static_cast<int>(r->as<ObjFloat>()->val);

        std::u16string result;
        for (int i = 0; i < count; ++i) {
          result += str->val;
        }
        str->val = result;
        return l;
      }
      // Handle number * string
      else if ((l->type.kind == Ty::Int || l->type.kind == Ty::Float) &&
               r->type.kind == Ty::String) {
        auto str = r->as<ObjString>();
        int count = (l->type.kind == Ty::Int) ? static_cast<int>(l->as<ObjInt>()->val)
                                              : static_cast<int>(l->as<ObjFloat>()->val);

        std::u16string result;
        for (int i = 0; i < count; ++i) {
          result += str->val;
        }
        str->val = result;
        return r;
      }
      // Handle number * number
      else if (l->type.kind == Ty::Int && r->type.kind == Ty::Int) {
        l->as<ObjInt>()->val *= r->as<ObjInt>()->val;
      } else if (l->type.kind == Ty::Float || r->type.kind == Ty::Float) {
        double lval = (l->type.kind == Ty::Int) ? l->as<ObjInt>()->val : l->as<ObjFloat>()->val;
        double rval = (r->type.kind == Ty::Int) ? r->as<ObjInt>()->val : r->as<ObjFloat>()->val;

        if (l->type.kind == Ty::Int) {
          l = new ObjFloat(lval * rval);
        } else {
          l->as<ObjFloat>()->val = lval * rval;
        }
      } else {
        // Type error - should be handled by type checker
        todoimpl;
      }

      return l;
    }

    default:
      todoimpl;
    }

    return Object::none;
  }

  void Evaluator::add_global_var(NdLet* let) {
    globals.push_back(let->init ? eval_expr(let->init) : nullptr);
  }

} // namespace superman