#ifndef __ZU_INDEXNODE_H__
#define __ZU_INDEXNODE_H__

namespace zu {

  /**
   * Class for describing the index operator
   */
  class index_node: public lvalue_node {
  private:
      cdk::expression_node *_base;
      cdk::expression_node *_value;
      
  public:
    inline index_node(int lineno, cdk::expression_node *base,
                      cdk::expression_node *value) :
                      lvalue_node(lineno), _base(base),
                      _value(value) {
    }

    inline cdk::expression_node *base() {
      return _base;
    }
    inline cdk::expression_node *value() {
      return _value;
    }
    
    /**
     * @param sp semantic processor visitor
     * @param level syntactic tree level
     */
    void accept(basic_ast_visitor *sp, int level) {
      sp->do_index_node(this, level);
    }

  };

} // zu

#endif
