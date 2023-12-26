#include "SyntaxPhase.hpp"


int main()
{

    char *input = 0;
    char fileName[] = "While.c";

    if (readFile(fileName, input)){return 1;} // Error reading file

    // char input[] = "while (ahmed < mohamed) {ahmed++;\nx=y* --z +2;printf(\"Mohamed\");i++;}";

    printf("%s \n\n\n\n", input);
    // start to process your data / extract strings here...
    char result[2000] = "";
    remove_whitespace(input, remove_space);
    printf("%s \n\n\n\n", input);

    insertCharacters(input, result);

    printf("%s \n\n\n",result);

    parseCFG(result, 0);
    free(input);

    return 0;
}