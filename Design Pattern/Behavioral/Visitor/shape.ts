interface Shape {
   accept(visitor: ShapeVisitor): void;
}

class Circle implements Shape {
   private readonly radius: number;

   constructor(radius: number) {
       this.radius = radius;
   }

   getRadius(): number {
       return this.radius;
   }

   accept(visitor: ShapeVisitor): void {
       visitor.visitCircle(this);
   }
}

class Rectangle implements Shape {
   private readonly width: number;
   private readonly height: number;

   constructor(width: number, height: number) {
       this.width = width;
       this.height = height;
   }

   getWidth(): number {
       return this.width;
   }

   getHeight(): number {
       return this.height;
   }

   accept(visitor: ShapeVisitor): void {
       visitor.visitRectangle(this);
   }
}

interface ShapeVisitor {
   visitCircle(circle: Circle): void;
   visitRectangle(rectangle: Rectangle): void;
}

class AreaCalculatorVisitor implements ShapeVisitor {
   visitCircle(circle: Circle): void {
       const area = Math.PI * circle.getRadius() * circle.getRadius();
       console.log("Area of Circle: " + area);
   }

   visitRectangle(rectangle: Rectangle): void {
       const area = rectangle.getWidth() * rectangle.getHeight();
       console.log("Area of Rectangle: " + area);
   }
}

class SvgExporterVisitor implements ShapeVisitor {
   visitCircle(circle: Circle): void {
       console.log("<circle r=\"" + circle.getRadius() + "\" />");
   }

   visitRectangle(rectangle: Rectangle): void {
       console.log("<rect width=\"" + rectangle.getWidth() + 
           "\" height=\"" + rectangle.getHeight() + "\" />");
   }
}

class VisitorPatternDemo {
   static main(): void {
       const shapes: Shape[] = [
           new Circle(5),
           new Rectangle(10, 4),
           new Circle(2.5)
       ];

       console.log("=== Calculating Areas ===");
       const areaCalculator: ShapeVisitor = new AreaCalculatorVisitor();
       for (const shape of shapes) {
           shape.accept(areaCalculator);
       }

       console.log("\n=== Exporting to SVG ===");
       const svgExporter: ShapeVisitor = new SvgExporterVisitor();
       for (const shape of shapes) {
           shape.accept(svgExporter);
       }
   }
}

VisitorPatternDemo.main();
