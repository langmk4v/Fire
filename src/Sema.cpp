#include <unordered_map>

#include "macro.h"
#include "Strings.hpp"
#include "Object.hpp"
#include "Node.hpp"
#include "Error.hpp"
#include "Sema.hpp"

/*
 * ＠自分へ
 *  ソースコードみてわけわかんないときはコンサータ飲んでください。
 *  また途中放棄とかはしないでください。
 *  宜しくお願いします。
 */

namespace superman::sema {

  //
  // new_var_symbol
  //  ローカル変数またはグローバル変数用のシンボル情報を作成
  //
  Symbol* Symbol::new_var_symbol(NdLet* let) {
    auto s = new Symbol(SymbolKind::Var, let->name.text, let);

    s->var_info = new VariableInfo();
    s->var_info->def_let = let;

    let->var_info_ptr = s->var_info;

    return s;
  }

  //
  // new_arg_symbol
  //  関数の引数用のシンボル情報を作成
  //
  Symbol* Symbol::new_arg_symbol(NdFunction::Argument* arg) {
    auto s = new Symbol(SymbolKind::Var, arg->name.text, arg);

    s->var_info = new VariableInfo();
    s->var_info->def_arg = arg;

    arg->var_info_ptr = s->var_info;

    return s;
  }

  UnnamedScope::UnnamedScope(NdScope* scope) : ScopeContext(scope) {
    scope->scope_ptr = this;

    for (auto item : scope->items) {
      if (item->is(NodeKind::Let)) {
        variables.append(Symbol::new_var_symbol(item->as<NdLet>()));
        continue;
      }

      if (item->is(NodeKind::Scope)) {
        auto sub = add_scope(new UnnamedScope(item->as<NdScope>()));
        sub->parent = this;
        continue;
      }
    }
  }

  FunctionScope::FunctionScope(NdFunction* func) : ScopeContext(func) {
    func->scope_ptr = this;

    for (auto& arg : func->args) {
      args.append(Symbol::new_arg_symbol(&arg));
    }

    body = new UnnamedScope(func->body);
    body->parent = this;
  }

  ModuleScope::ModuleScope(NdModule* mod) : ScopeContext(mod) {

    mod->scope_ptr = this;

    for (auto item : mod->items) {
      switch (item->kind) {
      case NodeKind::Let: {
        variables.append(Symbol::new_var_symbol(item->as<NdLet>()));
        break;
      }

      case NodeKind::Function: {
        auto func = item->as<NdFunction>();

        auto ctx = new FunctionScope(func);
        ctx->parent = this;

        auto fsym = functions.append(new Symbol(SymbolKind::Func, func->name.text, func));
        fsym->scope_ctx = ctx;

        break;
      }

      case NodeKind::Enum: {
        todoimpl;
      }

      case NodeKind::Class: {
        todoimpl;
      }

      default:
        todoimpl;
      }
    }
  }

  //
  // eval_expr
  //
  ExprTypeResult Sema::eval_expr(Node* node) {
    switch (node->kind) {
    case NodeKind::Value:
      return node->as<NdValue>()->obj->type;

    case NodeKind::Symbol: {
      auto sym = node->as<NdSymbol>();

      auto result = find_symbol(sym);

      todoimpl;
    }

    case NodeKind::CallFunc: {
      auto cf = node->as<NdCallFunc>();

      auto f_name = eval_expr(cf->callee);

      todoimpl;
    }
    }

    todoimpl;
  }

  //
  // eval_typename
  //
  ExprTypeResult Sema::eval_typename(NdSymbol* node) {
    std::pair<char const*, TypeKind> name_kind_pairs[]{
        {"none", TypeKind::None},         {"int", TypeKind::Int},     {"float", TypeKind::Float},
        {"bool", TypeKind::Bool},         {"char", TypeKind::Char},   {"string", TypeKind::String},
        {"vector", TypeKind::Vector},     {"tuple", TypeKind::Tuple}, {"dict", TypeKind::Dict},
        {"function", TypeKind::Function},
    };

    auto result = ExprTypeResult();

    // 基本型の名前から探す
    for (auto&& [s, k] : name_kind_pairs)
      if (node->name.text == s) {
        result.type.kind = k;

        // テンプレート引数が多すぎるまたは少なすぎる
        if (int C = TypeInfo::required_param_count(k), N = (int)node->te_args.size(); C != N)
          throw err::no_match_template_arguments(node->name, C, N);

        // 存在したらユーザー定義型の検索はスキップ
        goto _Skip_Find_Userdef;
      }

    {
      auto found = find_symbol(node);

      if (found.empty()) throw err::unknown_type_name(node->name);

      for (size_t i = 0; i < found.count();) {
        if (!found[i]->is_type_name()) {
          found.remove(i);
          continue;
        }
        i++;
      }

      if (found.count() >= 2) {
        throw err::ambiguous_symbol_name(node->name);
      }

      auto Final = found[0];
      (void)Final;

      todoimpl;
    }
  _Skip_Find_Userdef:;

    return result;
  }

  //
  // find_symbol
  //
  SymbolFindResult Sema::find_symbol(NdSymbol* node) {
    auto r = SymbolFindResult();

    (void)node;

    return r;
  }

  //
  // get_required_template_params_count
  //
  int Sema::get_required_template_params_count(Symbol* s) {
    if (s->is_type_name()) {
      if (s->is(SymbolKind::Enum)) {
        todoimpl;
      }
      if (s->is(SymbolKind::Class)) {
        todoimpl;
      }
    }

    if (s->is(SymbolKind::Func)) {
      todoimpl;
    }

    if (s->is(SymbolKind::Var)) {
      // variable is still not template yet... ?
      todoimpl;
    }

    return 0; // no needed.
  }

  void Sema::analyze_full() {
    check_module(root_scope);
  }

  void Sema::check_module(ModuleScope* mod) {

    auto node = mod->node->as<NdModule>();

    auto _save = cur_scope;
    cur_scope = mod;

    for (auto item : node->items) {
      switch (item->kind) {
      case NodeKind::Let: {

        auto let = item->as<NdLet>();

        auto& varinfo = *let->var_info_ptr;

        if (let->type) {
          // シャドウイングの実装のため、
          // is_type_deducted の確認はしない（常に上書き）
          varinfo.type = eval_typename(let->type).type;
          varinfo.is_type_deducted = true;
        }

        if (let->init) {
          auto result = eval_expr(let->init);

          if (result.is_succeed) {
            if (varinfo.is_type_deducted) {
              if (!varinfo.type.equals(result.type)) {
                throw err::mismatched_types(let->init->token, varinfo.type.to_string(),
                                            result.type.to_string());
              }
            } else {
              varinfo.is_type_deducted = true;
              varinfo.type = result.type;
            }
          } else {
            todoimpl;
          }
        }

        break;
      }

      case NodeKind::Function: {
        check_function(item->scope_ptr->as<FunctionScope>());
        break;
      }
      }
    }

    cur_scope = _save;
  }

  void Sema::check_function(FunctionScope* func) {
    auto node = func->node->as<NdFunction>();

    std::unordered_map<std::string, Token*> arg_dup_check;

    // check argument name duplications
    for (auto& arg : func->args) {
      if (auto it = arg_dup_check.find(arg->name); it != arg_dup_check.end()) {
        throw err::duplicate_of_definition(arg->var_info->def_arg->name, *it->second);
      } else {
        arg_dup_check[arg->name] = &arg->var_info->def_arg->name;
      }
    }

    // get function result type
    if (node->result_type) {
      func->result_type = eval_typename(node->result_type).type;
      func->is_result_type_specified = true;
    }

    auto _save = cur_scope;
    cur_scope = func;

    // check body scope
    check_scope(func->body);

    cur_scope = _save;
  }

  void Sema::check_scope(UnnamedScope* scope) {
    auto node = scope->node->as<NdScope>();

    auto _save = cur_scope;
    cur_scope = scope;

    for (auto stmt : node->items) {
      switch (stmt->kind) {
      case NodeKind::Scope:
        check_scope(stmt->scope_ptr->as<UnnamedScope>());
        break;

      case NodeKind::Return: {
        auto ret = stmt->as<NdReturn>();

        auto fs = get_cur_func_scope();
        assert(fs);

        // 戻り値なし ( "return" 単体 )
        if (!ret->expr) {
          // => 関数定義側で指定されている場合はエラー
          //    ( return 文に式が必要 )
          if (fs->is_result_type_specified) {
            err::mismatched_return_statement(ret->token).print();
            warns::show_note(fs->node->as<NdFunction>()->result_type->token,
                             "function result type was specified here, so return-statement must "
                             "have an expression.")();
          }
        }
        // 戻り値あり
        else {
          // => 定義側で指定されていない場合はエラー
          if (!fs->is_result_type_specified) {
            err::mismatched_return_statement(ret->token).print();
            warns::show_note(
                fs->node->as<NdFunction>()->token,
                "function result type is not specified, but return-statement has an expression.")();
          }

          // => 指定ありで型が一致しない場合はエラー
          if (auto ex = eval_expr(ret->expr).type; !fs->result_type.equals(ex)) {
            err::mismatched_types(ret->expr->token, fs->result_type.to_string(), ex.to_string())
                .print();
            warns::show_note(fs->node->as<NdFunction>()->result_type->token, "specified here")();
          }
        }

        break;
      }

      default:
        eval_expr(stmt);
        break;
      }
    }

    cur_scope = _save;
  }

  Sema::Sema(NdModule* mod) {
    root_scope = new ModuleScope(mod);
  }

} // namespace superman::sema