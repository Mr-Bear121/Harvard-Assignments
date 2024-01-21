#include <cs50.h>
#include <ctype.h>
#include <stdio.h>
#include <string.h>

const char alphabet[] = {'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h', 'i', 'j', 'k', 'l', 'm',
                         'n', 'o', 'p', 'q', 'r', 's', 't', 'u', 'v', 'w', 'x', 'y', 'z'};
string encrypt(string key, string plainText);
int countArr(string arr[]);
int checkKey(string key);
string upperString(string str);

int main(int argc, string argv[])
{
    int errorCode = 0;
    string plainText;
    string encryptedText = NULL;
    // sizeof() refuses to work so I have to do a workaround. looks like it hates "pointers"
    int arguementsize = countArr(argv);

    // we only want one argument.
    if (arguementsize == 1)
    {
        errorCode = checkKey(argv[1]);
        // check if the length of the argue ment is the full length of an alphabet '26'
        // check to see if the key is valid IE:'not duplicate and is aplhabetical'
        if (strlen(argv[1]) == 26 && errorCode == 0)
        {
            plainText = get_string("plaintext:");
            argv[1] = upperString(argv[1]);
            // this assumes the key is all caps
            encryptedText = encrypt(argv[1], plainText);
            printf("ciphertext:%s", encryptedText);
            printf("\n");
            return 0;
        }
        // since I have 2 checks that can cause an error;
        // I need to parse out which one it was with this check.
        else if (strlen(argv[1]) != 26)
        {
            // error message
            printf("an error has occured make sure that you have 26 characters in your key:\n");
            errorCode = 1;
            return errorCode;
        }
    }
    // since I have multiple checks that can cause an error;
    // I need to parse out which one it was with this check.
    else if (arguementsize != 1)
    {
        // error message
        printf("an error has occured check commandline input:\n");
        errorCode = 1;
        return errorCode;
    }
    return errorCode;
}

string encrypt(string key, string plainText)
{
    string stringEncoded = plainText;

    for (int letterPos = 0; plainText[letterPos]; letterPos++)
    {
        for (int character = 0; alphabet[character]; character++)
        {
            // if the character is a lowercase letter and matches a letter in the alphabet then--->
            if (islower(plainText[letterPos]) && plainText[letterPos] == alphabet[character])
            {
                //--->switch it to the character found in the provided key
                stringEncoded[letterPos] = tolower(key[character]);
                break;
            }

            // if the letter is uppercase and matches an uppercase letter --->
            else if (isupper(plainText[letterPos]) && plainText[letterPos] == toupper(alphabet[character]))
            {
                //--->switch it to the character found in the provided key as an uppercase
                stringEncoded[letterPos] = key[character];
                break;
            }
            // if its not an alphabetical character than leave it alone
            else if (!isalpha(plainText[letterPos]))
            {
                break;
            }
        }
    }
    return stringEncoded;
}

int countArr(string arr[])
{
    int size = 0;
    for (int i = 0; arr[i]; i++)
    {
        // counting without arguements yields strange results. i got a count of 61, seems to be because its dealing with a pointer
        i++;
        size = i;
    }
    return size;
}
int checkKey(string key)
{
    for (int i = 0; key[i]; i++)
    {

        for (int iTwo = i + 1; key[iTwo]; iTwo++)
        {
            // if the key is equal to the next value in the array-->
            if (key[i] == key[iTwo])
            {
                printf("key contains duplicate character. Please check input:\n");
                return 1;
            }
            // checks is a character exists in the alphabet
            else if (!isalpha(key[i]))
            {
                printf("key contains invalid character. Please check input:\n");
                // not an alphabet character
                return 1;
            }
        }
    }
    return 0;
}
string upperString(string str)
{
    for (int i = 0; str[i]; i++)
    {
        str[i] = toupper(str[i]);
    }
    return str;
}
