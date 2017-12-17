// $Id: function_call_node.h,v 1.4 2016/04/11 17:26:27 ist178190 Exp $ -*- c++ -*-
#ifndef __ZU_FUNCTIONCALLNODE_H__
#define __ZU_FUNCTIONCALLNODE_H__

#include <cdk/ast/basic_node.h>

namespace zu {

  /**
   * Class for describing function call nodes.
   */
  class function_call_node: public cdk::expression_node {
    std::string *_name;
    cdk::sequence_node *_args;

  public:
    inline function_call_node(int lineno, std::string *name, 
                              cdk::sequence_node *args) :
        cdk::expression_node(lineno), _name(name), _args(args) {
    }

  public:
    inline std::string *name() {
      return _name;
    }
    inline cdk::sequence_node *args() {
      return _args;
    }

    void accept(basic_ast_visitor *sp, int level) {
      sp->do_function_call_node(this, level);
    }

  };

} // zu

#endif
