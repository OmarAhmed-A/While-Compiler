#include <iostream>
#include <string>
#include <cctype>
#include <vector>
#include <map>

using namespace std;

enum class TokenType {
    Identifier,
    Number,
    KeywordWhile,
    KeywordPrintf,
    StringLiteral,
    Operator,
    Increment,
    Decrement,
    Unknown
};

map<TokenType, string> tokenNames = {
    {TokenType::Identifier, "Id"},
    {TokenType::Number, "Num"},
    {TokenType::KeywordWhile, "KeywordWhile"},
    {TokenType::KeywordPrintf, "KeywordPrintf"},
    {TokenType::StringLiteral, "String"},
    {TokenType::Operator, "Op"},
    {TokenType::Increment, "Increment"},
    {TokenType::Decrement, "Decrement"},
    {TokenType::Unknown, "Unknown"}
};

struct Token {
    TokenType type;
    string value;
    size_t start;
    size_t end;
};

int readFile(char *fname, char *&input)
{
    long length;
    FILE *f = fopen(fname, "rb");
    if (f)
    {
        fseek(f, 0, SEEK_END);
        length = ftell(f);
        fseek(f, 0, SEEK_SET);
        input = (char *)malloc(length + 5);
        input[length] = '\0';

        if (input)
        {
            fread(input, 1, length, f);
            fclose(f);
            return 0; // Success
        }
        else
        {
            printf("Failed to allocate memory for input.\n");
        }
        fclose(f);
    }
    else
    {
        printf("Failed to open file: %s\n", fname);
    }
    return 1; // Failure
}

void remove_whitespace(char *str, void (*modify)(char *))
{
    int i, j = 0;
    for (i = 0; str[i] != '\0'; i++)
    {
        if (!isspace(str[i]))
        {
            str[j] = str[i];
            j++;
        }
        else
        {
            modify(&str[i]);
        }
    }
    str[j] = '\0';
}
void remove_space(char *ch)
{
    *ch = '\0';
}

string lexAndReplace(const string& input) {
    vector<Token> tokens;
    string token;
    TokenType currentType = TokenType::Unknown;
    bool inString = false;
    size_t start = 0;

    for (size_t i = 0; i < input.size(); ++i) {
        char c = input[i];
        if (c == '"') {
            if (inString) {
                tokens.push_back({TokenType::StringLiteral, token, start, i + 1});
                token.clear();
                inString = false;
            } else {
                if (!token.empty()) {
                    if (token == "while") {
                        tokens.push_back({TokenType::KeywordWhile, token, start, i});
                    } else if (token == "printf") {
                        tokens.push_back({TokenType::KeywordPrintf, token, start, i});
                    } else {
                        tokens.push_back({currentType, token, start, i});
                    }
                    token.clear();
                }
                inString = true;
                start = i;
            }
            currentType = TokenType::Unknown;
        } else if (inString) {
            token += c;
        } else if (std::isalpha(c)) {
            if (currentType == TokenType::Identifier || currentType == TokenType::Unknown) {
                if (currentType == TokenType::Unknown) {
                    start = i;
                }
                currentType = TokenType::Identifier;
                token += c;
            } else {
                tokens.push_back({currentType, token, start, i});
                token = c;
                start = i;
                currentType = TokenType::Identifier;
            }
        } else if (std::isdigit(c)) {
            if (currentType == TokenType::Number || currentType == TokenType::Unknown) {
                if (currentType == TokenType::Unknown) {
                    start = i;
                }
                currentType = TokenType::Number;
                token += c;
            } else {
                tokens.push_back({currentType, token, start, i});
                token = c;
                start = i;
                currentType = TokenType::Number;
            }
        } else if (std::isspace(c)) {
            if (!token.empty()) {
                if (token == "while") {
                    tokens.push_back({TokenType::KeywordWhile, token, start, i});
                } else if (token == "printf") {
                    tokens.push_back({TokenType::KeywordPrintf, token, start, i});
                } else {
                    tokens.push_back({currentType, token, start, i});
                }
                token.clear();
            }
            currentType = TokenType::Unknown;
        } else if (c == '+' || c == '-' || c == '*' || c == '/' || c == '>' || c == '=' || c == '<') {
            if (!token.empty()) {
                tokens.push_back({currentType, token, start, i});
                token.clear();
            }
            if (c == '+' || c == '-' || c == '*' || c == '/' || c == '>' || c == '<' || c == '=') {
                token = c;
                currentType = TokenType::Operator;
            }
            tokens.push_back({currentType, token, i, i + token.size()});
            token.clear();
            currentType = TokenType::Unknown;
        } else if (c == '(' || c == ')' || c == '{' || c == '}' || c == ';') {
            if (!token.empty()) {
                tokens.push_back({currentType, token, start, i});
                token.clear();
            }
            tokens.push_back({TokenType::Unknown, string(1, c), i, i + 1});
            currentType = TokenType::Unknown;
        } else {
            // Handle other cases as needed
        }
    }

    if (!token.empty()) {
        if (token == "while") {
            tokens.push_back({TokenType::KeywordWhile, token, start, input.size()});
        } else if (token == "printf") {
            tokens.push_back({TokenType::KeywordPrintf, token, start, input.size()});
        } else {
            tokens.push_back({currentType, token, start, input.size()});
        }
    }

    string output = input;
    size_t offset = 0;
    for (const auto& token : tokens) {
        string replacement = tokenNames[token.type];
        if (token.type != TokenType::Increment && token.type != TokenType::Unknown && token.type != TokenType::Decrement) {
            output.replace(token.start + offset, token.end - token.start, replacement);
            offset += replacement.size() - (token.end - token.start);
        } 
    }

    return output;
}


