#include <atomic>
#include <chrono>
#include <condition_variable>
#include <iomanip>
#include <iostream>
#include <memory>
#include <mutex>
#include <queue>
#include <shared_mutex>
#include <thread>
#include <unordered_map>

using namespace std;

string generateId() {
  static atomic<int> counter = 0;
  return "msg_" + to_string(++counter);
}

class Message {
private:
  string id;
  string topic;
  string payload;
  chrono::system_clock::time_point timestamp;

public:
  Message(const string &topic, const string &payload)
      : id(generateId()), topic(topic), payload(payload),
        timestamp(chrono::system_clock::now()) {}

  string getTopic() const { return topic; }
  string getPayload() const { return payload; }

  string getFormattedTime() const {
    auto time = chrono::system_clock::to_time_t(timestamp);

    tm tmBuffer{};

    localtime_r(&time, &tmBuffer);

    stringstream ss;
    ss << put_time(&tmBuffer, "%Y-%m-%d %H:%M:%S");
    return ss.str();
  }
};

class Subscriber {
public:
  virtual void enqueue(const Message &message) = 0;
  virtual string getId() const = 0;
  virtual ~Subscriber() = default;
};

class Consumer : public Subscriber {
private:
  string id;

  queue<Message> messageQueue;

  mutable mutex mtx;
  condition_variable cv;

  bool running;
  thread worker;

  void processMessage() {
    while (running) {
      unique_lock<mutex> lock(mtx);

      cv.wait(lock, [this]() { return !messageQueue.empty() || !running; });

      while (!messageQueue.empty()) {
        Message msg = messageQueue.front();
        messageQueue.pop();

        lock.unlock();

        cout << "[Consumer: " << id << "] "
             << "Received Message => Topic: " << msg.getTopic()
             << " Payload: " << msg.getPayload()
             << " Time: " << msg.getFormattedTime() << endl;

        this_thread::sleep_for(chrono::milliseconds(100));

        lock.lock();
      }
    }
  }

public:
  Consumer(const string &id) : id(id), running(true) {
    worker = thread(&Consumer::processMessage, this);
  }

  void enqueue(const Message &message) override {
    {
      lock_guard<mutex> lock(mtx);

      if (messageQueue.size() >= 100) {
        cout << "[Consumer: " << id << "] Queue Full. Dropping Message.\n";
        return;
      }

      messageQueue.push(message);
    }

    cv.notify_one();
  }

  string getId() const override { return id; }

  ~Consumer() {
    {
      lock_guard<mutex> lock(mtx);
      running = false;
    }

    cv.notify_one();

    if (worker.joinable()) {
      worker.join();
    }
  }
};

class Topic {
private:
  string name;
  unordered_map<string, shared_ptr<Subscriber>> subscribers;
  mutable mutex mtx;

public:
  Topic(const string &name) : name(name) {}

  void subscribe(shared_ptr<Subscriber> subscriber) {
    lock_guard<mutex> lock(mtx);
    subscribers[subscriber->getId()] = subscriber;

    cout << subscriber->getId() << " subscribed to " << name << endl;
  }

  void unsubscribe(const string &subscriberId) {
    lock_guard<mutex> lock(mtx);
    subscribers.erase(subscriberId);
    cout << subscriberId << " unsubscribed from " << name << endl;
  }

  void publish(const Message &message) {
    vector<shared_ptr<Subscriber>> subscribersCopy;

    {
      lock_guard<mutex> lock(mtx);

      for (auto &[id, subscriber] : subscribers) {
        subscribersCopy.push_back(subscriber);
      }
    }

    for (auto &subscriber : subscribersCopy) {
      subscriber->enqueue(message);
    }
  }
};

class Broker {
private:
  unordered_map<string, shared_ptr<Topic>> topics;
  mutable shared_mutex topicMutex;

public:
  void createTopic(const string &topicName) {
    unique_lock<shared_mutex> lock(topicMutex);
    if (topics.count(topicName)) {
      return;
    }

    topics[topicName] = make_shared<Topic>(topicName);
    cout << "Topic Created: " << topicName << endl;
  }

  shared_ptr<Topic> getTopic(const string &topicName) {
    shared_lock<shared_mutex> lock(topicMutex);
    if (!topics.count(topicName)) {
      return nullptr;
    }

    return topics[topicName];
  }

  void subscribe(const string &topicName, shared_ptr<Subscriber> subscriber) {
    auto topic = getTopic(topicName);
    if (!topic) {
      cout << "Topic not found\n";
      return;
    }

    topic->subscribe(subscriber);
  }

  void unsubscribe(const string &topicName, const string &subscriberId) {
    auto topic = getTopic(topicName);
    if (!topic) {
      cout << "Topic not found\n";
      return;
    }

    topic->unsubscribe(subscriberId);
  }

  void publish(const Message &message) {
    auto topic = getTopic(message.getTopic());

    if (!topic) {
      cout << "Topic not found\n";
      return;
    }

    topic->publish(message);
  }
};

class Publisher {
private:
  string id;
  shared_ptr<Broker> broker;

public:
  Publisher(const string &id, shared_ptr<Broker> broker)
      : id(id), broker(broker) {}

  void publish(const string &topic, const string &payload) {
    Message msg(topic, payload);

    cout << "[Publisher: " << id << "] Publishing => " << payload << endl;

    broker->publish(msg);
  }
};

int main() {
  auto broker = make_shared<Broker>();

  broker->createTopic("sports");
  broker->createTopic("news");

  auto consumer1 = make_shared<Consumer>("consumer_1");
  auto consumer2 = make_shared<Consumer>("consumer_2");
  auto consumer3 = make_shared<Consumer>("consumer_3");

  broker->subscribe("sports", consumer1);
  broker->subscribe("sports", consumer2);

  broker->subscribe("news", consumer2);
  broker->subscribe("news", consumer3);

  Publisher pub1("publisher_1", broker);
  Publisher pub2("publisher_2", broker);

  thread t1([&]() {
    for (int i = 1; i < 5; i++) {
      pub1.publish("sports", "Sports Update " + to_string(i));
    }
  });

  thread t2([&]() {
    for (int i = 1; i <= 5; i++) {
      pub2.publish("news", "Breaking News " + to_string(i));

      this_thread::sleep_for(chrono::milliseconds(300));
    }
  });

  t1.join();
  t2.join();

  this_thread::sleep_for(chrono::seconds(2));

  return 0;
}
