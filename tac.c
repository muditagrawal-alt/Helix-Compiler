#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

char input[100];
int pos = 0;

// AST NODE
typedef struct Node {
    char value;
    struct Node* left;
    struct Node* right;

} Node;

// TAC STORAGE
char tac[100][50];
int tacIndex = 0;
int tempCount = 0;

//FUNCTION DECLARATIONS
Node* E();
Node* T();
Node* F();

//CREATE AST NODE
Node* createNode(char value, Node* left, Node* right) {
    Node* node = (Node*)malloc(sizeof(Node));
    node->value = value;
    node->left = left;
    node->right = right;
    return node;
}

//TEMPORARY VARIABLE GENERATOR
char* newTemp() {
    static char temp[10];
    sprintf(temp, "t%d", ++tempCount);

    char* result = (char*)malloc(strlen(temp) + 1);
    strcpy(result, temp);
    return result;
}

//F -> (E) | digit
Node* F() {
    Node* node = NULL;

    if (input[pos] == '(') {
        pos++;
        node = E();

        if (input[pos] == ')') {
            pos++;
        } else {
            printf("Error: missing )\n");
        }
    }
    else if (isdigit(input[pos])) {
        node = createNode(input[pos], NULL, NULL);
        pos++;
    }
    else {
        printf("Error: invalid character %c\n", input[pos]);
        pos++;
    }

    return node;
}

//T -> F * F
Node* T() {
    Node* left = F();

    while (input[pos] == '*') {
        char op = input[pos];
        pos++;

        Node* right = F();
        left = createNode(op, left, right);
    }

    return left;
}

//E -> T + T
Node* E() {
    Node* left = T();

    while (input[pos] == '+') {
        char op = input[pos];
        pos++;

        Node* right = T();
        left = createNode(op, left, right);
    }

    return left;
}

//GENERATE THREE ADDRESS CODE
char* generateTAC(Node* node) {
    if (node == NULL)
        return NULL;

    // Leaf node: operand
    if (node->left == NULL && node->right == NULL) {
        char* value = (char*)malloc(2);
        value[0] = node->value;
        value[1] = '\0';
        return value;
    }

    // Post-order traversal
    char* left = generateTAC(node->left);
    char* right = generateTAC(node->right);

    char* temp = newTemp();

    sprintf(tac[tacIndex++], "%s = %s %c %s",
            temp, left, node->value, right);

    return temp;
}

//PRINT TAC
void printTAC() {
    printf("\n--- Three Address Code ---\n");

    for (int i = 0; i < tacIndex; i++) {
        printf("%s\n", tac[i]);
    }
}

//RESET TAC
void resetTAC() {
    tacIndex = 0;
    tempCount = 0;
}

//MAIN
int main() {
    printf("Enter expression (e.g., 2*3+4): ");
    scanf("%s", input);

    pos = 0;
    resetTAC();

    Node* root = E();

    generateTAC(root);
    printTAC();

    return 0;
}