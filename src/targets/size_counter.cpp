// $Id: size_counter.cpp,v 1.2 2016/05/20 12:00:32 ist178190 Exp $ -*- c++ -*-
#include "ast/all.h"  // automatically generated
#include "targets/size_counter.h"

//---------------------------------------------------------------------------

void zu::size_counter::do_sequence_node(cdk::sequence_node * const node, int lvl) {
  for (size_t i = 0; i < node->size(); i++) {
    if (node->node(i) != nullptr) {
      node->node(i)->accept(this, lvl);
    }
  }
}

void zu::size_counter::do_variable_declaration_node(zu::variable_declaration_node * const node, int lvl) {
  _size += 4;
}

void zu::size_counter::do_variable_node(zu::variable_node * const node, int lvl) {
  
}

void zu::size_counter::do_integer_node(cdk::integer_node * const node, int lvl) {
  
}

void zu::size_counter::do_double_node(cdk::double_node * const node, int lvl) {

}

void zu::size_counter::do_string_node(cdk::string_node * const node, int lvl) {

}

//---------------------------------------------------------------------------

void zu::size_counter::do_neg_node(cdk::neg_node * const node, int lvl) {
  
}

void zu::size_counter::do_identity_node(zu::identity_node * const node, int lvl) {
  
}

void zu::size_counter::do_not_node(zu::not_node * const node, int lvl) {
  node->argument()->accept(this, lvl);
}

//---------------------------------------------------------------------------

void zu::size_counter::do_and_node(zu::and_node * const node, int lvl) {
  node->left()->accept(this, lvl);
  node->right()->accept(this, lvl);
}

void zu::size_counter::do_or_node(zu::or_node * const node, int lvl) {
  node->left()->accept(this, lvl);
  node->right()->accept(this, lvl);
}

void zu::size_counter::do_add_node(cdk::add_node * const node, int lvl) {
  node->left()->accept(this, lvl);
  node->right()->accept(this, lvl);
}
void zu::size_counter::do_sub_node(cdk::sub_node * const node, int lvl) {
  node->left()->accept(this, lvl);
  node->right()->accept(this, lvl);
}
void zu::size_counter::do_mul_node(cdk::mul_node * const node, int lvl) {
  node->left()->accept(this, lvl);
  node->right()->accept(this, lvl);
}
void zu::size_counter::do_div_node(cdk::div_node * const node, int lvl) {
  node->left()->accept(this, lvl);
  node->right()->accept(this, lvl);
}
void zu::size_counter::do_mod_node(cdk::mod_node * const node, int lvl) {
  node->left()->accept(this, lvl);
  node->right()->accept(this, lvl);
}
void zu::size_counter::do_lt_node(cdk::lt_node * const node, int lvl) {
  node->left()->accept(this, lvl);
  node->right()->accept(this, lvl);
}
void zu::size_counter::do_le_node(cdk::le_node * const node, int lvl) {
  node->left()->accept(this, lvl);
  node->right()->accept(this, lvl);
}
void zu::size_counter::do_ge_node(cdk::ge_node * const node, int lvl) {
  node->left()->accept(this, lvl);
  node->right()->accept(this, lvl);
}
void zu::size_counter::do_gt_node(cdk::gt_node * const node, int lvl) {
  node->left()->accept(this, lvl);
  node->right()->accept(this, lvl);
}
void zu::size_counter::do_ne_node(cdk::ne_node * const node, int lvl) {
  node->left()->accept(this, lvl);
  node->right()->accept(this, lvl);
}
void zu::size_counter::do_eq_node(cdk::eq_node * const node, int lvl) {
  node->left()->accept(this, lvl);
  node->right()->accept(this, lvl);
}

//---------------------------------------------------------------------------

void zu::size_counter::do_rvalue_node(zu::rvalue_node * const node, int lvl) {
  node->lvalue()->accept(this, lvl);
}

//---------------------------------------------------------------------------

void zu::size_counter::do_lvalue_node(zu::lvalue_node * const node, int lvl) {

}

//---------------------------------------------------------------------------

void zu::size_counter::do_assignment_node(zu::assignment_node * const node, int lvl) {
  node->lvalue()->accept(this, lvl);
  node->rvalue()->accept(this, lvl);
}

//---------------------------------------------------------------------------

void zu::size_counter::do_index_node(zu::index_node * const node, int lvl) {
  node->base()->accept(this, lvl);
  node->value()->accept(this, lvl);
}

void zu::size_counter::do_allocation_node(zu::allocation_node * const node, int lvl) {
  node->size()->accept(this, lvl);
}

void zu::size_counter::do_address_node(zu::address_node * const node, int lvl) {
  node->argument()->accept(this, lvl);
}

//---------------------------------------------------------------------------

void zu::size_counter::do_function_call_node(zu::function_call_node * const node, int lvl) {
  if (node->args() != nullptr) {
    int size = node->args()->size() * 4;
    _size += size;
  }
}

void zu::size_counter::do_function_declaration_node(zu::function_declaration_node * const node, int lvl) {
  
}

void zu::size_counter::do_function_definition_node(zu::function_definition_node * const node, int lvl) {
  if (node->block() != nullptr)
    node->block()->accept(this, lvl);
}

void zu::size_counter::do_return_node(zu::return_node * const node, int lvl) {
  
}

void zu::size_counter::do_evaluation_node(zu::evaluation_node * const node, int lvl) {
  node->argument()->accept(this, lvl);
}

void zu::size_counter::do_print_node(zu::print_node * const node, int lvl) {
  node->argument()->accept(this, lvl);
}  
    
void zu::size_counter::do_print_line_node(zu::print_line_node * const node, int lvl) {
  node->argument()->accept(this, lvl);
}

//---------------------------------------------------------------------------

void zu::size_counter::do_read_node(zu::read_node * const node, int lvl) {
  
}

//---------------------------------------------------------------------------

void zu::size_counter::do_for_node(zu::for_node * const node, int lvl) {
  if (node->init() != nullptr)
    node->init()->accept(this, lvl);
  if (node->block() != nullptr)
    node->block()->accept(this, lvl);
}

//---------------------------------------------------------------------------

void zu::size_counter::do_if_node(zu::if_node * const node, int lvl) {
  if (node->condition() != nullptr)
    node->condition()->accept(this, lvl);
  if (node->block() != nullptr)
    node->block()->accept(this, lvl);
}

void zu::size_counter::do_if_else_node(zu::if_else_node * const node, int lvl) { 
  if (node->condition() != nullptr)
    node->condition()->accept(this, lvl);
  if (node->thenblock() != nullptr)
    node->thenblock()->accept(this, lvl);
  if (node->elseblock() != nullptr)
    node->elseblock()->accept(this, lvl);
}

void zu::size_counter::do_break_node(zu::break_node * const node, int lvl) {
  
}

void zu::size_counter::do_continue_node(zu::continue_node * const node, int lvl) {
  
}

void zu::size_counter::do_block_node(zu::block_node * const node, int lvl) {
  if (node->decls() != nullptr)
    node->decls()->accept(this, lvl);
  if (node->instrs() != nullptr)
    node->instrs()->accept(this, lvl);
}
