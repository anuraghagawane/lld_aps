abstract class BeverageMaker {
    public prepareBeverage(): void {
        this.boilWater();
        this.brew();
        this.pourInCup();
        this.addCondiments();
    }

    private boilWater(): void {
        console.log("Boiling water...");
    }

    protected abstract brew(): void;

    private pourInCup(): void {
        console.log("Pouring into cup...");
    }

    protected abstract addCondiments(): void;
}

class TeaMaker extends BeverageMaker {
    protected brew(): void {
        // TODO: Print "Steeping the tea bag..."
        console.log("Steeping the tea bag...")
    }

    protected addCondiments(): void {
        // TODO: Print "Adding lemon..."
        console.log("Adding lemon...")
    }
}

class CoffeeMaker extends BeverageMaker {
    protected brew(): void {
        // TODO: Print "Dripping coffee through filter..."
        console.log("Dripping coffee through filter...")
    }

    protected addCondiments(): void {
        // TODO: Print "Adding sugar and milk..."
        console.log("Adding sugar and milk...")
    }
}

console.log("--- Making Tea ---");
new TeaMaker().prepareBeverage();

console.log();

console.log("--- Making Coffee ---");
new CoffeeMaker().prepareBeverage();
