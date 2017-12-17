%option c++ yyclass="zu_scanner" outfile="zu_scanner.cpp"
%option stack noyywrap yylineno 8bit
%option debug warn
%{ 
/* $Id: zu_scanner.l,v 1.11 2016/05/18 10:18:43 ist178190 Exp $ */
// make relevant includes before including the parser's tab file
#include <string>
#include <cdk/ast/sequence_node.h>
#include <cdk/ast/expression_node.h>
#include "zu_scanner.h"
#include "zu_parser.tab.h"

// don't change this
#define yyerror LexerError
%}

INTEGER                0|([1-9][0-9]*)

HEXA_DIG               [0-9a-fA-F]
HEXA_NUM               0[xX]{HEXA_DIG}+

SCI_NOT                [eE][0-9]+

DOUBLE1                [0-9]+\.[0-9]*({SCI_NOT})?
DOUBLE2                [0-9]*\.[0-9]+({SCI_NOT})?
DOUBLE3                [0-9]*{SCI_NOT}

DOUBLE                 {DOUBLE1}|{DOUBLE2}|{DOUBLE3}

%x X_STRING X_COMMENT X_SPECIAL
%%

"//".*$                ; /* ignore comments */

"/*"                   yy_push_state(X_COMMENT);
<X_COMMENT>"/*"        yy_push_state(X_COMMENT);
<X_COMMENT>"*/"        yy_pop_state();
<X_COMMENT>.|\n        ;   

\"                     yy_push_state(X_STRING); yylval.s = new std::string();
<X_STRING>\"           yy_pop_state(); return tSTRING;
<X_STRING>\\           yy_push_state(X_SPECIAL);
<X_STRING>.            *yylval.s += yytext;

<X_SPECIAL>t           { yy_pop_state(); /* *yylval.s += std::string(1, 9); */ }
<X_SPECIAL>n           { yy_pop_state(); /* *yylval.s += std::string(1, 10); */ }
<X_SPECIAL>r           { yy_pop_state(); /* *yylval.s += std::string(1, 13); */ }
<X_SPECIAL>\"          { yy_pop_state(); /* *yylval.s += std::string(1, 34); */ }
<X_SPECIAL>\\          { yy_pop_state(); /* *yylval.s += std::string(1, 92); */ }
<X_SPECIAL>{HEXA_DIG}{1,2}   yy_pop_state(); *yylval.s += std::string(1, strtol(yytext, nullptr, 16));
<X_SPECIAL>.|\n        yyerror("Unknown special character");

">="                   return tGE; 
"<="                   return tLE; 
"=="                   return tEQ; 
"!="                   return tNE;
"<>"                   return tCONTINUE;
"><"                   return tBREAK;
"!!!"                  return tRETURN;
"!!"                   return tPRINTLN;

[A-Za-z_][A-Za-z0-9_]*  yylval.s = new std::string(yytext); return tIDENTIFIER;

{DOUBLE}                { yylval.d = strtod(yytext, nullptr); return tDOUBLE;
                          if (errno == ERANGE)
                             yyerror("Lexical error: double overflow");
                        }

{INTEGER}               { yylval.i = strtol(yytext, nullptr, 10); return tINTEGER;
                          if (errno == ERANGE)
                             yyerror("Lexical error: integer overflow");
                        }
                       
{HEXA_NUM}              { yylval.i = strtol(yytext, nullptr, 16); return tINTEGER;
                          if (errno == ERANGE)
                             yyerror("Lexical error: hexadecimal overflow");
                        }

[%#$]                   return *yytext;
[-+*/=]                 return *yytext;
[;,:!?]                 return *yytext;
[()<>{}[\]\]\[]         return *yytext;
[&|~@#]                 return *yytext;

[ \t\n\r]+              ; /* ignore whitespace */

.                       { std::cout << "Unknown character " << yytext << "on line " << yylineno << std::endl;
                          yyerror(yytext);
                        }

%%
// Very, very dirty hack: flex is a mess generating C++ scanners.
int zu_scanner::yywrap() { return 1; }
