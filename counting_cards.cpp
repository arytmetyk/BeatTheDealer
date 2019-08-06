#include <iostream>
#include <ctime>
#include <map>

const int ALL_CARDS = 312;
double fiveOfThreeteenProbability = (double) 5 / 13;
double threeOfThreeteenProbability = (double) 3 / 13;

std::map<long long int, double> resultsMap;

long long int getStatusKey(unsigned char dealerCardsNumber,// 0 to 17
                           unsigned char dealerCardsSum,// 0 to 27
                           unsigned char dealerCardsWithAce,// bool
                           unsigned char playerCardsNumber,// 0 to 21
                           unsigned char playerCardsSum,// 0 to 31
                           bool playerCardsWithAce,// bool
                           bool playerPair,// bool
                           bool playerSplittedBefore,// bool
                           bool playerStanded,// bool
                           bool playerDoubled,// bool
                           short int currentValue,//-[ALL_CARDS] to [ALL_CARDS]
                           short int cardsGiven// ALL_CARDS to 0
) {
    if (dealerCardsNumber < 0 || dealerCardsNumber > 17)
        std::cout << "dealerCardsNumber out of range: " << dealerCardsNumber;
    if (dealerCardsSum < 0 || dealerCardsSum > 27)
        std::cout << "dealerCardsSum out of range: " << dealerCardsSum;
    if (playerCardsNumber < 0 || playerCardsNumber > 21)
        std::cout << "playerCardsNumber out of range: " << playerCardsNumber;
    if (playerCardsSum < 0 || playerCardsSum > 31)
        std::cout << "playerCardsSum out of range: " << playerCardsSum;
    if (currentValue < (-(ALL_CARDS)) || currentValue > (ALL_CARDS))
        std::cout << "currentValue out of range: " << (int) currentValue;
    if (cardsGiven < 0 || cardsGiven > ALL_CARDS)
        std::cout << "cardsGiven out of range: " << cardsGiven;


    currentValue += ALL_CARDS;

    long long int result = dealerCardsNumber;
    result *= 18;
    result += dealerCardsSum;
    result *= 28;
    result += dealerCardsWithAce;
    result *= 2;
    result += playerCardsNumber;
    result *= 22;
    result += playerCardsSum;
    result *= 32;
    result += playerCardsWithAce;
    result *= 2;
    result += playerPair;
    result *= 2;
    result += playerSplittedBefore;
    result *= 2;
    result += playerStanded;
    result *= 2;
    result += playerDoubled;
    result *= 2;
    result += currentValue;
    result *= (2 * ALL_CARDS + 2);
    result += cardsGiven;

    return result;
}

double getProbality(int cardsValue, int currentValue, int cardsGiven) {

    if (cardsValue <= 6) {
        if ((fiveOfThreeteenProbability * (ALL_CARDS - cardsGiven) - currentValue) < 0)
            return 0.0;
        return (fiveOfThreeteenProbability * (ALL_CARDS - cardsGiven) - currentValue) / (ALL_CARDS - cardsGiven) / 5;
    }

    if (cardsValue >= 10) {
        if ((fiveOfThreeteenProbability * (ALL_CARDS - cardsGiven) + currentValue) < 0)
            return 0.0;
        return (fiveOfThreeteenProbability * (ALL_CARDS - cardsGiven) + currentValue) / (ALL_CARDS - cardsGiven) / 5;
    }
    return
            threeOfThreeteenProbability / 3;
}

char methodHiLoValue(int card) {
    if (card <= 6) return 1;
    if (card >= 10) return -1;
    return 0;
}

//TODO actualise comments, source above
double get_reward(unsigned char dealerCardsNumber,// 1 to 17
                  unsigned char dealerCardsSum,// 1 to 27
                  unsigned char dealerCardsWithAce,// bool
                  unsigned char playerCardsNumber,// 1 to 21
                  unsigned char playerCardsSum,// 1 to 31
                  bool playerCardsWithAce,// bool
                  bool playerPair,// bool
                  bool playerSplittedBefore,// bool
                  bool playerStanded,// bool
                  bool playerDoubled,// bool
                  short int currentValue,//-[ALL_CARDS/2] to [ALL_CARDS/2]
                  short int cardsGiven// ALL_CARDS to 0
) {
    if (resultsMap.find(getStatusKey(dealerCardsNumber, dealerCardsSum, dealerCardsWithAce,
                                     playerCardsNumber,
                                     playerCardsSum, playerCardsWithAce, playerPair,
                                     playerSplittedBefore,
                                     playerStanded, playerDoubled, currentValue,
                                     cardsGiven)) != resultsMap.end())
        return
                resultsMap[getStatusKey(dealerCardsNumber, dealerCardsSum, dealerCardsWithAce,
                                        playerCardsNumber,
                                        playerCardsSum, playerCardsWithAce, playerPair,
                                        playerSplittedBefore,
                                        playerStanded, playerDoubled, currentValue,
                                        cardsGiven)];
    //stop if 80% cards are given
    if (cardsGiven > (ALL_CARDS * 0.8))
        return 0;

    //when no enough player cards
    if (playerCardsNumber < 2) {
        double total_reward = 0.0;
        for (int i = 2; i <= 11; i++)
            total_reward += getProbality(i, currentValue, cardsGiven) * get_reward(
                    dealerCardsNumber,
                    dealerCardsSum, dealerCardsWithAce,
                    playerCardsNumber + 1,
                    playerCardsSum + i,
                    playerCardsWithAce || (i == 11),
                    i == playerCardsSum,
                    playerSplittedBefore, playerStanded,
                    playerDoubled,
                    currentValue + methodHiLoValue(i),
                    cardsGiven + 1);
        resultsMap[getStatusKey(dealerCardsNumber, dealerCardsSum, dealerCardsWithAce,
                                playerCardsNumber,
                                playerCardsSum, playerCardsWithAce, playerPair,
                                playerSplittedBefore,
                                playerStanded, playerDoubled, currentValue,
                                cardsGiven)] = total_reward;
        return total_reward;
    }

    //when no enough dealer cards
    if (dealerCardsNumber < 1) {
        double total_reward = 0.0;
        for (int i = 2; i <= 11; i++)
            total_reward += getProbality(i, currentValue, cardsGiven) * get_reward(
                    dealerCardsNumber + 1,
                    dealerCardsSum + i, (i == 11),
                    playerCardsNumber,
                    playerCardsSum,
                    playerCardsWithAce,
                    playerPair,
                    playerSplittedBefore, playerStanded,
                    playerDoubled,
                    currentValue + methodHiLoValue(i),
                    cardsGiven + 1);
        resultsMap[getStatusKey(dealerCardsNumber, dealerCardsSum, dealerCardsWithAce,
                                playerCardsNumber,
                                playerCardsSum, playerCardsWithAce, playerPair,
                                playerSplittedBefore,
                                playerStanded, playerDoubled, currentValue,
                                cardsGiven)] = total_reward;
        return total_reward;
    }

    // when dealer busted but he has an ace
    if (dealerCardsSum > 21 &&
        dealerCardsWithAce) {
        double total_reward = get_reward(dealerCardsNumber, dealerCardsSum - 10, 0,
                                         playerCardsNumber,
                                         playerCardsSum, playerCardsWithAce, playerPair,
                                         playerSplittedBefore,
                                         playerStanded, playerDoubled, currentValue,
                                         cardsGiven);
        resultsMap[getStatusKey(dealerCardsNumber, dealerCardsSum, dealerCardsWithAce,
                                playerCardsNumber,
                                playerCardsSum, playerCardsWithAce, playerPair,
                                playerSplittedBefore,
                                playerStanded, playerDoubled, currentValue,
                                cardsGiven)] = total_reward;
        return total_reward;
    }

// when player busted but he has an ace
    if (playerCardsSum > 21 &&
        playerCardsWithAce) {
        double total_reward = get_reward(dealerCardsNumber, dealerCardsSum,
                                         dealerCardsWithAce,
                                         playerCardsNumber,
                                         playerCardsSum - 10, 0, playerPair,
                                         playerSplittedBefore,
                                         playerStanded, playerDoubled, currentValue,
                                         cardsGiven);
        resultsMap[getStatusKey(dealerCardsNumber, dealerCardsSum, dealerCardsWithAce,
                                playerCardsNumber,
                                playerCardsSum, playerCardsWithAce, playerPair,
                                playerSplittedBefore,
                                playerStanded, playerDoubled, currentValue,
                                cardsGiven)] = total_reward;
        return total_reward;
    }

// when player busted
    if (playerCardsSum > 21) {
        double local_reward;
        if (playerSplittedBefore)
            if (playerDoubled == 1)
                local_reward = -4.0;
            else
                local_reward = -2.0;
        else {
            if (playerDoubled)
                local_reward = -2.0;
            else
                local_reward = -1.0;
        }
        double total_reward = local_reward + get_reward(0, 0,
                                                        false,
                                                        0,
                                                        0, false, false,
                                                        false,
                                                        false, false, currentValue,
                                                        cardsGiven);
        resultsMap[getStatusKey(dealerCardsNumber, dealerCardsSum, dealerCardsWithAce,
                                playerCardsNumber,
                                playerCardsSum, playerCardsWithAce, playerPair,
                                playerSplittedBefore,
                                playerStanded, playerDoubled, currentValue,
                                cardsGiven)] = total_reward;
        return total_reward;
    }

// when splitted aces - player stands
    if (!playerStanded &&
        playerCardsWithAce &&
        playerSplittedBefore) {
        double total_reward = get_reward(dealerCardsNumber, dealerCardsSum,
                                         dealerCardsWithAce,
                                         playerCardsNumber,
                                         playerCardsSum, playerCardsWithAce, playerPair,
                                         playerSplittedBefore,
                                         true, playerDoubled, currentValue, cardsGiven);
        resultsMap[getStatusKey(dealerCardsNumber, dealerCardsSum, dealerCardsWithAce,
                                playerCardsNumber,
                                playerCardsSum, playerCardsWithAce, playerPair,
                                playerSplittedBefore,
                                playerStanded, playerDoubled, currentValue,
                                cardsGiven)] = total_reward;
        return total_reward;
    }

// when player has 21 - player stands
    if (!playerStanded &&
        playerCardsSum == 21) {

        double total_reward = get_reward(dealerCardsNumber, dealerCardsSum,
                                         dealerCardsWithAce,
                                         playerCardsNumber,
                                         playerCardsSum, playerCardsWithAce, playerPair,
                                         playerSplittedBefore,
                                         1, playerDoubled, currentValue, cardsGiven);
        resultsMap[getStatusKey(dealerCardsNumber, dealerCardsSum, dealerCardsWithAce,
                                playerCardsNumber,
                                playerCardsSum, playerCardsWithAce, playerPair,
                                playerSplittedBefore,
                                playerStanded, playerDoubled, currentValue,
                                cardsGiven)] = total_reward;
        return total_reward;
    }
//when dealer >=17
    if (dealerCardsSum >= 17) {
        double local_reward;
        if ((dealerCardsSum > 21) ||
            (playerCardsSum >
             dealerCardsSum) ||
            (dealerCardsSum == 21 &&
             playerCardsSum == 21 &&
             playerCardsNumber == 2 &&
             dealerCardsNumber != 2)) {//WIN
            if (playerCardsSum == 21 &&
                playerCardsNumber == 2) {
                if (playerSplittedBefore) {
                    if (playerDoubled)
                        local_reward = 4.0;
                    else
                        local_reward = 2.0;
                } else {
                    if (playerDoubled)
                        local_reward = 3.0;
                    else
                        local_reward = 1.5;
                }
            } else {
                if (playerSplittedBefore) {
                    if (playerDoubled)
                        local_reward = 4.0;
                    else
                        local_reward = 2.0;
                } else {
                    if (playerDoubled)
                        local_reward = 2.0;
                    else
                        local_reward = 1.0;
                }
            }

        } else if ((playerCardsSum <
                    dealerCardsSum) ||
                   (dealerCardsSum == 21 &&
                    playerCardsSum == 21 &&
                    playerCardsNumber != 2 &&
                    dealerCardsNumber == 2)) {//LOSE
            if (playerSplittedBefore) {
                if (playerDoubled)
                    local_reward = -4.0;
                else
                    local_reward = -2.0;
            } else {
                if (playerDoubled)
                    local_reward = -2.0;
                else
                    local_reward = -1.0;
            }
        } else {//TIE
            local_reward = 0;
        }
        double total_reward = local_reward + get_reward(0, 0,
                                                        false,
                                                        0,
                                                        0, false, false,
                                                        false,
                                                        false, false, currentValue,
                                                        cardsGiven);
        resultsMap[getStatusKey(dealerCardsNumber, dealerCardsSum, dealerCardsWithAce,
                                playerCardsNumber,
                                playerCardsSum, playerCardsWithAce, playerPair,
                                playerSplittedBefore,
                                playerStanded, playerDoubled, currentValue,
                                cardsGiven)] = total_reward;
        return total_reward;
    }
    // when player standed
    if (playerStanded) {
        double total_reward = 0.0;
        for (int i = 2; i <= 11; i++)
            total_reward +=
                    getProbality(i, currentValue, cardsGiven) *
                    get_reward(
                            dealerCardsNumber + 1,
                            dealerCardsSum + i,
                            dealerCardsWithAce || (i == 11),
                            playerCardsNumber,
                            playerCardsSum, playerCardsWithAce,
                            playerPair,
                            playerSplittedBefore, playerStanded,
                            playerDoubled,
                            currentValue + methodHiLoValue(i),
                            cardsGiven + 1);
        resultsMap[getStatusKey(dealerCardsNumber, dealerCardsSum, dealerCardsWithAce,
                                playerCardsNumber,
                                playerCardsSum, playerCardsWithAce, playerPair,
                                playerSplittedBefore,
                                playerStanded, playerDoubled, currentValue,
                                cardsGiven)] = total_reward;
        return total_reward;

    }



    double stand_reward = get_reward(dealerCardsNumber,
                                     dealerCardsSum,
                                     dealerCardsWithAce,
                                     playerCardsNumber,
                                     playerCardsSum,
                                     playerCardsWithAce,
                                     playerPair,
                                     playerSplittedBefore, true,
                                     playerDoubled, currentValue, cardsGiven);

    double max_reward = stand_reward;

    double split_reward = 0.0;
    if (playerPair &&
        !playerSplittedBefore) { // check if we can split
        split_reward = 0.0;
        for (int i = 2; i <= 11; i++)
            split_reward += getProbality(i, currentValue, cardsGiven) * get_reward(
                    dealerCardsNumber,
                    dealerCardsSum,
                    dealerCardsWithAce,
                    2,
                    playerCardsSum / 2 + i,
                    playerCardsWithAce || (i == 11),
                    (i == playerCardsSum / 2),
                    1, playerStanded,
                    playerDoubled,
                    currentValue + methodHiLoValue(i),
                    cardsGiven + 1);

        if (split_reward > max_reward)
            max_reward = split_reward;
    }
    double double_reward = 0.0;
    if (playerCardsNumber == 2) {//check if we can double
        double_reward = 0.0;
        for (int i = 2; i <= 11; i++)
            double_reward +=
                    getProbality(i, currentValue, cardsGiven) * get_reward(
                            dealerCardsNumber,
                            dealerCardsSum, dealerCardsWithAce,
                            playerCardsNumber + 1,
                            playerCardsSum + i,
                            playerCardsWithAce || (i == 11),
                            false,
                            playerSplittedBefore, true, true,
                            currentValue + methodHiLoValue(i),
                            cardsGiven + 1);

        if (double_reward > max_reward)
            max_reward = double_reward;
    }

    double hit_reward = 0.0;
    for (int i = 2; i <= 11; i++)
        hit_reward += getProbality(i, currentValue, cardsGiven) * get_reward(
                dealerCardsNumber,
                dealerCardsSum, dealerCardsWithAce,
                playerCardsNumber + 1,
                playerCardsSum + i,
                playerCardsWithAce || (i == 11),
                false,
                playerSplittedBefore, playerStanded,
                playerDoubled,
                currentValue + methodHiLoValue(i),
                cardsGiven + 1);

    if (hit_reward > max_reward)
        max_reward = hit_reward;

    resultsMap[getStatusKey(dealerCardsNumber, dealerCardsSum, dealerCardsWithAce,
                            playerCardsNumber,
                            playerCardsSum, playerCardsWithAce, playerPair,
                            playerSplittedBefore,
                            playerStanded, playerDoubled, currentValue,
                            cardsGiven)] = max_reward;
    return max_reward;
}

int main() {
    std::cout << std::time(0) << std::endl;
    std::cout << get_reward(0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0) << std::endl;
    std::cout << resultsMap.size() << std::endl;
    std::cout << std::time(0) << std::endl;
    return 0;
}
