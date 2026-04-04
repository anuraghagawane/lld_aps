interface Menu {
    display(indent: string): void;
    getItemCount(): number;
}

class MenuItem implements Menu {
    private readonly name: string;
    private readonly price: number;
    constructor(name: string, price: number) {
        this.name = name;
        this.price = price;
    }

    display(indent: string): void {
        console.log(`${indent}${this.name} - $${this.price.toFixed(2)}`);
    }

    getItemCount(): number { return 1; }
}

class SubMenu implements Menu {
    // TODO: Add a field to store the submenu name (string)
    // TODO: Add a field to store the list of Menu children (Menu[])
    private readonly name: string;
    private children: Menu[];

    constructor(name: string) {
        // TODO: Store the name and initialize the children array
        this.name = name;
        this.children = [];
    }

    addItem(item: Menu): void {
        // TODO: Add the given item to the children array
        this.children.push(item);
    }

    display(indent: string): void {
        // TODO: Print the submenu name with indent + colon
        // TODO: Loop through children, calling child.display() with indent + "  "
        console.log(`${indent}${this.name}:`);
        for(const child of this.children) {
            child.display(indent + "  ");
        }
    }

    getItemCount(): number {
        // TODO: Sum all children's getItemCount()
        let total = 0;
        for(const child of this.children) {
            total += child.getItemCount();
        }
        return total;
    }
}

const burger = new MenuItem("Burger", 8.99);
const fries = new MenuItem("Fries", 3.99);
const cola = new MenuItem("Cola", 1.99);
const water = new MenuItem("Water", 0.99);

const drinks = new SubMenu("Drinks");
drinks.addItem(cola);
drinks.addItem(water);

const mainMenu = new SubMenu("Main Menu");
mainMenu.addItem(burger);
mainMenu.addItem(fries);
mainMenu.addItem(drinks);

mainMenu.display("");
console.log("\nTotal items: " + mainMenu.getItemCount());
