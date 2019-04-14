from collections import Counter
import random


def colors(hand):
    return [card[0] for card in hand]


def nums(hand):
    return [card[1] for card in hand]


def isPair(hand):
    hand = list(Counter(nums(hand)).values())
    return 2 in hand


def isTwoPairs(hand):
    hand = list(Counter(nums(hand)).values())
    if 2 in hand:
        hand.remove(2)
        return 2 in hand
    return False


def isThree(hand):
    hand = list(Counter(nums(hand)).values())
    return 3 in hand


def isQuads(hand):
    hand = list(Counter(nums(hand)).values())
    return 4 in hand


def isFullHouse(hand):
    hand = list(Counter(nums(hand)).values())
    if 3 in hand:
        hand.remove(3)
        return 2 in hand
    return False


def isColor(hand):
    hand = Counter(colors(hand)).values()
    return 5 in hand


def isStrit(hand):
    hand = sorted([ord(x) for x in nums(hand)])
    val = hand[0]
    hand.remove(hand[0])
    for i in range (1, len(hand)):
        if hand[i] == val + 1:
            val += 1
        else:
            return False
    return True


def isPoker(hand):
    return isColor(hand) and isStrit(hand)


def winner(data):
    resB = [fun(data['blotkarz']) for fun in checks]
    resF = [fun(data['figurant']) for fun in checks]
    for i in range(len(resB)):
        if resF[i] is True:
            return 'figurant'
        if resB[i] is True:
            return 'blotkarz'
    return 'figurant'


def genHand(set):
    res = []
    while len(res) < 5:
        x = random.choice(set)
        if not(x in res):
            res.append(x)
    return res


def run_test(matches, bl_deck, fg_deck):
    wins = {
        'blotkarz': 0,
        'figurant': 0
    }
    for i in range(matches):
        data = {
            'blotkarz': genHand(bl_deck),
            'figurant': genHand(fg_deck)
        }
        wins[winner(data)] += 1
    return (float(wins['blotkarz']) / float(matches)) * 100.0


def generate_deck(colors, nums):
    res = []
    for x in colors:
        for y in nums:
            res.append(x + y)
    return res


if __name__ == '__main__':

    colours = ['S', 'H', 'D', 'C']
    blotkarz_num = [str(i) for i in range(10)]
    figurant_num = ['A', 'K', 'D', 'J']

    blotkarz_set = []
    figurant_set = []
    for c in colours:
        for n in blotkarz_num:
            blotkarz_set.append(c + n)
        for n in figurant_num:
            figurant_set.append(c + n)

    checks = [
        isPoker,
        isQuads,
        isFullHouse,
        isColor,
        isStrit,
        isThree,
        isTwoPairs,
        isPair
    ]
    

    a = run_test(60000, blotkarz_set, figurant_set)
    print(a)

    b = run_test(60000, generate_deck(['H'], blotkarz_num), figurant_set)
    print(b)
    c = run_test(60000, generate_deck(['H'], [str(x) for x in [0, 1, 2, 3, 4]]),
        figurant_set)
    print(c)
    d = run_test(60000, generate_deck(['S', 'H'], [str(x) for x in [4, 5, 6, 7, 8, 9]]),
        figurant_set)
    print(d)
