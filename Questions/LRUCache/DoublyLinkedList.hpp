#pragma once
#include "Node.hpp"
template <typename K, typename V> class DoublyLinkedList {
private:
  Node<K, V> *head;
  Node<K, V> *tail;

public:
  DoublyLinkedList() {
    // TODO: Create dummy head and tail nodes
    // Use default values for K and V (e.g., K{} and V{})
    // TODO: Link head->next to tail and tail->prev to head
    head = new Node<K, V>(K{}, V{});
    tail = new Node<K, V>(K{}, V{});
    head->next = tail;
    tail->prev = head;
  }

  ~DoublyLinkedList() {
    // TODO: Delete the dummy head and tail nodes
    // Note: Real data nodes are deleted by LRUCache
    delete head;
    delete tail;
  }

  void addFirst(Node<K, V> *node) {
    // TODO: Insert node right after head
    // Steps:
    // 1. node->next = head->next
    // 2. node->prev = head
    // 3. head->next->prev = node
    // 4. head->next = node
    node->next = head->next;
    node->prev = head;
    head->next->prev = node;
    head->next = node;
  }

  void remove(Node<K, V> *node) {
    // TODO: Detach node from its current position
    // Steps:
    // 1. node->prev->next = node->next
    // 2. node->next->prev = node->prev
    node->prev->next = node->next;
    node->next->prev = node->prev;
  }

  void moveToFront(Node<K, V> *node) {
    // TODO: Move an existing node to the front
    // Hint: Remove it first, then add it to front
    remove(node);
    addFirst(node);
  }

  Node<K, V> *removeLast() {
    // TODO: Remove and return the node just before tail (the LRU node)
    // Steps:
    // 1. Check if list is empty (tail->prev == head), return nullptr if so
    // 2. Get the last real node (tail->prev)
    // 3. Remove it using the remove() method
    // 4. Return the removed node
    if (tail->prev == head)
      return nullptr;
    Node<K, V> *node = tail->prev;
    remove(node);
    return node;
  }
};
