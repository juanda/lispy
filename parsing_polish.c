#include <stdio.h>
#include "mpc/mpc.h"

#include "mpc/mpc.h"

#ifdef __linux__
#include <editline.h>
#endif

#ifdef __APPLE__
#include <readline/readline.h>
#include <readline/history.h>
#endif


int number_of_nodes(mpc_ast_t* t) {
  if (t->children_num == 0) { return 1; }
  if (t->children_num >= 1) {
    int total = 1;
    for (int i = 0; i < t->children_num; i++) {
      total = total + number_of_nodes(t->children[i]);
    }
    return total;
  }
  return 0;
}

long eval_op(long x, char* op, long y){
	if (strcmp(op, "+") == 0) { return x + y; }
	if (strcmp(op, "-") == 0) { return x - y; }
	if (strcmp(op, "*") == 0) { return x * y; }
	if (strcmp(op, "/") == 0) { return x / y; }
	return 0;
}

long eval(mpc_ast_t* t){
	if(strstr(t->tag, "number")){
		return atoi(t->contents);
	}

	char* op = t->children[1]->contents;

	long x = eval(t->children[2]);

    int i = 3;

	while(strstr(t->children[i]->tag, "expr")){
		x = eval_op(x, op, eval(t->children[i]));
		i++;
	}

	return x;
}


int main(){

  mpc_parser_t* Dot       = mpc_new("dot");
  mpc_parser_t* Number    = mpc_new("number");
  mpc_parser_t* Operator  = mpc_new("operator");
  mpc_parser_t* Expr      = mpc_new("expr");
  mpc_parser_t* Lispy     = mpc_new("lispy");

  mpca_lang(MPCA_LANG_DEFAULT,
    "                                                     \
      dot      : '.' ;                                    \
      number   : /-?[0-9]+/ ;                             \
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
      mpc_ast_t* a = r.output;
	  long result = eval(a);
      printf("%li\n", result);
	  mpc_ast_print(a);
      mpc_ast_delete(r.output);
    } else {
      /* Otherwise Print the Error */
      mpc_err_print(r.error);
      mpc_err_delete(r.error);
    }

    free(input);
  }
  mpc_cleanup(4, Dot,Number, Operator, Expr, Lispy);
  return 0;
}
