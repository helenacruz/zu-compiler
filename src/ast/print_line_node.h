// $Id: print_line_node.h,v 1.2 2016/03/03 11:58:59 ist178190 Exp $ -*- c++ -*-
#ifndef __ZU_PRINTLINENODE_H__
#define __ZU_PRINTLINENODE_H__

#include <cdk/ast/expression_node.h>

namespace zu {

  /**
   * Class for describing print nodes.
   */
  class print_line_node: public cdk::basic_node {
    cdk::expression_node *_argument;

  public:
    inline print_line_node(int lineno, cdk::expression_node *argument) :
        cdk::basic_node(lineno), _argument(argument) {
    }

  public:
    inline cdk::expression_node *argument() {
      return _argument;
    }

    void accept(basic_ast_visitor *sp, int level) {
      sp->do_print_line_node(this, level);
    }

  };

} // zu

#endif
