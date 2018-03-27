//
// Created by yfy on 10/10/17.
//

#ifndef CENG778_PROJECT_DOUBLYLINKEDLIST_H
#define CENG778_PROJECT_DOUBLYLINKEDLIST_H

#include <cstddef>
#include <string>

template <class T>
class DoublyLinkedList {

 public:

  struct Node {
    T content;
    Node* next;
    Node* prev;

    Node() : next{nullptr}, prev{nullptr} {}
    Node(const T& c) : content{c} { Node(); }

    // Node& operator=(const Node& other) {
    //   this->next = other.next;
    //   this->prev = other.prev;
    // }
  };

  Node* head_;
  Node* tail_;

  DoublyLinkedList();
  virtual ~DoublyLinkedList();

  inline void Disconnect(Node* node) {
    node->prev->next = node->next;
    node->next->prev = node->prev;
  }

  void PushFront(Node*);
  void PushBack(Node*);
  void MoveToFront(Node*);
  void MoveToBack(Node*);
  void MoveAfter(Node*, Node*);
  void MoveBefore(Node*, Node*);
};

template <class T>
DoublyLinkedList<T>::DoublyLinkedList() {
  head_ = new Node();
  tail_ = new Node();

  head_->next = tail_;
  tail_->prev = head_;
}

template <class T>
void DoublyLinkedList<T>::PushFront(Node* node) {
  head_->next->prev = node;
  node->next = head_->next;
  head_->next = node;
  node->prev = head_;
}

template <class T>
void DoublyLinkedList<T>::PushBack(Node* node) {
  tail_->prev->next = node;
  node->prev = tail_->prev;
  tail_->prev = node;
  node->next = tail_;
}

template <class T>
DoublyLinkedList<T>::~DoublyLinkedList<T>() {
  Node* curr = head_->next;

  while (curr != nullptr) {
    delete curr->prev;
    curr = curr->next;
  }

  delete tail_;
}

template <class T>
void DoublyLinkedList<T>::MoveToFront(Node* node) {
  if (head_->next == node)
    return;

  Disconnect(node);
  PushFront(node);
}

template <class T>
void DoublyLinkedList<T>::MoveToBack(Node* node) {
  if (tail_->prev == node)
    return;

  Disconnect(node);
  PushBack(node);
}

template <class T>
void DoublyLinkedList<T>::MoveAfter(Node* node, Node* target) {
  if (target->next == node)
    return;

  Disconnect(node);
  target->next->prev = node;
  node->next = target->next;
  target->next = node;
  node->prev = target;
}

template <class T>
void DoublyLinkedList<T>::MoveBefore(Node* node, Node* target) {
  if (*target->prev == *node)
    return;

  Disconnect(node);
  target->prev->next = node;
  node->prev = target->prev;
  target->prev = node;
  node->next = target;
}

#endif //CENG778_PROJECT_DOUBLYLINKEDLIST_H
