interface PaymentStrategy {
  pay(amount: number): boolean;
}

class CreditCardPayment implements PaymentStrategy {
  private readonly cardNumber: string;
  private readonly expiryDate: string;

  constructor(cardNumber: string, expiryDate: string) {
    this.cardNumber = cardNumber;
    this.expiryDate = expiryDate;
  }

  pay(amount: number): boolean {
    console.log(`Charging $${amount} to credit card ending in ${this.cardNumber.slice(-4)}`);
    return true;
  }
}

class PayPalPayment implements PaymentStrategy {
  private readonly email: string;

  constructor(email: string) {
    this.email = email;
  }

  pay(amount: number): boolean {
    console.log(`Sending $${amount} via PayPal to ${this.email}`);
    return true;
  }
}

class CryptoPayment implements PaymentStrategy {
  private readonly walletAddress: string;

  constructor(walletAddress: string) {
    this.walletAddress = walletAddress;
  }

  pay(amount: number): boolean {
    console.log(`Transferring $${amount} in crypto to ${this.walletAddress}`);
    return true;
  }
}

class CheckoutService {
  private paymentStrategy: PaymentStrategy;

  constructor(paymentStrategy: PaymentStrategy) {
    this.paymentStrategy = paymentStrategy;
  }

  setPaymentStrategy(paymentStrategy: PaymentStrategy): void {
    this.paymentStrategy = paymentStrategy;
  }

  checkout(amount: number): boolean {
    return this.paymentStrategy.pay(amount);
  }
}

// Usage
const checkout = new CheckoutService(new CreditCardPayment("4111111111111111", "12/26"));
checkout.checkout(99.99);

checkout.setPaymentStrategy(new PayPalPayment("user@example.com"));
checkout.checkout(49.99);

checkout.setPaymentStrategy(new CryptoPayment("0xABC123..."));
checkout.checkout(149.99);
