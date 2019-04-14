import random

def minNumberOfFlips(list, windowSize):
    oneSymbol = 1
    leastFlips = len(list)
    OnesInList = 0
    for x in list:
        if x == oneSymbol:
            OnesInList += 1
    OnesInWindow = 0
    for i in range(windowSize):
        if list[i] == oneSymbol:
            OnesInWindow += 1
    for i in range(len(list) - windowSize):
        leastFlips = min(
            (windowSize - OnesInWindow) + (OnesInList - OnesInWindow),
            leastFlips
        )
        if list[i] == oneSymbol:
            OnesInWindow -= 1
        if list[i + windowSize] == oneSymbol:
            OnesInWindow += 1
    leastFlips = min(
        (windowSize - OnesInWindow) + (OnesInList - OnesInWindow),
        leastFlips
    )
    return leastFlips



if __name__ == '__main__':
    inFile = open('zad4_input.txt', 'r')
    lines = inFile.readlines()
    resultFile = open('zad4_output.txt', 'w')
    for line in lines:
        (bin, blockSize) = line.split()
        blockSize = int(blockSize)
        list = [int(bin[i]) for i in range(0, len(bin), 1)]
        result = minNumberOfFlips(list, blockSize)
        resultFile.write('{}\n'.format(result))