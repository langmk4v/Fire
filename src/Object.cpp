#include "macro.h"
#include "Strings.hpp"
#include "Object.hpp"

namespace superman {

  ObjNone* Object::none = new ObjNone();

  std::string Object::to_string() const {
    switch (type.kind) {
    case TypeKind::None:
      break;

    case TypeKind::Int:
      return std::to_string(as<ObjInt>()->val);

    default:
      todoimpl;
    }

    return "none";
  }

} // namespace superman