#include "lab08linked_list.hpp"


//linux系统上可运行，windows系统上无法运行
LinkedList::~LinkedList() {
    Node* current = head;
    while (current != nullptr) {
        Node* next = current->next;
        delete current;
        current = next;
    }
}

void LinkedList::insertFront(int data) {
    Node* newNode = new Node();
    newNode->data = data;
    newNode->next = head;
    head = newNode;
}

void LinkedList::insertBack(int data) {
    Node* newNode = new Node();
    newNode->data = data;
    newNode->next = nullptr;
    if (head == nullptr) {
        head = newNode;
        return;
    }
    Node* current = head;
    while (current->next != nullptr) {
        current = current->next;
    }
    current->next = newNode;
}

void LinkedList::insertAfterKey(int key, int data) {
    Node* current = head;
    while (current != nullptr && current->data != key) {
        current = current->next;
    }
    if (current == nullptr) {
        std::cout << "Key not found" << std::endl;
        return;
    }
    Node* newNode = new Node();
    newNode->data = data;
    newNode->next = current->next;
    current->next = newNode;
}

void LinkedList::print() {
    Node* current = head;
    while (current != nullptr) {
        std::cout << current->data << " ";
        current = current->next;
    }
    std::cout << std::endl;
}

bool LinkedList::searchKey(int key){
    Node* current = head;
    while (current != nullptr) {
        if (current->data == key) {
            return true;
        }
        current = current->next;
    }
    return false;
}

void LinkedList::deleteNode(int key){
    Node* current = head;
    Node* prev = nullptr;
    while(current != nullptr && current->data != key){
        prev = current;
        current = current->next;
    }
    if(current == nullptr){
        std::cout << "Key not found" << std::endl;
        return;
    }
    if(prev == nullptr){
        head = current->next;
    }else{
        prev->next = current->next;
    }
    delete current;
}

int main(){
    LinkedList list;
    list.insertFront(1);
    list.insertFront(2);
    list.insertFront(3);
    list.insertFront(4);
    list.insertFront(5);
    list.print();
    list.insertBack(6);
    list.insertBack(7);
    list.insertBack(8);
    list.insertBack(9);
    list.insertBack(10);
    list.print();
    list.insertAfterKey(5, 11);
    list.insertAfterKey(10, 12);
    list.print();
    list.deleteNode(5);
    list.deleteNode(12);
    list.print();
    std::cout << list.searchKey(5) << std::endl;
    std::cout << list.searchKey(12) << std::endl;
    return 0;
}