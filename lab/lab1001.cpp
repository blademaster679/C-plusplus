#include <iostream>
using namespace std;
struct ListNode {
    int val;
    ListNode *next;
    ListNode() : val(0), next(nullptr) {}
    ListNode(int x) : val(x), next(nullptr) {}
    ListNode(int x, ListNode *next) : val(x), next(next) {}
};
class Solution{
public:
    ListNode* ReverseList(ListNode* head){
        ListNode* pre = nullptr;
        ListNode* cur = head;
        while(cur != nullptr){
            ListNode* next_node = cur -> next;
            cur -> next = pre;
            pre = cur;
            cur = next_node;
        }
        return pre;
    }
};

int main() {
    // 创建链表：1 -> 2 -> 3 -> 4 -> 5
    ListNode* head = new ListNode(1);
    head->next = new ListNode(2);
    head->next->next = new ListNode(3);
    head->next->next->next = new ListNode(4);
    head->next->next->next->next = new ListNode(5);

    Solution solution;
    ListNode* reversedHead = solution.ReverseList(head);

    // 输出反转后的链表：5 -> 4 -> 3 -> 2 -> 1
    while (reversedHead != nullptr) {
        cout << reversedHead->val << " ";
        reversedHead = reversedHead->next;
    }

    return 0;
}
