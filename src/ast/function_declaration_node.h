// $Id: function_declaration_node.h,v 1.5 2016/05/18 10:18:44 ist178190 Exp $ -*- c++ -*-
#ifndef __ZU_FUNCTIONDECLARATIONNODE_H__
#define __ZU_FUNCTIONDECLARATIONNODE_H__

#include <cdk/ast/basic_node.h>
#include <cdk/basic_type.h>
#include <cdk/ast/expression_node.h>
#include <cdk/ast/sequence_node.h>

namespace zu {

  /**
   * Class for describing function declaration nodes.
   */
  class function_declaration_node: public cdk::basic_node {
    std::string *_name;
    cdk::sequence_node *_args;
    basic_type *_type;
    cdk::expression_node *_def_value;
    bool _isPrivate;
    bool _isExtern;

  public:
    inline function_declaration_node(int lineno, std::string *name,
      cdk::sequence_node *args, basic_type *type, cdk::expression_node *def_value,
      bool isPrivate, bool isExtern) :
      cdk::basic_node(lineno), _name(name), _args(args),
      _type(type), _def_value(def_value), _isPrivate(isPrivate), 
      _isExtern(isExtern) {
    }

  public:
    inline std::string *name() {
      return _name;
    }
    inline cdk::sequence_node *args() {
      return _args;
    }
    inline basic_type *type() {
      return _type;
    }
    inline cdk::expression_node *def_value() {
      return _def_value;
    }
    inline bool isPrivate() {
      return _isPrivate;
    }
    inline bool isExtern() {
      return _isExtern;
    }

    void accept(basic_ast_visitor *sp, int level) {
      sp->do_function_declaration_node(this, level);
    }

  };

} // zu

#endif
