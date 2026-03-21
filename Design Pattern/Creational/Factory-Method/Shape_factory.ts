interface Shape {
    area(): number;
    describe(): void;
}

class Circle implements Shape {
    // TODO: Add a private field for radius (number)
    private radius: number;

    constructor(radius: number) {
        // TODO: Store the radius
        this.radius = radius;
    }

    area(): number {
        // TODO: Return Math.PI * radius * radius
        return Math.PI * this.radius * this.radius;
    }

    describe(): void {
        // TODO: Print "Circle with area: " followed by area() formatted to 2 decimal places
        // Hint: Use console.log(`Circle with area: ${this.area().toFixed(2)}`)
        console.log(`Circle with area: ${this.area().toFixed(2)}`)
    }
}

class Rectangle implements Shape {
    // TODO: Add private fields for width and height (number)
    private width: number;
    private height: number;

    constructor(width: number, height: number) {
        // TODO: Store width and height
        this.width = width;
        this.height = height;
    }

    area(): number {
        // TODO: Return width * height
        return this.width * this.height;
    }

    describe(): void {
        // TODO: Print "Rectangle with area: " followed by area() formatted to 2 decimal places
        console.log(`Rectangle with area: ${this.area().toFixed(2)}`)
    }
}

class Triangle implements Shape {
    // TODO: Add private fields for base and height (number)
    private base: number;
    private height: number;

    constructor(base: number, height: number) {
        // TODO: Store base and height
        this.base = base;
        this.height = height;
    }

    area(): number {
        // TODO: Return 0.5 * base * height
        return 0.5 * this.base * this.height;
    }

    describe(): void {
        // TODO: Print "Triangle with area: " followed by area() formatted to 2 decimal places
        console.log(`Triangle with area: ${this.area().toFixed(2)}`)
    }
}

class ShapeCreator {
    // Factory method - subclasses decide which Shape to create
    createShape(): Shape {
        return null as any;
    }

    describe(): void {
        // TODO: Call createShape() to get a Shape instance
        // TODO: Call describe() on the shape
        const shape = this.createShape();
        shape.describe();
    }
}

class CircleCreator extends ShapeCreator {
    createShape(): Shape {
        // TODO: Return a new Circle with radius 5
        return new Circle(5);
    }
}

class RectangleCreator extends ShapeCreator {
    createShape(): Shape {
        // TODO: Return a new Rectangle with width 4 and height 6
        return new Rectangle(4, 6);
    }
}

class TriangleCreator extends ShapeCreator {
    createShape(): Shape {
        // TODO: Return a new Triangle with base 3 and height 8
        return new Triangle(3, 8);
    }
}

function main(): void {
    let creator: ShapeCreator;
    
    creator = new CircleCreator();
    creator.describe();
    
    creator = new RectangleCreator();
    creator.describe();
    
    creator = new TriangleCreator();
    creator.describe();
}

main();
