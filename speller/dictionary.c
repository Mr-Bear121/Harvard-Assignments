// Implements a dictionary's functionality

#include <ctype.h>
#include <stdbool.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <strings.h>

#include "dictionary.h"


// Represents a node in a hash table
typedef struct node
{
    char word[LENGTH + 1];
    struct node *next;
} node;

// TODO: Choose number of buckets in hash table
const unsigned int N = 500;

//NOTE TO SELF!!!: you dont have to make a hash table they already created a global one
// Hash table
node *table[N];
//will be used later on for the 'size' function
unsigned int wordCount = 0;
unsigned int hashIndex = 0;

// Returns true if word is in dictionary, else false
bool check(const char *word)
{
    // TODO
    hashIndex = hash(word);
    node *ptr = table[hashIndex];
    //as long as you have a value from your linked list
    while(ptr != 0)
    {
        //check a provided word to a word from our dict
        if(strcasecmp(word,ptr->word)==0)
        {
            return true;

        }
        ptr = ptr->next;

    }

    return false;
}

// Hashes word to a number
unsigned int hash(const char *word)
{
    // TODO: Improve this hash function
    unsigned long totalNumber = 0;
    //first standardize by lowercasing the letter.. then add up the ascii values to use as an index for our table
    for(int value=0; value < strlen(word); value++)
    {
        totalNumber += tolower(word[value]);
    }

    return totalNumber % N;
    //return toupper(word[0]) - 'A';
}

// Loads dictionary into memory, returning true if successful, else false
bool load(const char *dictionary)
{
    //I am referencing the provided information from cs50
    // TODO
    //dictionary is a filepath pointing to the location of a dictionary
    char line[LENGTH + 1];
    FILE *dict = fopen(dictionary, "r");

    if(dict == NULL)
    {
        return false;
    }
    //scan the file word by word untile there is an End Of Field
    while(fscanf(dict,"%s",line) != EOF)
    {
        //use malloc to allocate memory for a new node
        node *myNode = malloc(sizeof(node));
        //if no memory allocated then return false
        if(myNode == NULL)
        {
            return false;
        }
        //copy the line from the file into my node
        strcpy(myNode->word,line);
        //get the hash value from the hash funtion and use it as an index for your hash table
        hashIndex = hash(line);
        myNode->next = table[hashIndex];
        // assign my node as a node of your hash table based on index
        table[hashIndex] = myNode;
        wordCount++;

    }

    fclose(dict);
    return true;
}

// Returns number of words in dictionary if loaded, else 0 if not yet loaded
unsigned int size(void)
{
    // TODO
    // if my word count is greater than 0 then something was loaded. return my count
    if(wordCount > 0)
    {
        return wordCount;
    }
    return 0;
}

// Unloads dictionary from memory, returning true if successful, else false
bool unload(void)
{
    // TODO
    for(int index = 0;index < N; index++)
    {
        node *ptr = table[index];
        while(ptr)
        {
            //requires a temp variable to hold my previous pointer so that I can remove it and proceed to the next node
            node *temp = ptr;
            ptr = ptr->next;
            free(temp);
        }
    }
    return true;
}
