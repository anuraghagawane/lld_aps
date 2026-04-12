class TextEditor {
  private content: string = "";

  append(text: string): void { this.content += text; }

  deleteLast(count: number): string {
    const start = Math.max(0, this.content.length - count);
    const deleted = this.content.substring(start);
    this.content = this.content.substring(0, start);
    return deleted;
  }

  getContent(): string { return this.content; }
}

interface EditorCommand {
  execute(): void;
  undo(): void;
}

class TypeCommand implements EditorCommand {
  private readonly editor: TextEditor;
  private readonly text: string;

  constructor(editor: TextEditor, text: string) {
    this.editor = editor;
    this.text = text;
  }

  execute(): void {
    this.editor.append(this.text);
    console.log(`Typed: "${this.text}"`);
  }

  undo(): void {
    this.editor.deleteLast(this.text.length);
    console.log(`Undo type: "${this.text}"`);
  }
}

class DeleteCommand implements EditorCommand {
  private readonly editor: TextEditor;
  private readonly count: number;
  private deletedText: string = "";

  constructor(editor: TextEditor, count: number) {
    this.editor = editor;
    this.count = count;
  }

  execute(): void {
    this.deletedText = this.editor.deleteLast(this.count);
    console.log(`Deleted: "${this.deletedText}"`);
  }

  undo(): void {
    this.editor.append(this.deletedText);
    console.log(`Undo delete: restored "${this.deletedText}"`);
  }
}

class EditorInvoker {
  private undoStack: EditorCommand[] = [];
  private redoStack: EditorCommand[] = [];

  execute(command: EditorCommand): void {
    command.execute();
    this.undoStack.push(command);
    this.redoStack = [];
  }

  undo(): void {
    if (this.undoStack.length > 0) {
      const command = this.undoStack.pop() as EditorCommand;
      command.undo();
      this.redoStack.push(command);
    } else {
      console.log("Nothing to undo.");
    }
  }

  redo(): void {
    if (this.redoStack.length > 0) {
      const command = this.redoStack.pop() as EditorCommand;
      command.execute();
      this.undoStack.push(command);
    } else {
      console.log("Nothing to redo.");
    }
  }
}

const editor = new TextEditor();
const invoker = new EditorInvoker();

invoker.execute(new TypeCommand(editor, "Hello"));
invoker.execute(new TypeCommand(editor, " World"));
invoker.execute(new TypeCommand(editor, "!"));
console.log(`Content: "${editor.getContent()}"`);

console.log("\n--- Undo ---");
invoker.undo();
console.log(`Content: "${editor.getContent()}"`);

invoker.undo();
console.log(`Content: "${editor.getContent()}"`);

console.log("\n--- Redo ---");
invoker.redo();
console.log(`Content: "${editor.getContent()}"`);

console.log("\n--- New operation clears redo ---");
invoker.execute(new DeleteCommand(editor, 3));
console.log(`Content: "${editor.getContent()}"`);

invoker.redo();

console.log("\n--- Undo delete ---");
invoker.undo();
console.log(`Content: "${editor.getContent()}"`);
