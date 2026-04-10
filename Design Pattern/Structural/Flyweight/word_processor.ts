interface FontStyle {
    format(line: number, column: number, character: string): void;
}

class ConcreteFontStyle implements FontStyle {
    // TODO: Add fields (fontFamily, fontSize, bold, italic)
    private fontFamily: string;
    private fontSize: number;
    private bold: boolean;
    private italic: boolean;

    constructor(fontFamily: string, fontSize: number, bold: boolean, italic: boolean) {
        // TODO: Store fontFamily, fontSize, bold, italic
        this.fontFamily = fontFamily;
        this.fontSize = fontSize;
        this.bold = bold;
        this.italic = italic;
    }

    format(line: number, column: number, character: string): void {
        // TODO: Print "[fontFamily, fontSize, style] 'character' at line:column"
        // where style is "bold" if bold, "italic" if italic, otherwise "normal"
        // Example: "[Arial, 12, normal] 'H' at 1:1"
        console.log(`[${this.fontFamily}, ${this.fontSize}, ${this.bold ? "bold" : (this.italic ? "italic" : "normal")}] '${character}' at ${line}:${column}`);
    }
}

class FontStyleFactory {
    // TODO: Add a Map<string, FontStyle> cache field
    private cache: Map<string, FontStyle> = new Map();

    getFontStyle(fontFamily: string, fontSize: number, bold: boolean, italic: boolean): FontStyle {
        // TODO: Build key from fontFamily + fontSize + bold + italic
        // TODO: If key exists in cache, return cached instance
        // TODO: Otherwise create new ConcreteFontStyle, store in cache, return it
        const key = `${fontFamily}${fontSize}${bold}${italic}`;
        
        const cached = this.cache.get(key);
        if(cached){
            return cached;
        }
        const style = new ConcreteFontStyle(fontFamily, fontSize, bold, italic);
        this.cache.set(key, style);
        return style;
    }

    getStyleCount(): number {
        // TODO: Return the number of cached styles
        return this.cache.size;
    }
}

const factory = new FontStyleFactory();
const arial12 = factory.getFontStyle("Arial", 12, false, false);
const arial12Bold = factory.getFontStyle("Arial", 12, true, false);
const arial12Again = factory.getFontStyle("Arial", 12, false, false);

arial12.format(1, 1, "H");
arial12Bold.format(1, 2, "e");
arial12Again.format(1, 3, "l");

console.log("Same instance? " + (arial12 === arial12Again));
console.log("Total styles: " + factory.getStyleCount());
