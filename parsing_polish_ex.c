#include <stdio.h>
#include "mpc/mpc.h"

#include <readline/readline.h>
#include <readline/history.h>

int main(){

  mpc_parser_t* Dot       = mpc_new("dot");
  mpc_parser_t* Number    = mpc_new("number");
  mpc_parser_t* Operator  = mpc_new("operator");
  mpc_parser_t* Expr      = mpc_new("expr");
  mpc_parser_t* Lispy     = mpc_new("lispy");

  mpca_lang(MPCA_LANG_DEFAULT,
    "                                                     \
      dot      : '.' ;                                    \
      number   : /-?[0-9].?[0-9]*/ ;                     \
      operator : '+' | '-' | '*' | '/' | '%' ;            \
      expr     : <number> | '(' <operator> <expr>+ ')' ;  \
      lispy    : /^/ <operator> <expr>+ /$/ ;             \
    ",
    Dot, Number, Operator, Expr, Lispy);
  
  puts("Lispy version 0.1");

  while(1){
    char* input = readline("lispy> ");
    add_history(input);

    mpc_result_t r;
    if (mpc_parse("<stdin>", input, Lispy, &r)) {
      /* On Success Print the AST */
      mpc_ast_print(r.output);
      mpc_ast_delete(r.output);
    } else {
      /* Otherwise Print the Error */
      mpc_err_print(r.error);
      mpc_err_delete(r.error);
    }

    printf("Yo have entered: %s\n", input);

    free(input);
  }
  mpc_cleanup(4, Dot,Number, Operator, Expr, Lispy);
  return 0;
}
