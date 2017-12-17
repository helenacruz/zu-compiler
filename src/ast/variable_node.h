// $Id: variable_node.h,v 1.4 2016/05/04 16:28:35 ist178190 Exp $ -*- c++ -*-
#ifndef __ZU_VARIABLENODE_H__
#define __ZU_VARIABLENODE_H__

#include "lvalue_node.h"
#include <string>

namespace zu {

  /**
   * Class for describing variable nodes.
   */
  class variable_node: public zu::lvalue_node {
    std::string *_name;

  public:
    inline variable_node(int lineno, std::string *name) :
        zu::lvalue_node(lineno), _name(name) {
    }

  public:
    std::string * name() {
      return _name;
    }
    
    void accept(basic_ast_visitor *sp, int level) {
      sp->do_variable_node(this, level);
    }

  };

} // zu

#endif
