interface Message {
  setContent(to: string, body: string): void;
  format(): string;
}

interface Sender {
  send(message: Message): void;
}

class EmailMessage implements Message {
  private to = "";
  private body = "";

  setContent(to: string, body: string): void {
    this.to = to;
    this.body = body;
  }

  format(): string {
    return `Email to <${this.to}>: ${this.body}`;
  }
}

class EmailSender implements Sender {
  send(message: Message): void {
    console.log(`Sending via SMTP: ${message.format()}`);
  }
}

class SmsMessage implements Message {
    private to = "";
    private body = "";

    setContent(to: string, body: string): void {
        this.to = to;
        this.body = body.length > 160 ? body.substring(0, 160) : body;
    }

    format(): string {
        return `SMS to ${this.to}: ${this.body}`;
    }
}

class SmsSender implements Sender {
    send(message: Message): void {
        console.log(`Sending via carrier API: ${message.format()}`);
    }
}

interface NotificationFactory {
  createMessage(): Message;
  createSender(): Sender;
}

class EmailFactory implements NotificationFactory {
  createMessage(): Message {return new EmailMessage();}
  createSender(): Sender {return new EmailSender();}
}

class SmsFactory implements NotificationFactory {
    createMessage(): Message { return new SmsMessage(); }
    createSender(): Sender { return new SmsSender(); }
}

class NotificationService {
  private factory: NotificationFactory;

  constructor(factory: NotificationFactory) {
    this.factory = factory;
  }

  notify(to: string, body: string): void {
    const message = this.factory.createMessage();
    message.setContent(to, body);
    const sender = this.factory.createSender();
    sender.send(message);
  }
}

// Entry Point
console.log("=== Email Notification ===");
const emailService = new NotificationService(new EmailFactory());
emailService.notify("alice@example.com", "Your order has been shipped!");

console.log();

console.log("=== SMS Notification ===");
const smsService = new NotificationService(new SmsFactory());
smsService.notify("+1-555-0123", "Your order has been shipped!");
