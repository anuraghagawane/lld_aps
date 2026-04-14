class CashRequest {
  public amount: number;

  constructor(amount: number) {
    this.amount = amount;
  }
}

interface CashHandler {
  setNext(next: CashHandler): void;
  dispense(request: CashRequest): void;
}

abstract class BaseCashHandler implements CashHandler {
  protected next: CashHandler | null = null;
  protected denomination: number;

  constructor(denomination: number) {
    this.denomination = denomination;
  }

  setNext(next: CashHandler): void {
    this.next = next;
  }

  dispense(request: CashRequest): void {
    if (request.amount >= this.denomination) {
      const noteCount = Math.floor(request.amount / this.denomination);
      request.amount = request.amount % this.denomination;
      console.log(`Dispensing ${noteCount} x $${this.denomination}`);
    }
    this.forward(request);
  }

  protected forward(request: CashRequest): void {
    if (this.next !== null) {
      this.next.dispense(request);
    }
  }
}

class HundredDollarHandler extends BaseCashHandler {
  constructor() { super(100); }
}

class FiftyDollarHandler extends BaseCashHandler {
  constructor() { super(50); }
}

class TwentyDollarHandler extends BaseCashHandler {
  constructor() { super(20); }
}

class TenDollarHandler extends BaseCashHandler {
  constructor() { super(10); }
}

// Usage
const hundreds = new HundredDollarHandler();
const fifties = new FiftyDollarHandler();
const twenties = new TwentyDollarHandler();
const tens = new TenDollarHandler();

hundreds.setNext(fifties);
fifties.setNext(twenties);
twenties.setNext(tens);

console.log("--- Withdrawing $380 ---");
const request1 = new CashRequest(380);
hundreds.dispense(request1);
console.log(`Remaining: $${request1.amount}`);

console.log("\n--- Withdrawing $275 ---");
const request2 = new CashRequest(275);
hundreds.dispense(request2);
console.log(`Remaining: $${request2.amount}`);
