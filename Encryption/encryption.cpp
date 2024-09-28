#include <iostream>
using std::cout;
using std::cin;
using std::endl;
#include <fstream>
using std::ifstream;
using std::ofstream;
#include <unordered_map>
using std::unordered_map;
using std::unordered_multimap;
#include <unordered_set>
using std::unordered_set;
#include <string>
using std::string;
#include <sstream>
#include <vector>
using std::vector;
#include <stdexcept>

// Read in the file and open it. If filename is not found, or not given, raise
// error.
ifstream OpenFile(string& input_filename) {
  if (input_filename.size() == 0) {
    std::cerr << "No input file provided." << endl;
  }
  ifstream input_file(input_filename);
  try {
    if (!input_file.is_open())
      throw std::invalid_argument("Error: Cannot open '" + input_filename +
                                  "'.");
  } catch (const std::invalid_argument& e) {
    std::cerr << e.what() << endl;
    std::exit(1);
  }
  return input_file;
}
// Convert the contents of the file into a single string, maintaining its
// formatting.
string ReadFile(ifstream& input_file) {
  string file_to_string;
  if (input_file.is_open()) {
    string line;
    while (std::getline(input_file, line)) {
      file_to_string += line + "\n";
    }
  }
  return file_to_string;
}
// Remove duplicate lines from a string
string RemoveDuplicates(string& with_dupes) {
  std::istringstream iss(with_dupes);
  vector<string> v;
  unordered_set<string> uniqueLines;
  string line;
  while (std::getline(iss, line)) {
    v.push_back(line);
  }
  string no_dupes;
  for (const auto& line : v) {
    if (uniqueLines.find(line) ==
        uniqueLines.end()) {  // Checks if the vector element is already in the
                              // set. If not, it gets added to the resulting
                              // string, else it gets skipped.
      uniqueLines.insert(line);
      no_dupes += line + "\n";
    }
  }
  return no_dupes;
}

// Check for issues in the encoding string.
string CleanPairs(string& str_pairs) {
  std::istringstream iss(str_pairs);
  string pair;
  string clean_pairs = "";
  try {
    while (iss >> pair) {
      if (pair.size() == 2)
        clean_pairs += pair + " ";
      else if (pair == "q")
        break;
      else if (pair.size() < 2 || pair.size() > 2)
        throw std::invalid_argument("Error: Unknown encoding '" + pair + "'.");
    }
  } catch (const std::invalid_argument& e) {
    std::cerr << e.what() << endl;
    std::exit(1);
  }
  return clean_pairs;
}

// Read in the string of pairs and store them in a multimap
unordered_multimap<char, char> ReadEncodingPairs(string& str_pairs) {
  unordered_multimap<char, char> codes;
  string pair;
  std::istringstream iss(str_pairs);
  while (iss >> pair) {
    if (pair.size() == 2) codes.insert(std::make_pair(pair.at(0), pair.at(1)));
  }
  return codes;
}

// Check for errors in the encoding string, and raise appropriate errors.
void WrongEncoding(unordered_multimap<char, char> codes) {
  try {
    for (const auto& pair : codes) {
      char key = pair.first;
      auto count = codes.count(key);
      auto iter = codes.find(key);
      if (count == 2) {
        std::ostringstream oss;
        oss << "Error: The character '" << key << "' is encoded as both '"
            << iter->second << "' and '" << (++iter)->second << "'.";
        throw std::invalid_argument(oss.str());
      }
    }
  } catch (const std::invalid_argument& e) {
    std::cerr << e.what() << endl;
    std::exit(1);
  }
}
// Take in a string, and encode it. Since the file was stored in one string,
// this function alone processes the entire file.
string ApplyEncoding(string& line, unordered_multimap<char, char> codes) {
  string encoded_line = "";
  for (auto str_pos = line.begin(); str_pos != line.end(); ++str_pos) {
    for (auto map_pos = codes.begin(); map_pos != codes.end(); ++map_pos) {
      if (*str_pos == map_pos->first) {
        *str_pos = map_pos->second;
        break;
      }
    }
    encoded_line.push_back(*str_pos);
  }
  return encoded_line;
}

int main() {
  string filename;
  cin >> filename;
  ifstream input_file = OpenFile(filename);
  string file_content = ReadFile(input_file);
  string pairs;
  cin.ignore(1, '\n');
  std::getline(cin, pairs);
  string clean = CleanPairs(pairs);
  unordered_multimap<char, char> codes = ReadEncodingPairs(clean);
  WrongEncoding(codes);
  string enc_line = ApplyEncoding(file_content, codes);
  string no_dupes = RemoveDuplicates(enc_line);
  cout << no_dupes;
}