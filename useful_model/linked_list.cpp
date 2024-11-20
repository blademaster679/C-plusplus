#include <iostream>
#include <cassert>

class SinglyLinkedList {
public:

    class Node {
    public:
        Node *next;
        int data;

        Node(Node* next, int data) : next(next), data(data) {}
    };
    Node *head;
    SinglyLinkedList(): head(nullptr) {}
    void insert(int data){
        Node *node = new Node(head,data);
        head = node;
    } // insert front

    void insertBack(int data) {
    Node* newNode = new Node(nullptr, 0);
    newNode->data = data;
    newNode->next = nullptr;
    if (head == nullptr) {
        head = newNode; //如果链表为空，直接将新节点插入到头节点
        return;
    }
    Node* current = head;
    while (current->next != nullptr) {
        current = current->next; //找到链表的最后一个节点
    }
    current->next = newNode; //将新节点插入到链表的最后
} // insertBack


    void insertAfterKey(int key, int data) {
        Node* current = head; //从头节点开始，current指向当前节点，遍历链表
            while (current != nullptr && current->data != key) {
            current = current->next; //找到key所在的节点
        }
        if (current == nullptr) {
            std::cout << "Key not found" << std::endl;
            return;
        }
        Node* newNode = new Node(nullptr, 0);
        newNode->data = data;
        newNode->next = current->next; //新节点的next指向当前节点的next
        current->next = newNode; //将新节点插入到当前节点后面，当前节点的next指向新节点
    } // insertAfterKey


    void print_list() const {
    Node *it = head;
    while (it != nullptr) {
        std::cout << it->data << " ";
        it = it->next;
    }
    std::cout << std::endl;
}

};



int main(){
    SinglyLinkedList lst;
    lst.insert(1);
    lst.insert(2);
    lst.insert(3);
    lst.print_list();
}