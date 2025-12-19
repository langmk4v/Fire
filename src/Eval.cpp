#include "Eval.hpp"
#include "BuiltinFunc.hpp"

namespace superman {

  Evaluator::CallStack& Evaluator::push_stack() {
    return *call_stack.emplace_back(new CallStack);
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

    default:
      eval_expr(node);
    }
  }

  Object* Evaluator::eval_expr(Node* node) {

    assert(node != nullptr);

    switch (node->kind) {
    case NodeKind::Value:
      return node->as<NdValue>()->obj;

    case NodeKind::CallFunc: {
      auto cf = node->as<NdCallFunc>();

      (void)cf;

      todoimpl;
    }

    default:
      todoimpl;
    }

    return Object::none;
  }

} // namespace superman