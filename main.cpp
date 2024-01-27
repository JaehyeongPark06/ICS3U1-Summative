#include <algorithm>
#include <ctime>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <map>
#include <stdlib.h>
using namespace std;

// the length of each word, the max number of guesses, the number of words in
// the word bank
const int WORD_LENGTH = 5;
const int MAX_GUESSES = 6;
const int NUM_WORDS = 2315;

// the state of each letter in the guess the user inputs
const int NO_MATCH = 0;
const int PARTIAL_MATCH = 1;
const int FULL_MATCH = 2;

const string FILE_STATS = "stats.txt";

// function declarations

/**
 * @brief getRandomWord gets a random word from the targetWords.txt file
 * @param s is the random word
 * @return the random word
 */
string getRandomWord(string &s);

/**
 * @brief mainMenu prints the menu for the game
 */
inline void mainMenu();

/**
 * @brief isValidGuess returns whether the guess the user inputs is valid or
 * not. (checks if its a real word)
 * @param s is the guess the user inputs
 * @return true if the guess is valid and false if the guess is not valid
 */
inline bool isValidGuess(string &s);

/**
 * @brief toUpper turns all the characters in a string to uppercase
 * @param s is the guess the user inputs
 */
inline void toUpper(string &s);

/**
 * @brief coorect returns the value of input == word
 * @param input is the guess the user inputs
 * @param word is the target word
 * @return true if the guess is equal to the word and false if the guess is not
 * equal to the word
 */
inline bool correct(string input, string word);

/**
 * @brief checkWord checks which letters in the guess match with the target word
 * @param input is the word the user inputs
 * @param word is the target word
 * @param matches[MAX_GUESSES][WORD_LENGTH] is a 2D array that holds the state
 * of each letter in the input and the target word (fully matched, partially
 * matched, no match)
 */
void checkWord(string input, string word, int matches[MAX_GUESSES][WORD_LENGTH],
               int numGuess);

/**
 * @brief output prints the updated game with the most recent guess
 * @param guess[MAX_GUESSES] is all the guesses the user has inputed so far in
 *an array
 * @param matches[MAX_GUESSES][WORD_LENGTH] is a 2D array that holds the state
 * of each letter in the input and the target word (fully matched, partially
 * matched, no match)
 * @param numGuess is the number of guesses the user has inputed
 */
void output(string guess[MAX_GUESSES], int matches[MAX_GUESSES][WORD_LENGTH],
            int numGuess);

/**
 * @brief checkTypeWin returns the number of guesses used throughout the entire
 *game
 * @return the number of guesses used
 */
int checkTypeWin(int numGuess);

/**
 * @brief updateStats updates the stats of the user (winrate, games won in 1
 *guess, etc.)
 * @param numGuess is the number of guesses the user has inputed
 * @param won is a boolean indicating whether the user has won or not
 */
void updateStats(int numGues, bool won);

/**
 * @brief outputStats outputs the updated stats of the user
 * @param gamesPlayed is the total number of games played
 * @param oneGuess is the number of games won in one guess
 * @param twoGuess is the number of games won in two guesses
 * @param threeGuess is the number of games won in three guesses
 * @param fourGuess is the number of games won in four guesses
 * @param fiveGuess is the number of games won in five guesses
 * @param sixGuess is the number of games won in six guesses
 */
void outputStats(int gamesWon, int gamesPlayed, int oneGuess, int twoGuess,
                 int threeGuess, int fourGuess, int fiveGuess, int sixGuess);

string getRandomWord(string &s) {
  int numLines = 0, random = rand() % NUM_WORDS;
  ifstream WORDS("targetWords.txt");
  // chooses a random word in the file
  while (getline(WORDS, s)) {
    if (numLines == random) {
      break;
    }
    numLines++;
  }
  // closes the words file
  WORDS.close();

  // turns all the letters in the target word to capital
  toUpper(s);
  return s;
}

inline void mainMenu() {
  cout << "\033[0m" << endl;
  cout << "-----------------" << endl;
  cout << "   W O R D L E   " << endl;
  cout << "-----------------" << endl;
  // simple game description
  cout << "Wordle is a game where the objective is to guess a 5 letter english "
          "word in 6 attempts or less."
       << endl;
  cout << "\nIf a letter in the word does not match, it will be shown in white."
       << endl;
  cout << "\nIf a letter in the word appears in the target word, but is not in "
          "the right position, the letter will be yellow."
       << endl;
  cout << "\nIf a letter in the word appears in the target word and is in the "
          "right position, the letter will be green."
       << endl;
  cout << "\nRemember that the same letter can appear multiple times in the "
          "secret word, and there’s no special color coding for letters that "
          "appear repeatedly."
       << endl;
  cout
      << "\nFor example, if the secret word is BELLE and you guess a word with "
         "one L and one E, Wordle won’t tell you that both those letters "
         "actually appear twice."
      << endl;
}

inline bool isValidGuess(string &guess) {
  // checking if the guess is in the words.txt file
  // return true or false
  ifstream WORDS("words.txt");
  string word;
  while (getline(WORDS, word)) {
    toUpper(word);
    if (guess == word) {
      return true;
    }
  }
  return false;
}

inline void toUpper(string &s) {
  // changes all letters in the word to uppercase
  transform(s.begin(), s.end(), s.begin(), ::toupper);
}

inline bool correct(string input, string word) {
  // checks if input is equal to target word
  return input == word;
}

void checkWord(string input, string word, int matches[MAX_GUESSES][WORD_LENGTH],
               int numGuess) {
  for (int i = 0; i < WORD_LENGTH; i++) {
    // initially setting all the letters to no match so we can iterate through
    // them later and check which ones match
    matches[numGuess][i] = NO_MATCH;
  }

  map<char, int> letterFrequency;

  for (int i = 0; i < WORD_LENGTH; i++) {
    letterFrequency[word[i]]++;
  }

  // first, find all the correct guesses
  for (int i = 0; i < WORD_LENGTH; i++) {
    if (input[i] == word[i]) {
      matches[numGuess][i] = FULL_MATCH;
      letterFrequency[word[i]]--;
    }
  }

  for (int i = 0; i < WORD_LENGTH; i++) {
    for (int j = 0; j < WORD_LENGTH; j++) {
      if (input[i] == word[j] && i != j && letterFrequency[word[j]] != 0) {
        // if the indices are different, and they have the same letter, AND we
        // still have to find more of those letters
        matches[numGuess][i] = PARTIAL_MATCH;
        letterFrequency[word[j]]--;
      }
    }
  }
}

void output(string guess[MAX_GUESSES], int matches[MAX_GUESSES][WORD_LENGTH],
            int numGuess) {
  system("clear");
  cout << "\033[0m" << endl;
  cout << "-----------------" << endl;
  cout << "   W O R D L E   " << endl;

  // formatting and colored text
  for (int i = 0; i <= numGuess && i < MAX_GUESSES; i++) {
    string padding = "--";
    string coloredText = " ";

    for (int j = 0; j < WORD_LENGTH; j++) {
      padding += "---";
      coloredText += " ";

      // checking each of the guesses and  change the color depending on its
      // state (0, 1, 2)
      char letter = toupper(guess[i][j]);

      if (matches[i][j] == PARTIAL_MATCH) {
        coloredText += "\033[33m"; // yellow text
      } else if (matches[i][j] == FULL_MATCH) {
        coloredText += "\033[32m"; // green text
      }
      coloredText += letter;
      coloredText += " ";

      // change text back to white/default
      if (matches[i][j] == PARTIAL_MATCH || matches[i][j] == FULL_MATCH) {
        coloredText += "\033[0m"; // white text
      }
    }
    if (i == 0) {
      cout << padding << endl;
    }
    cout << coloredText << endl;
    cout << padding << endl;
  }
}

void updateStats(int numGuess, bool won) {
  ifstream STATS(FILE_STATS);
  string line;
  int gamesPlayed, gamesWon, oneGuess, twoGuess, threeGuess, fourGuess,
      fiveGuess, sixGuess;
  if (getline(STATS, line)) {
    istringstream iss(line);
    if (iss >> gamesPlayed >> gamesWon >> oneGuess >> twoGuess >> threeGuess >>
        fourGuess >> fiveGuess >> sixGuess) {
      gamesPlayed++;
      // checking if they won and with how many guesses
      // one less than the guesses used, because of the way the program is set
      // up
      if (won) {
        gamesWon++;
        switch (numGuess) {
        case 0:
          oneGuess++;
          break;
        case 1:
          twoGuess++;
          break;
        case 2:
          threeGuess++;
          break;
        case 3:
          fourGuess++;
          break;
        case 4:
          fiveGuess++;
          break;
        case 5:
          sixGuess++;
          break;
        }
      }
    }
  }
  STATS.close();

  ofstream OUTPUT(FILE_STATS);
  OUTPUT << gamesPlayed << ' ' << gamesWon << ' ' << oneGuess << ' ' << twoGuess
         << ' ' << threeGuess << ' ' << fourGuess << ' ' << fiveGuess << ' '
         << sixGuess;
  outputStats(gamesWon, gamesPlayed, oneGuess, twoGuess, threeGuess, fourGuess,
              fiveGuess, sixGuess);
  OUTPUT.close();
}

void outputStats(int gamesWon, int gamesPlayed, int oneGuess, int twoGuess,
                 int threeGuess, int fourGuess, int fiveGuess, int sixGuess) {
  cout << "\033[0m" << endl;
  cout << "-----------------------" << endl;
  cout << "  S T A T I S T I C S  " << endl;
  cout << "-----------------------" << endl;
  cout << "Total Wins: " << gamesWon << endl;
  cout << "Games Played: " << gamesPlayed << endl;
  cout << "Your win rate is " << fixed << setprecision(2)
       << 100 * ((float)gamesWon / (float)gamesPlayed) << "%." << endl;
  cout << "--------------------------------------" << endl;
  cout << "  G U E S S  D I S T R I B U T I O N  " << endl;
  cout << "--------------------------------------" << endl;
  cout << "1: " << oneGuess << endl;
  cout << "2: " << twoGuess << endl;
  cout << "3: " << threeGuess << endl;
  cout << "4: " << fourGuess << endl;
  cout << "5: " << fiveGuess << endl;
  cout << "6: " << sixGuess << endl;
}
