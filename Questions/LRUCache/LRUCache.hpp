#pragma once
#include "DoublyLinkedList.hpp"
#include <mutex>
#include <optional>
#include <unordered_map>
template <typename K, typename V> class LRUCache {
private:
  int capacity;
  std::unordered_map<K, Node<K, V> *> map;
  DoublyLinkedList<K, V> list;
  std::mutex mtx;

public:
  LRUCache(int cap) {
    // TODO: Initialize capacity
    // The map and list are default-initialized
    this->capacity = cap;
  }

  ~LRUCache() {
    // TODO: Delete all nodes stored in the map
    // Iterate through the map and delete each node pointer
    for (auto &[key, value] : map) {
      delete value;
    }
  }

  std::optional<V> get(const K &key) {
    std::lock_guard<std::mutex> lock(mtx);

    // TODO: Implement get operation
    // Steps:
    // 1. If key not in map, return std::nullopt
    // 2. Get the node pointer from the map
    // 3. Move the node to front (mark as most recently used)
    // 4. Return the node's value
    if (map.find(key) == map.end())
      return std::nullopt;
    Node<K, V> *node = map[key];
    list.moveToFront(node);
    return node->value;
  }

  void put(const K &key, const V &value) {
    std::lock_guard<std::mutex> lock(mtx);

    // TODO: Implement put operation
    // Case 1: Key already exists
    //   - Get the existing node
    //   - Update its value
    //   - Move it to front
    //
    // Case 2: Key is new
    //   - If at capacity, evict LRU item:
    //     - Remove last node from list
    //     - Remove its key from map
    //     - Delete the node
    //   - Create new node
    //   - Add to front of list
    //   - Add to map
    if (map.find(key) != map.end()) {
      Node<K, V> *node = map[key];
      node->value = value;
      list.moveToFront(node);
    } else {
      if (map.size() == this->capacity) {
        Node<K, V> *node = list.removeLast();
        if (node != nullptr) {
          map.erase(node->key);
          delete node;
        }
      }
      Node<K, V> *node = new Node<K, V>(key, value);
      list.addFirst(node);
      map[key] = node;
    }
  }
};
