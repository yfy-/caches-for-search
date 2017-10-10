//
// Created by yfy on 10/10/17.
//

#ifndef CENG778_PROJECT_NODEDOUBLYLINKEDLIST_H
#define CENG778_PROJECT_NODEDOUBLYLINKEDLIST_H

#include <string>

class NodeDoublyLinkedList {

 public:

  struct Node {
    std::string data;
    Node* next;
    Node* prev;
    explicit Node(const std::string &d) : data{d}, next{nullptr}, prev{nullptr} {}
  };

  NodeDoublyLinkedList();
  virtual ~NodeDoublyLinkedList();
  void PushFront(Node*);
  void PushBack(Node*);

  inline void Disconnect(Node* node) {
    node->prev->next = node->next;
    node->next->prev = node->prev;
  }

  void MoveToFront(Node*);
  void MoveToBack(Node*);
  void MoveAfter(Node*, Node*);
  void MoveBefore(Node*, Node*);
  std::string ToString();

 public:
  // Made public for iteration (implementing an iterator is too much work for now).
  Node* head_;
  Node* tail_;
};

#endif //CENG778_PROJECT_NODEDOUBLYLINKEDLIST_H
