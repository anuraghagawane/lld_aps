interface DocumentState {
    edit(context: Document, content: string): void;
    submitForReview(context: Document): void;
    approve(context: Document): void;
    reject(context: Document): void;
    unpublish(context: Document): void;
}

class DraftState implements DocumentState {
    edit(context: Document, content: string): void {
        console.log("Editing document: " + content);
        context.setContent(content);
    }

    submitForReview(context: Document): void {
        console.log("Document submitted for review.");
        context.setState(new UnderReviewState());
    }

    approve(context: Document): void {
        console.log("Cannot approve a draft. Submit for review first.");
    }

    reject(context: Document): void {
        console.log("Cannot reject a draft. Submit for review first.");
    }

    unpublish(context: Document): void {
        console.log("Document is already a draft.");
    }
}

class UnderReviewState implements DocumentState {
    edit(context: Document, content: string): void {
        console.log("Cannot edit while under review.");
    }

    submitForReview(context: Document): void {
        console.log("Document is already under review.");
    }

    approve(context: Document): void {
        console.log("Document approved and published.");
        context.setState(new PublishedState());
    }

    reject(context: Document): void {
        console.log("Document rejected. Returning to draft.");
        context.setState(new DraftState());
    }

    unpublish(context: Document): void {
        console.log("Document is not published yet.");
    }
}

class PublishedState implements DocumentState {
    edit(context: Document, content: string): void {
        console.log("Cannot edit a published document. Unpublish first.");
    }

    submitForReview(context: Document): void {
        console.log("Document is already published.");
    }

    approve(context: Document): void {
        console.log("Document is already published.");
    }

    reject(context: Document): void {
        console.log("Cannot reject a published document.");
    }

    unpublish(context: Document): void {
        console.log("Document unpublished. Returning to draft.");
        context.setState(new DraftState());
    }
}

class Document {
    private currentState: DocumentState;
    private content: string = "";

    constructor() { this.currentState = new DraftState(); }

    setState(state: DocumentState): void { this.currentState = state; }
    setContent(content: string): void { this.content = content; }
    getContent(): string { return this.content; }

    edit(content: string): void { this.currentState.edit(this, content); }
    submitForReview(): void { this.currentState.submitForReview(this); }
    approve(): void { this.currentState.approve(this); }
    reject(): void { this.currentState.reject(this); }
    unpublish(): void { this.currentState.unpublish(this); }
}

// Usage
const doc = new Document();

doc.edit("First draft of the article.");
doc.approve();                    // Rejected: cannot approve a draft
doc.submitForReview();
doc.edit("Trying to edit");       // Rejected: under review
doc.reject();                     // Back to draft
doc.edit("Revised draft.");
doc.submitForReview();
doc.approve();                    // Published
doc.edit("Trying to edit");       // Rejected: published
doc.unpublish();                  // Back to draft
