// $Id: variable_declaration_node.h,v 1.4 2016/05/18 10:18:44 ist178190 Exp $ -*- c++ -*-
#ifndef __ZU_VARIABLEDECLARATIONNODE_H__
#define __ZU_VARIABLEDECLARATIONNODE_H__

#include <cdk/ast/basic_node.h>
#include <cdk/basic_type.h>

namespace zu {

  /**
   * Class for describing variable declaration nodes.
   */
  class variable_declaration_node: public cdk::basic_node {
    std::string *_name;
    cdk::expression_node *_value;
    basic_type *_type;
    bool _isPrivate;
    bool _isExtern;

  public:
    inline variable_declaration_node(int lineno, std::string *name,
      cdk::expression_node *value, basic_type *type, bool isPrivate,
      bool isExtern) : cdk::basic_node(lineno), _name(name), _value(value),
        _type(type), _isPrivate(isPrivate), _isExtern(isExtern) {
    }

  public:
    inline std::string *name() {
      return _name;
    }
    inline cdk::expression_node *value() {
      return _value;
    }
    inline basic_type *type() {
      return _type;
    }
    inline bool isPrivate() {
      return _isPrivate;
    }
    inline bool isExtern() {
      return _isExtern;
    }

    void accept(basic_ast_visitor *sp, int level) {
      sp->do_variable_declaration_node(this, level);
    }

  };

} // zu

#endif
