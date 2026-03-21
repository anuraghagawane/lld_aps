#include <iostream>
using namespace std;

class ThemeColor {
public:
  virtual void apply() = 0;
  virtual ~ThemeColor() = default;
};

class ThemeFont {
public:
  virtual void render() = 0;
  virtual ~ThemeFont() = default;
};

class LightColor : public ThemeColor {
public:
  void apply() override {
    // TODO: Print "Applying light color: #FFFFFF background, #000000 text"
    cout << "Applying light color: #FFFFFF background, #000000 text" << endl;
  }
};

class DarkColor : public ThemeColor {
public:
  void apply() override {
    // TODO: Print "Applying dark color: #1E1E1E background, #FFFFFF text"
    cout << "Applying dark color: #1E1E1E background, #FFFFFF text" << endl;
  }
};

class LightFont : public ThemeFont {
public:
  void render() override {
    // TODO: Print "Rendering light theme font: Arial, 14px"
    cout << "Rendering light theme font: Arial, 14px" << endl;
  }
};

class DarkFont : public ThemeFont {
public:
  void render() override {
    // TODO: Print "Rendering dark theme font: Consolas, 14px"
    cout << "Rendering dark theme font: Consolas, 14px" << endl;
  }
};

class ThemeFactory {
public:
  virtual ThemeColor *createColor() = 0;
  virtual ThemeFont *createFont() = 0;
  virtual ~ThemeFactory() = default;
};

class LightThemeFactory : public ThemeFactory {
public:
  ThemeColor *createColor() override {
    return new LightColor(); // TODO: Return a new LightColor instance
  }

  ThemeFont *createFont() override {
    return new LightFont(); // TODO: Return a new LightFont instance
  }
};

class DarkThemeFactory : public ThemeFactory {
public:
  ThemeColor *createColor() override {
    return new DarkColor(); // TODO: Return a new DarkColor instance
  }

  ThemeFont *createFont() override {
    return new DarkFont(); // TODO: Return a new DarkFont instance
  }
};

class ThemeClient {
  ThemeColor *color;
  ThemeFont *font;

public:
  ThemeClient(ThemeFactory *factory) {
    color = factory->createColor();
    font = factory->createFont();
  }

  void applyTheme() {
    color->apply();
    font->render();
  }
};

int main() {
  cout << "=== Light Theme ===" << endl;
  LightThemeFactory lightFactory;
  ThemeClient lightClient(&lightFactory);
  lightClient.applyTheme();

  cout << endl;

  cout << "=== Dark Theme ===" << endl;
  DarkThemeFactory darkFactory;
  ThemeClient darkClient(&darkFactory);
  darkClient.applyTheme();

  return 0;
}
