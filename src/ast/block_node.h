// // $Id: block_node.h,v 1.2 2016/05/19 17:38:16 ist178190 Exp $ -*- c++ -*-
#ifndef __ZU_BLOCKNODE_H__
#define __ZU_BLOCKNODE_H__

#include <cdk/ast/sequence_node.h>

namespace zu {

  /**
   * Class for describing block nodes.
   */
  class block_node: public cdk::basic_node {
    cdk::sequence_node *_decls;
    cdk::sequence_node *_instrs;

  public:
    inline block_node(int lineno, cdk::sequence_node *decls, 
                    cdk::sequence_node *instrs) :
        cdk::basic_node(lineno), _decls(decls), 
        _instrs(instrs) {
    }

  public:
    inline cdk::sequence_node *decls() {
        return _decls;
    }
    inline cdk::sequence_node *instrs() {
      return _instrs;
    }

    void accept(basic_ast_visitor *sp, int level) {
      sp->do_block_node(this, level);
    }

  };

} // zu

#endif
