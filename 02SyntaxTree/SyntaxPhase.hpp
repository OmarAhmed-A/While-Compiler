#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <iostream>
// Function prototypes
void parseStatement(char *input, int depth);
void parseWhileLoop(char *input, int depth);
void parseExpression(char *input, int depth);
void parseCompoundStatement(char *input, int depth);
void parseStatements(char *input, int depth);
void parseOtherStatement(char *input, int depth);
void parseExpressionStatement(char *input, int depth);
void parsePrintStatement(char *input, int depth);
void parseIdentifiers(char *input, int depth);
void parseNumber(char *input, int depth);
void parseoperator(char *input, int depth);
void parseStringLiteral(char *input, int depth);
int readFile(char *fname, char *&input);
void parsenumOrIdentifier(char *input, int depth);
void insertCharacters(char *input, char *result);
void parseCFG(char *input, int depth);
void removeWhiteSpace(char *str);

using namespace std;

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

int isWordCharacter(char c)
{
    return isalpha(c) || c == '_';
}

void insertCharacters(char *input, char *result)
{
    // char result[500] = ""; // Adjust the size according to your needs
    int idx = 0;
    int len = strlen(input);
    int inPrintfString = 0;
    int length;

    for (int i = 0; i < len; ++i)
    {

        if (strncmp(input + i, "printf(\"", 8) == 0)
        {
            length = 0;
            while (input[i + length + 8] != '\"')
            {
                length++;
            }

            inPrintfString = 1;
            strcat(result, "printf(\""); // Copy 'printf(\"' without modification
            idx += 8;
            i += 8;
        }
        else if (input[i] == '\"' && strncmp(input + i - length - 8, "printf(\"", 8) == 0)
        {
            inPrintfString = 0;
            // result[idx++] = '\"'; // Copy closing quote of printf string
        }

        if (inPrintfString)
        {
            result[idx++] = input[i]; // Copy characters inside printf string
        }
        else if (isWordCharacter(input[i]))
        {
            int wordStart = i;
            while (isWordCharacter(input[i]))
            {
                ++i;
            }
            int wordEnd = i;

            char word[50] = "";
            strncpy(word, input + wordStart, wordEnd - wordStart);
            word[wordEnd - wordStart] = '\0';

            if (strcmp(word, "while") == 0)
            {
                strcat(result, word); // Add 'while' without 'i'
                idx += strlen(word);
            }
            else
            {
                result[idx++] = 'i'; // Insert 'i' before a word
                strcat(result, word);
                idx += strlen(word);
            }
            --i;
        }
        else if (isdigit(input[i]))
        {
            result[idx++] = 'n'; // Insert 'n' before a number
            while (isdigit(input[i]))
            {
                result[idx++] = input[i++];
            }
            --i;
        }
        else
        {
            result[idx++] = input[i];
        }
    }
    result[idx] = '\0';

    printf("Modified String: %s\n", result);
}

// Function to print indentation based on recursion depth
void printIndent(int depth)
{
    for (int i = 0; i < depth; ++i)
    {
        printf("  ");
    }
}

// Function to parse and display the recursive structure of the CFG
void parseCFG(char *input, int depth)
{
    parseStatement(input, depth);
}

void parseStatement(char *input, int depth)
{
    printIndent(depth);
    printf("Statement\n");
    if (strstr(input, "while") == input)
    {
        parseWhileLoop(input, depth + 1);
    }
    else if (strstr(input, "printf") == input)
    {
        parsePrintStatement(input, depth + 1);
    }
    else
    {
        parseExpressionStatement(input, depth + 1);
    }
}

void parseWhileLoop(char *input, int depth)
{
    printIndent(depth);
    printf("WhileLoop\n");

    // Assuming 'while' '(' Expression ')' CompoundStatement
    char *start = strstr(input, "(");
    if (start == NULL)
    {
        printf("Error: Start of while loop not found\n");
        return;
    }
    char *end = strstr(input, ")");
    if (end == NULL)
    {
        printf("Error: End of while loop not found\n");
        return;
    }
    if (start && end)
    {
        // Extracting the Expression between '(' and ')'
        char expression[100];
        strncpy(expression, start + 1, end - start - 1);
        expression[end - start - 1] = '\0';

        parseExpression(expression, depth + 1);
    }
    else
    {
        printf("Error: Start or End of while loop not found\n");
    }

    // Move to the next part of the WhileLoop
    char *compoundStart = strstr(end, "{");

    if (compoundStart)
    {
        parseCompoundStatement(compoundStart, depth + 1);
    }
    else
    {
        printf("Error: { not found\n");
    }
}

void parseExpression(char *input, int depth)
{
    printIndent(depth);
    // printf("salma: %s\n", input);
    printf("Expression\n");

    char *operators[] = {"<=", ">=", ">", "<", "==", "!=", "+", "-", "*", "/", NULL}; // List of supported operator1s

    for (int i = 0; operators[i] != NULL; ++i)
    {
        char *operator1 = strstr(input, operators[i]);
        if (operator1)
        {
            // Extracting Identifiers and Number
            size_t operatorLen = strlen(operators[i]);
            char left[50], right[50];

            // 1.if condition hena ya2oly dah number wala la2a

            strncpy(left, input, operator1 - input); // llidentifier
            left[operator1 - input] = '\0';

            strcpy(right, operator1 + operatorLen); // llnumber

            // ba send el batal3o
            parsenumOrIdentifier(left, depth + 1);
            parseoperator(operators[i], depth + 1);
            parsenumOrIdentifier(right, depth + 1);
            return; // Stop after parsing the first occurrence of an operator1
        }
    }
}

void parseCompoundStatement(char *input, int depth)
{
    printIndent(depth);
    printf("CompoundStatement\n");

    // Assuming '{' Statements '}'
    char *statementsStart = strstr(input, "{");
    if (statementsStart == NULL)
    {
        printf("Error: { not found\n");
        return;
    }

    char *statementsEnd = strstr(input, "}");
    if (statementsEnd == NULL)
    {
        printf("Error: } not found\n");
        return;
    }
    if (statementsStart && statementsEnd)
    {
        // Extracting Statements between '{' and '}'
        char statements[100];
        strncpy(statements, statementsStart + 1, statementsEnd - statementsStart - 1);
        statements[statementsEnd - statementsStart - 1] = '\0';

        parseStatements(statements, depth + 1);
    }
    else
    {
        printf("Error: { or } not found\n");
    }
    // 2.if()if wa7da ah aw wa7da la yab2aa error yab2a el curly brackets msh ma2fola
}

void parseStatements(char *input, int depth)
{
    printIndent(depth);
    printf("Statements\n");
    // input:i++;printf(\"%d\\n\", i);ii=z3;

    // Assuming Statement Statements | Îµ
    char *statementDelimiter = strstr(input, ";");
 

    if (statementDelimiter)
    {
        // Extracting the first Statement
        char statement[100];
        strncpy(statement, input, statementDelimiter - input);

        statement[statementDelimiter - input] = '\0';

        parseStatement(statement, depth + 1);

        // Move to the next part of Statements
        parseStatements(statementDelimiter + 1, depth + 1);
    }

}

void parseExpressionStatement(char *input, int depth)
{
    printIndent(depth);
    printf("Expression Statement\n");

    char *current = input;
    while (*current != '\0')
    {
        // printf("%s\n",current);
        if (*current == 'i' || *current == 'n')
        {
            char *start = current;
            while (!ispunct(*current) && (isalnum(*current) || *current == '_'))
            {
                current++;
            }
            int startsize = strlen(start);
            int currentsize = strlen(current);
            int len = startsize - currentsize;
            string s = start;
            s = s.substr(0, len);
            char *send = new char[s.length() + 1];
            strcpy(send, s.c_str());

            if (*start == 'i')
            {
                parsenumOrIdentifier(send, depth + 1);
            }
            else
            {
                parsenumOrIdentifier(send, depth + 1);
            }
        }
        else if (ispunct(*current))
        {
            char *operatorz = current;
            while (*current != '\0' && ispunct(*current))
            {
                current++;
            }
            int startsize = strlen(operatorz);
            int currentsize = strlen(current);
            int len = startsize - currentsize;
            string s = operatorz;
            s = s.substr(0, len);
            char *send = new char[s.length() + 1];
            strcpy(send, s.c_str());
            parseoperator(send, depth + 1);
        }
        else
        {
            current++;
        }
    }
}

// Add more conditions based on your grammar rules
void parsePrintStatement(char *input, int depth)
{
    printIndent(depth);
    printf("PrintStatement\n");

    // Assuming 'printf' '(' StringLiteral ', ' Identifier ')' ';'
    char *stringLiteralStart = strstr(input, "\"");
    if (stringLiteralStart == NULL)
    {
        printf("Error: StringLiteralStart is null\n");
    }

    char *stringLiteralEnd = strstr(stringLiteralStart + 1, "\"");
    if (stringLiteralEnd == NULL)
    {
        printf("Error: StringLiteralEnd is null\n");
    }

    // Extracting StringLiteral between '"' and '"'
    if (stringLiteralStart && stringLiteralEnd)
    {
        char stringLiteral[100];
        strncpy(stringLiteral, stringLiteralStart + 1, stringLiteralEnd - stringLiteralStart - 1);
        stringLiteral[stringLiteralEnd - stringLiteralStart - 1] = '\0';

        parseStringLiteral(stringLiteral, depth + 1);
    }
    else
    {
        printf("Error: StringLiteralStart or StringLiteralEnd is null\n");
    }

    // Assuming ',' Identifier ')' ';'
    char *identifierStart = strstr(stringLiteralEnd + 1, ",");
    if (identifierStart == NULL)
    {
        printf("Error: IdentifierStart is null\n");
    }

    char *identifierEnd = strstr(identifierStart + 1, ")");
    if (identifierEnd == NULL)
    {
        printf("Error: IdentifierEnd is null\n");
    }

    // Extracting Identifier between ',' and ')'
    if (identifierStart && identifierEnd)
    {
        char identifier[50];
        strncpy(identifier, identifierStart + 1, identifierEnd - identifierStart - 1);
        identifier[identifierEnd - identifierStart - 1] = '\0';

        parsenumOrIdentifier(identifier, depth + 1);
    }
    else
    {
        printf("Error: IdentifierStart or IdentifierEnd is null\n");
    }
}

void parsenumOrIdentifier(char *input, int depth)
{
    const char *tok = " ";
    input = strtok(input, tok);
    char first = input[0];
    char *rest = input + 1;

    if (first == 'i')
    {
        parseIdentifiers(rest, depth);
    }
    else if (first == 'n')
    {
        parseNumber(rest, depth);
    }
    else
    {
        printf("The first character is neither 'i' nor 'n'.\n");
    }
}

void parseIdentifiers(char *input, int depth)
{
    printIndent(depth);
    printf("Identifiers: %s\n", input);
}

void parseNumber(char *input, int depth)
{
    printIndent(depth);
    printf("Number: %s\n", input);
}

void parseoperator(char *input, int depth)
{
    printIndent(depth);
    if (strcmp(input, "=") == 0)
    {
        printf("assignment: %s\n", input);
    }
    else if (strcmp(input, "++") == 0)
    {
        printf("increment: %s\n", input);
    }
    else if (strcmp(input, "--") == 0)
    {
        printf("decrement: %s\n", input);
    }
    else if (strcmp(input, ">=") == 0 || strcmp(input, "<=") == 0 || strcmp(input, "<") == 0 || strcmp(input, ">") == 0)
    {
        printf("comparison: %s\n", input);
    }
    else
    {
        printf("operator: %s\n", input);
    }
}

void parseStringLiteral(char *input, int depth)
{
    printIndent(depth);
    printf("StringLiteral: %s\n", input);
}