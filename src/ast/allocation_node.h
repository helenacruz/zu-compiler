#ifndef __ZU_ALLOCATIONNODE_H__
#define __ZU_ALLOCATIONNODE_H__

#include <cdk/ast/expression_node.h>

namespace zu {

  /**
   * Class for describing allocation nodes.
   */
  class allocation_node: public cdk::expression_node {
    cdk::expression_node *_size;

  public:
    inline allocation_node(int lineno, cdk::expression_node *size) :
        expression_node(lineno), _size(size) {
    }

  public:
    inline cdk::expression_node * size() {
      return _size;
    }
    
    void accept(basic_ast_visitor *sp, int level) {
      sp->do_allocation_node(this, level);
    }

  };

} // zu

#endif
