// Mediator interface
interface ChatMediator {
    sendMessage(message: string, sender: ChatUser): void;
    addUser(user: ChatUser): void;
}

class ChatRoom implements ChatMediator {
    private users: ChatUser[] = [];

    addUser(user: ChatUser): void {
        this.users.push(user);
        console.log(user.name + " joined the chat.");
    }

    sendMessage(message: string, sender: ChatUser): void {
        for (const user of this.users) {
            if (user !== sender) {
                user.receive(message, sender.name);
            }
        }
    }
}

class ChatUser {
    private mediator: ChatMediator;
    public name: string;

    constructor(mediator: ChatMediator, name: string) {
        this.mediator = mediator;
        this.name = name;
    }

    send(message: string): void {
        console.log(this.name + " sends: " + message);
        this.mediator.sendMessage(message, this);
    }

    receive(message: string, from: string): void {
        console.log(this.name + " receives from " + from + ": " + message);
    }
}

const chatRoom = new ChatRoom();

const alice = new ChatUser(chatRoom, "Alice");
const bob = new ChatUser(chatRoom, "Bob");
const charlie = new ChatUser(chatRoom, "Charlie");

chatRoom.addUser(alice);
chatRoom.addUser(bob);
chatRoom.addUser(charlie);

console.log();
alice.send("Hey everyone!");
console.log();
bob.send("Hi Alice!");
