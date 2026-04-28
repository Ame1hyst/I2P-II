struct ListNode {
    int value;
    struct ListNode* next;
};

ListNode* solve(ListNode* head) {
    if (!head || !head->next) return head;

    static ListNode* arr[500005];
    int n = 0;
    ListNode* curr = head;
    while (curr) {
        arr[n++] = curr;
        curr = curr->next;
    }

    int left = 0, right = n - 1;
    while (left < right) {
        arr[left]->next = arr[right];
        left++;
        if (left == right) break;
        arr[right]->next = arr[left];
        right--;
    }
    arr[left]->next = 0;

    return head;
}