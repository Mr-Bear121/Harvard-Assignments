#include <cs50.h>
#include <stdio.h>

int main(void)
{
    int startSize = 0;
    int endSize = 0;
    int deadLlamas = 0;
    int newLlamas = 0;
    int currentPopulation = 0;
    int years = 0;
    do{
        // TODO: Prompt for start size
        startSize = get_int("What is the starting size of llamas? ");

    }
    while(startSize < 9);

    do
    {
        // TODO: Prompt for end size
        endSize = get_int("What is the ending size of llamas? ");
    }
    while(startSize > endSize);
    currentPopulation = startSize;
    while (currentPopulation < endSize)
    {
        // TODO: Calculate number of years until we reach threshold
        newLlamas = currentPopulation/3;
        deadLlamas = currentPopulation/4;
        currentPopulation += newLlamas - deadLlamas;
        years++;
    }
    // TODO: Print number of years
    printf("Years: %i\n",years);
}
