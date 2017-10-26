//
// Created by yfy on 10/10/17.
//

#include "cache/node_doubly_linked_list.h"

NodeDoublyLinkedList::NodeDoublyLinkedList() {
  head_ = new Node("", 0);
  tail_ = new Node("", 0);

  head_->next = tail_;
  tail_->prev = head_;
}

void NodeDoublyLinkedList::PushFront(NodeDoublyLinkedList::Node* node) {
  head_->next->prev = node;
  node->next = head_->next;
  head_->next = node;
  node->prev = head_;
}

void NodeDoublyLinkedList::PushBack(NodeDoublyLinkedList::Node *node) {
  tail_->prev->next = node;
  node->prev = tail_->prev;
  tail_->prev = node;
  node->next = tail_;
}

std::string NodeDoublyLinkedList::ToString() {
  std::string result = "head -> ";

  Node* curr = head_->next;

  while (curr != tail_) {
    result += curr->data + ":" + std::to_string(curr->score) + " -> ";
    curr = curr->next;
  }

  result += "tail";

  return result;
}

NodeDoublyLinkedList::~NodeDoublyLinkedList() {
  Node* curr = head_->next;

  while (curr != nullptr) {
    delete curr->prev;
    curr = curr->next;
  }

  delete tail_;
}

void NodeDoublyLinkedList::MoveToFront(NodeDoublyLinkedList::Node* node) {
  if (head_->next == node)
    return;
  Disconnect(node);
  PushFront(node);
}

void NodeDoublyLinkedList::MoveToBack(NodeDoublyLinkedList::Node* node) {
  if (tail_->prev == node)
    return;
  Disconnect(node);
  PushBack(node);
}

void NodeDoublyLinkedList::MoveAfter(NodeDoublyLinkedList::Node* node, NodeDoublyLinkedList::Node* target) {
  if (target->next == node)
    return;
  Disconnect(node);
  target->next->prev = node;
  node->next = target->next;
  target->next = node;
  node->prev = target;
}

void NodeDoublyLinkedList::MoveBefore(NodeDoublyLinkedList::Node* node, NodeDoublyLinkedList::Node* target) {
  if (target->prev == node)
    return;
  Disconnect(node);
  target->prev->next = node;
  node->prev = target->prev;
  target->prev = node;
  node->next = target;
}
