interface MessageSender {
    sendMessage(content: string): void;
}

class EmailSender implements MessageSender {
    sendMessage(content: string): void {
        console.log(`Email: ${content}`);
    }
}

class SmsSender implements MessageSender {
    sendMessage(content: string): void {
        console.log(`SMS: ${content}`);
    }
}

abstract class Message {
    // TODO: Add a protected field for the MessageSender reference
    // TODO: Add a protected field for the content (string)
    protected sender: MessageSender;
    protected content: string;
    constructor(sender: MessageSender, content: string) {
        // TODO: Store the sender and content
        this.sender = sender;
        this.content = content;
    }

    abstract send(): void;
}

class TextMessage extends Message {
    constructor(sender: MessageSender, content: string) {
        super(sender, content);
    }

    send(): void {
        // TODO: Delegate to this.sender.sendMessage() with the content as-is
        this.sender.sendMessage(this.content);
    }
}

class UrgentMessage extends Message {
    constructor(sender: MessageSender, content: string) {
        super(sender, content);
    }

    send(): void {
        // TODO: Delegate to this.sender.sendMessage() with "[URGENT] " prepended to the content
        this.sender.sendMessage("[URGENT] " + this.content);
    }
}

const email: MessageSender = new EmailSender();
const sms: MessageSender = new SmsSender();
const m1 = new TextMessage(email, "Hello there");
const m2 = new UrgentMessage(sms, "Server is down");
m1.send();
m2.send();
