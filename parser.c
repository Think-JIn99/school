#include<stdio.h>
#include<ctype.h>
#include<stdlib.h>
void addChar();
void getChar();
void lex();
int expr();
/* Character classes */
// #define LETTER 0
#define DIGIT 1
#define OP 99
#define UNKNOWN 18
/* Token codes */
#define ADD_OP 21
#define SUB_OP 22
#define MULT_OP 23
#define DIV_OP 24
#define LEFT_PAREN 25
#define RIGHT_PAREN 26

const int sentence_length = 30; //문장 길이
const int lexeme_length = 9; //lexeme의 최대 길이
char sentence[sentence_length]; //문장의 총길이는 30자 
int current_index = 0;
char ch; //현재 읽는 글자
int char_class; //첫 글자를 하나 읽은 뒤 타입을 지정한다
int token_class;
char lex_temp[lexeme_length];

int lookup(char ch){
    int next_token;
    switch (ch) { 
        case '(':
            next_token = LEFT_PAREN; 
            break;
        case ')':
            next_token = RIGHT_PAREN; 
            break;
        case '+': 
            next_token = ADD_OP;
            break;
        case '-':
            next_token = SUB_OP; 
            break;
        case '*':
            next_token = MULT_OP;
            break;
        case '/':
            next_token = DIV_OP;
            break;
        default: 
            next_token = EOF; 
            break;
    }
    return next_token; 
}

char next(){
    if (sentence[current_index] != 0){ //널문자가 나오면 입력을 모두 순회한 것이므로 종료한다.
        ch = sentence[current_index++];
        return 1;
    }
    else
        return EOF;
}

int add_char(char ch, char lexeme[], int lexeme_len){
    if (lexeme_len < lexeme_length){
        lexeme[lexeme_len] = ch;
        return 1;
    }
    else {
        printf("Number size out of bound\n"); //렉심의 크기가 지나치게 크면 예외를 발생 시킨다.
        return 0;
    }
}

void get_char(){
  //현재의 char 한 글자를 반환하고 포인터를 앞으로 이동시킴
    if (next() != EOF){
        if(isdigit(ch)){
            char_class = DIGIT;
        }
        else if (isalpha(ch)){
            char_class = UNKNOWN;
        }
        else char_class = OP; //숫자가 아닐 경우 연산자다 +,*,() 등..
    }
    else char_class = EOF;
}

void lex(){
    char lexeme[lexeme_length] = {0,};//렉심 하나 당 최대 길이는 8 (1억 자리의 연산이 마지노선) 마지막은 EOF
    int lexeme_len = 0;
    switch (char_class){
        case DIGIT:
            while (char_class == DIGIT){ //공백이나 문자를 만나기 이전까지 반복한다.
                if (!add_char(ch, lexeme, lexeme_len++)) exit(0); //예외 발생 시 종료시킨다.
                get_char();
            }
            token_class = DIGIT;
            break;

        case OP:
            if (!add_char(ch, lexeme, lexeme_len)) exit(0);
            token_class = lookup(ch); //현재 읽고 있는 글자의 연산자 타입을 지정해준다.
            get_char(); //포인터를 옮겨준다.
            break;

        case EOF:
            token_class = EOF;
            lexeme[0] = 'E';
            lexeme[1] = 'O';
            lexeme[2] = 'F';
            lexeme[3] = 0;
            break;
        
        default:
            printf("Undefined token type!\n");
            break;
    }

    // printf("Next token is: %d, Next lexeme is %s\n", token_class, lexeme);
    for(int i=0; i < lexeme_length; i++) lex_temp[i] = lexeme[i];
}

void syntax_error(){
    printf("Syntax Error!\n");
    exit(0);
}

int number(){
    int i = 0;
    char ch = lex_temp[i++];
    int result = 0;
    while (ch != 0){
        result += ch - '0';
        result *= 10;
        ch = lex_temp[i++];
    }
    return result / 10;
}

int factor() {
    int result;
    // printf("Enter factor \n");
    int flag = 1;
    if (token_class == SUB_OP){
        lex();
        flag = -1;
    }
    if (token_class == LEFT_PAREN) {
        lex();
        result = expr();
        if (token_class == RIGHT_PAREN) lex();
        else syntax_error();
    }

    else if (token_class == DIGIT){
        result = number(); //터미널로 변환을 진행
        printf("%d\n",result);
        lex();
    }

    else syntax_error();
    // printf("Exit factor \n");
    return result * flag;
}

int term() {
    // printf("Enter term \n");
    int result;
    result = factor();
    while (token_class == MULT_OP || token_class == DIV_OP) { 
        if (token_class == MULT_OP){
            lex();
            result *= factor(); 
        }
        if (token_class == DIV_OP){
            lex();
            result /= factor();
        }
    }
    // printf("Exit term \n");
    return result;
}

int expr(){
    // printf("Enter expr \n");
    int result;
    result = term();
    while (token_class == ADD_OP || token_class == SUB_OP) { 
        if (token_class == ADD_OP){
            lex();
            result += term(); 
        }
        if (token_class == SUB_OP){
            lex();
            result -= term();
        }       
    }
    // printf("Exit expr \n");
    return result;
}

int main(){
    char raw_input[sentence_length];
    scanf("%[^\n]s",raw_input);
    int sentece_index = 0;
    for(int i = 0; i < sentence_length; i++) { //공백을 제거
        if(!isspace(raw_input[i])) sentence[sentece_index++] = raw_input[i];
    }
    
    get_char();
    lex();
    printf("%d\n",expr());
    
    //에러가 발생하면 마저 파싱
    while (token_class != EOF){
        expr();
    }
    return 0;
}