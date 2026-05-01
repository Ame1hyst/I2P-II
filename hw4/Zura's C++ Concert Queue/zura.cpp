
struct ListNode {
    int value;
    struct ListNode* next;
};

ListNode* solve(ListNode* head){
    if (!head || !head->next) return head;
    ListNode *arr[100001];
    int idx = 0;
    for(ListNode *cur = head; cur != nullptr; cur = cur->next){
        arr[idx++] = cur;
    }

    int left = 0, right = idx - 1;
    while (left < right)
    {
        arr[left++]->next = arr[right];
        if(left == right) break;
        arr[right--]->next = arr[left];
    }

    arr[left]->next = nullptr;

    return head;
}


