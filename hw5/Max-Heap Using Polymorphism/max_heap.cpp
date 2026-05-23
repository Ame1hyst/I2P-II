#include "13131.h"
using namespace std;

// ─── Array_MAX_HEAP ──────────────────────────────────────────────────────────

Array_MAX_HEAP::Array_MAX_HEAP() : MAX_HEAP() {}

void Array_MAX_HEAP::PUSH(const int &x)
{
    array[++Count] = x;
    int i = Count;

    while (i > 1 && array[i] > array[i/2])
    {
        swap(array[i], array[i/2]);
        i /= 2;
    }
}

int Array_MAX_HEAP::MAX() const
{
    return Count == 0 ? -1 : array[1];
}

int Array_MAX_HEAP::POP()
{
    if (Count == 0) return -1;
    int target = array[1];
    array[1] = array[Count--];
    int i = 1;

    while (true)
    {
        int largest = i;
        int left  = 2*i;
        int right = 2*i + 1;

        if (left  <= Count && array[largest] < array[left])  largest = left;
        if (right <= Count && array[largest] < array[right]) largest = right;
        if (largest == i) break;

        swap(array[largest], array[i]);
        i = largest;
    }

    return target;
}

// ─── List_MAX_HEAP ───────────────────────────────────────────────────────────

List_MAX_HEAP::List_MAX_HEAP() : MAX_HEAP(), root(NULL) {}

void List_MAX_HEAP::deleteTree(ListNode *node)
{
    if (!node) return;
    deleteTree(node->left);
    deleteTree(node->right);
    delete node;
}

void List_MAX_HEAP::PUSH(const int &x)
{
    ListNode* node = new ListNode(x);
    ++Count;
    if (Count == 1) { root = node; return; }

    ListNode* par = findparent(Count, root);
    node->parent = par;
    if (Count % 2 == 0) par->left  = node;
    else                 par->right = node;

    ListNode* cur = node;
    while (cur->parent && cur->value > cur->parent->value)
    {
        swap(cur->value, cur->parent->value);
        cur = cur->parent;
    }
}

int List_MAX_HEAP::MAX() const
{
    return Count == 0 ? -1 : root->value;
}

int List_MAX_HEAP::POP()
{
    if (Count == 0) return -1;
    int target = root->value;

    if (Count == 1)
    {
        delete root;
        root = NULL;
        Count = 0;
        return target;
    }

    ListNode* lastNode = (Count % 2 == 0) ? findparent(Count, root)->left
                                           : findparent(Count, root)->right;
    root->value = lastNode->value;
    if (Count % 2 == 0) lastNode->parent->left  = NULL;
    else                 lastNode->parent->right = NULL;
    delete lastNode;
    --Count;

    ListNode* cur = root;
    while (true)
    {
        ListNode* largest = cur;
        if (cur->left  && cur->left->value  > largest->value) largest = cur->left;
        if (cur->right && cur->right->value > largest->value) largest = cur->right;
        if (largest == cur) break;

        swap(cur->value, largest->value);
        cur = largest;
    }

    return target;
}