#include <stdio.h>
#include <string.h>
#include "lexiLR.hpp"

using namespace std;
struct ProductionRule
{
    char left[100];
    char right[100];
} Rules[100];

int ReadRules();

int main()
{
    // read input string from file
    char *str = 0;
    char fileName[] = "input.c";
    FILE * foutput = fopen("Output.txt","w");

    if (readFile(fileName, str)){return 1;} // Error reading file

    // string in1 = "while (33<2){ printf (\"fds\", x); x=2+3;}";
    string output = lexAndReplace(str);
    
    char tmp[1000];

    strcpy(tmp, output.c_str()); // Copying the content

    remove_whitespace(tmp, remove_space);
    cout << tmp << '\n';

    char input[2000], stack[500], temp[500], ch[20], *token1, *token2, *substring;
    int i, j, stack_length, substring_length, stack_top, rule_count;

    stack[0] = '\0';

    rule_count = ReadRules();

    strcpy(input, tmp);

    i = 0;
    while (1)
    {
        if (i == 0)
        {
            fprintf(foutput,"\nStack\t\t\t\t\tInput\tAction\n");
        }

        // If there are more characters in the input string, add the next character to the stack
        if (i < strlen(input))
        {
            ch[0] = input[i];
            ch[1] = '\0';
            i++;
            strcat(stack, ch);
            fprintf(foutput,"%s\t\t\t\t\t", stack);
            for (int k = i; k < strlen(input); k++)
            {
                fprintf(foutput,"%c", input[k]);
            }
            fprintf(foutput,"\tShift %s\n", ch);
        }

        // Iterate through the production rules
        for (j = 0; j < rule_count; j++)
        {
            // Check if the right-hand side of the production rule matches a substring in the stack
            substring = strstr(stack, Rules[j].right);
            if (substring != NULL)
            {
                // Replace the matched substring with the left-hand side of the production rule
                stack_length = strlen(stack);
                substring_length = strlen(substring);
                stack_top = stack_length - substring_length;
                stack[stack_top] = '\0';
                strcat(stack, Rules[j].left);
                fprintf(foutput,"%s\t\t\t\t\t", stack);
                for (int k = i; k < strlen(input); k++)
                {
                    fprintf(foutput,"%c", input[k]);
                }
                fprintf(foutput,"\tReduce %s->%s\n", Rules[j].left, Rules[j].right);
                j = -1; // Restart the loop to ensure immediate reduction of the newly derived production rule
            }
        }

        // Check if the stack contains only the start symbol and if the entire input string has been processed
        if (strcmp(stack, Rules[0].left) == 0 && i == strlen(input))
        {
            fprintf(foutput,"\nAccepted");
            break;
        }

        // Check if the entire input string has been processed but the stack doesn't match the start symbol
        if (i == strlen(input))
        {
            fprintf(foutput,"\nNot Accepted");
            break;
        }
    }

    return 0;
}

int ReadRules()
{
    const char *fileName = "rules.txt";

    FILE *file = fopen(fileName, "r");
    if (!file)
    {
        fprintf(stderr, "Error opening file: %s\n", fileName);
        return 1;
    }
    printf("File opened successfully\n");
    int ruleIndex = 0;
    char line[256];
    while (fgets(line, sizeof(line), file))
    {
        // Skip empty lines or lines starting with '\n'
        if (line[0] == '\n' || line[0] == '\0')
            continue;

        char *arrowPos = strstr(line, "->");
        if (!arrowPos)
        {
            fprintf(stderr, "Invalid rule: %s\n", line);
            continue;
        }

        *arrowPos = '\0'; // Separate left and right sides
        char *left = line;
        char *right = arrowPos + 2;

        size_t len = strlen(right);
        if (len > 0 && right[len - 1] == '\n')
        {
            right[len - 1] = '\0'; // Remove the trailing '\n'
        }

        strncpy(Rules[ruleIndex].left, left, sizeof(Rules[ruleIndex].left) - 1);
        Rules[ruleIndex].left[sizeof(Rules[ruleIndex].left) - 1] = '\0';

        strncpy(Rules[ruleIndex].right, right, sizeof(Rules[ruleIndex].right) - 1);
        Rules[ruleIndex].right[sizeof(Rules[ruleIndex].right) - 1] = '\0';

        ruleIndex++;
    }

    fclose(file);

    // Printing the rules for verification
    for (int i = 0; i < ruleIndex; ++i)
    {
        printf("Rule %d: |%s| -> |%s|\n", i + 1, Rules[i].left, Rules[i].right);
    }
    return ruleIndex;
}
