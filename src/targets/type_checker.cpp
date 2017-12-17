// $Id: type_checker.cpp,v 1.25 2016/05/20 12:00:32 ist178190 Exp $ -*- c++ -*-
#include <string>
#include "targets/type_checker.h"
#include "ast/all.h"  // automatically generated

#define ASSERT_UNSPEC \
    { if (node->type() != nullptr && \
          node->type()->name() != basic_type::TYPE_UNSPEC) return; }

//---------------------------------------------------------------------------

void zu::type_checker::do_variable_declaration_node(zu::variable_declaration_node * const node, int lvl) {
  const std::string id = *node->name();
  if (node->value() != nullptr) {
    node->value()->accept(this, lvl);
    if (node->type()->name() == basic_type::TYPE_POINTER && 
        node->value()->type()->name() == basic_type::TYPE_INT);
    else if (node->value()->type()->name() != node->type()->name())
      throw std::string("wrong type on variable declaration");
  }
  if (_symtab.find_local(id))
    throw std::string(id + " already declared");
}

void zu::type_checker::do_variable_node(zu::variable_node * const node, int lvl) {
  const std::string id = *node->name();
  std::shared_ptr<zu::symbol> symbol = _symtab.find(id);
  if (symbol == nullptr)
    throw std::string(id + " undeclared");
  
  if (symbol->type()->name() == basic_type::TYPE_INT)
    node->type(new basic_type(4, basic_type::TYPE_INT));
  else if (symbol->type()->name() == basic_type::TYPE_DOUBLE)
    node->type(new basic_type(8, basic_type::TYPE_DOUBLE));
  else if (symbol->type()->name() == basic_type::TYPE_STRING)
    node->type(new basic_type(4, basic_type::TYPE_STRING));
  else if (symbol->type()->name() == basic_type::TYPE_POINTER)
    node->type(new basic_type(4, basic_type::TYPE_POINTER));
}

void zu::type_checker::do_integer_node(cdk::integer_node * const node, int lvl) {
  ASSERT_UNSPEC;
  node->type(new basic_type(4, basic_type::TYPE_INT));
}

void zu::type_checker::do_double_node(cdk::double_node * const node, int lvl) {
  ASSERT_UNSPEC;
  node->type(new basic_type(8, basic_type::TYPE_DOUBLE));
}

void zu::type_checker::do_string_node(cdk::string_node * const node, int lvl) {
  ASSERT_UNSPEC;
  node->type(new basic_type(4, basic_type::TYPE_STRING));
}

//---------------------------------------------------------------------------
/*
inline void zu::type_checker::processUnaryExpression(cdk::unary_expression_node * const node, int lvl) {
  node->argument()->accept(this, lvl + 2);
  if (node->argument()->type()->name() != basic_type::TYPE_INT)
    throw std::string("wrong type in argument of unary expression");

  // in Zu, expressions are always int
  node->type(new basic_type(4, basic_type::TYPE_INT));
}
*/

inline void zu::type_checker::processIDUnaryExpression(cdk::unary_expression_node * const node, int lvl) {
  node->argument()->accept(this, lvl + 2);
  
  if (node->argument()->type()->name() == basic_type::TYPE_INT)
    node->type(new basic_type(4, basic_type::TYPE_INT));
  else if (node->argument()->type()->name() == basic_type::TYPE_DOUBLE)
    node->type(new basic_type(8, basic_type::TYPE_DOUBLE));
  else if (node->argument()->type()->name() == basic_type::TYPE_STRING)
    throw std::string("identity and symmetric don't accept strings");
  else if (node->argument()->type()->name() == basic_type::TYPE_POINTER)
    throw std::string("identity and symmetric don't accept pointers");
}

inline void zu::type_checker::processIUnaryExpression(cdk::unary_expression_node * const node, int lvl) {
  node->argument()->accept(this, lvl + 2);
  
  if (node->argument()->type()->name() == basic_type::TYPE_INT)
    node->type(new basic_type(4, basic_type::TYPE_INT));
  else if (node->argument()->type()->name() == basic_type::TYPE_DOUBLE)
        throw std::string("not doesn't accept doubles");
  else if (node->argument()->type()->name() == basic_type::TYPE_STRING)
    throw std::string("not doesn't accept strings");
  else if (node->argument()->type()->name() == basic_type::TYPE_POINTER)
    throw std::string("not doesn't accept pointers");
}

void zu::type_checker::do_neg_node(cdk::neg_node * const node, int lvl) {
  processIDUnaryExpression(node, lvl);
}

void zu::type_checker::do_identity_node(zu::identity_node * const node, int lvl) {
  processIDUnaryExpression(node, lvl + 2);
}

void zu::type_checker::do_not_node(zu::not_node * const node, int lvl) {
  processIUnaryExpression(node, lvl + 2);
}

//---------------------------------------------------------------------------

inline void zu::type_checker::processPIDBinaryExpression(cdk::binary_expression_node * const node, int lvl) {
  ASSERT_UNSPEC;
  
  node->left()->accept(this, lvl + 2);
  node->right()->accept(this, lvl + 2);
  
  if (node->left()->type()->name() == basic_type::TYPE_INT &&
      node->right()->type()->name() == basic_type::TYPE_DOUBLE)
    node->type(new basic_type(8, basic_type::TYPE_DOUBLE));
  
  else if (node->right()->type()->name() == basic_type::TYPE_INT &&
      node->left()->type()->name() == basic_type::TYPE_DOUBLE)
    node->type(new basic_type(8, basic_type::TYPE_DOUBLE));
  
  else if (node->left()->type()->name() == basic_type::TYPE_INT &&
      node->right()->type()->name() == basic_type::TYPE_INT)
    node->type(new basic_type(4, basic_type::TYPE_INT));
      
  else if (node->left()->type()->name() == basic_type::TYPE_DOUBLE &&
      node->right()->type()->name() == basic_type::TYPE_DOUBLE)
    node->type(new basic_type(8, basic_type::TYPE_DOUBLE));
  
  else if (node->left()->type()->name() == basic_type::TYPE_POINTER &&
      node->right()->type()->name() == basic_type::TYPE_INT)
    node->type(new basic_type(4, basic_type::TYPE_POINTER));
  
  else if (node->right()->type()->name() == basic_type::TYPE_POINTER &&
      node->left()->type()->name() == basic_type::TYPE_INT)
    node->type(new basic_type(4, basic_type::TYPE_POINTER));
  
  else if (node->left()->type()->name() == basic_type::TYPE_POINTER &&
      node->right()->type()->name() == basic_type::TYPE_DOUBLE)
    throw std::string("can't add pointers with doubles");
  
  else if (node->right()->type()->name() == basic_type::TYPE_POINTER &&
      node->left()->type()->name() == basic_type::TYPE_DOUBLE)
    throw std::string("can't add pointers with doubles");
  
  else if (node->left()->type()->name() == basic_type::TYPE_POINTER &&
      node->right()->type()->name() == basic_type::TYPE_STRING)
    throw std::string("can't add pointers with strings");
  
  else if (node->right()->type()->name() == basic_type::TYPE_POINTER &&
      node->left()->type()->name() == basic_type::TYPE_STRING)
    throw std::string("can't add pointers with strings");
  
  else if (node->left()->type()->name() == basic_type::TYPE_STRING &&
      node->right()->type()->name() == basic_type::TYPE_INT)
    throw std::string("can't add strings with integers");
  
  else if (node->right()->type()->name() == basic_type::TYPE_STRING &&
      node->left()->type()->name() == basic_type::TYPE_INT)
    throw std::string("can't add strings with integers"); 
  
  else if (node->left()->type()->name() == basic_type::TYPE_STRING &&
      node->right()->type()->name() == basic_type::TYPE_DOUBLE)
    throw std::string("can't add strings with doubles");
      
   else if (node->right()->type()->name() == basic_type::TYPE_STRING &&
      node->left()->type()->name() == basic_type::TYPE_DOUBLE)
    throw std::string("can't add strings with doubles");
  
  else if (node->left()->type()->name() == basic_type::TYPE_STRING &&
      node->right()->type()->name() == basic_type::TYPE_STRING)
    throw std::string("can't add strings with strings");
}

inline void zu::type_checker::processIDBinaryExpression(cdk::binary_expression_node * const node, int lvl) {
  ASSERT_UNSPEC;
  
  node->left()->accept(this, lvl + 2);
  node->right()->accept(this, lvl + 2);
  
  if (node->left()->type()->name() == basic_type::TYPE_INT &&
      node->right()->type()->name() == basic_type::TYPE_DOUBLE)
    node->type(new basic_type(8, basic_type::TYPE_DOUBLE));
  
  else if (node->right()->type()->name() == basic_type::TYPE_INT &&
      node->left()->type()->name() == basic_type::TYPE_DOUBLE)
    node->type(new basic_type(8, basic_type::TYPE_DOUBLE));
  
  else if (node->left()->type()->name() == basic_type::TYPE_INT &&
      node->right()->type()->name() == basic_type::TYPE_INT)
    node->type(new basic_type(4, basic_type::TYPE_INT));
      
  else if (node->left()->type()->name() == basic_type::TYPE_DOUBLE &&
      node->right()->type()->name() == basic_type::TYPE_DOUBLE)
    node->type(new basic_type(8, basic_type::TYPE_DOUBLE));
  
  else if (node->left()->type()->name() == basic_type::TYPE_POINTER &&
      node->right()->type()->name() == basic_type::TYPE_INT)
    throw std::string("mul, div, mod and comparison operators don't accept pointers");
  
  else if (node->right()->type()->name() == basic_type::TYPE_POINTER &&
      node->left()->type()->name() == basic_type::TYPE_INT)
    throw std::string("mul, div, mod and comparison operators don't accept pointers");
  
  else if (node->left()->type()->name() == basic_type::TYPE_POINTER &&
      node->right()->type()->name() == basic_type::TYPE_DOUBLE)
    throw std::string("mul, div, mod and comparison operators don't accept pointers");
  
  else if (node->right()->type()->name() == basic_type::TYPE_POINTER &&
      node->left()->type()->name() == basic_type::TYPE_DOUBLE)
    throw std::string("mul, div, mod and comparison operators don't accept pointers");
  
  else if (node->left()->type()->name() == basic_type::TYPE_POINTER &&
      node->right()->type()->name() == basic_type::TYPE_STRING)
    throw std::string("mul, div, mod and comparison operators don't accept pointers or strings");
  
  else if (node->right()->type()->name() == basic_type::TYPE_POINTER &&
      node->left()->type()->name() == basic_type::TYPE_STRING)
    throw std::string("mul, div, mod and comparison operators don't accept pointers or strings");
  
  else if (node->left()->type()->name() == basic_type::TYPE_STRING &&
      node->right()->type()->name() == basic_type::TYPE_INT)
    throw std::string("mul, div, mod and comparison operators don't accept strings");
  
  else if (node->right()->type()->name() == basic_type::TYPE_STRING &&
      node->left()->type()->name() == basic_type::TYPE_INT)
    throw std::string("mul, div, mod and comparison operators don't accept strings");
  
  else if (node->left()->type()->name() == basic_type::TYPE_STRING &&
      node->right()->type()->name() == basic_type::TYPE_DOUBLE)
    throw std::string("mul, div, mod and comparison operators don't accept strings");
      
   else if (node->right()->type()->name() == basic_type::TYPE_STRING &&
      node->left()->type()->name() == basic_type::TYPE_DOUBLE)
    throw std::string("mul, div, mod and comparison operators don't accept strings");
  
  else if (node->left()->type()->name() == basic_type::TYPE_STRING &&
      node->right()->type()->name() == basic_type::TYPE_STRING)
    throw std::string("mul, div, mod and comparison operators don't accept strings");
}

inline void zu::type_checker::processIBinaryExpression(cdk::binary_expression_node * const node, int lvl) {
  ASSERT_UNSPEC;
  
  node->left()->accept(this, lvl + 2);
  node->right()->accept(this, lvl + 2);
  
  if (node->left()->type()->name() == basic_type::TYPE_INT &&
      node->right()->type()->name() == basic_type::TYPE_INT)
    node->type(new basic_type(4, basic_type::TYPE_INT));
  
  else if (node->left()->type()->name() == basic_type::TYPE_INT &&
      node->right()->type()->name() == basic_type::TYPE_DOUBLE)
  throw std::string("and operator and or don't accept doubles");
  
  else if (node->right()->type()->name() == basic_type::TYPE_INT &&
      node->left()->type()->name() == basic_type::TYPE_DOUBLE)
      throw std::string("and operator and or don't accept doubles");
      
  else if (node->left()->type()->name() == basic_type::TYPE_DOUBLE &&
      node->right()->type()->name() == basic_type::TYPE_DOUBLE)
      throw std::string("and operator and or don't accept doubles");
  
  else if (node->left()->type()->name() == basic_type::TYPE_POINTER &&
      node->right()->type()->name() == basic_type::TYPE_INT)
      throw std::string("and operator and or don't accept doubles or pointers");
  
  else if (node->right()->type()->name() == basic_type::TYPE_POINTER &&
      node->left()->type()->name() == basic_type::TYPE_INT)
      throw std::string("and operator and or don't accept pointers");
  
  else if (node->left()->type()->name() == basic_type::TYPE_POINTER &&
      node->right()->type()->name() == basic_type::TYPE_DOUBLE)
      throw std::string("and operator and or don't accept doubles or pointers");
  
  else if (node->right()->type()->name() == basic_type::TYPE_POINTER &&
      node->left()->type()->name() == basic_type::TYPE_DOUBLE)
      throw std::string("and operator and or don't accept doubles or pointers");
  
  else if (node->left()->type()->name() == basic_type::TYPE_POINTER &&
      node->right()->type()->name() == basic_type::TYPE_STRING)
      throw std::string("and operator and or don't accept pointers or strings");
  
  else if (node->right()->type()->name() == basic_type::TYPE_POINTER &&
      node->left()->type()->name() == basic_type::TYPE_STRING)
      throw std::string("and operator and or don't accept pointers or strings");
  
  else if (node->left()->type()->name() == basic_type::TYPE_STRING &&
      node->right()->type()->name() == basic_type::TYPE_INT)
      throw std::string("and operator and or don't accept strings");
  
  else if (node->right()->type()->name() == basic_type::TYPE_STRING &&
      node->left()->type()->name() == basic_type::TYPE_INT)
      throw std::string("and operator and or don't accept strings");
  
  else if (node->left()->type()->name() == basic_type::TYPE_STRING &&
      node->right()->type()->name() == basic_type::TYPE_DOUBLE)
      throw std::string("and operator and or don't accept strings or doubles");
      
   else if (node->right()->type()->name() == basic_type::TYPE_STRING &&
      node->left()->type()->name() == basic_type::TYPE_DOUBLE)
      throw std::string("and operator and or don't accept doubles or strings");
  
  else if (node->left()->type()->name() == basic_type::TYPE_STRING &&
      node->right()->type()->name() == basic_type::TYPE_STRING)
      throw std::string("and operator and or don't accept strings");
}

inline void zu::type_checker::processPPIDBinaryExpression(cdk::binary_expression_node * const node, int lvl) {
  ASSERT_UNSPEC;
  
  node->left()->accept(this, lvl + 2);
  node->right()->accept(this, lvl + 2);
  
  if (node->left()->type()->name() == basic_type::TYPE_INT &&
      node->right()->type()->name() == basic_type::TYPE_DOUBLE)
    node->type(new basic_type(8, basic_type::TYPE_DOUBLE));
  
  else if (node->right()->type()->name() == basic_type::TYPE_INT &&
      node->left()->type()->name() == basic_type::TYPE_DOUBLE)
    node->type(new basic_type(8, basic_type::TYPE_DOUBLE));
  
  else if (node->left()->type()->name() == basic_type::TYPE_INT &&
      node->right()->type()->name() == basic_type::TYPE_INT)
    node->type(new basic_type(4, basic_type::TYPE_INT));
      
  else if (node->left()->type()->name() == basic_type::TYPE_DOUBLE &&
      node->right()->type()->name() == basic_type::TYPE_DOUBLE)
    node->type(new basic_type(8, basic_type::TYPE_DOUBLE));
  
  else if (node->left()->type()->name() == basic_type::TYPE_POINTER &&
      node->right()->type()->name() == basic_type::TYPE_INT)
    node->type(new basic_type(4, basic_type::TYPE_POINTER));
  
  else if (node->right()->type()->name() == basic_type::TYPE_POINTER &&
      node->left()->type()->name() == basic_type::TYPE_INT)
    node->type(new basic_type(4, basic_type::TYPE_POINTER));
  
  else if (node->left()->type()->name() == basic_type::TYPE_POINTER &&
      node->right()->type()->name() == basic_type::TYPE_POINTER)
    node->type(new basic_type(4, basic_type::TYPE_POINTER));
  
  else if (node->left()->type()->name() == basic_type::TYPE_POINTER &&
      node->right()->type()->name() == basic_type::TYPE_DOUBLE)
    throw std::string("can't sub or compare pointers with doubles");
  
  else if (node->right()->type()->name() == basic_type::TYPE_POINTER &&
      node->left()->type()->name() == basic_type::TYPE_DOUBLE)
    throw std::string("can't sub or compare pointers with doubles");
  
  else if (node->left()->type()->name() == basic_type::TYPE_POINTER &&
      node->right()->type()->name() == basic_type::TYPE_STRING)
    throw std::string("can't sub or compare pointers with strings");
  
  else if (node->right()->type()->name() == basic_type::TYPE_POINTER &&
      node->left()->type()->name() == basic_type::TYPE_STRING)
    throw std::string("can't sub or compare pointers with strings");
  
  else if (node->left()->type()->name() == basic_type::TYPE_STRING &&
      node->right()->type()->name() == basic_type::TYPE_INT)
    throw std::string("can't sub or compare strings with integers");
  
  else if (node->right()->type()->name() == basic_type::TYPE_STRING &&
      node->left()->type()->name() == basic_type::TYPE_INT)
    throw std::string("can't sub or compare strings with integers");
  
  else if (node->left()->type()->name() == basic_type::TYPE_STRING &&
      node->right()->type()->name() == basic_type::TYPE_DOUBLE)
    throw std::string("can't sub or compare strings with doubles");
      
   else if (node->right()->type()->name() == basic_type::TYPE_STRING &&
      node->left()->type()->name() == basic_type::TYPE_DOUBLE)
    throw std::string("can't sub or compare strings with doubles");
  
  else if (node->left()->type()->name() == basic_type::TYPE_STRING &&
      node->right()->type()->name() == basic_type::TYPE_STRING)
    throw std::string("can't sub or compare strings");
}

inline void zu::type_checker::processPIDSBinaryExpression(zu::assignment_node * const node, int lvl) {
  ASSERT_UNSPEC;
  
  node->lvalue()->accept(this, lvl + 2);
  node->rvalue()->accept(this, lvl + 2);
  
  if (node->lvalue()->type()->name() == basic_type::TYPE_INT &&
      node->rvalue()->type()->name() == basic_type::TYPE_INT) {
    node->type(new basic_type(4, basic_type::TYPE_INT));
  }
      
  else if (node->lvalue()->type()->name() == basic_type::TYPE_DOUBLE &&
      node->rvalue()->type()->name() == basic_type::TYPE_DOUBLE) {
    node->type(new basic_type(8, basic_type::TYPE_DOUBLE));
  }
  
  else if (node->lvalue()->type()->name() == basic_type::TYPE_POINTER &&
      node->rvalue()->type()->name() == basic_type::TYPE_POINTER) {
      
    node->type(new basic_type(4, basic_type::TYPE_POINTER));
  
    if (node->lvalue()->type()->_subtype->name() == basic_type::TYPE_INT) {
      node->type()->_subtype = new basic_type(4, basic_type::TYPE_INT);
      _types.push(node->type()->_subtype);
    }
    else if (node->lvalue()->type()->_subtype->name() == basic_type::TYPE_DOUBLE) {
      node->type()->_subtype = new basic_type(8, basic_type::TYPE_DOUBLE);
      _types.push(node->type()->_subtype);
    }
    else if (node->lvalue()->type()->_subtype->name() == basic_type::TYPE_STRING) {
      node->type()->_subtype = new basic_type(4, basic_type::TYPE_STRING);
      _types.push(node->type()->_subtype);
    }
    else if (node->lvalue()->type()->_subtype->name() == basic_type::TYPE_POINTER) {
      node->type()->_subtype = new basic_type(4, basic_type::TYPE_POINTER);
      _types.push(node->type()->_subtype);
    }
  }
  
  else if (node->lvalue()->type()->name() == basic_type::TYPE_STRING &&
      node->rvalue()->type()->name() == basic_type::TYPE_STRING)
    node->type(new basic_type(4, basic_type::TYPE_STRING));
 
  else if (node->lvalue()->type()->name() == basic_type::TYPE_POINTER &&
      node->rvalue()->type()->name() == basic_type::TYPE_INT) {
      
    node->type(new basic_type(4, basic_type::TYPE_POINTER));
      node->type(new basic_type(4, basic_type::TYPE_POINTER));
  
    if (node->lvalue()->type()->_subtype->name() == basic_type::TYPE_INT) {
      node->type()->_subtype = new basic_type(4, basic_type::TYPE_INT);
      _types.push(node->type()->_subtype);
    }
    else if (node->lvalue()->type()->_subtype->name() == basic_type::TYPE_DOUBLE) {
      node->type()->_subtype = new basic_type(8, basic_type::TYPE_DOUBLE);
      _types.push(node->type()->_subtype);
    }
    else if (node->lvalue()->type()->_subtype->name() == basic_type::TYPE_STRING) {
      node->type()->_subtype = new basic_type(4, basic_type::TYPE_STRING);
      _types.push(node->type()->_subtype);
    }
    else if (node->lvalue()->type()->_subtype->name() == basic_type::TYPE_POINTER) {
      node->type()->_subtype = new basic_type(4, basic_type::TYPE_POINTER);
      _types.push(node->type()->_subtype);
    }
  }
  
  else if (node->rvalue()->type()->name() == basic_type::TYPE_POINTER &&
      node->lvalue()->type()->name() == basic_type::TYPE_INT) 
    node->type(new basic_type(4, basic_type::TYPE_POINTER));
  
  else if (node->rvalue()->type()->name() == basic_type::TYPE_INT &&
      node->lvalue()->type()->name() == basic_type::TYPE_DOUBLE)
    throw std::string("can't assign expressions of different types: int != double");
  
  else if (node->lvalue()->type()->name() == basic_type::TYPE_INT &&
      node->rvalue()->type()->name() == basic_type::TYPE_DOUBLE)
    throw std::string("can't assign expressions of different types: int != double");
  
  else if (node->lvalue()->type()->name() == basic_type::TYPE_POINTER &&
      node->rvalue()->type()->name() == basic_type::TYPE_DOUBLE)
    throw std::string("can't assign expressions of different types: pointer != double");

  else if (node->rvalue()->type()->name() == basic_type::TYPE_POINTER &&
      node->lvalue()->type()->name() == basic_type::TYPE_DOUBLE)
    throw std::string("can't assign expressions of different types: pointer != double");
  
  else if (node->lvalue()->type()->name() == basic_type::TYPE_POINTER &&
      node->rvalue()->type()->name() == basic_type::TYPE_STRING)
    throw std::string("can't assign expressions of different types: pointer != string");
  
  else if (node->rvalue()->type()->name() == basic_type::TYPE_POINTER &&
      node->lvalue()->type()->name() == basic_type::TYPE_STRING)
    throw std::string("can't assign expressions of different types: pointer != string");
  
  else if (node->lvalue()->type()->name() == basic_type::TYPE_STRING &&
      node->rvalue()->type()->name() == basic_type::TYPE_INT)
    throw std::string("can't assign expressions of different types: int != string");
  
  else if (node->rvalue()->type()->name() == basic_type::TYPE_STRING &&
      node->lvalue()->type()->name() == basic_type::TYPE_INT)
    throw std::string("can't assign expressions of different types: int != string");
  
  else if (node->lvalue()->type()->name() == basic_type::TYPE_STRING &&
      node->rvalue()->type()->name() == basic_type::TYPE_DOUBLE)
    throw std::string("can't assign expressions of different types: double != string");
      
   else if (node->rvalue()->type()->name() == basic_type::TYPE_STRING &&
      node->lvalue()->type()->name() == basic_type::TYPE_DOUBLE)
    throw std::string("can't assign expressions of different types: double != string");
}

void zu::type_checker::do_and_node(zu::and_node * const node, int lvl) {
  processIBinaryExpression(node, lvl);
}

void zu::type_checker::do_or_node(zu::or_node * const node, int lvl) {
  processIBinaryExpression(node, lvl);
}

void zu::type_checker::do_add_node(cdk::add_node * const node, int lvl) {
  processPIDBinaryExpression(node, lvl);
}
void zu::type_checker::do_sub_node(cdk::sub_node * const node, int lvl) {
  processPPIDBinaryExpression(node, lvl);
}
void zu::type_checker::do_mul_node(cdk::mul_node * const node, int lvl) {
  processIDBinaryExpression(node, lvl);
}
void zu::type_checker::do_div_node(cdk::div_node * const node, int lvl) {
  processIDBinaryExpression(node, lvl);
}
void zu::type_checker::do_mod_node(cdk::mod_node * const node, int lvl) {
  processIBinaryExpression(node, lvl);
}
void zu::type_checker::do_lt_node(cdk::lt_node * const node, int lvl) {
  processIDBinaryExpression(node, lvl);
}
void zu::type_checker::do_le_node(cdk::le_node * const node, int lvl) {
  processIDBinaryExpression(node, lvl);
}
void zu::type_checker::do_ge_node(cdk::ge_node * const node, int lvl) {
  processIDBinaryExpression(node, lvl);
}
void zu::type_checker::do_gt_node(cdk::gt_node * const node, int lvl) {
  processIDBinaryExpression(node, lvl);
}
void zu::type_checker::do_ne_node(cdk::ne_node * const node, int lvl) {
  processPPIDBinaryExpression(node, lvl);
}
void zu::type_checker::do_eq_node(cdk::eq_node * const node, int lvl) {
  processPPIDBinaryExpression(node, lvl);
}

//---------------------------------------------------------------------------

void zu::type_checker::do_rvalue_node(zu::rvalue_node * const node, int lvl) {
  node->lvalue()->accept(this, lvl);
  node->type(node->lvalue()->type());
}

//---------------------------------------------------------------------------

void zu::type_checker::do_lvalue_node(zu::lvalue_node * const node, int lvl) {

}

//---------------------------------------------------------------------------

void zu::type_checker::do_assignment_node(zu::assignment_node * const node, int lvl) {
  ASSERT_UNSPEC;
  processPIDSBinaryExpression(node, lvl);
}

//---------------------------------------------------------------------------

void zu::type_checker::do_index_node(zu::index_node * const node, int lvl) {
  node->base()->accept(this, lvl + 2);
  node->value()->accept(this, lvl + 2);
  
  if (node->base()->type()->name() != basic_type::TYPE_POINTER)
    throw std::string("index: left side must be a pointer");
  if (node->value()->type()->name() != basic_type::TYPE_INT) 
    throw std::string("index must be an integer");
}

void zu::type_checker::do_allocation_node(zu::allocation_node * const node, int lvl) {
  node->size()->accept(this, lvl + 2);
  //node->type(new basic_type(4, basic_type::TYPE_POINTER));
  if (node->size()->type()->name() != basic_type::TYPE_INT)
    throw std::string("allocation: size must be an integer");
  //node->type()->_subtype = new basic_type(4, basic_type::TYPE_INT);
}

void zu::type_checker::do_address_node(zu::address_node * const node, int lvl) {
  if (isLeftValue(node) == false)
    throw std::string("only left values can return their address");
}

//---------------------------------------------------------------------------

void zu::type_checker::do_function_call_node(zu::function_call_node * const node, int lvl) {
  std::string id = *node->name();
  std::shared_ptr<zu::symbol> symbol = _symtab.find(id);
  
  if (symbol == nullptr) {
    throw std::string(id + " undeclared");
  }
  else {
    node->type(symbol->type());
  } 
}

void zu::type_checker::do_function_declaration_node(zu::function_declaration_node * const node, int lvl) {
  
}

void zu::type_checker::do_function_definition_node(zu::function_definition_node * const node, int lvl) {
  if (node->args() != nullptr) 
    node->args()->accept(this, lvl);
  if (node->def_value() != nullptr)
    node->def_value()->accept(this, lvl);
}

void zu::type_checker::do_return_node(zu::return_node * const node, int lvl) {
  
}

void zu::type_checker::do_evaluation_node(zu::evaluation_node * const node, int lvl) {
  node->argument()->accept(this, lvl + 2);
}

void zu::type_checker::do_print_node(zu::print_node * const node, int lvl) {
  if (node->argument() != nullptr)
    node->argument()->accept(this, lvl + 2);

}  
    
void zu::type_checker::do_print_line_node(zu::print_line_node * const node, int lvl) {
  if (node->argument() != nullptr)
    node->argument()->accept(this, lvl + 2);
}

//---------------------------------------------------------------------------

void zu::type_checker::do_read_node(zu::read_node * const node, int lvl) {
  
}

//---------------------------------------------------------------------------

void zu::type_checker::do_for_node(zu::for_node * const node, int lvl) {
  if (node->init() != nullptr) 
    node->init()->accept(this, lvl + 4);
  if (node->cond() != nullptr)
    node->cond()->accept(this, lvl + 4);
  if (node->incr() != nullptr)
    node->incr()->accept(this, lvl + 4);
}

//---------------------------------------------------------------------------

void zu::type_checker::do_if_node(zu::if_node * const node, int lvl) {
  node->condition()->accept(this, lvl + 4);
}

void zu::type_checker::do_if_else_node(zu::if_else_node * const node, int lvl) {
  node->condition()->accept(this, lvl + 4);
}

void zu::type_checker::do_break_node(zu::break_node * const node, int lvl) {
  
}

void zu::type_checker::do_continue_node(zu::continue_node * const node, int lvl) {
  
}

void zu::type_checker::do_block_node(zu::block_node * const node, int lvl) {
  
}
