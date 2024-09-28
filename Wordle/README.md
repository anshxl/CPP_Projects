# Wordle Game in C++
## Project Description
This project is a simple implementation of the popular word-guessing game Wordle in C++. The game allows a player to guess a secret five-letter word within six attempts. After each guess, the game provides feedback on which letters are correct and in the correct position, which letters are correct but in the wrong position, and which letters are not in the secret word at all.

## How to Play
**Secret Word**: The game starts by asking the player to input a secret five-letter word.

**Guesses**: The player has six attempts to guess the secret word.

**Feedback**:
- Correct letters in the correct position are displayed as is.
- Correct letters in the wrong position are marked with a ?.
- Incorrect letters are marked with a ..
  
## Winning and Losing:
- If the player guesses the secret word within six attempts, they win.
- If the player fails to guess the word within six attempts, they lose.
  
## Code Overview
**Main Function**: The main function handles the game loop, taking input for the secret word and guesses, and providing feedback after each guess.

## Usage Instructions
**Compile the Code**: Use a C++ compiler to compile the code.
```bash
g++ -o wordle Wordle.cpp
```
**Run the Game**: Execute the compiled program.
```bash
./wordle
```
**Follow the Prompts**: Enter the secret word and make guesses as prompted by the game.

## Dependencies
C++ Standard Library

## License
This project is licensed under the MIT License. See the LICENSE file for more details.

## Acknowledgements
This project is inspired by the popular Wordle game and serves as a simple console-based implementation for educational purposes.
