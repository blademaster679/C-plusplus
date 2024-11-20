#include <iostream>
#include <sstream>
#include <string>

using namespace std;

// Definition for singly-linked list
struct ListNode {
    int val;
    ListNode* next;
    ListNode() : val(0), next(nullptr) {}
    ListNode(int x) : val(x), next(nullptr) {}
    ListNode(int x, ListNode* next) : val(x), next(next) {}
};

class LinkedList {
private:
    ListNode* head;

public:
    LinkedList() : head(nullptr) {}
    ListNode* createLinkedList(const string& input);
    ListNode* swapNodes(ListNode* head, int k);
    void printLinkedList(ListNode* head);
    ~LinkedList();
};

// Destructor to delete all nodes in the linked list
LinkedList::~LinkedList() {
    ListNode* current = head;
    while (current != nullptr) {
        ListNode* next = current->next;
        delete current;
        current = next;
    }
}

// Function to create the linked list from a string input
ListNode* LinkedList::createLinkedList(const string& input) {
    istringstream iss(input);
    int val;
    ListNode* head = nullptr;
    ListNode* current = nullptr;

    while (iss >> val) {
        ListNode* newNode = new ListNode(val);
        if (!head) {
            head = newNode; // first node
        } else {
            current->next = newNode; // attach to the list
        }
        current = newNode; // move current to the new node
    }

    return head;
}

// Function to swap the k-th node from the beginning and end
ListNode* LinkedList::swapNodes(ListNode* head, int k) {
    if (!head || k <= 0) return head;

    // Step 1: Get the length of the list
    int length = 0;
    ListNode* current = head;
    while (current) {
        length++;
        current = current->next;
    }

    // Check if k is valid
    if (k > length) return head;

    // Step 2: Find the k-th node from the start and the k-th node from the end
    ListNode* first = nullptr;
    ListNode* second = nullptr;
    current = head;

    // Get the k-th node from the beginning
    for (int i = 1; i < k; ++i) {
        current = current->next;
    }
    first = current;

    // Get the k-th node from the end
    current = head;
    for (int i = 1; i < length - k + 1; ++i) {
        current = current->next;
    }
    second = current;

    // Step 3: Swap the values of the two nodes
    if (first && second) {
        swap(first->val, second->val);
    }

    return head;
}

// Function to print the linked list
void LinkedList::printLinkedList(ListNode* head) {
    ListNode* current = head;
    while (current != nullptr) {
        cout << current->val;
        if (current->next != nullptr) cout << " ";
        current = current->next;
    }
    cout << endl;
}

int main() {
    LinkedList list;
    int k;

    string first_line;
    // The first line is the linked list
    getline(cin, first_line);

    // Create the linked list
    ListNode* head = list.createLinkedList(first_line);

    // Read the position k
    cin >> k;

    // Swap the nodes
    ListNode* newHead = list.swapNodes(head, k);

    // Print the modified linked list
    list.printLinkedList(newHead);

    return 0;
}
