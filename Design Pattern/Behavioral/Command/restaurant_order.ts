interface OrderCommand {
    execute(): void;
    undo(): void;
}

class Kitchen {
    prepareDish(dish: string): void {
        // TODO: Print "Preparing: " + dish
        console.log("Preparing: " + dish);
    }

    cancelDish(dish: string): void {
        // TODO: Print "Cancelling: " + dish
        console.log("Cancelling: "+ dish);
    }
}

class PlaceOrderCommand implements OrderCommand {
    private kitchen: Kitchen;
    private dish: string;

    constructor(kitchen: Kitchen, dish: string) {
        this.kitchen = kitchen;
        this.dish = dish;
    }

    execute(): void {
        // TODO: Call this.kitchen.prepareDish(this.dish)
        this.kitchen.prepareDish(this.dish)
    }

    undo(): void {
        // TODO: Call this.kitchen.cancelDish(this.dish)
        this.kitchen.cancelDish(this.dish)
    }
}

class CancelOrderCommand implements OrderCommand {
    private kitchen: Kitchen;
    private dish: string;

    constructor(kitchen: Kitchen, dish: string) {
        this.kitchen = kitchen;
        this.dish = dish;
    }

    execute(): void {
        // TODO: Call 
        this.kitchen.cancelDish(this.dish)
    }

    undo(): void {
        // TODO: Call this.kitchen.prepareDish(this.dish)
        this.kitchen.prepareDish(this.dish)
    }
}

class Waiter {
    private pending: OrderCommand[] = [];
    private history: OrderCommand[] = [];

    takeOrder(command: OrderCommand): void {
        // TODO: Add command to pending queue
        this.pending.push(command);
    }

    submitOrders(): void {
        // TODO: Execute all pending commands, move to history, clear pending
        for(const cmd of this.pending) {
            cmd.execute();
            this.history.push(cmd);
        }
        this.pending = [];
    }

    undoLast(): void {
        // TODO: Pop from history and call undo()
        if (this.history.length > 0) {
            this.history.pop()!.undo();
        }
    }
}

const kitchen = new Kitchen();
const waiter = new Waiter();
waiter.takeOrder(new PlaceOrderCommand(kitchen, "Pasta"));
waiter.takeOrder(new PlaceOrderCommand(kitchen, "Salad"));
waiter.submitOrders();
waiter.takeOrder(new CancelOrderCommand(kitchen, "Salad"));
waiter.submitOrders();
waiter.undoLast();
