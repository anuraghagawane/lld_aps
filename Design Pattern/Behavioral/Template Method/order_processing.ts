class Order {
  public id: string;
  public subtotal: number;

  constructor(id: string, subtotal: number) {
    this.id = id;
    this.subtotal = subtotal;
  }
}

abstract class OrderProcessor {
  public processOrder(order: Order): void {
    this.validateOrder(order);
    this.calculateTotal(order);
    this.applyDiscount(order);
    this.processPayment(order);
    this.sendConfirmation(order);
    console.log("Order processing complete: " + order.id);
  }

  protected abstract validateOrder(order: Order): void;
  protected abstract calculateTotal(order: Order): void;
  protected abstract processPayment(order: Order): void;

  protected applyDiscount(order: Order): void {}

  protected sendConfirmation(order: Order): void {
    console.log("Sending email confirmation for order " + order.id);
  }
}

class StandardOrderProcessor extends OrderProcessor {
  protected validateOrder(order: Order): void {
    console.log("Validating standard order: checking stock and address...");
  }

  protected calculateTotal(order: Order): void {
    const shipping = 5.99;
    const total = order.subtotal + shipping;
    console.log(`Standard total: $${total} (including $${shipping} shipping)`);
  }

  protected processPayment(order: Order): void {
    console.log("Processing payment via standard gateway...");
  }
}

class PrimeOrderProcessor extends OrderProcessor {
  protected validateOrder(order: Order): void {
    console.log("Validating Prime order: checking membership and stock...");
  }

  protected calculateTotal(order: Order): void {
    console.log(`Prime total: $${order.subtotal} (free shipping)`);
  }

  protected processPayment(order: Order): void {
    console.log("Processing payment via Prime billing...");
  }

  protected applyDiscount(order: Order): void {
    console.log("Applying 10% Prime member discount...");
  }
}

class InternationalOrderProcessor extends OrderProcessor {
  protected validateOrder(order: Order): void {
    console.log(
      "Validating international order: customs, restricted items, address format..."
    );
  }

  protected calculateTotal(order: Order): void {
    const shipping = 24.99;
    const customs = order.subtotal * 0.15;
    const total = order.subtotal + shipping + customs;
    console.log(
      `International total: $${total} (shipping: $${shipping}, customs: $${customs})`
    );
  }

  protected processPayment(order: Order): void {
    console.log("Processing payment with currency conversion...");
  }

  protected sendConfirmation(order: Order): void {
    console.log(
      "Sending multi-language confirmation with tracking for order " + order.id
    );
  }
}

const order1 = new Order("ORD-001", 49.99);
const order2 = new Order("ORD-002", 149.99);
const order3 = new Order("ORD-003", 89.99);

const standard: OrderProcessor = new StandardOrderProcessor();
standard.processOrder(order1);

console.log();

const prime: OrderProcessor = new PrimeOrderProcessor();
prime.processOrder(order2);

console.log();

const international: OrderProcessor = new InternationalOrderProcessor();
international.processOrder(order3);
