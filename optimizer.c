#include <stdio.h>
#include <string.h>

#define MAX 100

// Sample TAC input (you can modify or connect later)
char tac[MAX][50] = {
    "t1 = a * b",
    "t2 = t1 + c",
    "t3 = a * b",
    "t4 = t3 + d"
};

int tacIndex = 4;

// ---------- OPTIMIZER ----------
void optimizeTAC(char tac[][50], int tacIndex) {

    char expr[MAX][50];     // stores expressions like "a * b"
    char result[MAX][10];   // stores corresponding temp (t1, t2...)
    int exprCount = 0;

    printf("\n--- Optimized TAC (DAG Prototype) ---\n");

    for (int i = 0; i < tacIndex; i++) {

        char lhs[10], op1[10], op2[10];
        char operator;

        // Parse TAC line
        sscanf(tac[i], "%s = %s %c %s", lhs, op1, &operator, op2);

        char currentExpr[50];
        sprintf(currentExpr, "%s %c %s", op1, operator, op2);

        int found = -1;

        // Check if expression already exists
        for (int j = 0; j < exprCount; j++) {
            if (strcmp(expr[j], currentExpr) == 0) {
                found = j;
                break;
            }
        }

        // If found → reuse previous temp (CSE)
        if (found != -1) {
            printf("%s = %s\n", lhs, result[found]);
        }
        else {
            // Store new expression
            strcpy(expr[exprCount], currentExpr);
            strcpy(result[exprCount], lhs);
            exprCount++;

            printf("%s\n", tac[i]);
        }
    }
}

// ---------- MAIN ----------
int main() {

    printf("--- Original TAC ---\n");
    for (int i = 0; i < tacIndex; i++) {
        printf("%s\n", tac[i]);
    }

    optimizeTAC(tac, tacIndex);

    return 0;
}