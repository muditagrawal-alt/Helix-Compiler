#include <stdio.h>
#include <ctype.h>

char input[100];
int pos = 0;

// Forward declarations for recursive descent parser
int E();
int T();
int F();

// F() - Factor
int F() {
    int value = 0;

    // Handling parentheses
    if (input[pos] == '(') {
        pos++;  // skip '('
        value = E();

        if (input[pos] == ')')
            pos++;  // skip ')'
        else
            printf("Error: missing )\n");
    }

    // Handling multi-digit numbers
    else if (isdigit(input[pos])) {
        value = input[pos] - '0';
        pos++;
    }

    else {
        printf("Error: invalid character %c\n", input[pos]);
        pos++;
    }

    return value;
}

// T() - Term
int T() {
    int value = F();

    while (input[pos] == '*' || input[pos] == '/') {
        char op = input[pos++];

        if (op == '*')
            value *= F();
        else {
            int divisor = F();
            if (divisor == 0) {
                printf("Error: Division by zero\n");
                return 0;
            }
            value /= divisor;
        }
    }

    return value;
}

// E() - Expression
int E() {
    int value = T();

    while (input[pos] == '+' || input[pos] == '-') {
        char op = input[pos++];

        if (op == '+')
            value += T();
        else
            value -= T();
    }

    return value;
}

// MAIN
int main() {
    printf("Enter expression (e.g., 2*3+4): ");
    scanf("%s", input);

    pos = 0;  // reset position

    int result = E();

    printf("Result: %d\n", result);

    return 0;
}