#include <commands/calc.h>

#include <string/stoi.h>
#include <string/strlen.h>

#include <console/print.h>
#include <console/print_dec.h>
#include <console/print_sdec.h>
#include <console/newline.h>
#include <console/put.h>

#include <math/pow.h>

enum { ADD, SUB, MUL, DIV, MOD, POW, };

__CDECL uint32_t command_calc(uint32_t argc, const char ** argv) {
    if (argc != 4) {
        console_print("Invalid arguments!\nExpected 3 got ");
        console_print_dec(argc - 1);
        console_newline();
        return 1;
    }

    if (strlen(argv[2]) != 1) {
        console_print("Invalid length for operator argument.\n");
        return 2;
    }

    int32_t operand_a, operand_b;
    uint8_t  operator;

    if (stoi(&operand_a, argv[1])) {
        console_print("Operand ");
        console_print(argv[1]);
        console_print(" is not a valid number!\n");
        return 3;
    }

    switch (argv[2][0]) {
        case '+': operator = ADD; break;
        case '-': operator = SUB; break;
        case '*': operator = MUL; break;
        case '/': operator = DIV; break;
        case '%': operator = MOD; break;
        case '^': operator = POW; break;
        default: {
            console_print("Invalid operator\nMust be one of: [ +, -, *, /, %, ^ ]\n");
            return 4;
        }
    }

    if (stoi(&operand_b, argv[3])) {
        console_print("Operand ");
        console_print(argv[3]);
        console_print(" is not a valid number!\n");
        return 5;
    }

    int32_t result;

    switch (operator) {
        case ADD: result = operand_a + operand_b; break;
        case SUB: result = operand_a - operand_b; break;
        case MUL: result = operand_a * operand_b; break;
        case DIV: result = operand_a / operand_b; break;
        case MOD: result = operand_a % operand_b; break;
        case POW: result = math_pow(operand_a, operand_b); break; // TODO: do
        default:  result = 0; break;
    }

    console_print_sdec(operand_a);
    console_put(' ');
    console_put(argv[2][0]);
    console_put(' ');
    console_print_sdec(operand_b);
    console_print(" = ");
    console_print_sdec(result);
    console_newline();

    return 0;
}