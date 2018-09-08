// Created by yfy on 10/10/17.

#ifndef CENG778PROJECT_DOUBLYLINKEDLIST_H
#define CENG778PROJECT_DOUBLYLINKEDLIST_H

#include <string>
#include <type_traits>

template <class T>
class DoublyLinkedList {

 public:

  struct Node {
    T content;
    Node* next;
    Node* prev;

    Node() : next{nullptr}, prev{nullptr}, content{0} {}
    Node(T c) : next{nullptr}, prev{nullptr}, content{c} {}
  };

  Node* head_;
  Node* tail_;

  DoublyLinkedList();
  DoublyLinkedList(DoublyLinkedList<T>&);
  ~DoublyLinkedList();

  // node has to be connected
  inline void Disconnect(Node* node) {
    node->prev->next = node->next;
    node->next->prev = node->prev;
  }

  void PushFront(Node*);
  void PushBack(Node*);
  void PushAfter(Node*, Node*);
  void PushBefore(Node*, Node*);
  void MoveToFront(Node*);
  void MoveToBack(Node*);
  void MoveAfter(Node*, Node*);
  void MoveBefore(Node*, Node*);
  bool IsEmpty();
};

template <class T>
DoublyLinkedList<T>::DoublyLinkedList() {
  head_ = new Node();
  tail_ = new Node();

  head_->next = tail_;
  tail_->prev = head_;
}

template <class T>
DoublyLinkedList<T>::DoublyLinkedList(DoublyLinkedList<T>& other) {
  this->head_ = other.head_;
  this->tail_ = other.tail_;
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
DoublyLinkedList<T>::~DoublyLinkedList() {
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
void DoublyLinkedList<T>::PushAfter(Node* node, Node* target) {
  target->next->prev = node;
  node->next = target->next;
  target->next = node;
  node->prev = target;
}

template <class T>
void DoublyLinkedList<T>::PushBefore(Node* node, Node* target) {
  target->prev->next = node;
  node->prev = target->prev;
  target->prev = node;
  node->next = target;
}

template <class T>
void DoublyLinkedList<T>::MoveAfter(Node* node, Node* target) {
  if (target->next == node)
    return;

  Disconnect(node);
  PushAfter(node, target);
}

template <class T>
void DoublyLinkedList<T>::MoveBefore(Node* node, Node* target) {
  if (target->prev == node)
    return;

  Disconnect(node);
  PushBefore(node, target);
}

template <class T>
bool DoublyLinkedList<T>::IsEmpty() {
  return head_->next == tail_;
}

#endif // CENG778PROJECT_DOUBLYLINKEDLIST_H
