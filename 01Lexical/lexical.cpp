#include <stdio.h>
#include <ctype.h>
#include <string.h>
#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>

#define MAX_LENGTH 300
using namespace std;
char stack[256];
int cnt = -1;

typedef enum {
    Keyword, num, identifier, symbol, comment, string_literal
} TokenType;

const char* tokenNames[] = {
    "Keyword", "Number","identifier", "symbol", "comment","string_literal"
};

typedef struct {
    TokenType type;
    char* lexeme;
} Token;
vector<Token> tokenList;



void push(char c) {
    cnt++;
    stack[cnt] = c;
}

char pop() {
    if (cnt == -1)
        return 0;
    else
        return stack[cnt--];
}

char top() {
    if (cnt > -1) {
        return stack[cnt];
    } else
        return 0;
}

int isempty() {
    if (cnt == -1)
        return 1;
    else
        return 0;
}

int chck_brakects(char* ae) {
    int len = strlen(ae);
    int i = 0, k;
    while (i < len) {
        k = isempty();
        if (ae[i] == '(' || ae[i] == '[' || ae[i] == '{')
            push(ae[i]);
        else if (k == 0 && top() == '(' && ae[i] == ')')
            pop();
        else if (k == 0 && top() == '[' && ae[i] == ']')
            pop();
        else if (k == 0 && top() == '{' && ae[i] == '}')
            pop();
        else if ((k == 1 && ae[i] == ')') || ae[i] == ']' || ae[i] == '}')
            return 0;
        i++;
    }
    if (isempty())
        return 1;
    return 0;
}

int isSymbol(char c) {
    return strchr("+-*/=;(){}[]<>#", c) != NULL;
}

int isKeyword(char* str) {
    char keywords[16][10] = {"break","char", "const", "continue","double", "float",
                            "int", "long", "printf", "return",
                            "stdioh",
                             "void", "while", "include", "main"};
    for (int i = 0; i < 34; ++i) {
        if (strcmp(keywords[i], str) == 0) {
            return 1;
        }
    }
    return 0;
}

void tokenize(const char* input) {
    char token[MAX_LENGTH] = {0};
    int i = 0, j = 0;
    bool inSingleLineComment = false;
    bool inMultiLineComment = false;
    bool inStringLiteral = false;
    tokenList.reserve(1000);
    while (input[i] != '\0') {
        if (input[i] == '/' && input[i + 1] == '/') {
            inSingleLineComment = true;
            j = 0;
            while (input[i] != '\n') {
                token[j++] = input[i++];
            }
            token[j] = '\0';
            inSingleLineComment = false;
            Token newToken;
            newToken.type = comment;
            newToken.lexeme = strdup(token);
            tokenList.push_back(newToken);
            j = 0;
        } else if (input[i] == '/' && input[i + 1] == '*') {
            inMultiLineComment = true;
            j = 0;
            while (!(input[i] == '*' && input[i + 1] == '/')) {
                token[j++] = input[i++];
            }
            token[j] = '\0';
            i += 2;
            inMultiLineComment = false;
            Token newToken;
            newToken.type = comment;
            newToken.lexeme = strdup(token);
            tokenList.push_back(newToken);
            j = 0;
        } else if (input[i-1] == '\"') {
            inStringLiteral = true;
            j = 0;
            while (input[i] != '\"') {
                token[j++] = input[i++];
            }
            token[j] = '\0';
            i += 1;
            inStringLiteral = false;
            Token newToken;
            newToken.type = string_literal;
            newToken.lexeme = strdup(token);
            tokenList.push_back(newToken);
            j = 0;
        }

        if (!inSingleLineComment && !inMultiLineComment && !inStringLiteral) {
             if (isalnum(input[i])) {
                 token[j++] = input[i];
             } else if (isSymbol(input[i]) || input[i] == ' ' || input[i] == '\n') {
                 if (j != 0) {
                     token[j] = '\0';
                     Token newToken;

                     if (isdigit(token[0])) {
                         newToken.type = num;
                     } else if (isKeyword(token)) {
                         newToken.type = Keyword;
                     } else {
                         newToken.type = identifier;
                     }

                     newToken.lexeme = strdup(token);
                     tokenList.push_back(newToken);
                     j = 0;
                 }
                 if (isSymbol(input[i])) {
                     Token newToken;
                     newToken.type = symbol; // Assign symbol type to all symbols
                     char symbol[2] = {input[i], '\0'};
                     newToken.lexeme = strdup(symbol);
                     tokenList.push_back(newToken);
                 }
             }
         }
         i++;
     }
}

int main() {
    const char* input;
        ifstream f("Input.c"); //taking file as inputstream
        string str;
        if(f) {
            ostringstream ss;
            ss << f.rdbuf(); // reading data
            str = ss.str();
        }
        input = str.c_str();
    size_t length= strlen(input);
    char* myString = (char*)malloc(sizeof(char) * (length + 1)); // Allocating memory
    strcpy(myString, input); // Copying the content
    if (chck_brakects(myString) == 1) {
    
        tokenize(input);
        ofstream outputFile("output.txt", ios::app); // Output file stream in append mode
           if (outputFile.is_open()) {
               for (size_t i = 0; i < tokenList.size(); ++i) {
                   outputFile << tokenNames[tokenList[i].type] << " : " << tokenList[i].lexeme << "\n";
                   free(tokenList[i].lexeme);
               }
               outputFile.close();
               cout << "Output has been appended to 'output.txt'." << endl;
           } else {
               cout << "Unable to open the output file." << endl;
           }
    }
    else
       printf("Bracket is missing");

    



    return 0;
}