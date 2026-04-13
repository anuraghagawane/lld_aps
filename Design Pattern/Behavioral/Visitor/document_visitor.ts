interface DocumentElement {
    accept(visitor: DocumentVisitor): void;
}

interface DocumentVisitor {
    visitParagraph(paragraph: Paragraph): void;
    visitHeading(heading: Heading): void;
    visitImage(image: Image): void;
}

// Elements
class Paragraph implements DocumentElement {
    readonly text: string;
    constructor(text: string) {
        this.text = text;
    }

    accept(visitor: DocumentVisitor): void {
        // TODO: Call visitor.visitParagraph(this)
        visitor.visitParagraph(this)
    }
}

class Heading implements DocumentElement {
    readonly text: string;
    readonly level: number;
    constructor(text: string, level: number) {
        this.text = text;
        this.level = level;
    }

    accept(visitor: DocumentVisitor): void {
        // TODO: Call visitor.visitHeading(this)
        visitor.visitHeading(this)
    }
}

class Image implements DocumentElement {
    readonly url: string;
    readonly altText: string;
    constructor(url: string, altText: string) {
        this.url = url;
        this.altText = altText;
    }

    accept(visitor: DocumentVisitor): void {
        // TODO: Call visitor.visitImage(this)
        visitor.visitImage(this)
    }
}

// Visitors
class WordCountVisitor implements DocumentVisitor {
    private wordCount: number = 0;

    getWordCount(): number {
        return this.wordCount;
    }

    visitParagraph(paragraph: Paragraph): void {
        // TODO: Split paragraph text by spaces, add count to wordCount
        this.wordCount += paragraph.text.split(" ").length;
    }

    visitHeading(heading: Heading): void {
        // TODO: Split heading text by spaces, add count to wordCount
        this.wordCount += heading.text.split(" ").length;
    }

    visitImage(image: Image): void {
        // TODO: Images contribute 0 words, nothing to do
    }
}

class HtmlExportVisitor implements DocumentVisitor {
    visitParagraph(paragraph: Paragraph): void {
        // TODO: Print "<p>text</p>"
        console.log(`<p>${paragraph.text}</p>`)
    }

    visitHeading(heading: Heading): void {
        // TODO: Print "<h{level}>text</h{level}>"
        console.log(`<h${heading.level}>${heading.text}</h${heading.level}>`)
    }

    visitImage(image: Image): void {
        // TODO: Print "<img src=\"url\" alt=\"altText\" />"
        console.log(`<img src=\"${image.url}\" alt=\"${image.altText}\" />`)
    }
}

const doc: DocumentElement[] = [
    new Heading("My Document", 1),
    new Paragraph("This is the first paragraph with some text."),
    new Image("photo.jpg", "A photo"),
    new Paragraph("Another paragraph here.")
];

const counter = new WordCountVisitor();
for (const el of doc) el.accept(counter);
console.log(`Word count: ${counter.getWordCount()}`);

const exporter = new HtmlExportVisitor();
for (const el of doc) el.accept(exporter);
