interface StockObserver {
  onPriceUpdate(exchange: StockExchange): void;
}

class StockExchange {
    private prices: Map<string, number> = new Map();
    private observers: StockObserver[] = [];
    private lastUpdatedSymbol: string = "";

    registerObserver(observer: StockObserver): void { this.observers.push(observer); }

    removeObserver(observer: StockObserver): void {
        const index = this.observers.indexOf(observer);
        if (index > -1) this.observers.splice(index, 1);
    }

    updatePrice(symbol: string, price: number): void {
        this.prices.set(symbol, price);
        this.lastUpdatedSymbol = symbol;
        console.log("\nExchange: " + symbol + " updated to $" + price);
        for (const observer of [...this.observers]) {
            observer.onPriceUpdate(this);
        }
    }

    getPrice(symbol: string): number { return this.prices.get(symbol) ?? 0; }
    getLastUpdatedSymbol(): string { return this.lastUpdatedSymbol; }
}

class PriceDisplay implements StockObserver {
    onPriceUpdate(exchange: StockExchange): void {
        const symbol = exchange.getLastUpdatedSymbol();
        console.log("Display -> " + symbol + ": $" + exchange.getPrice(symbol));
    }
}

class AlertService implements StockObserver {
    private thresholds: Map<string, number> = new Map();

    setAlert(symbol: string, threshold: number): void { this.thresholds.set(symbol, threshold); }

    onPriceUpdate(exchange: StockExchange): void {
        const symbol = exchange.getLastUpdatedSymbol();
        const threshold = this.thresholds.get(symbol);
        if (threshold !== undefined) {
            const price = exchange.getPrice(symbol);
            if (price >= threshold) {
                console.log("ALERT -> " + symbol + " hit $" + price +
                    " (threshold: $" + threshold + ")");
            }
        }
    }
}

class TradingBot implements StockObserver {
    private previousPrices: Map<string, number> = new Map();

    onPriceUpdate(exchange: StockExchange): void {
        const symbol = exchange.getLastUpdatedSymbol();
        const currentPrice = exchange.getPrice(symbol);
        const previousPrice = this.previousPrices.get(symbol) ?? currentPrice;

        if (currentPrice > previousPrice) {
            console.log("Bot -> " + symbol + " rising ($" + previousPrice +
                " -> $" + currentPrice + "). HOLD.");
        } else if (currentPrice < previousPrice) {
            console.log("Bot -> " + symbol + " dropping ($" + previousPrice +
                " -> $" + currentPrice + "). BUY.");
        }

        this.previousPrices.set(symbol, currentPrice);
    }
}

// Client code
const exchange = new StockExchange();

const display = new PriceDisplay();
const alerts = new AlertService();
const bot = new TradingBot();

exchange.registerObserver(display);
exchange.registerObserver(alerts);
exchange.registerObserver(bot);

alerts.setAlert("AAPL", 180.0);
alerts.setAlert("GOOG", 140.0);

exchange.updatePrice("AAPL", 175.50);
exchange.updatePrice("GOOG", 138.25);
exchange.updatePrice("AAPL", 182.00);
exchange.updatePrice("GOOG", 141.75);
