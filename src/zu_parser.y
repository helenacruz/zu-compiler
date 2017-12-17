%{
// $Id: zu_parser.y,v 1.26 2016/05/19 23:43:55 ist178190 Exp $
//-- don't change *any* of these: if you do, you'll break the compiler.
#include <cdk/compiler.h>
#include "ast/all.h"
#define LINE       compiler->scanner()->lineno()
#define yylex()    compiler->scanner()->scan()
#define yyerror(s) compiler->scanner()->error(s)
#define YYPARSE_PARAM_TYPE std::shared_ptr<cdk::compiler>
#define YYPARSE_PARAM      compiler
//-- don't change *any* of these --- END!
%}

%union {
  int                  i;	/* integer value */
  double               d;
  std::string          *s;	/* symbol name or string literal */
  cdk::basic_node      *node;	/* node pointer */
  cdk::sequence_node   *sequence;
  cdk::expression_node *expression; /* expression nodes */
  zu::lvalue_node      *lval;
  basic_type           *type;
  zu::block_node       *block;
};

%token <i> tINTEGER
%token <d> tDOUBLE
%token <s> tIDENTIFIER tSTRING
%token tPRINTLN tREAD tBEGIN tEND
%token tRETURN

%nonassoc tBREAK
%nonassoc tCONTINUE

%right '='
%nonassoc '!'
%nonassoc '?'
%nonassoc ':'
%left '|'
%left '&'
%nonassoc '~'
%left tEQ tNE
%left tGE tLE '>' '<'
%left '+' '-'
%left '*' '/' '%'
%nonassoc tUNARY

%type <node> arg cond for var instr functiondecl functiondef decl declvar
%type <sequence> decls declvars instrs args exprs func_args vars
%type <expression> expr literal funcall address alloc
%type <lval> lval
%type <type> type
%type <s> string
%type <block> block
/*  */
%{
//-- The rules below will be included in yyparse, the main parsing function.
%}
%%

file : decls { compiler->ast($1); }
     |       { compiler->ast(new cdk::nil_node(LINE)); }
     ;
     
decls : decl       { $$ = new cdk::sequence_node(LINE, $1); }
      | decls decl { $$ = new cdk::sequence_node(LINE, $2, $1); }
      ;

decl : declvar       { $$ = $1; }
     | functiondef   { $$ = $1; }
     | functiondecl  { $$ = $1; }
     ;
    
declvars : declvar          { $$ = new cdk::sequence_node(LINE, $1); }
         | declvars declvar { $$ = new cdk::sequence_node(LINE, $2, $1); }
         ;
         
declvar : var ';' { $$ = $1; }
        ;
        
vars : var                 { $$ = new cdk::sequence_node(LINE, $1); }
     | vars ',' var { $$ = new cdk::sequence_node(LINE, $3, $1); }
     ;
        
        
var : type tIDENTIFIER              { $$ = new zu::variable_declaration_node(LINE, $2, nullptr, $1, true, false); }
    | type tIDENTIFIER '!'          { $$ = new zu::variable_declaration_node(LINE, $2, nullptr, $1, false, false); }
    | type tIDENTIFIER '?'          { $$ = new zu::variable_declaration_node(LINE, $2, nullptr, $1, true, true); }
    | type tIDENTIFIER '=' expr     { $$ = new zu::variable_declaration_node(LINE, $2, $4, $1, true, false); }
    | type tIDENTIFIER '!' '=' expr { $$ = new zu::variable_declaration_node(LINE, $2, $5, $1, false, false); }
    ;
         
functiondef : type tIDENTIFIER '(' func_args ')' block                  { $$ = new zu::function_definition_node(LINE, $2, $4, $1, $6, nullptr, true); }
            | type tIDENTIFIER '!' '(' func_args ')' block              { $$ = new zu::function_definition_node(LINE, $2, $5, $1, $7, nullptr, false); }
            | type tIDENTIFIER '!' '(' func_args ')' '=' literal block  { $$ = new zu::function_definition_node(LINE, $2, $5, $1, $9, $8, false); }
            | type tIDENTIFIER '(' func_args ')' '=' literal block      { $$ = new zu::function_definition_node(LINE, $2, $4, $1, $8, $7, true); }
            ;
            
functiondecl : type tIDENTIFIER '(' func_args ')'                  { $$ = new zu::function_declaration_node(LINE, $2, $4, $1, nullptr, true, false); }
             | type tIDENTIFIER '?' '(' func_args ')'              { $$ = new zu::function_declaration_node(LINE, $2, $5, $1, nullptr, false, true); }
             | type tIDENTIFIER '!' '(' func_args ')'              { $$ = new zu::function_declaration_node(LINE, $2, $5, $1, nullptr, false, false); }
             | type tIDENTIFIER '!' '(' func_args ')' '=' literal  { $$ = new zu::function_declaration_node(LINE, $2, $5, $1, $8, false, false); }
             | type tIDENTIFIER '(' func_args ')' '=' literal      { $$ = new zu::function_declaration_node(LINE, $2, $4, $1, $7, true, false); }
             ;
         
literal : string     { $$ = new cdk::string_node(LINE, $1); }
        | tINTEGER   { $$ = new cdk::integer_node(LINE, $1); }
        | tDOUBLE    { $$ = new cdk::double_node(LINE, $1); }
        ;
        
string  : tSTRING         { $$ = $1; }
        | string tSTRING  { $$ = new std::string(*$1 + *$2); delete $1; delete $2;}
        ;
       
func_args : args  { $$ = $1; }
          |       { $$ = nullptr; }
          ;
          
args : arg           { $$ = new cdk::sequence_node(LINE, $1); }
     | args ',' arg  { $$ = new cdk::sequence_node(LINE, $3, $1); }
     ;
     
arg : type tIDENTIFIER     { $$ = new zu::variable_declaration_node(LINE, $2, nullptr, $1, true, false); }
    ;
    
type : '#'           { $$ = new basic_type(4, basic_type::TYPE_INT); }
     | '%'           { $$ = new basic_type(8, basic_type::TYPE_DOUBLE); }
     | '$'           { $$ = new basic_type(4, basic_type::TYPE_STRING); }
     | '<' type '>'  { $$ = new basic_type(4, basic_type::TYPE_POINTER); $$->_subtype = $2; }
     | '!'           { $$ = new basic_type(0, basic_type::TYPE_VOID); }
     ;
     
block: '{' declvars instrs '}'   { $$ = new zu::block_node(LINE, $2, $3); }
     | '{' declvars '}'          { $$ = new zu::block_node(LINE, $2, nullptr); }
     | '{' instrs '}'            { $$ = new zu::block_node(LINE, nullptr, $2); }
     | '{' '}'                   { $$ = new zu::block_node(LINE, nullptr, nullptr); }
     ;
     
instrs : instr         { $$ = new cdk::sequence_node(LINE, $1); }
       | instrs instr  { $$ = new cdk::sequence_node(LINE, $2, $1); }
       ;
       
instr : expr ';'       { $$ = $1; }
      | expr tPRINTLN  { $$ = new zu::print_line_node(LINE, $1); }
      | expr '!'       { $$ = new zu::print_node(LINE, $1); }
      | tBREAK         { $$ = new zu::break_node(LINE); }
      | tCONTINUE      { $$ = new zu::continue_node(LINE); }
      | tRETURN        { $$ = new zu::return_node(LINE); }
      | cond           { $$ = $1; }
      | for            { $$ = $1; }
      | block          { $$ = $1; }
      ;
      
cond : '[' expr ']' '#' instr                      { $$ = new zu::if_node(LINE, $2, $5); }
     | '[' expr ']' '?' instr                      { $$ = new zu::if_node(LINE, $2, $5); }
     | '[' expr ']' '?' instr ':' instr            { $$ = new zu::if_else_node(LINE, $2, $5, $7); }
     ;
     
for : '[' ';' ';' ']' instr                        { $$ = new zu::for_node(LINE, nullptr, nullptr, nullptr, $5); }
    | '[' vars ';' ';' ']' instr                   { $$ = new zu::for_node(LINE, $2, nullptr, nullptr, $6); }
    | '[' vars ';' exprs ';' ']' instr             { $$ = new zu::for_node(LINE, $2, $4, nullptr, $7); }
    | '[' vars ';' exprs ';' exprs ']' instr       { $$ = new zu::for_node(LINE, $2, $4, $6, $8); }
    | '[' ';' exprs ';' exprs ']' instr            { $$ = new zu::for_node(LINE, nullptr, $3, $5, $7); }
    | '[' ';' exprs ';' ']' instr                  { $$ = new zu::for_node(LINE, nullptr, $3, nullptr, $6); }
    | '[' ';' ';' exprs ']' instr                  { $$ = new zu::for_node(LINE, nullptr, nullptr, $4, $6); }
    | '[' vars ';' ';' exprs ']' instr             { $$ = new zu::for_node(LINE, $2, nullptr, $5, $7); }
    | '[' exprs ';' ';' ']' instr                  { $$ = new zu::for_node(LINE, $2, nullptr, nullptr, $6); }
    | '[' exprs ';' exprs ';' ']' instr            { $$ = new zu::for_node(LINE, $2, $4, nullptr, $7); }
    | '[' exprs ';' exprs ';' exprs ']' instr      { $$ = new zu::for_node(LINE, $2, $4, $6, $8); }
    | '[' exprs ';' ';' exprs ']' instr            { $$ = new zu::for_node(LINE, $2, nullptr, $5, $7); }
    ;
          
exprs : exprs ',' expr   { $$ = new cdk::sequence_node(LINE, $3, $1); }
      | expr             { $$ = new cdk::sequence_node(LINE, $1); }
      ;
      
expr : literal                 { $$ = $1; }
     | funcall                 { $$ = $1; }
     | '-' expr %prec tUNARY   { $$ = new cdk::neg_node(LINE, $2); }
     | '+' expr %prec tUNARY   { $$ = new zu::identity_node(LINE, $2); }
     | expr '|' expr           { $$ = new zu::or_node(LINE, $1, $3); }
     | expr '&' expr           { $$ = new zu::and_node(LINE, $1, $3); }
     | address                 { $$ = $1; }
     | '~' expr                { $$ = new zu::not_node(LINE, $2); }
     | '@'                     { $$ = new zu::read_node(LINE); }
     | expr '+' expr	       { $$ = new cdk::add_node(LINE, $1, $3); }
     | expr '-' expr	       { $$ = new cdk::sub_node(LINE, $1, $3); }
     | expr '*' expr	       { $$ = new cdk::mul_node(LINE, $1, $3); }
     | expr '/' expr	       { $$ = new cdk::div_node(LINE, $1, $3); }
     | expr '%' expr	       { $$ = new cdk::mod_node(LINE, $1, $3); }
     | expr '<' expr           { $$ = new cdk::lt_node(LINE, $1, $3); }
     | expr '>' expr	       { $$ = new cdk::gt_node(LINE, $1, $3); }
     | expr tGE expr	       { $$ = new cdk::ge_node(LINE, $1, $3); }
     | expr tLE expr           { $$ = new cdk::le_node(LINE, $1, $3); }
     | expr tNE expr	       { $$ = new cdk::ne_node(LINE, $1, $3); }
     | expr tEQ expr	       { $$ = new cdk::eq_node(LINE, $1, $3); }
     | alloc                   { $$ = $1; }
     | lval                    { $$ = new zu::rvalue_node(LINE, $1); }
     | lval '=' expr           { $$ = new zu::assignment_node(LINE, $1, $3); }
     | '(' expr ')'            { $$ = $2; }
     ;
     
address : lval '?'             { $$ = new zu::address_node(LINE, $1); }   
        ;
        
alloc : '[' expr ']'           { $$ = new zu::allocation_node(LINE, $2); }
      ;
    
lval : tIDENTIFIER               { $$ = new zu::variable_node(LINE, $1); }
     | lval '[' expr ']'         { $$ = new zu::index_node(LINE, $1, $3); }
     | address '[' expr ']'      { $$ = new zu::index_node(LINE, $1, $3); }
     | funcall '[' expr ']'      { $$ = new zu::index_node(LINE, $1, $3); }
     | '(' expr ')' '[' expr ']' { $$ = new zu::index_node(LINE, $2, $5); }
     | alloc '[' expr ']'        { $$ = new zu::index_node(LINE, $1, $3); }
     ;

     
funcall : tIDENTIFIER '(' exprs ')'  { $$ = new zu::function_call_node(LINE, $1, $3); }
        | tIDENTIFIER '(' ')'        { $$ = new zu::function_call_node(LINE, $1, nullptr); }
        ;

%%
