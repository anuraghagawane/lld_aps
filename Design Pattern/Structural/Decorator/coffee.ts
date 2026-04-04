interface Coffee {
  getCost(): number;
  getDescription(): string;
}

class SimpleCoffee implements Coffee {
  getCost(): number {
    return 1.00;
  }

  getDescription(): string {
    return "Simple Coffee";
  }
}

abstract class CoffeeDecorator implements Coffee {
  protected readonly inner: Coffee;
  constructor(inner: Coffee) {
    this.inner = inner;
  }
  abstract getCost(): number;
  abstract getDescription(): string;
}

class MilkDecorator extends CoffeeDecorator {
  constructor(inner: Coffee) {
    super(inner);
  }

  getCost(): number {
    return this.inner.getCost() + 0.50;
  }

  getDescription(): string {
    return this.inner.getDescription() + ", milk";
  }
}

class SugarDecorator extends CoffeeDecorator {
    constructor(inner: Coffee) {
        super(inner);
    }
    getCost(): number {
        return this.inner.getCost() + 0.20;
    }
    getDescription(): string {
        return this.inner.getDescription() + ", sugar";
    }
}

class WhippedCreamDecorator extends CoffeeDecorator {
    constructor(inner: Coffee) {
        super(inner);
    }
    getCost(): number {
        return this.inner.getCost() + 1.00;
    }
    getDescription(): string {
        return this.inner.getDescription() + ", whipped cream";
    }
}

const order1: Coffee = new SimpleCoffee();
console.log(`Order 1: ${order1.getDescription()} | $${order1.getCost().toFixed(2)}`);

const order2: Coffee = new SugarDecorator(new MilkDecorator(new SimpleCoffee()));
console.log(`Order 2: ${order2.getDescription()} | $${order2.getCost().toFixed(2)}`);

const order3: Coffee = new WhippedCreamDecorator(
    new SugarDecorator(new MilkDecorator(new MilkDecorator(new SimpleCoffee()))));
console.log(`Order 3: ${order3.getDescription()} | $${order3.getCost().toFixed(2)}`);
