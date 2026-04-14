#include <iostream>
#include <stack>
#include <string>

using namespace std;
class TextEditorMemento {
private:
  string state;

public:
  TextEditorMemento(string state) : state(state) {}

  string getState() const { return state; }
};

class TextEditor {
private:
  string content;

public:
  TextEditor() : content("") {}

  void type(const string &newText) {
    content += newText;
    cout << "Typed: \"" << newText << "\"" << endl;
  }

  string getContent() const { return content; }

  TextEditorMemento save() {
    cout << "Saving state: \"" << content << "\"" << endl;
    return TextEditorMemento(content);
  }

  void restore(const TextEditorMemento &memento) {
    content = memento.getState();
    cout << "Restored state to: \"" << content << "\"" << endl;
  }
};

class TextEditorUndoManager {
private:
  stack<TextEditorMemento> history;

public:
  void save(TextEditor &editor) { history.push(editor.save()); }

  void undo(TextEditor &editor) {
    if (!history.empty()) {
      editor.restore(history.top());
      history.pop();
    } else {
      cout << "Nothing to undo." << endl;
    }
  }

  int historySize() const { return history.size(); }
};

int main() {
  TextEditor editor;
  TextEditorUndoManager undoManager;

  editor.type("Hello");
  undoManager.save(editor);

  editor.type(" World");
  undoManager.save(editor);

  editor.type("!");
  cout << "Current: " << editor.getContent() << endl;

  cout << "\n--- Undo 1 ---" << endl;
  undoManager.undo(editor);
  cout << "Content: " << editor.getContent() << endl;

  cout << "\n--- Undo 2 ---" << endl;
  undoManager.undo(editor);
  cout << "Content: " << editor.getContent() << endl;

  cout << "\n--- Undo 3 ---" << endl;
  undoManager.undo(editor);
  return 0;
}
