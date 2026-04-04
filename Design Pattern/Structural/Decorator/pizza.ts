interface Pizza {
    getCost(): number;
    getDescription(): string;
}

class PlainPizza implements Pizza {
    getCost(): number { return 5.00; }
    getDescription(): string { return "Plain pizza"; }
}

abstract class PizzaDecorator implements Pizza {
    // TODO: Add a protected field to store the wrapped Pizza reference
    protected inner: Pizza;

    constructor(pizza: Pizza) {
        // TODO: Store the wrapped pizza
        this.inner = pizza;
    }

    getCost(): number {
        // TODO: Delegate to the wrapped pizza's getCost()
        return this.inner.getCost();
    }

    getDescription(): string {
        // TODO: Delegate to the wrapped pizza's getDescription()
        return this.inner.getDescription();
    }
}

class CheeseDecorator extends PizzaDecorator {
    constructor(pizza: Pizza) {
        super(pizza);
    }

    getCost(): number {
        // TODO: Return the wrapped pizza's cost + 1.50
        return this.inner.getCost() + 1.50;
    }

    getDescription(): string {
        // TODO: Return the wrapped pizza's description + ", cheese"
        return this.inner.getDescription() + ", cheese";
    }
}

class OliveDecorator extends PizzaDecorator {
    constructor(pizza: Pizza) {
        super(pizza);
    }

    getCost(): number {
        // TODO: Return the wrapped pizza's cost + 2.00
        return this.inner.getCost() + 2.00;
    }

    getDescription(): string {
        // TODO: Return the wrapped pizza's description + ", olives"
        return this.inner.getDescription() + ", olives";
    }
}

class MushroomDecorator extends PizzaDecorator {
    constructor(pizza: Pizza) {
        super(pizza);
    }

    getCost(): number {
        // TODO: Return the wrapped pizza's cost + 1.00
        return this.inner.getCost() + 1.00;
    }

    getDescription(): string {
        // TODO: Return the wrapped pizza's description + ", mushrooms"
        return this.inner.getDescription() + ", mushrooms";
    }
}

const plain: Pizza = new PlainPizza();
console.log(`${plain.getDescription()} | $${plain.getCost().toFixed(2)}`);

const cheeseOlive: Pizza = new OliveDecorator(new CheeseDecorator(new PlainPizza()));
console.log(`${cheeseOlive.getDescription()} | $${cheeseOlive.getCost().toFixed(2)}`);

const loaded: Pizza = new MushroomDecorator(
    new OliveDecorator(new CheeseDecorator(new PlainPizza())));
console.log(`${loaded.getDescription()} | $${loaded.getCost().toFixed(2)}`);
