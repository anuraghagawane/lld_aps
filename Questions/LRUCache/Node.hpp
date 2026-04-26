template <typename K, typename V> struct Node {
  K key;
  V value;
  Node *prev;
  Node *next;

  Node(K k, V v) {
    // TODO: Initialize key and value
    // TODO: Set prev and next to nullptr
    this->key = k;
    this->value = v;
    this->prev = nullptr;
    this->next = nullptr;
  }
};
