import csv
import sys


def main():
    csvDataBase = None
    sequenceFile = None
    reader = None
    strSeq = ""
    dnaData = []

    # TODO: Check for command-line usage
    if len(sys.argv) != 3:
        sys.exit(1)

    # TODO: Read database file into a variable
    csvDataBase = open(sys.argv[1], "r")
    reader = csv.DictReader(csvDataBase)
    for row in reader:
        # appends our data that is found in our database to a list
        # note: our data is in dictionary format
        dnaData.append(row)

    # TODO: Read DNA sequence file into a variable
    sequenceFile = open(sys.argv[2], "r")
    # since there is only 1 long textual line this is read as a string
    strSeq = sequenceFile.read()
    # is accessing a list that contains dictionaries aka "each row"
    subSequences = list(dnaData[0].keys())[1:]

    # TODO: Find longest match of each STR in DNA sequence
    result = {}
    for s in subSequences:
        # note: subSequences is now containing a list of keys in dna format
        result[s] = longest_match(strSeq, s)

    # TODO: Check database for matching profiles
    for person in dnaData:
        match = 0
        for s in subSequences:
            if int(person[s]) == result[s]:
                match += 1
        if match == len(subSequences):
            print(person["name"])
            return
    print("no Match")
    return


def longest_match(sequence, subsequence):
    """Returns length of longest run of subsequence in sequence."""

    # Initialize variables
    longest_run = 0
    subsequence_length = len(subsequence)
    sequence_length = len(sequence)

    # Check each character in sequence for most consecutive runs of subsequence
    for i in range(sequence_length):
        # Initialize count of consecutive runs
        count = 0

        # Check for a subsequence match in a "substring" (a subset of characters) within sequence
        # If a match, move substring to next potential match in sequence
        # Continue moving substring and checking for matches until out of consecutive matches
        while True:
            # Adjust substring start and end
            start = i + count * subsequence_length
            end = start + subsequence_length

            # If there is a match in the substring
            if sequence[start:end] == subsequence:
                count += 1

            # If there is no match in the substring
            else:
                break

        # Update most consecutive matches found
        longest_run = max(longest_run, count)

    # After checking for runs at each character in seqeuence, return longest run found
    return longest_run


main()
