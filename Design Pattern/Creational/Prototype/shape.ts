interface Cloneable {
    clone(): Cloneable;
}

class Circle implements Cloneable {
    color: string;
    radius: number;
    constructor(color: string, radius: number) {
        this.color = color;
        this.radius = radius;
    }

    // TODO: Implement clone()
    clone() : Cloneable {
        return new Circle(this.color, this.radius);
    }

    printInfo(): void {
        console.log(`Circle [Color: ${this.color}, Radius: ${this.radius.toFixed(1)}]`);
    }
}

class Rectangle implements Cloneable {
    color: string;
    width: number;
    height: number;
    constructor(color: string, width: number, height: number) {
        this.color = color;
        this.width = width;
        this.height = height;
    }

    // TODO: Implement clone()
    clone() : Cloneable {
        return new Rectangle(this.color, this.width, this.height);
    }

    printInfo(): void {
        console.log(`Rectangle [Color: ${this.color}, Width: ${this.width.toFixed(1)}, ` +
                     `Height: ${this.height.toFixed(1)}]`);
    }
}

// After implementing:
const original = new Circle("Red", 5.0);
const cloned = original.clone() as Circle;
cloned.radius = 10.0;
original.printInfo();
cloned.printInfo();

const rect = new Rectangle("Blue", 4.0, 6.0);
const clonedRect = rect.clone() as Rectangle;
clonedRect.width = 8.0;
rect.printInfo();
clonedRect.printInfo();
