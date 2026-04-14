// Memento
class GameMemento {
    private readonly _health: number;
    private readonly _level: number;
    private readonly _position: number;

    constructor(health: number, level: number, position: number) {
        // TODO: Store health, level, and position
        this._health = health;
        this._level = level;
        this._position = position;
    }

    get health(): number {
        // TODO: Return health
        return this._health;
    }

    get level(): number {
        // TODO: Return level
        return this._level;
    }

    get position(): number {
        // TODO: Return position
        return this._position;
    }
}

// Originator
class Game {
    private health: number = 100;
    private level: number = 1;
    private position: number = 0;

    play(): void {
        // TODO: Increment level by 1 and position by 10
        // TODO: Print "Playing... Level: X, Position: Y, Health: Z"
        this.level++;
        this.position += 10;
        console.log(`Playing... Level: ${this.level}, Position: ${this.position}, Health: ${this.health}`);
    }

    takeDamage(amount: number): void {
        // TODO: Reduce health by amount
        // TODO: Print "Took X damage. Health: Y"
        this.health -= amount;
        console.log(`Took ${amount} damage. Health: ${this.health}`);
    }

    save(): GameMemento {
        // TODO: Return a new GameMemento with current state
        return new GameMemento(this.health, this.level, this.position);
    }

    restore(memento: GameMemento): void {
        // TODO: Restore health, level, and position from memento
        // TODO: Print "Game loaded: Level: X, Position: Y, Health: Z"
        this.health = memento.health;
        this.level = memento.level;
        this.position = memento.position;
        console.log(`Game loaded: Level: ${this.level}, Position: ${this.position}, Health: ${this.health}`);
    }
}

// Caretaker
class SaveManager {
    private readonly slots: Map<number, GameMemento> = new Map();

    save(game: Game, slot: number): void {
        // TODO: Save game state to the given slot
        this.slots.set(slot, game.save()); 
    }

    load(game: Game, slot: number): void {
        // TODO: Load game state from the given slot
        // TODO: Print "No save in slot X." if slot is empty
        const memento = this.slots.get(slot);
        if (!memento) {
            console.log(`No save in slot ${slot}.`);
            return;
        }
        game.restore(memento);
    }
}

const game = new Game();
const saveManager = new SaveManager();

game.play();                // Level 2, Position 10
game.play();                // Level 3, Position 20
saveManager.save(game, 0);  // Save to slot 0

game.takeDamage(50);        // Health: 50
game.play();                // Level 4, Position 30
saveManager.save(game, 1);  // Save to slot 1

game.takeDamage(40);        // Health: 10
console.log("\n--- Load Slot 0 ---");
saveManager.load(game, 0);  // Back to: Health 100, Level 3, Position 20

console.log("\n--- Load Slot 1 ---");
saveManager.load(game, 1);  // Back to: Health 50, Level 4, Position 30
