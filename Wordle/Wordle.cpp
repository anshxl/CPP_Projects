#include <iostream>
#include <string>

using std::cin;
using std::cout;
using std::endl;
using std::string;

int main() {
  string secret;
  string guess;

  cout << "Give me a secret word: ";
  cin >> secret;
  int tries = 0;
  for (int k=0; k < 6; k++) {
    cout << endl <<  "Give me a guess: " << endl;
    cin >> guess;
    string result = "     ";
    if (!guess.empty()){
    for (size_t i = 0; i < 5; i++) {
      int count = 0;
      for (size_t j = 0; j < 5; j++) {
        if (guess.at(i) == secret.at(j)) {
          if (i == j) {
            result.at(i) = guess.at(i);
          } else {
            if (!(result.at(i) == guess.at(i)))
            result.at(i) = '?';
          }
        } else {
          count++;
        }
      }
      if (count == 5) result.at(i) = '.';
    }
    cout << result;
    if (result == secret) break;
    tries++;
    }
    if(cin.eof()){
        if (tries == 6){
            break;
        }
        if (tries < 6){
        tries = 6;
        cout << endl << "Give me a guess: " << endl;
        break;
        }
    }
  }
  if (tries < 6)
    cout << endl << "You Win!" << endl;
  if (tries == 6)
    cout << endl << "You Lose." << endl;
}