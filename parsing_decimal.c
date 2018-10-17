#include <stdio.h>
#include "mpc/mpc.h"

#include <readline/readline.h>
#include <readline/history.h>

int main(){

  mpc_parser_t* Number    = mpc_new("number");

  mpca_lang(MPCA_LANG_DEFAULT,
    "                                                     \
      number   : /-?[0-9]+.?[0-9]*/ ;                     \
    ",
    Number);
  
  puts("Lispy version 0.1");

  while(1){
    char* input = readline("lispy> ");
    add_history(input);

    mpc_result_t r;
    if (mpc_parse("<stdin>", input, Number, &r)) {
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
  mpc_cleanup(4,Number);
  return 0;
}
