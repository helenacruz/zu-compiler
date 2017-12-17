// $Id: symbol.h,v 1.4 2016/05/19 17:38:16 ist178190 Exp $ -*- c++ -*-
#ifndef __ZU_SEMANTICS_SYMBOL_H__
#define __ZU_SEMANTICS_SYMBOL_H__

#include <string>
#include <cdk/basic_type.h>

namespace zu {

    class symbol {
      basic_type *_type;
      std::string _name;
      cdk::sequence_node *_args;
      bool _isFunction;
      bool _isDefined;
      size_t _argsNr;
      int _offset;

    public:
      inline symbol(basic_type *type, const std::string &name, 
          cdk::sequence_node *args, bool isFunction, bool isDefined, size_t argsNr, int offset) :
          _type(type), _name(name), _args(args), _isFunction(isFunction),
          _isDefined(isDefined), _argsNr(argsNr), _offset(offset) {
      }

      virtual ~symbol() {
        delete _type;
      }

      inline basic_type *type() {
        return _type;
      }
      inline const std::string &name() const {
        return _name;
      }
      inline cdk::sequence_node *args() const {
        return _args;
      }
      inline size_t argsNr() const {
        return _argsNr;
      }
      inline bool isFunction() const {
        return _isFunction;
      }
      inline bool isDefined() const {
        return _isDefined;
      }
      inline int offset() {
        return _offset;
      }
    };

} // zu

#endif
