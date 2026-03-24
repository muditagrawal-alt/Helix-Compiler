#include <stdio.h>
#include <string.h>
#include <ctype.h>

#define MAX 5000

// ---------- KEYWORDS ----------
char *keywords[] = {"int", "if", "while", "return", "main"};
int keyword_count = 5;

// ---------- OPERATORS ----------
char *multi_ops[] = {"==", "+=", "-=", "*=", "/=", ">=", "<="};
int multi_ops_count = 6;

char single_ops[] = "+-*/=;(){}><";

// ---------- CHECK FUNCTIONS ----------
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

int is_number(char str[]) {
    int i = 0;
    if (str[0] == '-') i = 1;
    if (str[i] == '\0') return 0;

    for (; str[i] != '\0'; i++)
        if (!isdigit(str[i]))
            return 0;
    return 1;
}

// ---------- LEXER ----------
void lexer(char code[]) {
    int i = 0;
    char token[100];

    while (code[i] != '\0') {

        // Skip whitespace
        if (isspace(code[i])) {
            i++;
            continue;
        }

        // Skip // comments
        if (code[i] == '/' && code[i+1] == '/') {
            while (code[i] != '\n' && code[i] != '\0')
                i++;
            continue;
        }

        // Multi-character operator
        if (code[i] && code[i+1]) {
            char op[3] = {code[i], code[i+1], '\0'};
            if (is_multi_operator(op)) {
                printf("Token: %s -> OPERATOR\n", op);
                i += 2;
                continue;
            }
        }

        // Single-character operator
        if (is_single_operator(code[i])) {
            printf("Token: %c -> OPERATOR\n", code[i]);
            i++;
            continue;
        }

        // Identifier / keyword / number
        int t = 0;
        while (code[i] &&
               !isspace(code[i]) &&
               !is_single_operator(code[i])) {
            token[t++] = code[i++];
        }
        token[t] = '\0';

        if (is_keyword(token))
            printf("Token: %s -> KEYWORD\n", token);
        else if (is_number(token))
            printf("Token: %s -> NUMBER\n", token);
        else
            printf("Token: %s -> IDENTIFIER\n", token);
    }
}

// ---------- HX EXTENSION CHECK ----------
int has_hx_extension(const char *filename) {
    const char *dot = strrchr(filename, '.');
    if (!dot || dot == filename)
        return 0;
    return strcmp(dot, ".hx") == 0;
}

// ---------- MAIN ----------
int main() {
    const char *filename = "test.hx";   // Helix source file
    FILE *fp = fopen(filename, "r");

    if (!has_hx_extension(filename)) {
        printf("Error: Helix source file must have .hx extension\n");
        return 1;
    }

    if (!fp) {
        printf("Error: Cannot open %s\n", filename);
        return 1;
    }

    char code[MAX] = "";
    char line[500];

    // Read entire file
    while (fgets(line, sizeof(line), fp))
        strcat(code, line);

    fclose(fp);

    printf("--- TOKENS ---\n");
    lexer(code);

    return 0;
}