#include <algorithm>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>
using namespace std;

class TextFormatter {
public:
  virtual ~TextFormatter() {}
  virtual string format(const string &text) = 0;
};

class UpperCaseFormatter : public TextFormatter {
public:
  string format(const string &text) override {
    string str = text;
    std::transform(str.begin(), str.end(), str.begin(),
                   [](unsigned char c) { return std::toupper(c); });
    return str; // TODO: Return text converted to upper case
  }
};

class LowerCaseFormatter : public TextFormatter {
public:
  string format(const string &text) override {
    string str = text;
    std::transform(str.begin(), str.end(), str.begin(),
                   [](unsigned char c) { return std::tolower(c); });
    return str; // TODO: Return text converted to lower case
  }
};

class TitleCaseFormatter : public TextFormatter {
public:
  string format(const string &text) override {
    stringstream ss(text);
    string word;
    vector<string> words;

    while (ss >> word) {
      word[0] = toupper(word[0]);
      words.push_back(word + " ");
    }
    string str = "";
    for (auto &w : words)
      str += w;
    return str; // TODO: Split by spaces, capitalize first char of each word
  }
};

class TextEditor {
private:
  TextFormatter *formatter;

public:
  TextEditor(TextFormatter *formatter) : formatter(formatter) {}

  void setFormatter(TextFormatter *formatter) { this->formatter = formatter; }

  void publishText(const string &text) {
    cout << formatter->format(text) << endl;
  }
};

int main() {
  UpperCaseFormatter upper;
  TextEditor editor(&upper);
  editor.publishText("hello world from strategy pattern");

  LowerCaseFormatter lower;
  editor.setFormatter(&lower);
  editor.publishText("Hello World From Strategy Pattern");

  TitleCaseFormatter title;
  editor.setFormatter(&title);
  editor.publishText("hello world from strategy pattern");

  return 0;
}
