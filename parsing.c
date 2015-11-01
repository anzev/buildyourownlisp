#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <editline/readline.h>
#include "mpc.h"


double evaluateOp(char* op, double a, double b) {
    double result;
    if (strcmp(op, "+") == 0) {
        result = a + b;
    } else if (strcmp(op, "-") == 0) {
        result = a - b;
    } else if (strcmp(op, "*") == 0) {
        result = a * b;
    } else if (strcmp(op, "/") == 0) {
        result = a / b;
    }
    return result;
}


double evaluate(mpc_ast_t* ast) {

    if (strstr(ast->tag, "number")) {
        return atof(ast->contents);
    }

    char* op = ast->children[1]->contents;

    // Eval first argument
    double a = evaluate(ast->children[2]);

    int i;
    for (i = 3; i < ast->children_num; i++) {
        if (strstr(ast->children[i]->tag, "expression")) {
            double b = evaluate(ast->children[i]);
            a = evaluateOp(op, a, b);
        }
    }

    return a;
}


int main(int argc, char** argv) {
    
    // Create reverse polish notation grammar
    mpc_parser_t* Number     = mpc_new("number");
    mpc_parser_t* Operator   = mpc_new("operator");
    mpc_parser_t* Expression = mpc_new("expression");
    mpc_parser_t* Lispy      = mpc_new("lispy");

    mpca_lang(MPCA_LANG_DEFAULT,
        "number     : /-?[0-9]+\\.?[0-9]*/ ;                          \
         operator   : '-' | '+' | '*' | '/' ;                         \
         expression : <number> | '(' <operator> <expression>+ ')' ;   \
         lispy      : /^/ <operator> <expression>+ /$/ ;              \
        ",
        Number, Operator, Expression, Lispy);

    printf("Lispy version 0.0.0.0.1\n");
    printf("Press Ctrl+c to exit\n");

    while (1) {
        char* input = readline("lispy> ");
        add_history(input);

        // Parse user input
        mpc_result_t r;
        if (mpc_parse("<stdin>", input, Lispy, &r)) {
            printf("= %.2f\n", evaluate(r.output));
            mpc_ast_delete(r.output);
        } else {
            mpc_err_print(r.error);
            mpc_err_delete(r.error);
        }

        free(input);
    }
    mpc_cleanup(4, Number, Operator, Expression, Lispy);

    return 0;
}
