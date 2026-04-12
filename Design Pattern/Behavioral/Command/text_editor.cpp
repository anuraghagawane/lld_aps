#include <iostream>
#include <stack>
#include <string>

using namespace std;

class TextEditor {
private:
  string content;

public:
  void append(const string &text) { content += text; }

  string deleteLast(int count) {
    int start = max(0, (int)content.length() - count);
    string deleted = content.substr(start);
    content = content.substr(0, start);
    return deleted;
  }

  string getContent() const { return content; }
};

class EditorCommand {
public:
  virtual ~EditorCommand() = default;
  virtual void execute() = 0;
  virtual void undo() = 0;
};

class TypeCommand : public EditorCommand {
private:
  TextEditor *editor;
  string text;

public:
  TypeCommand(TextEditor *editor, const string &text)
      : editor(editor), text(text) {}

  void execute() override {
    editor->append(text);
    cout << "Typed: \"" << text << "\"" << endl;
  }

  void undo() override {
    editor->deleteLast(text.length());
    cout << "Undo type: \"" << text << "\"" << endl;
  }
};

class DeletedCommand : public EditorCommand {
private:
  TextEditor *editor;
  int count;
  string deletedText;

public:
  DeletedCommand(TextEditor *editor, int count)
      : editor(editor), count(count) {}

  void execute() override {
    deletedText = editor->deleteLast(count);
    cout << "Deleted: \"" << deletedText << "\"" << endl;
  }

  void undo() override {
    editor->append(deletedText);
    cout << "Undo delete: restored \"" << deletedText << "\"" << endl;
  }
};

class EditorInvoker {
private:
  stack<EditorCommand *> undoStack;
  stack<EditorCommand *> redoStack;

  void clearStack(stack<EditorCommand *> &s) {
    while (!s.empty())
      s.pop();
  }

public:
  void execute(EditorCommand *command) {
    command->execute();
    undoStack.push(command);
    clearStack(redoStack);
  }

  void undo() {
    if (!undoStack.empty()) {
      EditorCommand *command = undoStack.top();
      undoStack.pop();
      command->undo();
      redoStack.push(command);
    } else {
      cout << "Nothing to undo." << endl;
    }
  }

  void redo() {
    if (!redoStack.empty()) {
      EditorCommand *command = redoStack.top();
      redoStack.pop();
      command->execute();
      undoStack.push(command);
    } else {
      cout << "Nothing to redo." << endl;
    }
  }
};

int main() {
  TextEditor editor;
  EditorInvoker invoker;

  TypeCommand type1(&editor, "Hello");
  TypeCommand type2(&editor, " World");
  TypeCommand type3(&editor, "!");

  invoker.execute(&type1);
  invoker.execute(&type2);
  invoker.execute(&type3);

  cout << "Content: \"" << editor.getContent() << "\"" << endl;
  cout << "\n--- Undo ---" << endl;
  invoker.undo();
  cout << "Content: \"" << editor.getContent() << "\"" << endl;

  invoker.undo();
  cout << "Content: \"" << editor.getContent() << "\"" << endl;

  cout << "\n--- Redo ---" << endl;
  invoker.redo();
  cout << "Content: \"" << editor.getContent() << "\"" << endl;

  cout << "\n--- New operation clears redo ---" << endl;

  DeletedCommand del(&editor, 3);
  invoker.execute(&del);
  cout << "Content: \"" << editor.getContent() << "\"" << endl;

  invoker.redo();

  cout << "\n--- Undo delete ---" << endl;
  invoker.undo();
  cout << "Content: \"" << editor.getContent() << "\"" << endl;

  return 0;
}
