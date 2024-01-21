#include <cs50.h>
#include <stdio.h>
#include <string.h>

// Max voters and candidates
#define MAX_VOTERS 100
#define MAX_CANDIDATES 9

// preferences[i][j] is jth preference for voter i
int preferences[MAX_VOTERS][MAX_CANDIDATES];

// Candidates have name, vote count, eliminated status
typedef struct
{
    string name;
    int votes;
    bool eliminated;
} candidate;

// Array of candidates
candidate candidates[MAX_CANDIDATES];

// Numbers of voters and candidates
int voter_count;
int candidate_count;

// Function prototypes
bool vote(int voter, int rank, string name);
void tabulate(void);
bool print_winner(void);
int find_min(void);
bool is_tie(int min);
void eliminate(int min);

int main(int argc, string argv[])
{
    // Check for invalid usage
    if (argc < 2)
    {
        printf("Usage: runoff [candidate ...]\n");
        return 1;
    }

    // Populate array of candidates
    candidate_count = argc - 1;
    if (candidate_count > MAX_CANDIDATES)
    {
        printf("Maximum number of candidates is %i\n", MAX_CANDIDATES);
        return 2;
    }
    for (int i = 0; i < candidate_count; i++)
    {
        candidates[i].name = argv[i + 1];
        candidates[i].votes = 0;
        candidates[i].eliminated = false;
    }

    voter_count = get_int("Number of voters: ");
    if (voter_count > MAX_VOTERS)
    {
        printf("Maximum number of voters is %i\n", MAX_VOTERS);
        return 3;
    }

    // Keep querying for votes
    for (int i = 0; i < voter_count; i++)
    {

        // Query for each rank
        for (int j = 0; j < candidate_count; j++)
        {
            string name = get_string("Rank %i: ", j + 1);

            // Record vote, unless it's invalid
            if (!vote(i, j, name))
            {
                printf("Invalid vote.\n");
                return 4;
            }
        }

        printf("\n");
    }

    // Keep holding runoffs until winner exists
    while (true)
    {
        // Calculate votes given remaining candidates
        tabulate();

        // Check if election has been won
        bool won = print_winner();
        if (won)
        {
            break;
        }

        // Eliminate last-place candidates
        int min = find_min();
        bool tie = is_tie(min);

        // If tie, everyone wins
        if (tie)
        {
            for (int i = 0; i < candidate_count; i++)
            {
                if (!candidates[i].eliminated)
                {
                    printf("%s\n", candidates[i].name);
                }
            }
            break;
        }

        // Eliminate anyone with minimum number of votes
        eliminate(min);

        // Reset vote counts back to zero
        for (int i = 0; i < candidate_count; i++)
        {
            candidates[i].votes = 0;
        }
    }
    return 0;
}

// Record preference if vote is valid
bool vote(int voter, int rank, string name)
{

    // TODO
    for (int j = 0; candidates[j].name != NULL; j++)
    {

        if (strcmp(name, candidates[j].name) == 0)
        {
            // according to the problem it says we need to store the candidate index and not name...
            // since this is replicating a ballot I figured name would be more efficient.. but you da boss.
            preferences[voter][rank] = j;
            return true;
        }
    }

    return false;
}

// Tabulate votes for non-eliminated candidates
void tabulate(void)
{
    // TODO
    // gather up the ballots
    for (int voter = 0; voter != voter_count; voter++)
    {
        for (int candid = 0; candid != candidate_count; candid++)
        {
            int vote = 0;
            // if the candidate is still in the race then count their vote
            if (!(candidates[candid].eliminated))
            {
                // if a voter has a vote
                if ((preferences[voter][candid] != -1))
                {
                    // take their vote, "vote is an index"
                    vote = preferences[voter][candid];
                    // distribute it to the relavant candidate
                    candidates[vote].votes += 1;
                    // mark their vote as counted.
                    preferences[voter][candid] = -1;
                    break;
                }
            }
        }
    }
    return;
}

// Print the winner of the election, if there is one
bool print_winner(void)
{
    // TODO
    int voteTotal = 0;
    int numberOfContenders = 0;
    float winningTotal = 0;
    // gather up the total votes
    do
    {
        voteTotal += candidates[numberOfContenders].votes;
        numberOfContenders++;
    }
    while (candidates[numberOfContenders].name != NULL);

    winningTotal = voteTotal / 2;

    for (int contender = 0; contender != candidate_count; contender++)
    {
        if (candidates[contender].votes > winningTotal)
        {
            printf("%s\n", candidates[contender].name);
            return true;
        }
    }

    return false;
}

// Return the minimum number of votes any remaining candidate has
int find_min(void)
{
    candidate nominee = candidates[0];
    // TODO
    for (int contender = 0; candidates[contender].name != NULL; contender++)
    {
        // since its true that they are eliminated and false they are not.... need to reverse the logic with !
        if (!(candidates[contender].eliminated))
        {
            if (nominee.votes > candidates[contender].votes)
            {
                nominee = candidates[contender];
            }
        }
    }
    return nominee.votes;
}

// Return true if the election is tied between all candidates, false otherwise
bool is_tie(int min)
{
    // TODO
    int tiedCandidates = 0;
    int eliminatedCandidates = 0;
    for (int nominee = 0; nominee != candidate_count; nominee++)
    {
        // if a nominee is still in the running and he isnt last place then ...
        if (!(candidates[nominee].eliminated) && candidates[nominee].votes != min)
        {
            return false;
        }
    }

    return true;
}

// Eliminate the candidate (or candidates) in last place
void eliminate(int min)
{
    // TODO
    for (int contender = 0; candidates[contender].name != NULL; contender++)
    {
        if (candidates[contender].votes == min)
        {
            candidates[contender].eliminated = true;
        }
    }
    return;
}