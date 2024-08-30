#include <stdlib.h>
#include <ctype.h>
#include <stdio.h>
#include <string.h>
#define N 2000

int brackets(char *str) { // просто проверка на кол-во открывающихся и закрывающихся скобок
    int open_bracket = 0;
    int close_bracket = 0;
    int flg = 1;
    int len = (int)strlen(str);
    for(int i = 0; i < len; i++){
        if(flg == 2){
            return 0;
        }
        if(str[i] == '('){
            open_bracket += 1;
            flg -= 1;
        }
        if(str[i] == ')'){
            close_bracket += 1;
            flg += 1;
        }
    }
    if(open_bracket == close_bracket) {
        return 1;
    }
    else{
        return 0;
    }
}

int right_symbols(char *mass) { // Проверка на корректность вводимых символов, чтобы не было букав
    char alpha[] = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ";
    int len = (int)strlen(mass);
    int len_alpha = (int)strlen(alpha);
    for(int i = 0; i < len; i++) {
        for(int j = 0; j < len_alpha; j++) {
            if(mass[i] == alpha[j]){
                return 0;
            }
        }
    }
    return 1;
}

int point(char *mass) { // проверка, чтобы не было точки
    int len = (int)strlen(mass);
    for(int i = 0; i < len; i++){
        if(mass[i] == '.') {
            return 0;
        }
    }
    return 1;
}

int left_right_brackets(char *mass) { // проверка, чтобы не было вот такого ()
    int len = (int)strlen(mass);
    for(int i = 0; i < len - 1; i++) {
        if((mass[i] == '(') && (mass[i + 1] == ')')) {
            return 0;
        }
    }
    return 1;
}



int precedence(char operator) { // приоритетность операций
    if (operator == '+' || operator == '-')
        return 1;
    else if (operator == '*' || operator == '/')
        return 2;
    return 0;
}

int operations(char *mass) { // проверка, чтобы не было двух операций, идущих подряд
    int len = (int)strlen(mass);
    for(int i = 0; i < len - 1; i++) {
        if((precedence(mass[i]) > 0) && (precedence(mass[i + 1]) > 0)) {
            return 0;
        }
    }
    return 1;
}

int last_sym(char *mass){ // проверка, чтобы последним символов не была операция
    int len = (int)strlen(mass);
    if(precedence(mass[len - 2]) > 0) {
        return 0;
    }
    return 1;
}

int lonely_operation(char *mass) { // проверка, чтобы не было такого (-), или (+), или (*), или (/)
    int len = (int)strlen(mass);
    for(int i = 1; i < len; i++){
        if((mass[i - 1] == '(') && (precedence(mass[i]) > 0) && (mass[i + 1] == ')')) {
            return 0;
        }
    }
    return 1;
}

int havent_operation(char *mass) { // проверка, чтобы между двумя числами не было пробела
    int len = (int)strlen(mass);
    for(int i = 1; i < len; i++) {
        if(isdigit((int)mass[i - 1]) > 0){
            if(mass[i] == ' ') {
                if((isdigit((int)mass[i + 1]) > 0)) {
                    return 0;
                }
            }
        }
    }
    return 1;
}


int apply_operator(int a, int b, char operator) {
    switch (operator) {
        case '+':
            return a + b;
        case '-':
            return a - b;
        case '*':
            return a * b;
        case '/':
            if(b == 0) {
                printf("division by zero");
                exit(0);
            }
            return a / b;
        default:
            printf("syntax error");
            return 0; // Что-то не так
    }
}

int evaluate_expression(char *expression) { // основная функция
    int values[250];
    char operators[250];
    int values_flg = -1;
    int operators_flg = -1;

    while (*expression) { // идём до конца массива
        if (isspace(*expression)) { // пропуск пробельных символов, чтобы прога не рушилась
            ++expression;
            continue;
        }

        if (isdigit(*expression)) { // если число, кладём в массив values
            values[++values_flg] = strtod(expression, &expression);
        } else if (*expression == '(') { // если нашли открывающуюся скобку, положим её в operators
            operators[++operators_flg] = *expression;
            ++expression;
        } else if (*expression == ')') { // находим закрывающуюся скобку и идём назад, до открывающейся, попутно выполняя операции
            while (operators_flg != -1 && operators[operators_flg] != '(') {
                int  b = values[values_flg--];
                int  a = values[values_flg--];
                char op = operators[operators_flg--];
                values[++values_flg] = apply_operator(a, b, op);
            }

            if (operators_flg == -1 || operators[operators_flg] != '(') {
                // Некорректное расположение скобок
                printf("syntax error");
                return 0;
            }

            --operators_flg;
            ++expression;
        } else { // на этом моменте выполняем операции по списку приоритетности, и складываем новые операторы в operators
            while (operators_flg != -1 && precedence(operators[operators_flg]) >= precedence(*expression)) {
                int b = values[values_flg--];
                int a = values[values_flg--];
                char op = operators[operators_flg--];
                values[++values_flg] = apply_operator(a, b, op);
            }

            operators[++operators_flg] = *expression;
            ++expression;
        }
    }

    while (operators_flg != -1) { // если не встретили скобок и различной мишуры, просто выполняем операции в порядке очереди
        int b = values[values_flg--];
        int a = values[values_flg--];
        char op = operators[operators_flg--];
        values[++values_flg] = apply_operator(a, b, op);
    }
    int result = values[values_flg];
    return result;
}

int check_all(char *expression) { // всевозможные проверки
        if(brackets(expression) == 0) {
            printf("syntax error");
            return 0;
        }
        if(right_symbols(expression) == 0) {
            printf("syntax error");
            return 0;
        }

        if(point(expression) == 0) {
            printf("syntax error");
            return 0;
        }

        if(left_right_brackets(expression) == 0) {
            printf("syntax error");
            return 0;
        }

        if(operations(expression) == 0) {
            printf("syntax error");
            return 0;
        }

        if(last_sym(expression) == 0) {
            printf("syntax error");
            return 0;
        }

        if(lonely_operation(expression) == 0) {
            printf("syntax error");
            return 0;
        }

        if(havent_operation(expression) == 0) {
            printf("syntax error");
            return 0;
        }
        return 1;
}


int main() {
    char expression[N];
    if(fgets(expression, sizeof(expression), stdin) == 0) {
        return 0;
    }
    if((int)strlen(expression) == 1) {
        printf("syntax error");
        return 0;
    }
    if(check_all(expression) == 0){
        return 0;
    }

    int result = evaluate_expression(expression);
    printf("%d\n", (int)result);

    exit(EXIT_SUCCESS);
}
