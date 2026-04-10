// Flyweight interface
interface TreeType {
  render(x: number, y: number): void;
}

// Concrete flyweight
class ConcreteTreeType implements TreeType {
  private readonly name: string;
  private readonly color: string;
  private readonly texture: string;

  constructor(name: string, color: string, texture: string) {
    this.name = name;
    this.color = color;
    this.texture = texture;
  }

  render(x: number, y: number): void {
    console.log(
      `Rendering ${this.name} tree [color=${this.color}, texture=${this.texture}] at (${x},${y})`
    );
  }
}

// Flyweight factory
class TreeTypeFactory {
  private readonly treeTypes: Map<string, TreeType> = new Map();

  getTreeType(name: string, color: string, texture: string): TreeType {
    const key = `${name}_${color}_${texture}`;
    if (!this.treeTypes.has(key)) {
      this.treeTypes.set(key, new ConcreteTreeType(name, color, texture));
    }
    // Non-null because we ensure set() above
    return this.treeTypes.get(key) as TreeType;
  }

  getTypeCount(): number {
    return this.treeTypes.size;
  }
}

// Extrinsic state holder
class Tree {
  private readonly type: TreeType;
  private readonly x: number;
  private readonly y: number;

  constructor(type: TreeType, x: number, y: number) {
    this.type = type;
    this.x = x;
    this.y = y;
  }

  draw(): void {
    this.type.render(this.x, this.y);
  }
}

// Client
class Forest {
  private readonly factory = new TreeTypeFactory();
  private readonly trees: Tree[] = [];

  plantTree(x: number, y: number, name: string, color: string, texture: string): void {
    const type = this.factory.getTreeType(name, color, texture);
    this.trees.push(new Tree(type, x, y));
  }

  render(): void {
    for (const tree of this.trees) {
      tree.draw();
    }
    console.log(`\nTotal trees planted: ${this.trees.length}`);
    console.log(`Unique tree types created: ${this.factory.getTypeCount()}`);
  }
}

const forest = new Forest();

forest.plantTree(10, 20, "Oak", "dark green", "rough bark");
forest.plantTree(50, 80, "Pine", "green", "needle texture");
forest.plantTree(30, 60, "Oak", "dark green", "rough bark");
forest.plantTree(70, 40, "Birch", "light green", "white bark");
forest.plantTree(90, 10, "Pine", "green", "needle texture");

forest.render();
