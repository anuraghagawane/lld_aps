class TextEditorMemento {
   private readonly state: string;

   constructor(state: string) {
       this.state = state;
   }

   getState(): string {
       return this.state;
   }
}

class TextEditor {
    private content: string = "";

    type(newText: string): void {
        this.content += newText;
        console.log(`Typed: "${newText}"`);
    }

    getContent(): string {
        return this.content;
    }

    save(): TextEditorMemento {
        console.log(`Saving state: "${this.content}"`);
        return new TextEditorMemento(this.content);
    }

    restore(memento: TextEditorMemento): void {
        this.content = memento.getState();
        console.log(`Restored state to: "${this.content}"`);
    }
}

class TextEditorUndoManager {
    private readonly history: TextEditorMemento[] = [];

    save(editor: TextEditor): void {
        this.history.push(editor.save());
    }

    undo(editor: TextEditor): void {
        if (this.history.length > 0) {
            const memento = this.history.pop()!;
            editor.restore(memento);
        } else {
            console.log("Nothing to undo.");
        }
    }

    historySize(): number {
        return this.history.length;
    }
}

const editor = new TextEditor();
const undoManager = new TextEditorUndoManager();

editor.type("Hello");
undoManager.save(editor);

editor.type(" World");
undoManager.save(editor);

editor.type("!");
console.log("Current: " + editor.getContent());

console.log("\n--- Undo 1 ---");
undoManager.undo(editor);
console.log("Content: " + editor.getContent());

console.log("\n--- Undo 2 ---");
undoManager.undo(editor);
console.log("Content: " + editor.getContent());

console.log("\n--- Undo 3 ---");
undoManager.undo(editor);
