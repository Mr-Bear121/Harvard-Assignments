#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

#define BLOCK_SIZE 512
typedef uint8_t BYTE;

int erroneousInput(int argc);
// checks to see if there is a file opened. if there isnt then print an error message with the file name that was attempted to be
// opened.
int isFileNull(FILE *file, char *fileName);
void recoverFiles(FILE **recoverFile, char recoveryName[], FILE **outputFile);
int main(int argc, char *argv[])
{
    FILE *recoverFile;
    char *recoverFileName;
    FILE *outputFile = NULL;
    char fileName[8];

    if (erroneousInput(argc) == 1)
    {
        return 1;
    }

    recoverFileName = argv[1];
    recoverFile = fopen(recoverFileName, "r");

    if (isFileNull(recoverFile, recoverFileName) == 1)
    {
        return 1;
    }

    recoverFiles(&recoverFile, fileName, &outputFile);

    fclose(recoverFile);
    if (outputFile != 0)
    {
        fclose(outputFile);
    }

    return 0;
}

// returns 1 or 0 for true or false respectively
int isFileNull(FILE *file, char *fileName)
{
    if (file == NULL)
    {
        printf("Could not open file %s.\n", fileName);
        return 1;
    }
    return 0;
}

int erroneousInput(int argc)
{
    if (argc != 2)
    {
        printf("Erroneous command\n");
        return 1;
    }
    return 0;
}

void recoverFiles(FILE **recoverFile, char recoveryName[], FILE **outputFile)
{
    uint8_t buffer[BLOCK_SIZE];
    // will be used to count how many jpg file we have found
    int count = 0;
    while (fread(buffer, BLOCK_SIZE, 1, *recoverFile))
    {
        if (buffer[0] == 0xff && buffer[1] == 0xd8 && buffer[2] == 0xff && (buffer[3] & 0xf0) == 0xe0)
        {
            if (*outputFile != 0)
            {
                fclose(*outputFile);
            }
            sprintf(recoveryName, "%03d.jpg", count);
            *outputFile = fopen(recoveryName, "w");
            count++;
        }

        if (*outputFile != 0)
        {
            fwrite(buffer, BLOCK_SIZE, 1, *outputFile);
        }
    }
}
