#include <iostream>
#include <map>
#include <string>
using namespace std;

class FontStyle {
public:
  virtual ~FontStyle() {}
  virtual void format(int line, int column, char character) = 0;
};

class ConcreteFontStyle : public FontStyle {
  // TODO: Add fields (fontFamily, fontSize, bold, italic)
  string fontFamily;
  int fontSize;
  bool bold;
  bool italic;

public:
  ConcreteFontStyle(const string &fontFamily, int fontSize, bool bold,
                    bool italic) {
    // TODO: Store fontFamily, fontSize, bold, italic
    this->fontFamily = fontFamily;
    this->fontSize = fontSize;
    this->bold = bold;
    this->italic = italic;
  }

  void format(int line, int column, char character) override {
    // TODO: Print "[fontFamily, fontSize, style] 'character' at line:column"
    // where style is "bold" if bold, "italic" if italic, otherwise "normal"
    // Example: "[Arial, 12, normal] 'H' at 1:1"
    string style = bold ? "bold" : (italic ? "italic" : "normal");
    cout << "[" << fontFamily << ", " << fontSize << ", " << style << "]"
         << " '" << character << "' at " << line << ":" << column << endl;
  }
};

class FontStyleFactory {
  // TODO: Add a map<string, FontStyle*> cache field
  map<string, FontStyle *> cache;

public:
  FontStyle *getFontStyle(const string &fontFamily, int fontSize, bool bold,
                          bool italic) {
    // TODO: Build key from fontFamily + fontSize + bold + italic
    // TODO: If key exists in cache, return cached instance
    // TODO: Otherwise create new ConcreteFontStyle, store in cache, return it
    string key = fontFamily + to_string(fontSize) + (bold ? "true" : "false") +
                 (italic ? "true" : "false");
    if (cache.find(key) != cache.end()) {
      return cache[key];
    }
    FontStyle *style =
        new ConcreteFontStyle(fontFamily, fontSize, bold, italic);
    cache[key] = style;
    return style;
  }

  int getStyleCount() {
    // TODO: Return the number of cached styles
    return cache.size();
  }
};

int main() {
  FontStyleFactory factory;
  FontStyle *arial12 = factory.getFontStyle("Arial", 12, false, false);
  FontStyle *arial12Bold = factory.getFontStyle("Arial", 12, true, false);
  FontStyle *arial12Again = factory.getFontStyle("Arial", 12, false, false);

  arial12->format(1, 1, 'H');
  arial12Bold->format(1, 2, 'e');
  arial12Again->format(1, 3, 'l');

  cout << "Same instance? " << (arial12 == arial12Again ? "true" : "false")
       << endl;
  cout << "Total styles: " << factory.getStyleCount() << endl;
  return 0;
}
