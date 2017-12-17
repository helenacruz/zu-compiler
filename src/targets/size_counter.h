// $Id: size_counter.h,v 1.1 2016/05/20 10:23:09 ist178190 Exp $ -*- c++ -*-
#ifndef __ZU_SEMANTICS_SIZE_COUNTER_H__
#define __ZU_SEMANTICS_SIZE_COUNTER_H__

#include <string>
#include <iostream>
#include <cdk/ast/basic_node.h>
#include "targets/symbol.h"
#include "targets/basic_ast_visitor.h"

namespace zu {

  /**
   * Calculates the size of the stack
   */
  class size_counter: public basic_ast_visitor {
    int _size;
    
  public:
    size_counter(std::shared_ptr<cdk::compiler> compiler, cdk::basic_node * const node, int lvl) : 
      basic_ast_visitor(compiler)
    {
      _size = 0;
      node->accept(this, lvl);
    }

  public:
    ~size_counter() {
      os().flush();
    }
    inline int size() {
      return _size;
    }

  public:
    void do_sequence_node(cdk::sequence_node * const node, int lvl);

  protected:
    template<typename T>
    void processSimple(cdk::simple_value_node<T> * const node, int lvl) {
    }

  public:
    void do_variable_declaration_node(zu::variable_declaration_node * const node, int lvl);
    void do_variable_node(zu::variable_node * const node, int lvl);
    void do_integer_node(cdk::integer_node * const node, int lvl);
    void do_double_node(cdk::double_node * const node, int lvl);
    void do_string_node(cdk::string_node * const node, int lvl);

  protected:
    void processIUnaryExpression(cdk::unary_expression_node * const node, int lvl);
    void processIDUnaryExpression(cdk::unary_expression_node * const node, int lvl);

  public:
    void do_neg_node(cdk::neg_node * const node, int lvl);
    void do_identity_node(zu::identity_node * const node, int lvl);
    void do_not_node(zu::not_node * const node, int lvl);

  protected:
    void processPIDBinaryExpression(cdk::binary_expression_node * const node, int lvl);
    void processPIDSBinaryExpression(zu::assignment_node * const node, int lvl);
    void processIDBinaryExpression(cdk::binary_expression_node * const node, int lvl);
    void processIBinaryExpression(cdk::binary_expression_node * const node, int lvl);
    void processPPIDBinaryExpression(cdk::binary_expression_node * const node, int lvl);

  public:
    void do_and_node(zu::and_node * const node, int lvl);
    void do_or_node(zu::or_node * const node, int lvl);
    void do_add_node(cdk::add_node * const node, int lvl);
    void do_sub_node(cdk::sub_node * const node, int lvl);
    void do_mul_node(cdk::mul_node * const node, int lvl);
    void do_div_node(cdk::div_node * const node, int lvl);
    void do_mod_node(cdk::mod_node * const node, int lvl);
    void do_lt_node(cdk::lt_node * const node, int lvl);
    void do_le_node(cdk::le_node * const node, int lvl);
    void do_ge_node(cdk::ge_node * const node, int lvl);
    void do_gt_node(cdk::gt_node * const node, int lvl);
    void do_ne_node(cdk::ne_node * const node, int lvl);
    void do_eq_node(cdk::eq_node * const node, int lvl);

  public:
    void do_lvalue_node(zu::lvalue_node * const node, int lvl);
    void do_rvalue_node(zu::rvalue_node * const node, int lvl);

  public:
    void do_function_call_node(zu::function_call_node * const node, int lvl);
    void do_function_declaration_node(zu::function_declaration_node * const node, int lvl);
    void do_function_definition_node(zu::function_definition_node * const node, int lvl);
    void do_return_node(zu::return_node * const node, int lvl);
    void do_evaluation_node(zu::evaluation_node * const node, int lvl);
    void do_print_node(zu::print_node * const node, int lvl);
    void do_print_line_node(zu::print_line_node * const node, int lvl);
    void do_read_node(zu::read_node * const node, int lvl);
    void do_assignment_node(zu::assignment_node * const node, int lvl);
    
  public:
    void do_index_node(zu::index_node * const node, int lvl);
    void do_allocation_node(zu::allocation_node * const node, int lvl);
    void do_address_node(zu::address_node * const node, int lvl);

  public:
    void do_for_node(zu::for_node * const node, int lvl);
    void do_if_node(zu::if_node * const node, int lvl);
    void do_if_else_node(zu::if_else_node * const node, int lvl);
    void do_break_node(zu::break_node * const node, int lvl);
    void do_continue_node(zu::continue_node * const node, int lvl);
    void do_block_node(zu::block_node * const node, int lvl);

  };

} // zu

#endif
