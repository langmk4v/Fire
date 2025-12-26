#include "Utils.hpp"
#include "Error.hpp"
#include "Node.hpp"
#include "Sema/Sema.hpp"

namespace fire {

  TypeInfo Sema::eval_expr_ty(Node* node, NdVisitorContext ctx) {
    (void)node;
    (void)ctx;

    switch (node->kind) {
      case NodeKind::Value:
        return node->as<NdValue>()->obj->type;
    }

    todo;
  }

  TypeInfo Sema::eval_typename_ty(NdSymbol* node, NdVisitorContext ctx) {
    (void)node;
    (void)ctx;
    todo;
  }

} // namespace fire