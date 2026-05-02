#include <stdio.h>
#include <string.h>

#define MAX 100

char tac[MAX][50];
int tacIndex = 0;

// ---------- PARSE STRICT TAC ----------
int parseTAC(char *line, char *lhs, char *op1, char *op2, char *operator) {

    // Try standard format: t1 = a * b
    if (sscanf(line, "%s = %s %c %s", lhs, op1, operator, op2) == 4) {
        return 1;
    }

    // Try compact format: t1=a*b
    if (sscanf(line, "%[^=]=%[^+-*/]%c%[^+-*/]", lhs, op1, operator, op2) == 4) {

        // Remove leading spaces
        while (*lhs == ' ') lhs++;
        while (*op1 == ' ') op1++;
        while (*op2 == ' ') op2++;

        return 1;
    }

    return 0;
}

// ---------- OPTIMIZER ----------
void optimizeTAC(char tac[][50], int tacIndex) {

    char expr[MAX][50];     // stores expressions like "a * b"
    char result[MAX][10];   // stores corresponding temp (t1, t2...)
    int exprCount = 0;

    printf("\n--- Optimized TAC (DAG Prototype) ---\n");

    for (int i = 0; i < tacIndex; i++) {

        char lhs[10], op1[10], op2[10];
        char operator;

        // Strict parsing check
        if (!parseTAC(tac[i], lhs, op1, op2, &operator)) {
            printf("Invalid TAC (skipped): %s\n", tac[i]);
            continue;
        }

        // Detect invalid expressions like: a*b + d
        if (strchr(op1, '+') || strchr(op1, '-') || strchr(op1, '*') || strchr(op1, '/') ||
        strchr(op2, '+') || strchr(op2, '-') || strchr(op2, '*') || strchr(op2, '/')) {
            printf("Invalid TAC (complex expr skipped): %s\n", tac[i]);
            continue;
        }

        char currentExpr[50];
        sprintf(currentExpr, "%s %c %s", op1, operator, op2);

        int found = -1;

        // Check for common subexpression
        for (int j = 0; j < exprCount; j++) {
            if (strcmp(expr[j], currentExpr) == 0) {
                found = j;
                break;
            }
        }

        if (found != -1) {
            // Reuse previous computation
            printf("%s = %s\n", lhs, result[found]);
        } else {
            // Store new expression
            strcpy(expr[exprCount], currentExpr);
            strcpy(result[exprCount], lhs);
            exprCount++;

            printf("%s = %s\n", lhs, currentExpr);
        }
    }
}

// ---------- MAIN ----------
int main() {

    printf("Enter number of TAC instructions: ");
    scanf("%d", &tacIndex);
    getchar();

    printf("Enter TAC instructions (e.g., t1 = a * b):\n");

    for (int i = 0; i < tacIndex; i++) {
        fgets(tac[i], sizeof(tac[i]), stdin);
        tac[i][strcspn(tac[i], "\n")] = '\0';
    }

    printf("\n--- Original TAC ---\n");
    for (int i = 0; i < tacIndex; i++) {
        printf("%s\n", tac[i]);
    }

    optimizeTAC(tac, tacIndex);

    return 0;
}