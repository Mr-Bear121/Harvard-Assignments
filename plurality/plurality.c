#include <cs50.h>
#include <ctype.h>
#include <stdio.h>
#include <string.h>

// Max number of candidates
#define MAX 9

// Candidates have name and vote count
typedef struct
{
    string name;
    int votes;
} candidate;

// Array of candidates
candidate candidates[MAX];

// Number of candidates
int candidate_count;

// Function prototypes
bool vote(string name);
void print_winner(void);

int main(int argc, string argv[])
{
    // Check for invalid usage
    if (argc < 2)
    {
        printf("Usage: plurality [candidate ...]\n");
        return 1;
    }

    // Populate array of candidates
    candidate_count = argc - 1;
    if (candidate_count > MAX)
    {
        printf("Maximum number of candidates is %i\n", MAX);
        return 2;
    }
    for (int i = 0; i < candidate_count; i++)
    {
        candidates[i].name = argv[i + 1];
        candidates[i].votes = 0;
    }

    int voter_count = get_int("Number of voters: ");

    // Loop over all voters
    for (int i = 0; i < voter_count; i++)
    {
        string name = get_string("Vote: ");

        // Check for invalid vote
        if (!vote(name))
        {
            printf("Invalid vote.\n");
        }
    }

    // Display winner of election
    print_winner();
}

// Update vote totals given a new vote
bool vote(string name)
{
    // TODO
    char *cName = name;
    // printf("%s\n", candidates[0].name);
    for (int candidate = 0; candidate != sizeof(candidates[candidate]); candidate++)
    {
        string Player = candidates[candidate].name;
        // if(strcmp(candidates[candidate].name, name) == 0)
        if (Player == NULL)
        {
            return false;
        }
        else if (strcmp(Player, cName) == 0)
        {
            candidates[candidate].votes += 1;
            return true;
        }
    }
    return false;
}

// Print the winner (or winners) of the election
void print_winner(void)
{
    // there will always be at least 1 candidate or else its not an election
    string winner = candidates[0].name;
    int tie = 0;
    int winnerVotes = candidates[0].votes;
    // TODO
    for (int candidate = 0; candidate != sizeof(candidates) - 1; candidate++)
    {

        if (winnerVotes < candidates[candidate + 1].votes)
        {
            winner = candidates[candidate + 1].name;
            winnerVotes = candidates[candidate + 1].votes;
        }
        else if (winnerVotes == candidates[candidate + 1].votes)
        {
            tie = 1;
            printf("%s\n", winner);
            printf("\n");
            printf("%s\n", candidates[candidate + 1].name);
            printf("\n");
        }
    }
    // if there is no tie then print the winner otherwise winner has already been printed
    if (tie == 0)
    {
        printf("%s\n", winner);
    }

    return;
}