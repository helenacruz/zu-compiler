// $Id: postfix_writer.cpp,v 1.35 2016/05/20 12:00:32 ist178190 Exp $ -*- c++ -*-
#include <string>
#include <sstream>
#include "targets/type_checker.h"
#include "targets/postfix_writer.h"
#include "targets/size_counter.h"
#include "ast/all.h"  // all.h is automatically generated

// //---------------------------------------------------------------------------
//     THIS IS THE VISITOR'S DEFINITION
//---------------------------------------------------------------------------

void zu::postfix_writer::do_sequence_node(cdk::sequence_node * const node, int lvl) {
  CHECK_TYPES(_compiler, _symtab, node);
  for (size_t i = 0; i < node->size(); i++) {
    if (node->node(i) != nullptr) {
      node->node(i)->accept(this, lvl);
    }
  }
}

//---------------------------------------------------------------------------
void zu::postfix_writer::do_variable_declaration_node(zu::variable_declaration_node * const node, int lvl) {
  CHECK_TYPES(_compiler, _symtab, node);
  int lbl1 = ++_lbl;
  int off = 0;
  std::string name = *node->name();
  
  if (node->isExtern() == true)
    return;

  if (_insideFunction == false) {
    if (node->type()->name() == basic_type::TYPE_INT) {
      if (node->value() == nullptr) {
        _pf.BSS();
        _pf.ALIGN();
        _pf.LABEL(name);
        _pf.BYTE(4);
      }
      else {
        _pf.DATA();
        _pf.ALIGN();
        _pf.LABEL(name);
        node->value()->accept(this, lvl);
      }
    }
    if (node->type()->name() == basic_type::TYPE_DOUBLE) {
      if (node->value() == nullptr) {
        _pf.BSS();
        _pf.ALIGN();
        _pf.BYTE(8);
        _pf.LABEL(name);
      }
      else {
        _pf.DATA();
        _pf.ALIGN();
        _pf.LABEL(name);
        node->value()->accept(this, lvl);
      }
    }
    if (node->type()->name() == basic_type::TYPE_POINTER) {
      if (node->value() == nullptr) {
        _pf.BSS();
        _pf.ALIGN();
        _pf.BYTE(4);
        _pf.LABEL(name);
      }
      else {
        _pf.DATA();
        _pf.ALIGN();
        _pf.LABEL(name);
        node->value()->accept(this, lvl);
      }
    }
    if (node->type()->name() == basic_type::TYPE_STRING) {
      if (node->value() == nullptr) {
        _pf.BSS();
        _pf.ALIGN();
        _pf.BYTE(4);
        _pf.LABEL(name);
      }
      else {
        _pf.RODATA();
        _pf.ALIGN();
        _pf.LABEL(mklbl(lbl1));
        node->value()->accept(this, lvl);
        _pf.DATA();
        _pf.ALIGN();
        _pf.LABEL(name);
        _pf.ID(mklbl(lbl1));
      }
    }
  }
  else if (_isArg == false && _insideFunction == true) {
    if (node->type()->name() == basic_type::TYPE_INT) {
      if (node->value() == nullptr) {
        _pf.INT(0);
        _pf.LOCA(_offset);
        off = -4;
      }
      else {
        node->value()->accept(this, lvl);
       _pf.LOCA(_offset);
        off = -4;
      }
    }
    else if (node->type()->name() == basic_type::TYPE_DOUBLE) {
      _pf.RODATA();
      _pf.ALIGN();
      _pf.LABEL(mklbl(lbl1 = ++_lbl));
      if (node->value() == nullptr) {
        _pf.DOUBLE(0);
        _pf.TEXT();
        _pf.ALIGN();
        _pf.ADDR(mklbl(lbl1 = ++_lbl));
        _pf.DLOAD();
        _pf.LOCAL(_offset);
        _pf.DSTORE();
        off = -8;
      }
      else {
        node->value()->accept(this, lvl);
        _pf.TEXT();
        _pf.ALIGN();
        _pf.ADDR(mklbl(lbl1 = ++_lbl));
        _pf.DLOAD();
        _pf.LOCAL(_offset);
        _pf.DSTORE();
        off = -8;    
      }
    }
    
    else if (node->type()->name() == basic_type::TYPE_STRING) {
      _pf.RODATA();
      _pf.ALIGN();
      _pf.LABEL(mklbl(lbl1 = ++_lbl));
      if (node->value() == nullptr) {
        _pf.STR("");
        _pf.TEXT();
        _pf.ALIGN();
        _pf.ADDR(mklbl(lbl1));
        _pf.LOCA(_offset);
        off = -4;
      }
      else {
        node->value()->accept(this, lvl);
        _pf.TEXT();
        _pf.ALIGN();
        _pf.ADDR(mklbl(lbl1));
        _pf.LOCA(_offset);
        off = -4;
      }
    }
    
    else if (node->type()->name() == basic_type::TYPE_POINTER) {
      if (node->value() == nullptr) {
        _pf.DATA();
        _pf.ALIGN();
        _pf.LOCA(_offset);
        off = -4;
      }
      else {
        _pf.DATA();
        _pf.ALIGN();
        node->value()->accept(this, lvl);
      }
    }
  }
  else {
      off = 4;
      if (node->type()->name() == basic_type::TYPE_DOUBLE)
        off = 8;
  }

  _symtab.insert(*node->name(), std::make_shared<zu::symbol>(node->type(), *node->name(), nullptr, false, false, 0, _offset));
  _offset += off;
}

void zu::postfix_writer::do_variable_node(zu::variable_node * const node, int lvl) {
  CHECK_TYPES(_compiler, _symtab, node);
  const std::string name = *node->name();
  std::shared_ptr<zu::symbol> symbol = _symtab.find(name);
  
  if (symbol == nullptr)
    throw name + "undeclared";
  
  if (symbol->isFunction() == false) {
    if (symbol->offset() == 0) {
      _pf.ADDR(name);
    }
    else {
      _pf.LOCAL(symbol->offset());
    }
  }
}

void zu::postfix_writer::do_integer_node(cdk::integer_node * const node, int lvl) {
  if (_insideFunction == false) {
    _pf.CONST(node->value()); // memory
  }
  else {
    _pf.INT(node->value()); // stack
  }
}

void zu::postfix_writer::do_double_node(cdk::double_node * const node, int lvl) {
  int lbl1 = ++_lbl;
  if (_insideFunction == false) {
    _pf.RODATA();
    _pf.ALIGN();
    _pf.LABEL(mklbl(lbl1));
    _pf.DOUBLE(node->value());
    _pf.TEXT();
    _pf.ADDR(mklbl(lbl1));
  }
  else {
    _pf.RODATA();
    _pf.ALIGN();
    _pf.LABEL(mklbl(lbl1));
    _pf.DOUBLE(node->value());
    _pf.TEXT();
    _pf.ADDR(mklbl(lbl1));
    _pf.DLOAD();
  }
}

void zu::postfix_writer::do_string_node(cdk::string_node * const node, int lvl) {
  int lbl1;

  if (_insideFunction == true) {
    /* generate the string */
    _pf.RODATA(); // strings are DATA readonly
    _pf.ALIGN(); // make sure we are aligned
    _pf.LABEL(mklbl(lbl1 = ++_lbl)); // give the string a name
    _pf.STR(node->value()); // output string characters

    /* leave the address on the stack */
    _pf.TEXT(); // return to the TEXT segment
    _pf.ADDR(mklbl(lbl1)); // the string to be printed
  }
  else {
    _pf.STR(node->value());
  }
}

//---------------------------------------------------------------------------

void zu::postfix_writer::do_neg_node(cdk::neg_node * const node, int lvl) {
  CHECK_TYPES(_compiler, _symtab, node);
  node->argument()->accept(this, lvl); // determine the value
  if (node->argument()->type()->name() == basic_type::TYPE_DOUBLE) {
    _pf.DNEG();
  }
  else {
    _pf.NEG();
  }
}

void zu::postfix_writer::do_identity_node(zu::identity_node * const node, int lvl) {
  CHECK_TYPES(_compiler, _symtab, node);
  node->argument()->accept(this, lvl + 2);
}

void zu::postfix_writer::do_not_node(zu::not_node * const node, int lvl) {
  CHECK_TYPES(_compiler, _symtab, node);
  node->argument()->accept(this, lvl + 2);
  _pf.NOT();
}

//---------------------------------------------------------------------------

void zu::postfix_writer::do_and_node(zu::and_node * const node, int lvl) {
  int end = ++_lbl;
  CHECK_TYPES(_compiler, _symtab, node);  
  node->left()->accept(this, lvl);
  _pf.JZ(mklbl(end));
  node->right()->accept(this, lvl);
  _pf.AND();  
  _pf.LABEL(mklbl(end));
}

void zu::postfix_writer::do_or_node(zu::or_node * const node, int lvl) {
  int end = ++_lbl;
  CHECK_TYPES(_compiler, _symtab, node);
  node->left()->accept(this, lvl);
  _pf.JNZ(mklbl(end));
  node->right()->accept(this, lvl);
  _pf.OR(); 
  _pf.LABEL(mklbl(end));
}

void zu::postfix_writer::do_add_node(cdk::add_node * const node, int lvl) {
  CHECK_TYPES(_compiler, _symtab, node);
  if (node->type()->name() == basic_type::TYPE_INT) {
    node->left()->accept(this, lvl);
    node->right()->accept(this, lvl);
    _pf.ADD();
  }
  else if (node->left()->type()->name() == basic_type::TYPE_DOUBLE &&
      node->right()->type()->name() == basic_type::TYPE_INT) {
    node->right()->accept(this, lvl);
    _pf.I2D();
    node->left()->accept(this, lvl);
    _pf.DADD();
  }
  else if (node->right()->type()->name() == basic_type::TYPE_DOUBLE &&
      node->left()->type()->name() == basic_type::TYPE_INT) {
    node->left()->accept(this, lvl);
    _pf.I2D();
    node->right()->accept(this, lvl);
    _pf.DADD();
  }
  else if (node->right()->type()->name() == basic_type::TYPE_DOUBLE &&
      node->left()->type()->name() == basic_type::TYPE_DOUBLE) {
    node->left()->accept(this, lvl);
    node->right()->accept(this, lvl);
    _pf.DADD();
  }
  else if (node->left()->type()->name() == basic_type::TYPE_POINTER &&
      node->right()->type()->name() == basic_type::TYPE_INT) {
    node->left()->accept(this, lvl);
    node->right()->accept(this, lvl);
    _pf.INT(4);
    _pf.MUL();
    _pf.ADD();
  }
  else if (node->left()->type()->name() == basic_type::TYPE_INT &&
      node->right()->type()->name() == basic_type::TYPE_POINTER) {
    node->right()->accept(this, lvl);
    node->left()->accept(this, lvl);
    _pf.INT(4);
    _pf.MUL();
    _pf.ADD();
  }
}
void zu::postfix_writer::do_sub_node(cdk::sub_node * const node, int lvl) {
  CHECK_TYPES(_compiler, _symtab, node);
  if (node->left()->type()->name() == basic_type::TYPE_INT &&
      node->right()->type()->name() == basic_type::TYPE_INT) {
    node->left()->accept(this, lvl);
    node->right()->accept(this, lvl);
    _pf.SUB();
  }
  else if (node->left()->type()->name() == basic_type::TYPE_DOUBLE &&
      node->right()->type()->name() == basic_type::TYPE_INT) {
    node->left()->accept(this, lvl);
    node->right()->accept(this, lvl);
    _pf.I2D();
    _pf.DSUB();
  }
  else if (node->right()->type()->name() == basic_type::TYPE_DOUBLE &&
      node->left()->type()->name() == basic_type::TYPE_INT) {
    node->left()->accept(this, lvl);
    _pf.I2D();
    node->right()->accept(this, lvl);
    _pf.DSUB();
  }
  else if (node->right()->type()->name() == basic_type::TYPE_DOUBLE &&
      node->left()->type()->name() == basic_type::TYPE_DOUBLE) {
    node->left()->accept(this, lvl);
    node->right()->accept(this, lvl);
    _pf.DSUB();
  }
  else if (node->left()->type()->name() == basic_type::TYPE_POINTER &&
      node->right()->type()->name() == basic_type::TYPE_INT) {
    node->left()->accept(this, lvl);
    node->right()->accept(this, lvl);
    _pf.INT(4);
    _pf.MUL();
    _pf.SUB();
  }
  else if (node->left()->type()->name() == basic_type::TYPE_POINTER &&
      node->right()->type()->name() == basic_type::TYPE_POINTER) {
    node->left()->accept(this, lvl);
    node->right()->accept(this, lvl);
    _pf.SUB();
  }
}
void zu::postfix_writer::do_mul_node(cdk::mul_node * const node, int lvl) {
  CHECK_TYPES(_compiler, _symtab, node);
  if (node->left()->type()->name() == basic_type::TYPE_INT &&
      node->right()->type()->name() == basic_type::TYPE_INT) {
    node->left()->accept(this, lvl);
    node->right()->accept(this, lvl);
    _pf.MUL();
  }
  else if (node->left()->type()->name() == basic_type::TYPE_DOUBLE &&
      node->right()->type()->name() == basic_type::TYPE_INT) {
    node->right()->accept(this, lvl);
    _pf.I2D();
    node->left()->accept(this, lvl);
    _pf.DMUL();
  }
  else if (node->right()->type()->name() == basic_type::TYPE_DOUBLE &&
      node->left()->type()->name() == basic_type::TYPE_INT) {
    node->left()->accept(this, lvl);
    _pf.I2D();
    node->right()->accept(this, lvl);
    _pf.DMUL();
  }
  else if (node->right()->type()->name() == basic_type::TYPE_DOUBLE &&
      node->left()->type()->name() == basic_type::TYPE_DOUBLE) {
    node->left()->accept(this, lvl);
    node->right()->accept(this, lvl);
    _pf.DMUL();
  }
}
void zu::postfix_writer::do_div_node(cdk::div_node * const node, int lvl) {
  CHECK_TYPES(_compiler, _symtab, node);
  if (node->left()->type()->name() == basic_type::TYPE_INT &&
      node->right()->type()->name() == basic_type::TYPE_INT) {
    node->left()->accept(this, lvl);
    node->right()->accept(this, lvl);
    _pf.DIV();
  }
  else if (node->left()->type()->name() == basic_type::TYPE_DOUBLE &&
      node->right()->type()->name() == basic_type::TYPE_INT) {
    node->left()->accept(this, lvl);
    node->right()->accept(this, lvl);
    _pf.I2D();
    _pf.DDIV();
  }
  else if (node->right()->type()->name() == basic_type::TYPE_DOUBLE &&
      node->left()->type()->name() == basic_type::TYPE_INT) {
    node->left()->accept(this, lvl);
    _pf.I2D();
    node->right()->accept(this, lvl);
    _pf.DDIV();
  }
  else if (node->right()->type()->name() == basic_type::TYPE_DOUBLE &&
      node->left()->type()->name() == basic_type::TYPE_DOUBLE) {
    node->left()->accept(this, lvl);
    node->right()->accept(this, lvl);
    _pf.DDIV();
  }
}
void zu::postfix_writer::do_mod_node(cdk::mod_node * const node, int lvl) {
  CHECK_TYPES(_compiler, _symtab, node);
  node->left()->accept(this, lvl);
  node->right()->accept(this, lvl);
  _pf.MOD();
}
void zu::postfix_writer::do_lt_node(cdk::lt_node * const node, int lvl) {
  CHECK_TYPES(_compiler, _symtab, node);
  if (node->left()->type()->name() == basic_type::TYPE_INT &&
      node->right()->type()->name() == basic_type::TYPE_INT) {
    node->left()->accept(this, lvl);
    node->right()->accept(this, lvl);
    _pf.LT();
  }
  else if (node->left()->type()->name() == basic_type::TYPE_DOUBLE &&
      node->right()->type()->name() == basic_type::TYPE_INT) {
    node->left()->accept(this, lvl);
    node->right()->accept(this, lvl);
    _pf.I2D();
    _pf.DCMP();
    _pf.INT(0);
    _pf.LT();
  }
  else if (node->right()->type()->name() == basic_type::TYPE_DOUBLE &&
      node->left()->type()->name() == basic_type::TYPE_INT) {
    node->left()->accept(this, lvl);
    _pf.I2D();
    node->right()->accept(this, lvl);
    _pf.DCMP();
    _pf.INT(0);
    _pf.LT();
  }
  else if (node->right()->type()->name() == basic_type::TYPE_DOUBLE &&
      node->left()->type()->name() == basic_type::TYPE_INT) {
    node->left()->accept(this, lvl);
    node->right()->accept(this, lvl);
    _pf.DCMP();
    _pf.INT(0);
    _pf.LT();
  }
}
void zu::postfix_writer::do_le_node(cdk::le_node * const node, int lvl) {
  CHECK_TYPES(_compiler, _symtab, node);
  if (node->left()->type()->name() == basic_type::TYPE_INT &&
      node->right()->type()->name() == basic_type::TYPE_INT) {
    node->left()->accept(this, lvl);
    node->right()->accept(this, lvl);
    _pf.LE();
  }
  else if (node->left()->type()->name() == basic_type::TYPE_DOUBLE &&
      node->right()->type()->name() == basic_type::TYPE_INT) {
    _pf.I2D();
    node->left()->accept(this, lvl);
    node->right()->accept(this, lvl);
    _pf.DCMP();
    _pf.INT(0);
    _pf.LE();
  }
  else if (node->right()->type()->name() == basic_type::TYPE_DOUBLE &&
      node->left()->type()->name() == basic_type::TYPE_INT) {
    node->left()->accept(this, lvl);
    _pf.I2D();
    node->right()->accept(this, lvl);
    _pf.DCMP();
    _pf.INT(0);
    _pf.LE();
  }
  else if (node->right()->type()->name() == basic_type::TYPE_DOUBLE &&
      node->left()->type()->name() == basic_type::TYPE_INT) {
    node->left()->accept(this, lvl);
    node->right()->accept(this, lvl);
    _pf.DCMP();
    _pf.INT(0);
    _pf.LE();
  }
}

void zu::postfix_writer::do_ge_node(cdk::ge_node * const node, int lvl) {
  CHECK_TYPES(_compiler, _symtab, node);
  node->left()->accept(this, lvl);
  node->right()->accept(this, lvl);
  if (node->type()->name() == basic_type::TYPE_INT) {
    node->left()->accept(this, lvl);
    node->right()->accept(this, lvl);
    _pf.GE();
  }
  else if (node->left()->type()->name() == basic_type::TYPE_DOUBLE &&
      node->right()->type()->name() == basic_type::TYPE_INT) {
    node->left()->accept(this, lvl);
    node->right()->accept(this, lvl);
    _pf.I2D();
    _pf.DCMP();
    _pf.INT(0);
    _pf.GE();
  }
  else if (node->left()->type()->name() == basic_type::TYPE_INT &&
      node->right()->type()->name() == basic_type::TYPE_DOUBLE) {
    node->left()->accept(this, lvl);
    _pf.I2D();
    node->right()->accept(this, lvl);
    _pf.DCMP();
    _pf.INT(0);
    _pf.GE();
  }
  else if (node->left()->type()->name() == basic_type::TYPE_DOUBLE &&
      node->right()->type()->name() == basic_type::TYPE_INT) {
    node->left()->accept(this, lvl);
    node->right()->accept(this, lvl);
    _pf.DCMP();
    _pf.INT(0);
    _pf.GE();
  }
}
void zu::postfix_writer::do_gt_node(cdk::gt_node * const node, int lvl) {
  CHECK_TYPES(_compiler, _symtab, node);
  if (node->left()->type()->name() == basic_type::TYPE_INT &&
      node->right()->type()->name() == basic_type::TYPE_INT) {
    node->left()->accept(this, lvl);
    node->right()->accept(this, lvl);
    _pf.GT();
  }
  else if (node->left()->type()->name() == basic_type::TYPE_DOUBLE &&
      node->right()->type()->name() == basic_type::TYPE_INT) {
    node->left()->accept(this, lvl);
    node->right()->accept(this, lvl);
    _pf.I2D();
    _pf.DCMP();
    _pf.INT(0);
    _pf.GT();
  }
  else if (node->right()->type()->name() == basic_type::TYPE_DOUBLE &&
      node->left()->type()->name() == basic_type::TYPE_INT) {
    node->left()->accept(this, lvl);
    _pf.I2D();
    node->right()->accept(this, lvl);
    _pf.DCMP();
    _pf.INT(0);
    _pf.GT();
  }
  else if (node->right()->type()->name() == basic_type::TYPE_DOUBLE &&
      node->left()->type()->name() == basic_type::TYPE_INT) {
    node->left()->accept(this, lvl);
    node->right()->accept(this, lvl);
    _pf.DCMP();
    _pf.INT(0);
    _pf.GT();
  }
}
void zu::postfix_writer::do_ne_node(cdk::ne_node * const node, int lvl) {
  CHECK_TYPES(_compiler, _symtab, node);
  if (node->left()->type()->name() == basic_type::TYPE_INT &&
      node->right()->type()->name() == basic_type::TYPE_INT) {
    node->left()->accept(this, lvl);
    node->right()->accept(this, lvl);
    _pf.NE();
  }
  else if (node->left()->type()->name() == basic_type::TYPE_DOUBLE &&
      node->right()->type()->name() == basic_type::TYPE_INT) {
    node->left()->accept(this, lvl);
    node->right()->accept(this, lvl);
    _pf.I2D();
    _pf.DCMP();
    _pf.INT(0);
    _pf.NE();
  }
  else if (node->right()->type()->name() == basic_type::TYPE_DOUBLE &&
      node->left()->type()->name() == basic_type::TYPE_INT) {
    node->left()->accept(this, lvl);
    _pf.I2D();
    node->right()->accept(this, lvl);
    _pf.DCMP();
    _pf.INT(0);
    _pf.NE();
  }
  else if (node->right()->type()->name() == basic_type::TYPE_DOUBLE &&
      node->left()->type()->name() == basic_type::TYPE_INT) {
    node->left()->accept(this, lvl);
    node->right()->accept(this, lvl);
    _pf.DCMP();
    _pf.INT(0);
    _pf.NE();
  }
}
void zu::postfix_writer::do_eq_node(cdk::eq_node * const node, int lvl) {
  CHECK_TYPES(_compiler, _symtab, node);
  if (node->left()->type()->name() == basic_type::TYPE_INT &&
      node->right()->type()->name() == basic_type::TYPE_INT) {
    node->left()->accept(this, lvl);
    node->right()->accept(this, lvl);
    _pf.EQ();
  }
  else if (node->left()->type()->name() == basic_type::TYPE_DOUBLE &&
      node->right()->type()->name() == basic_type::TYPE_INT) {
    node->left()->accept(this, lvl);
    node->right()->accept(this, lvl);
    _pf.I2D();
    _pf.DCMP();
    _pf.INT(0);
    _pf.EQ();
  }
  else if (node->right()->type()->name() == basic_type::TYPE_DOUBLE &&
      node->left()->type()->name() == basic_type::TYPE_INT) {
    node->left()->accept(this, lvl);
    _pf.I2D();
    node->right()->accept(this, lvl);
    _pf.DCMP();
    _pf.INT(0);
    _pf.EQ();
  }
  else if (node->right()->type()->name() == basic_type::TYPE_DOUBLE &&
      node->left()->type()->name() == basic_type::TYPE_INT) {
    node->left()->accept(this, lvl);
    node->right()->accept(this, lvl);
    _pf.DCMP();
    _pf.INT(0);
    _pf.EQ();
  }
}

//---------------------------------------------------------------------------

void zu::postfix_writer::do_rvalue_node(zu::rvalue_node * const node, int lvl) {
  CHECK_TYPES(_compiler, _symtab, node);
  node->lvalue()->accept(this, lvl);
  if (node->lvalue()->type()->name() == basic_type::TYPE_DOUBLE)
      _pf.DLOAD();
  else
    _pf.LOAD();
}

//---------------------------------------------------------------------------

void zu::postfix_writer::do_lvalue_node(zu::lvalue_node * const node, int lvl) {
  
}

//---------------------------------------------------------------------------

void zu::postfix_writer::do_assignment_node(zu::assignment_node * const node, int lvl) {
  CHECK_TYPES(_compiler, _symtab, node);  
  node->rvalue()->accept(this, lvl); // determine the new value
  
  zu::variable_node *v = (zu::variable_node *) node->lvalue();
  
  if (v->name()->compare(_functionName) == 0) {
    return;
  }
  else {
        
    if (node->type()->name() == basic_type::TYPE_DOUBLE)
        _pf.DDUP();
    else
        _pf.DUP();
        
    node->lvalue()->accept(this, lvl); // where to store the value
        
    if (node->type()->name() == basic_type::TYPE_DOUBLE)
        _pf.DSTORE(); // store the value at address
    else
        _pf.STORE();
    
   /* if (node->type()->name() == basic_type::TYPE_DOUBLE)
      _pf.TRASH(8);
    else
      _pf.TRASH(4);
  */
  }
}

//---------------------------------------------------------------------------

void zu::postfix_writer::do_index_node(zu::index_node * const node, int lvl) {
  CHECK_TYPES(_compiler, _symtab, node);
  node->base()->accept(this, lvl);
  _pf.LOAD();
  _pf.INT(4);
  _pf.MUL();
  _pf.ADD();
}

void zu::postfix_writer::do_allocation_node(zu::allocation_node * const node, int lvl) {
  CHECK_TYPES(_compiler, _symtab, node);
  _pf.INT(4);
  node->size()->accept(this, lvl);
  _pf.MUL();
  _pf.ALLOC();
  _pf.SP();
}

void zu::postfix_writer::do_address_node(zu::address_node * const node, int lvl) {
  CHECK_TYPES(_compiler, _symtab, node);
  node->argument()->accept(this, lvl);
}

//---------------------------------------------------------------------------

void zu::postfix_writer::do_function_call_node(zu::function_call_node * const node, int lvl) {
  CHECK_TYPES(_compiler, _symtab, node);
  
  std::string name = *node->name();
  cdk::sequence_node *args = node->args();
  zu::size_counter sc(_compiler, node, lvl);
  int size = sc.size();
  
  if (args != nullptr) {
    args->accept(this, lvl);
  }
  
  if (args != nullptr) {
    for (size_t i = 0; i < args->size(); i++) {
      // XXX assuming they're not doubles
      size += 4;
    }
  }
  
  _pf.CALL(name);

  _pf.TRASH(size);
  
  std::shared_ptr<zu::symbol> s = _symtab.find(name);
    
  if (s->type()->name() == basic_type::TYPE_DOUBLE)
    _pf.DPUSH();
  else
    _pf.PUSH();
}

void zu::postfix_writer::do_function_declaration_node(zu::function_declaration_node * const node, int lvl) {
  CHECK_TYPES(_compiler, _symtab, node); 
  int nrArgs = 0;
  std::shared_ptr<zu::symbol> symbol;
  
  if (node->args() != nullptr)
  nrArgs = node->args()->size();
  
  if (_symtab.find(*node->name()) == nullptr)
    _symtab.insert(*node->name(), std::make_shared<zu::symbol>(node->type(), *node->name(), node->args(), false, true, nrArgs, 0));
}

void zu::postfix_writer::do_function_definition_node(zu::function_definition_node * const node, int lvl) {
  CHECK_TYPES(_compiler, _symtab, node);
  std::string name;
  int retsize = 0;
  _insideFunction = true;
  _offset = 0;
  _functionName = *node->name();
  int nrArgs = 0;
  std::shared_ptr<zu::symbol> symbol;
  zu::size_counter sc(_compiler, node, lvl);
  int stack_size = sc.size();
  
  _pf.TEXT();
  _pf.ALIGN();
  
  if (node->def_value() != nullptr)
    retsize = node->def_value()->type()->size();
  
  if (node->name()->compare("zu") == 0) 
      name = "_main";
  else if (node->name()->compare("_main") == 0)
      name = "zu";
  else
      name = *node->name();
  
  if (node->args() != nullptr)
    nrArgs = node->args()->size();

  if ((symbol = _symtab.find_local(name)) != nullptr) {
    if (symbol->isDefined()) {
      std::cerr << name + " already defined" << std::endl;
    }
    else {
      _symtab.replace(*node->name(), std::make_shared<zu::symbol>(node->type(), *node->name(), node->args(), false, false, nrArgs, 0));
    }
  }
  else {
    _symtab.insert(*node->name(), std::make_shared<zu::symbol>(node->type(), *node->name(), node->args(), true, true, nrArgs, 0));
  }
  
  _symtab.push();
  
  _offset = 8;
  _isArg = true;
  if (node->args() != nullptr)
    node->args()->accept(this, lvl);
  _isArg = false;
  
  _offset = -4 - retsize;
  
  if (!node->isPrivate())
    _pf.GLOBAL(name, _pf.FUNC());
  
  _pf.LABEL(name);
  _pf.ENTER(stack_size + retsize);
  
  if (node->def_value() != nullptr) {
    node->def_value()->accept(this, lvl);
    _pf.LOCA(-retsize);
  }
  
  if (node->block() != nullptr) {
    node->block()->accept(this, lvl);
  }
    
  if (node->def_value() != nullptr) {
    node->def_value()->accept(this, lvl);
    _pf.LOCV(-retsize);
  }
  
  if (node->name()->compare("zu") == 0) {
    _pf.INT(0);
  }
  
  _pf.POP();
  _pf.LEAVE();
  _pf.RET();

  // these are just a few library function imports
  _pf.EXTERN("readi");
  _pf.EXTERN("printi");
  _pf.EXTERN("prints");
  _pf.EXTERN("println");
  _pf.EXTERN("argc");
  _pf.EXTERN("argv");
  
  _insideFunction = false;
  _symtab.pop();
}

void zu::postfix_writer::do_return_node(zu::return_node * const node, int lvl) {
  CHECK_TYPES(_compiler, _symtab, node);
  _pf.POP();
  _pf.LEAVE();
  _pf.RET();
}

//---------------------------------------------------------------------------

void zu::postfix_writer::do_evaluation_node(zu::evaluation_node * const node, int lvl) {
  CHECK_TYPES(_compiler, _symtab, node);
  node->argument()->accept(this, lvl); // determine the value
  if (node->argument()->type()->name() == basic_type::TYPE_INT) {
    _pf.TRASH(4); // delete the evaluated value
  }
  else if (node->argument()->type()->name() == basic_type::TYPE_STRING) {
    _pf.TRASH(4); // delete the evaluated value's address
  }
  else if (node->argument()->type()->name() == basic_type::TYPE_DOUBLE) {
    _pf.TRASH(8); // delete the evaluated value's address
  }
  else if (node->argument()->type()->name() == basic_type::TYPE_POINTER) {
    _pf.TRASH(4); // delete the evaluated value's address
  }
  else {
    std::cerr << "ERROR: CANNOT HAPPEN!" << std::endl;
    exit(1);
  }
}

void zu::postfix_writer::do_print_node(zu::print_node * const node, int lvl) {
  CHECK_TYPES(_compiler, _symtab, node);
  node->argument()->accept(this, lvl); // determine the value to print
  if (node->argument()->type()->name() == basic_type::TYPE_INT) {
    _pf.CALL("printi");
    _pf.TRASH(4); // delete the printed value
  }
  else if (node->argument()->type()->name() == basic_type::TYPE_STRING) {
    _pf.CALL("prints");
    _pf.TRASH(4); // delete the printed value's address
  }
  else if (node->argument()->type()->name() == basic_type::TYPE_DOUBLE) {
    _pf.CALL("printd");
    _pf.TRASH(8); // delete the printed value's address
  }
  else {
    std::cerr << "ERROR: CANNOT HAPPEN!" << std::endl;
    exit(1);
  }
}

void zu::postfix_writer::do_print_line_node(zu::print_line_node * const node, int lvl) {
  CHECK_TYPES(_compiler, _symtab, node);
  node->argument()->accept(this, lvl); // determine the value to print
  if (node->argument()->type()->name() == basic_type::TYPE_INT) {
    _pf.CALL("printi");
    _pf.TRASH(4); // delete the printed value
  }
  else if (node->argument()->type()->name() == basic_type::TYPE_STRING) {
    _pf.CALL("prints");
    _pf.TRASH(4); // delete the printed value's address
  }
  else if (node->argument()->type()->name() == basic_type::TYPE_DOUBLE) {
    _pf.CALL("printd");
    _pf.TRASH(8); // delete the printed value's address
  }
  else {
    std::cerr << "ERROR: CANNOT HAPPEN!" << std::endl;
    exit(1);
  }
  _pf.CALL("println"); // print a newline
}

//---------------------------------------------------------------------------

void zu::postfix_writer::do_read_node(zu::read_node * const node, int lvl) {
  CHECK_TYPES(_compiler, _symtab, node);
  if (node->type()->name() == basic_type::TYPE_INT) {
    _pf.CALL("readi");
    _pf.PUSH();
    _pf.STORE();
  } 
  else if (node->type()->name() == basic_type::TYPE_DOUBLE) {
    _pf.CALL("readd");
    _pf.DPUSH();
    _pf.DSTORE();
  }
}

//---------------------------------------------------------------------------

void zu::postfix_writer::do_for_node(zu::for_node * const node, int lvl) {
  CHECK_TYPES(_compiler, _symtab, node);
  int cond = ++_lbl;
  int incr = ++_lbl;
  int end = ++_lbl;
  
  std::string break_lbl = mklbl(end);
  std::string continue_lbl = mklbl(incr);
  
  _breaks.push(break_lbl);
  _continues.push(continue_lbl);
  
  _symtab.push();
  
  if (node->init() != nullptr)
    node->init()->accept(this, lvl);
  
  _pf.LABEL(mklbl(cond));
  if (node->cond() != nullptr) {
    node->cond()->accept(this, lvl);
    _pf.JZ(mklbl(end));
  }
  
  if (node->block() != nullptr)
    node->block()->accept(this, lvl);
  
  _pf.LABEL(mklbl(incr));
  if (node->incr() != nullptr)
    node->incr()->accept(this, lvl);
  
  _pf.JMP(mklbl(cond));
  _pf.LABEL(mklbl(end));
  
  _breaks.pop();
  _continues.pop();
  
  _symtab.pop();
}

//---------------------------------------------------------------------------

void zu::postfix_writer::do_if_node(zu::if_node * const node, int lvl) {
  CHECK_TYPES(_compiler, _symtab, node);
  int lbl1;
  if (node->condition() != nullptr) {
    node->condition()->accept(this, lvl);
  }
  _pf.JZ(mklbl(lbl1 = ++_lbl));
  if (node->block() != nullptr) {
    node->block()->accept(this, lvl + 2);
  }
  _pf.LABEL(mklbl(lbl1));
}

//---------------------------------------------------------------------------

void zu::postfix_writer::do_if_else_node(zu::if_else_node * const node, int lvl) {
  CHECK_TYPES(_compiler, _symtab, node);
  int lbl1, lbl2;
  if (node->condition() != nullptr) {
    node->condition()->accept(this, lvl);
  }
  _pf.JZ(mklbl(lbl1 = ++_lbl));
  if (node->thenblock() != nullptr) {
    node->thenblock()->accept(this, lvl + 2);
  }
  _pf.JMP(mklbl(lbl2 = ++_lbl));
  _pf.LABEL(mklbl(lbl1));
  if (node->elseblock() != nullptr) {
    node->elseblock()->accept(this, lvl + 2);
  }
  _pf.LABEL(mklbl(lbl1 = lbl2));
}

void zu::postfix_writer::do_break_node(zu::break_node * const node, int lvl) {
  CHECK_TYPES(_compiler, _symtab, node);
  std::string label = _breaks.top();
  _pf.JMP(label);
}

void zu::postfix_writer::do_continue_node(zu::continue_node * const node, int lvl) {
  CHECK_TYPES(_compiler, _symtab, node);
  std::string label = _continues.top();
  _pf.JMP(label);
}

void zu::postfix_writer::do_block_node(zu::block_node * const node, int lvl) {
  CHECK_TYPES(_compiler, _symtab, node);
  _symtab.push();
  if (node->decls() != nullptr)
    node->decls()->accept(this, lvl + 2);
  if (node->instrs() != nullptr)
    node->instrs()->accept(this, lvl + 2);
  _symtab.pop();
}
