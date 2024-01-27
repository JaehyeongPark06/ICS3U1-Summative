// Programmed by Jaden Park
// Programmed for ICS3U1 Summative
// Date: June 6, 2023 --> June 15, 2023
// This file contains the functions for the game

#include "game.h"
#include <algorithm>
#include <ctime>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <map>
#include <stdlib.h>
using namespace std;

int main() {
  srand(time(0)); // RNG

  // stores the user's guesses
  string guesses[MAX_GUESSES];
  // array to hold the state of the letters match (none, partial, full)
  int matches[MAX_GUESSES][WORD_LENGTH];
  // holds the state of the game
  bool won = false;
  // number of guesses used, number of lines in file, random number
  int numGuess = 0;
  // line in file, users input
  string line, input;

  getRandomWord(line);
  mainMenu();

  while (numGuess < MAX_GUESSES) {
    do {
      cout << "\n\033[94mEnter your guess (q or Q to quit): ";
      cin >> input;
      toUpper(input);
      if (input == "Q")
        break;
      else if (!isValidGuess(input))
        cout << "\n\033[31mINVALID INPUT." << endl;
    } while ((input != "q" || input != "Q") && !isValidGuess(input));

    if (input == "q" || input == "Q") {
      cout << "\n\033[31mYOU QUIT THE GAME. THE WORD WAS " << line << "."
           << endl;
      return 0;
    }
    // change string to all lower or upper to avoid error
    guesses[numGuess] = input;

    // check if the guess matches and display to console
    checkWord(input, line, matches, numGuess);
    output(guesses, matches, numGuess);

    // if the users guessed correctly, end the game
    if (correct(input, line)) {
      cout << "\n\033[0mYOU WIN! THE WORD WAS " << line << "." << endl;
      won = true;
      updateStats(numGuess, won);
      break;
    }

    numGuess++;
  }
  // if the game is over and user hasn't won
  if (!won) {
    cout << "\n\033[31mGAME OVER! THE WORD WAS " << line << "\033[31m." << endl;
    updateStats(numGuess, won);
  }
}
