// TODO: Implement as ES module singleton or class-based singleton

class Counter {
    private count: number = 0;
    private static instance: Counter;

    private constructor() {}

    static getInstance(): Counter {
        if(!Counter.instance) {
            Counter.instance = new Counter();
        }
        return Counter.instance;
    }

    increment(): void {
        // TODO: Increment count
        this.count++;
    }

    getCount(): number {
        // TODO: Return current count
        return this.count;
    }
}

// export const counter = new Counter();

// After implementing, usage should look like:
const c1 = Counter.getInstance();
const c2 = Counter.getInstance();
console.log("Same instance:", c1 === c2);
for (let i = 0; i < 5; i++) {
    c1.increment();
}
console.log("Count after 5 increments:", c1.getCount());
