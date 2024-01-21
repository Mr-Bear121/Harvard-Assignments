#include <cs50.h>
#include <math.h>
#include <stdio.h>
#include <string.h>
int getSentenceNumb(char *sentence, int length);
int getWordCount(char *sentence, int length);
int getLetterCount(char *sentence, int length);
// allocate memory based on the size of the previous sentance that you wish to copy
// this was in response with trying to work around pointers
// string allocateMemory(string mString);
int main(void)
{
    int numbSentences;
    int letterCount;
    int wordCount;
    int colemansFormula;
    string sentence = get_string("Text: ");

    numbSentences = getSentenceNumb(sentence, strlen(sentence));
    letterCount = getLetterCount(sentence, strlen(sentence));
    wordCount = getWordCount(sentence, strlen(sentence));

    float averageWord = 0.0588 * letterCount / wordCount * 100;
    float avgSentence = 0.296 * numbSentences / wordCount * 100;

    colemansFormula = round(averageWord - avgSentence - 15.8);

    if (colemansFormula < 1)
    {
        printf("Before Grade 1\n");
    }
    else if (colemansFormula > 16)
    {

        printf("Grade 16+\n");
    }
    else
    {
        printf("Grade %i\n", colemansFormula);
    }
}
int getSentenceNumb(char *sentence, int length)
{
    int numbSentences = 0;

    for (int numb = 0; numb != length; numb++)
    {
        string test = &sentence[numb];
        if (sentence[numb] == '.' || sentence[numb] == '!' || sentence[numb] == '?')
        {
            numbSentences++;
        }
    }
    return numbSentences;
}
// we have not gone over pointers and other such things in class yet... but the IDE kept complaining about them
// had to do independent research to find the solution
int getLetterCount(char *sentence, int length)
{

    int numbSentences = 0;

    for (int numb = 0; numb != length; numb++)
    {
        string test = &sentence[numb];
        // from previous experince I knew of switch case statements
        // decided to use this because there was an error I wasn`t able to solve with a reqular boolean statement
        switch (sentence[numb])
        {
            case '.':
                break;
            case '!':
                break;
            case '?':
                break;
            case ',':
                break;
            case ' ':
                break;
            case '\'':
                break;
            default:
                numbSentences++;
        }
    }
    return numbSentences;
}

int getWordCount(char *sentence, int length)
{
    string parsedSentence = strtok(sentence, " ");
    int numbWords = 0;
    while (parsedSentence != NULL)
    {
        // printf("%s\n",parsedSentence);
        parsedSentence = strtok(NULL, " ");
        numbWords++;
    }

    return numbWords;
}

/*
int getSentenceNumb(string sentence)
{
    string parsedSentence = strtok(sentence,".");
    int numbSentences = 0;
    while (parsedSentence != NULL)
    {
        //printf("%s\n",parsedSentence);
        parsedSentence = strtok(NULL,".");
        numbSentences++;
    }

    return numbSentences;
}
int getWordCount(string sentence)
{
    string parsedSentence = allocateMemory(sentence);
    parsedSentence = strtok(sentence,".");
    string parsedWords = allocateMemory(parsedSentence);
    parsedWords = strtok(sentence," ");
    int numbLetters = 0;
    while (parsedSentence != NULL)
    {
        //printf("%lu\n",strlen(parsedSentence));
        numbLetters += strlen(parsedWords);
        parsedWords = strtok(NULL," ");
        if (parsedWords == NULL)
        {
                parsedSentence = strtok(NULL,".");
                parsedWords = allocateMemory(parsedSentence);
                parsedWords = strtok(sentence," ");
        }

    }

    return numbLetters;
}
string allocateMemory(string mString){
    // calloc requests a new spot in memory to fill.
    return calloc(strlen(mString)+1, sizeof(char));
}
*/
