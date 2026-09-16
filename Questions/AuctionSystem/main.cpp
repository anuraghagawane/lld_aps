#include <atomic>
#include <chrono>
#include <condition_variable>
#include <functional>
#include <iostream>
#include <memory>
#include <mutex>
#include <queue>
#include <stdexcept>
#include <thread>
#include <unordered_map>

using namespace std;

class User {
private:
  string id;
  string name;
  string email;

public:
  User(const string &id, const string &name, const string &email)
      : id(id), name(name), email(email) {}

  string getId() const { return id; }
  string getName() const { return name; }
  string getEmail() const { return email; }
};

class Bid {
private:
  string bidId;
  string bidderId;
  int amount;
  chrono::system_clock::time_point ts;
  string lotId;

public:
  Bid(const string &bidId, const string &bidderId, int amount,
      const string &lotId)
      : bidId(bidId), bidderId(bidderId), amount(amount), lotId(lotId),
        ts(chrono::system_clock::now()) {}

  string getBidId() const { return bidId; }
  string getBidderId() const { return bidderId; }
  int getAmount() const { return amount; }
};

class BidStore {
private:
  unordered_map<string, shared_ptr<Bid>> bids;
  atomic<int> count;
  mutex mtx;

public:
  string createBid(const string &bidderId, const int amount,
                   const string &lotId) {
    lock_guard<mutex> lock(mtx);
    const string id = "BID_" + to_string(count.fetch_add(1));

    auto bid = make_shared<Bid>(id, bidderId, amount, lotId);

    bids[id] = bid;

    return id;
  }

  shared_ptr<Bid> getBid(const string &bidId) {
    lock_guard<mutex> lock(mtx);
    if (!bids.count(bidId)) {
      throw runtime_error("Bid not found!");
    }

    return bids[bidId];
  }
};

enum class LotStatus { OPEN, CLOSED };

class Lot {
private:
  string id;
  string sellerId;
  string name;
  string itemDetails;
  int basePrice;
  chrono::system_clock::time_point endTime;
  LotStatus status;
  int currentHighest;
  string highestBidderId;
  string winnerId;
  mutex mtx;

public:
  Lot(const string &id, const string &name, const string &sellerId,
      const string &itemDetails, const int basePrice,
      chrono::system_clock::time_point endTime)
      : id(id), name(name), sellerId(sellerId), itemDetails(itemDetails),
        basePrice(basePrice), endTime(endTime) {
    status = LotStatus::OPEN;
    highestBidderId = "";
    winnerId = "";
    currentHighest = basePrice;
  }

  string getId() const { return id; }
  string getName() const { return name; }
  string getSellerId() const { return sellerId; }
  int getBasePrice() const { return basePrice; }
  chrono::system_clock::time_point getEndTime() { return endTime; }

  bool isExpired() { return chrono::system_clock::now() >= endTime; }

  bool checkAndExpire() {
    if (chrono::system_clock::now() >= endTime) {
      status = LotStatus::CLOSED;

      if (highestBidderId != "")
        winnerId = highestBidderId;
      return true;
    }

    return false;
  }

  string getHighestBidder() { return highestBidderId; }
  int getCurrentHighestBid() { return currentHighest; }

  bool canBid(string bidderId, int amount) {
    if (bidderId == sellerId)
      return false;
    if (currentHighest >= amount) {
      return false;
    }

    return true;
  }

  bool placeBid(string bidderId, int amount) {
    if (bidderId == sellerId)
      return false;
    if (currentHighest >= amount) {
      return false;
    }

    currentHighest = amount;
    highestBidderId = bidderId;
    return true;
  }

  void printWinner() {
    if (winnerId != "") {
      cout << "Winner of Lot: " << id << " is " << winnerId << endl;
    }
  }

  mutex &getMutex() { return mtx; }
};

class NotificationService {
public:
  void sendNotification(string to, string message) {
    cout << "To: " << to << " Message: " << message << endl;
  }
};

class AuctionService {
private:
  typedef pair<chrono::system_clock::time_point, string> activeRecordType;
  unordered_map<string, unique_ptr<User>> users;
  unordered_map<string, shared_ptr<Lot>> lots;
  priority_queue<activeRecordType, vector<activeRecordType>,
                 greater<activeRecordType>>
      active;

  atomic<int> lotCounter;
  mutex mtx;
  BidStore &bidStore;
  NotificationService &notificationService;
  mutex activeMtx;
  condition_variable cv;

public:
  AuctionService(BidStore &bidStore, NotificationService &notificationService)
      : bidStore(bidStore), notificationService(notificationService) {}
  string addUser(const string &id, const string &name, const string &email) {
    auto user = make_unique<User>(id, name, email);
    users[id] = std::move(user);
    return id;
  }

  string addLot(const string &sellerId, const string &name,
                const string &itemDetails, const int basePrice,
                chrono::system_clock::time_point endTime) {
    const string lotId = "LOT_" + to_string(lotCounter.fetch_add(1));
    {
      lock_guard<mutex> lock(mtx);

      auto lot = make_shared<Lot>(lotId, name, sellerId, itemDetails, basePrice,
                                  endTime);

      lots[lotId] = std::move(lot);
    }
    {
      lock_guard<mutex> lock(activeMtx);
      active.emplace(endTime, lotId);
    }
    cv.notify_one();
    return lotId;
  }

  bool placeBid(const string &lotId, const string &bidderId, const int amount) {
    shared_ptr<Lot> lot;
    {
      lock_guard<mutex> lock(mtx);
      if (!lots.count(lotId)) {
        cout << "Lot not present" << endl;
        return false;
      }
      lot = lots[lotId];
    }

    string previousHighest;
    {
      lock_guard<mutex> lock(lot->getMutex());

      previousHighest = lot->getHighestBidder();
      if (lot->isExpired()) {
        cout << "Lot expired" << endl;
        return false;
      }

      if (!lot->canBid(bidderId, amount)) {
        cout << "Cannot place the bid" << endl;
        return false;
      }

      bidStore.createBid(bidderId, amount, lotId);

      if (!lot->placeBid(bidderId, amount)) {
        cout << "Failed to place bid" << endl;
        return false;
      }

      cout << "Placed Bid for " << lotId << " by " << bidderId << " of amount"
           << amount << endl;
    }
    if (previousHighest != "" && previousHighest != bidderId) {
      notificationService.sendNotification(
          previousHighest, "New Leader for lot " + lotId + " is " + bidderId);
    }

    return true;
  }

  void handleExpiry() {
    while (true) {
      vector<activeRecordType> expiredLots;
      {
        unique_lock<mutex> lock(activeMtx);
        if (active.empty()) {
          cv.wait(lock, [&]() { return !active.empty(); });
        }
        if (active.top().first > chrono::system_clock::now()) {
          cv.wait_until(lock, active.top().first);
        }
        while (!active.empty() &&
               active.top().first <= chrono::system_clock::now()) {
          auto lot = active.top();
          active.pop();
          expiredLots.push_back(lot);
        }
      }
      for (auto [endTime, lotId] : expiredLots) {
        shared_ptr<Lot> lot;
        {
          lock_guard<mutex> lock(mtx);
          lot = lots[lotId];
        }
        lock_guard<mutex> lock(lot->getMutex());
        lot->checkAndExpire();
        lot->printWinner();
      }
    }
  }

  void search(const string &name, pair<int, int> *priceRange) {
    vector<shared_ptr<Lot>> tempLots;
    {
      lock_guard<mutex> lock(mtx);
      for (auto [key, lot] : this->lots) {
        tempLots.push_back(lot);
      }
    }

    cout << "Found: " << endl;
    for (auto lot : tempLots) {
      if ((name != "" && lot->getName() == name) ||
          (priceRange != nullptr && lot->getBasePrice() >= priceRange->first &&
           lot->getBasePrice() <= priceRange->second)) {
        cout << "Lot: " << lot->getId() << endl;
      }
    }
  }
};

int main() {
  NotificationService notificationService;
  BidStore bidStore;
  AuctionService auctionService(bidStore, notificationService);

  std::thread expiryWorker(&AuctionService::handleExpiry, &auctionService);

  std::thread t([&]() {
    auctionService.addUser("U1", "AH", "A@b.c");
    auctionService.addUser("U2", "XY", "X@b.c");
    auctionService.addUser("U3", "MN", "M@b.c");

    auto lotId =
        auctionService.addLot("U1", "Lamp", "Good lamp", 10,
                              chrono::system_clock::now() + chrono::seconds(5));

    auctionService.placeBid(lotId, "U2", 20);
    auctionService.placeBid(lotId, "U3", 15);
    auctionService.placeBid(lotId, "U3", 25);

    this_thread::sleep_for(chrono::seconds(5));

    auctionService.placeBid(lotId, "U2", 30);
  });

  expiryWorker.join();
  t.join();
  return 0;
}
