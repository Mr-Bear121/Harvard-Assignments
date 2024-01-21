from cs50 import get_string

# TODO


def main():
    sentence = get_string("Text: ")
    numbSentences = 0
    wordCount = 0
    letterCount = 0
    colemansFormula = 0

    numbSentences = getSentenceNumb(sentence)
    letterCount = getLetterCount(sentence)
    wordCount = getWordCount(sentence)

    averageWord = letterCount / wordCount * 100
    avgSentence = numbSentences / wordCount * 100

    colemansFormula = round(0.0588 * averageWord - 0.296 * avgSentence - 15.8)

    if colemansFormula < 1:
        print("Before Grade 1")

    elif colemansFormula > 16:
        print("Grade 16+")
    else:
        print(f"Grade {colemansFormula}")


def getSentenceNumb(sentence):
    numberOfSentences = 0
    for character in sentence:
        if character == "." or character == "!" or character == "?":
            numberOfSentences += 1
    return numberOfSentences


def getWordCount(sentence):
    splitSentence = sentence.split()
    return len(splitSentence)


def getLetterCount(sentence):
    letterCount = 0

    for letter in sentence:
        if letter.isalpha():
            letterCount += 1
    return letterCount


if __name__ == "__main__":
    main()
