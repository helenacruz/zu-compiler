// $Id: for_node.h,v 1.3 2016/03/16 10:41:02 ist178190 Exp $ -*- c++ -*-
#ifndef __ZU_FORNODE_H__
#define __ZU_FORNODE_H__

#include <cdk/ast/sequence_node.h>

namespace zu {

  /**
   * Class for describing for-loop nodes.
   */
  class for_node: public cdk::basic_node {
    cdk::sequence_node *_init;
    cdk::sequence_node *_cond;
    cdk::sequence_node *_incr;
    cdk::basic_node *_block;

  public:
    inline for_node(int lineno, cdk::sequence_node *init, 
                    cdk::sequence_node *cond, 
                    cdk::sequence_node *incr,
                    cdk::basic_node *block) :
        basic_node(lineno), _init(init), 
        _cond(cond), _incr(incr),
        _block(block) {
    }

  public:
    inline cdk::sequence_node *init() {
        return _init;
    }
    inline cdk::sequence_node *cond() {
      return _cond;
    }
    inline cdk::sequence_node *incr() {
        return _incr;
    }
    inline cdk::basic_node *block() {
      return _block;
    }

    void accept(basic_ast_visitor *sp, int level) {
      sp->do_for_node(this, level);
    }

  };

} // zu

#endif
