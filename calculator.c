#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>

void removeSpaces(char* str) {
    int i = 0, j = 0;
    while (str[i]) {
        if (str[i] != ' ')
            str[j++] = str[i];
        i++;
    }
    str[j] = '\0';
}


int isValidExpression(const char* expr) {
    for (int i = 0; expr[i]; i++) {
        if (!isdigit(expr[i]) && expr[i] != '+' && expr[i] != '-' &&
            expr[i] != '*' && expr[i] != '/' && expr[i] != '\0') {
            return 0; 
        }
    }
    return 1;
}


int performOperation(int a, int b, char op, int* error) {
    switch (op) {
        case '+': return a + b;
        case '-': return a - b;
        case '*': return a * b;
        case '/':
            if (b == 0) {
                *error = 1;
                return 0;
            }
            return a / b;
        default: return 0;
    }
}


int evaluateExpression(char* expr, int* error) {
    int numbers[100];
    char operators[100];
    int numTop = -1, opTop = -1;
    int i = 0;

    while (expr[i] != '\0') {
        if (isdigit(expr[i])) {
            int val = 0;
            while (isdigit(expr[i])) {
                val = val * 10 + (expr[i] - '0');
                i++;
            }
            numbers[++numTop] = val;
        } else {
            char op = expr[i++];

           
            while (opTop >= 0 &&
                  ((op == '+' || op == '-') ||
                   ((op == '*' || op == '/') &&
                    (operators[opTop] == '*' || operators[opTop] == '/')))) {

                int b = numbers[numTop--];
                int a = numbers[numTop--];
                char oper = operators[opTop--];

                int res = performOperation(a, b, oper, error);
                if (*error) return 0;
                numbers[++numTop] = res;
            }
            operators[++opTop] = op;
        }
    }

    
    while (opTop >= 0) {
        int b = numbers[numTop--];
        int a = numbers[numTop--];
        char oper = operators[opTop--];

        int res = performOperation(a, b, oper, error);
        if (*error) return 0;
        numbers[++numTop] = res;
    }

    return numbers[numTop];
}

int main() {
    char expression[100];
    printf("Enter expression: ");
    fgets(expression, sizeof(expression), stdin);

    removeSpaces(expression);
    expression[strcspn(expression, "\n")] = '\0'; 

    if (!isValidExpression(expression)) {
        printf("Error: this Invalid expression.\n");
        return 0;
    }

    int error = 0;
    int result = evaluateExpression(expression, &error);

    if (error) {
        printf("Error: Division by zero.\n");
    } else {
        printf("%d\n", result);
    }

    return 0;
}
//