#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define MAX_CODE 5000
#define MAX_TAC 100
#define MAX_LINE 200

char *keywords[] = {"int", "if", "while", "return", "main"};
int keyword_count = 5;

char *multi_ops[] = {"==", "+=", "-=", "*=", "/=", ">=", "<="};
int multi_ops_count = 7;

char single_ops[] = "+-*/=;(){}><";

int is_keyword(char str[]) {
    for (int i = 0; i < keyword_count; i++)
        if (strcmp(str, keywords[i]) == 0)
            return 1;
    return 0;
}

int is_single_operator(char ch) {
    for (int i = 0; i < strlen(single_ops); i++)
        if (ch == single_ops[i])
            return 1;
    return 0;
}

int is_multi_operator(char *str) {
    for (int i = 0; i < multi_ops_count; i++)
        if (strcmp(str, multi_ops[i]) == 0)
            return 1;
    return 0;
}

int is_number_token(char str[]) {
    int i = 0;

    if (str[0] == '-')
        i = 1;

    if (str[i] == '\0')
        return 0;

    for (; str[i] != '\0'; i++)
        if (!isdigit(str[i]))
            return 0;

    return 1;
}

void lexer(char code[]) {
    int i = 0;
    char token[100];

    printf("\n--- TOKENS ---\n");

    while (code[i] != '\0') {
        if (isspace(code[i])) {
            i++;
            continue;
        }

        if (code[i] == '/' && code[i + 1] == '/') {
            while (code[i] != '\n' && code[i] != '\0')
                i++;
            continue;
        }

        if (code[i] && code[i + 1]) {
            char op[3] = {code[i], code[i + 1], '\0'};

            if (is_multi_operator(op)) {
                printf("Token: %s -> OPERATOR\n", op);
                i += 2;
                continue;
            }
        }

        if (is_single_operator(code[i])) {
            printf("Token: %c -> OPERATOR\n", code[i]);
            i++;
            continue;
        }

        int t = 0;

        while (code[i] && !isspace(code[i]) && !is_single_operator(code[i])) {
            token[t++] = code[i++];
        }

        token[t] = '\0';

        if (is_keyword(token))
            printf("Token: %s -> KEYWORD\n", token);
        else if (is_number_token(token))
            printf("Token: %s -> NUMBER\n", token);
        else
            printf("Token: %s -> IDENTIFIER\n", token);
    }
}

int has_hxi_extension(const char *filename) {
    const char *dot = strrchr(filename, '.');

    if (!dot || dot == filename)
        return 0;

    return strcmp(dot, ".hxi") == 0;
}

typedef struct Node {
    char value;
    struct Node* left;
    struct Node* right;
} Node;

char expression[100];
int pos = 0;

char tac[MAX_TAC][50];
int tacIndex = 0;
int tempCount = 0;

Node* E();
Node* T();
Node* F();

Node* createNode(char value, Node* left, Node* right) {
    Node* node = (Node*)malloc(sizeof(Node));

    node->value = value;
    node->left = left;
    node->right = right;

    return node;
}

Node* F() {
    Node* node = NULL;

    if (expression[pos] == '(') {
        pos++;
        node = E();

        if (expression[pos] == ')')
            pos++;
        else
            printf("Error: missing )\n");
    }
    else if (isdigit(expression[pos])) {
        node = createNode(expression[pos], NULL, NULL);
        pos++;
    }
    else {
        printf("Error: invalid character %c\n", expression[pos]);
        pos++;
    }

    return node;
}

Node* T() {
    Node* left = F();

    while (expression[pos] == '*' || expression[pos] == '/') {
        char op = expression[pos++];
        Node* right = F();

        left = createNode(op, left, right);
    }

    return left;
}

Node* E() {
    Node* left = T();

    while (expression[pos] == '+' || expression[pos] == '-') {
        char op = expression[pos++];
        Node* right = T();

        left = createNode(op, left, right);
    }

    return left;
}

int evaluateAST(Node* node) {
    if (node == NULL)
        return 0;

    if (node->left == NULL && node->right == NULL)
        return node->value - '0';

    int left = evaluateAST(node->left);
    int right = evaluateAST(node->right);

    if (node->value == '+')
        return left + right;

    if (node->value == '-')
        return left - right;

    if (node->value == '*')
        return left * right;

    if (node->value == '/') {
        if (right == 0) {
            printf("Error: Division by zero\n");
            return 0;
        }

        return left / right;
    }

    return 0;
}

char* newTemp() {
    char temp[10];

    sprintf(temp, "t%d", ++tempCount);

    char* result = (char*)malloc(strlen(temp) + 1);
    strcpy(result, temp);

    return result;
}

char* generateTAC(Node* node) {
    if (node == NULL)
        return NULL;

    if (node->left == NULL && node->right == NULL) {
        char* value = (char*)malloc(2);

        value[0] = node->value;
        value[1] = '\0';

        return value;
    }

    char* left = generateTAC(node->left);
    char* right = generateTAC(node->right);
    char* temp = newTemp();

    sprintf(tac[tacIndex++], "%s = %s %c %s", temp, left, node->value, right);

    return temp;
}

void printTAC() {
    printf("\n--- THREE ADDRESS CODE ---\n");

    for (int i = 0; i < tacIndex; i++) {
        printf("%s\n", tac[i]);
    }
}

void resetTAC() {
    tacIndex = 0;
    tempCount = 0;
}

void optimizeTAC(char tac[][50], int tacIndex) {
    char expr[MAX_TAC][50];
    char result[MAX_TAC][10];
    int exprCount = 0;

    printf("\n--- OPTIMIZED TAC (DAG PROTOTYPE) ---\n");

    for (int i = 0; i < tacIndex; i++) {
        char lhs[10], op1[10], op2[10];
        char op;

        if (sscanf(tac[i], "%s = %s %c %s", lhs, op1, &op, op2) != 4) {
            printf("Invalid TAC skipped: %s\n", tac[i]);
            continue;
        }

        char currentExpr[50];
        sprintf(currentExpr, "%s %c %s", op1, op, op2);

        int found = -1;

        for (int j = 0; j < exprCount; j++) {
            if (strcmp(expr[j], currentExpr) == 0) {
                found = j;
                break;
            }
        }

        if (found != -1) {
            printf("%s = %s\n", lhs, result[found]);
        }
        else {
            strcpy(expr[exprCount], currentExpr);
            strcpy(result[exprCount], lhs);
            exprCount++;

            printf("%s\n", tac[i]);
        }
    }
}

int readFile(const char *filename, char code[]) {
    FILE *fp = fopen(filename, "r");
    char line[MAX_LINE];

    if (!fp) {
        printf("Error: Cannot open %s\n", filename);
        return 0;
    }

    code[0] = '\0';

    while (fgets(line, sizeof(line), fp)) {
        if (strlen(code) + strlen(line) < MAX_CODE)
            strcat(code, line);
    }

    fclose(fp);

    return 1;
}

void extractExpression(char code[], char expr[]) {
    int j = 0;

    for (int i = 0; code[i] != '\0'; i++) {
        if (isdigit(code[i]) ||
            code[i] == '+' ||
            code[i] == '-' ||
            code[i] == '*' ||
            code[i] == '/' ||
            code[i] == '(' ||
            code[i] == ')') {
            expr[j++] = code[i];
        }
        else if (code[i] == ';') {
            break;
        }
    }

    expr[j] = '\0';
}

int main(int argc, char *argv[]) {
    char code[MAX_CODE];
    const char *filename;

    if (argc > 1)
        filename = argv[1];
    else
        filename = "test.hxi";

    if (!has_hxi_extension(filename)) {
        printf("Error: Helix source file must have .hxi extension\n");
        return 1;
    }

    if (!readFile(filename, code))
        return 1;

    printf("--- HELIX COMPILER ---\n");
    printf("Source file: %s\n", filename);

    lexer(code);

    extractExpression(code, expression);

    if (strlen(expression) == 0) {
        printf("\nError: No valid arithmetic expression found.\n");
        printf("Use an expression like: 2*3+4;\n");
        return 1;
    }

    printf("\n--- EXPRESSION SENT TO PARSER ---\n");
    printf("%s\n", expression);

    pos = 0;
    Node* root = E();

    printf("\n--- PARSER / EVALUATOR RESULT ---\n");
    printf("Result: %d\n", evaluateAST(root));

    resetTAC();
    generateTAC(root);
    printTAC();

    optimizeTAC(tac, tacIndex);

    return 0;
}