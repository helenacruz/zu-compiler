// $Id: xml_writer.cpp,v 1.21 2016/05/18 10:18:44 ist178190 Exp $ -*- c++ -*-
#include <string>
#include "targets/xml_writer.h"
#include "targets/type_checker.h"
#include "ast/all.h"  // automatically generated

//---------------------------------------------------------------------------

void zu::xml_writer::do_sequence_node(cdk::sequence_node * const node, int lvl) {
  os() << std::string(lvl, ' ') << "<sequence_node size='" << node->size() << "'>" << std::endl;
  for (size_t i = 0; i < node->size(); i++) {
      if (node->node(i) != nullptr) {
        node->node(i)->accept(this, lvl + 2);
      }
  }
  closeTag(node, lvl);
}

//---------------------------------------------------------------------------

void zu::xml_writer::do_variable_node(zu::variable_node * const node, int lvl) {
  openTag(node, lvl);
  os() << std::string(lvl + 2, ' ') << "<variable_node name='" << *node->name() << "'>" << std::endl;
  closeTag(node, lvl);
}



void zu::xml_writer::do_variable_declaration_node(zu::variable_declaration_node * const node, int lvl) {
  std::string isPrivate;
  
  if (node->isPrivate())
      isPrivate = "private";
  else
      isPrivate = "public";
    
  openTag(node, lvl);
  os() << std::string(lvl + 2, ' ') << "<variable_declaration_node name='" << *node->name() << "'>" << std::endl;
  openTag("value", lvl + 2);
  if (node->value() != nullptr) {
    node->value()->accept(this, lvl + 4);
  }
  closeTag("value", lvl + 2);
  os() << std::string(lvl + 2, ' ') << "<Scope>" << isPrivate << "</scope>" << std::endl;
  closeTag(node, lvl);
}

void zu::xml_writer::do_integer_node(cdk::integer_node * const node, int lvl) {
  processSimple(node, lvl);
}

void zu::xml_writer::do_double_node(cdk::double_node * const node, int lvl) {
    processSimple(node, lvl);
}

void zu::xml_writer::do_string_node(cdk::string_node * const node, int lvl) {
  processSimple(node, lvl);
}

//---------------------------------------------------------------------------

inline void zu::xml_writer::processUnaryExpression(cdk::unary_expression_node * const node, int lvl) {
  openTag(node, lvl);
  node->argument()->accept(this, lvl + 2);
  closeTag(node, lvl);
}

void zu::xml_writer::do_neg_node(cdk::neg_node * const node, int lvl) {
  processUnaryExpression(node, lvl);
}

void zu::xml_writer::do_identity_node(zu::identity_node * const node, int lvl) {
  processUnaryExpression(node, lvl);
}

void zu::xml_writer::do_not_node(zu::not_node * const node, int lvl) {
  processUnaryExpression(node, lvl);
}

//---------------------------------------------------------------------------

inline void zu::xml_writer::processBinaryExpression(cdk::binary_expression_node * const node, int lvl) {
  openTag(node, lvl);
  node->left()->accept(this, lvl + 2);
  node->right()->accept(this, lvl + 2);
  closeTag(node, lvl);
}

void zu::xml_writer::do_and_node(zu::and_node * const node, int lvl) {
  processBinaryExpression(node, lvl);
}

void zu::xml_writer::do_or_node(zu::or_node * const node, int lvl) {
  processBinaryExpression(node, lvl);
}

void zu::xml_writer::do_add_node(cdk::add_node * const node, int lvl) {
  processBinaryExpression(node, lvl);
}
void zu::xml_writer::do_sub_node(cdk::sub_node * const node, int lvl) {
  processBinaryExpression(node, lvl);
}
void zu::xml_writer::do_mul_node(cdk::mul_node * const node, int lvl) {
  processBinaryExpression(node, lvl);
}
void zu::xml_writer::do_div_node(cdk::div_node * const node, int lvl) {
  processBinaryExpression(node, lvl);
}
void zu::xml_writer::do_mod_node(cdk::mod_node * const node, int lvl) {
  processBinaryExpression(node, lvl);
}
void zu::xml_writer::do_lt_node(cdk::lt_node * const node, int lvl) {
  processBinaryExpression(node, lvl);
}
void zu::xml_writer::do_le_node(cdk::le_node * const node, int lvl) {
  processBinaryExpression(node, lvl);
}
void zu::xml_writer::do_ge_node(cdk::ge_node * const node, int lvl) {
  processBinaryExpression(node, lvl);
}
void zu::xml_writer::do_gt_node(cdk::gt_node * const node, int lvl) {
  processBinaryExpression(node, lvl);
}
void zu::xml_writer::do_ne_node(cdk::ne_node * const node, int lvl) {
  processBinaryExpression(node, lvl);
}
void zu::xml_writer::do_eq_node(cdk::eq_node * const node, int lvl) {
  processBinaryExpression(node, lvl);
}

//---------------------------------------------------------------------------

void zu::xml_writer::do_rvalue_node(zu::rvalue_node * const node, int lvl) {
  openTag(node, lvl);
  node->lvalue()->accept(this, lvl + 4);
  closeTag(node, lvl);
}

//---------------------------------------------------------------------------

void zu::xml_writer::do_lvalue_node(zu::lvalue_node * const node, int lvl) {

}

//---------------------------------------------------------------------------

void zu::xml_writer::do_assignment_node(zu::assignment_node * const node, int lvl) {

  openTag(node, lvl); 
  node->lvalue()->accept(this, lvl + 2);
  openTag("rvalue", lvl + 2);
  node->rvalue()->accept(this, lvl + 4);
  closeTag("rvalue", lvl + 2);
  closeTag(node, lvl);
}

//---------------------------------------------------------------------------

void zu::xml_writer::do_index_node(zu::index_node * const node, int lvl) {
  openTag(node, lvl);
  node->base()->accept(this, lvl + 2);
  openTag("value", lvl + 2);
  node->value()->accept(this, lvl + 4);
  closeTag("value", lvl + 4);
  closeTag(node, lvl);
}

void zu::xml_writer::do_allocation_node(zu::allocation_node * const node, int lvl) {
  openTag(node, lvl);
  node->size()->accept(this, lvl + 2);
  closeTag(node, lvl);
}

void zu::xml_writer::do_address_node(zu::address_node * const node, int lvl) {
  processUnaryExpression(node, lvl);
}

//---------------------------------------------------------------------------

void zu::xml_writer::do_function_call_node(zu::function_call_node * const node, int lvl) {
  openTag(node, lvl);
  os() << std::string(lvl + 2, ' ') << "<function_call_node name='" << *node->name() << "'>" << std::endl;
  if (node->args() != nullptr)
    node->args()->accept(this, lvl + 4);
  closeTag(node, lvl);
}

void zu::xml_writer::do_function_declaration_node(zu::function_declaration_node * const node, int lvl) {
  std::string type;
  std::string isPrivate;
  
  if (node->type()->name() == 1UL<<0)
      type = "integer";
  else if (node->type()->name() == 1UL<<1)
      type = "double";
  else if (node->type()->name() == 1UL<<3)
      type = "string";
  else if (node->type()->name() == 1UL<<4)
      type = "pointer subtype = ";
  else if (node->type()->name() == 1UL<<30)
      type = "void";
  
  openTag(node, lvl);
  os() << std::string(lvl + 2, ' ') << "<function_declaration_node name='" << *node->name() << "'>" << std::endl;
  if (node->args() != nullptr)
    node->args()->accept(this, lvl + 4);
  os() << std::string(lvl + 2, ' ') << "<type>" << "</" << type << ">" << std::endl;
  openTag("default value", lvl + 2);
  if (node->def_value() != nullptr)
    node->def_value()->accept(this, lvl + 4);
  closeTag("default value", lvl + 2);
  os() << std::string(lvl + 2, ' ') << "<Scope>" << isPrivate << "</scope>" << std::endl;
  closeTag(node, lvl);
}

void zu::xml_writer::do_function_definition_node(zu::function_definition_node * const node, int lvl) {
  std::string type;
  std::string isPrivate;
  
  if (node->type()->name() == 1UL<<0)
      type = "integer";
  else if (node->type()->name() == 1UL<<1)
      type = "double";
  else if (node->type()->name() == 1UL<<3)
      type = "string";
  else if (node->type()->name() == 1UL<<4)
      type = "pointer subtype = ";
  else if (node->type()->name() == 1UL<<30)
      type = "void";
  
  if (node->isPrivate())
      isPrivate = "private";
  else
      isPrivate = "public";
  
  openTag(node, lvl);
  os() << std::string(lvl + 2, ' ') << "<function_definition_node name='" << *node->name() << "'>" << std::endl;
  if (node->args() != nullptr) {
    node->args()->accept(this, lvl + 4);
  }
  os() << std::string(lvl + 2, ' ') << "<type>" << "</" << type << ">" << std::endl;
  openTag("default value", lvl + 2);
  if (node->def_value() != nullptr) {
    node->def_value()->accept(this, lvl + 4);
  }
  closeTag("default value", lvl + 2);
  os() << std::string(lvl + 2, ' ') << "<Scope>" << isPrivate << "</scope>" << std::endl;
  openTag("block", lvl + 2);
  if (node->block() != nullptr) {
    node->block()->accept(this, lvl + 4);
  }
  closeTag("block", lvl + 2);
  closeTag(node, lvl);
}

void zu::xml_writer::do_return_node(zu::return_node * const node, int lvl) {
  openTag(node, lvl);
  closeTag(node, lvl);
}

//---------------------------------------------------------------------------

void zu::xml_writer::do_evaluation_node(zu::evaluation_node * const node, int lvl) {
  openTag(node, lvl);
  if (node->argument() != nullptr) {
    node->argument()->accept(this, lvl + 2);
  }
  closeTag(node, lvl);
}

void zu::xml_writer::do_print_node(zu::print_node * const node, int lvl) {
  openTag(node, lvl);
  node->argument()->accept(this, lvl + 2);
  closeTag(node, lvl);
}

void zu::xml_writer::do_print_line_node(zu::print_line_node * const node, int lvl) {
  openTag(node, lvl);
  node->argument()->accept(this, lvl + 2);
  closeTag(node, lvl);
}

//---------------------------------------------------------------------------

void zu::xml_writer::do_read_node(zu::read_node * const node, int lvl) {
  openTag(node, lvl);
  closeTag(node, lvl);
}

//---------------------------------------------------------------------------

void zu::xml_writer::do_for_node(zu::for_node * const node, int lvl) {
  openTag(node, lvl);
  openTag("init", lvl + 2);
  if (node->init() != nullptr) {
    node->init()->accept(this, lvl + 4);
  }
  closeTag("init", lvl + 2);
  openTag("cond", lvl + 2);
  if (node->cond() != nullptr) {
    node->cond()->accept(this, lvl + 4);
  }
  closeTag("cond", lvl + 2);
  openTag("incr", lvl + 2);
  if (node->incr() != nullptr) {
    node->incr()->accept(this, lvl + 4);
  }
  closeTag("incr", lvl + 2);
  openTag("block", lvl + 2);
  if (node->block() != nullptr) {
    node->block()->accept(this, lvl + 4);
  }
  closeTag("block", lvl + 2);
  closeTag(node, lvl);
}

//---------------------------------------------------------------------------

void zu::xml_writer::do_if_node(zu::if_node * const node, int lvl) {
  openTag(node, lvl);
  openTag("condition", lvl + 2);
  if (node->condition() != nullptr) {
    node->condition()->accept(this, lvl + 4);
  }
  closeTag("condition", lvl + 2);
  openTag("then", lvl + 2);
  if (node->block() != nullptr) {
    node->block()->accept(this, lvl + 4);
  }
  closeTag("then", lvl + 2);
  closeTag(node, lvl);
}

void zu::xml_writer::do_if_else_node(zu::if_else_node * const node, int lvl) {
  openTag(node, lvl);
  openTag("condition", lvl + 2);
  if (node->condition() != nullptr) {
    node->condition()->accept(this, lvl + 4);
  }
  closeTag("condition", lvl + 2);
  openTag("then", lvl + 2);
  if (node->thenblock() != nullptr) {
    node->thenblock()->accept(this, lvl + 4);
  }
  closeTag("then", lvl + 2);
  openTag("else", lvl + 2);
  if (node->elseblock() != nullptr) {
    node->elseblock()->accept(this, lvl + 4);
  }
  closeTag("else", lvl + 2);
  closeTag(node, lvl);
}

void zu::xml_writer::do_break_node(zu::break_node * const node, int lvl) {
  openTag(node, lvl);
  closeTag(node, lvl);
}

void zu::xml_writer::do_continue_node(zu::continue_node * const node, int lvl) {
  openTag(node, lvl);
  closeTag(node, lvl);
}

void zu::xml_writer::do_block_node(zu::block_node * const node, int lvl) {
  openTag(node, lvl);
  openTag("decls", lvl + 2);
  if (node->decls() != nullptr) {
    node->decls()->accept(this, lvl + 4);
  }
  closeTag("decls", lvl + 2);
  openTag("instrs", lvl + 2);
  if (node->instrs() != nullptr) {
    node->instrs()->accept(this, lvl + 4);
  }
  closeTag("instrs", lvl + 2);
  closeTag(node, lvl);
}