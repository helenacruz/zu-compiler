// $Id: function_definition_node.h,v 1.9 2016/05/19 17:38:16 ist178190 Exp $ -*- c++ -*-
#ifndef __ZU_FUNCTIONDEFINITIONNODE_H__
#define __ZU_FUNCTIONDEFINITIONNODE_H__

#include <cdk/ast/basic_node.h>
#include <cdk/basic_type.h>
#include <cdk/ast/expression_node.h>

namespace zu {

  /**
   * Class for describing function definition nodes.
   */
  class function_definition_node: public cdk::basic_node {
    std::string *_name;
    cdk::sequence_node *_args;
    basic_type *_type;
    zu::block_node *_block;
    cdk::expression_node *_def_value;
    bool _isPrivate;

  public:
    inline function_definition_node(int lineno, std::string *name,
      cdk::sequence_node *args, basic_type *type, zu::block_node *block, 
      cdk::expression_node *def_value, bool isPrivate) :
      cdk::basic_node(lineno), _name(name), _args(args),
      _type(type), _block(block), _def_value(def_value) {
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
    inline zu::block_node *block() {
      return _block;
    }
    inline cdk::expression_node *def_value() {
      return _def_value;
    }
    
    inline bool isPrivate() {
      return _isPrivate;
    }
    
    void accept(basic_ast_visitor *sp, int level) {
      sp->do_function_definition_node(this, level);
    }

  };

} // zu

#endif
