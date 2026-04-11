#include <algorithm>
#include <iostream>
#include <map>
#include <string>
#include <vector>

using namespace std;

class StockExchange;

class StockObserver {
public:
  virtual ~StockObserver() {}
  virtual void onPriceUpdate(StockExchange *exchange) = 0;
};

class StockExchange {
private:
  map<string, double> prices;
  vector<StockObserver *> observers;
  string lastUpdatedSymbol;

public:
  void registerObserver(StockObserver *observer) {
    observers.push_back(observer);
  }

  void removeObserver(StockObserver *observer) {
    observers.erase(remove(observers.begin(), observers.end(), observer),
                    observers.end());
  }

  void updatePrice(const string &symbol, double price) {
    prices[symbol] = price;

    lastUpdatedSymbol = symbol;
    cout << "\nExchange: " << symbol << " updated to $" << price << endl;
    for (StockObserver *observer : vector<StockObserver *>(observers)) {
      observer->onPriceUpdate(this);
    }
  }

  double getPrice(const string &symbol) const {
    auto it = prices.find(symbol);
    return it != prices.end() ? it->second : 0.0;
  }

  string getLastUpdatedSymbol() const { return lastUpdatedSymbol; }
};

class PriceDisplay : public StockObserver {
public:
  void onPriceUpdate(StockExchange *exchange) override {
    string symbol = exchange->getLastUpdatedSymbol();
    cout << "Display -> " << symbol << ": $" << exchange->getPrice(symbol)
         << endl;
  }
};

class AlertService : public StockObserver {
private:
  map<string, double> thresholds;

public:
  void setAlert(const string &symbol, double threshold) {
    thresholds[symbol] = threshold;
  }

  void onPriceUpdate(StockExchange *exchange) override {
    string symbol = exchange->getLastUpdatedSymbol();
    auto it = thresholds.find(symbol);
    if (it != thresholds.end()) {
      double price = exchange->getPrice(symbol);
      if (price >= it->second) {
        cout << "ALERT -> " << symbol << " hit $" << price << " (threshold: $"
             << it->second << ")" << endl;
      }
    }
  }
};

class TradingBot : public StockObserver {
private:
  map<string, double> previousPrices;

public:
  void onPriceUpdate(StockExchange *exchange) override {
    string symbol = exchange->getLastUpdatedSymbol();
    double currentPrice = exchange->getPrice(symbol);
    double previousPrice =
        previousPrices.count(symbol) ? previousPrices[symbol] : currentPrice;

    if (currentPrice > previousPrice) {
      cout << "Bot -> " << symbol << " rising ($" << previousPrice << " -> $"
           << currentPrice << "). HOLD." << endl;
    } else if (currentPrice < previousPrice) {
      cout << "Bot -> " << symbol << " dropping ($" << previousPrice << " -> $"
           << currentPrice << "). BUY." << endl;
    }

    previousPrices[symbol] = currentPrice;
  }
};

int main() {
  StockExchange exchange;

  PriceDisplay display;
  AlertService alerts;
  TradingBot bot;

  exchange.registerObserver(&display);
  exchange.registerObserver(&alerts);
  exchange.registerObserver(&bot);

  alerts.setAlert("AAPL", 180.0);
  alerts.setAlert("GOOG", 140.0);

  exchange.updatePrice("AAPL", 175.50);
  exchange.updatePrice("GOOG", 138.25);
  exchange.updatePrice("AAPL", 182.00);
  exchange.updatePrice("GOOG", 141.75);

  return 0;
}
