#include "13131.h"
using namespace std;

// Array Max heap

Array_MAX_HEAP::Array_MAX_HEAP(): MAX_HEAP() {}

void Array_MAX_HEAP::PUSH(const int &num){
    array[++Count] = num;
    int i = Count;

    while (i > 1 && array[i] > array[i/2])
    {
        swap(array[i], array[i/2]);
        i /= 2;
    }   
}

int Array_MAX_HEAP::MAX()const{
    return Count == 0 ? -1 : array[1];
}

int Array_MAX_HEAP::POP(){
    if (Count == 0) return -1;
    int target = array[1];
    array[1] = array[Count--];
    int i = 1;
    while (true)
    {
        int largest = i;
        int left = 2*i;
        int right = 2*i + 1;

        if(left <= Count && array[left] > array[largest]) largest = left;
        if(right <= Count && array[right] > array[largest]) largest = right;
        if(i == largest) break;
        swap(array[largest], array[i]);
        i = largest;
    }
    return target; 
}

// List Max Heap
List_MAX_HEAP::List_MAX_HEAP() : MAX_HEAP() {}
void List_MAX_HEAP::deleteTree(ListNode *root){
    if(root == NULL) return;
    deleteTree(root->left);
    deleteTree(root->right);
    delete root;
}

void List_MAX_HEAP::PUSH(const int &num){
    ListNode *node = new ListNode(num);
    ++Count;
    if(Count == 1) {root = node; return;}
    
    ListNode *parent = findparent(Count, root);
    if(Count % 2 == 0) parent->left = node;
    else parent->right = node;
    node->parent = parent;

    ListNode* cur = node;
    while (cur->parent && cur->value > cur->parent->value)
    {
        swap(cur->value, cur->parent->value);
        cur = cur->parent;
    }
}
int List_MAX_HEAP::MAX() const {
    return (Count == 0) ? -1: root->value;
}

int List_MAX_HEAP::POP(){
    if (Count == 0) return -1;
    int target = root->value;

    if(Count == 1) {
        delete root;
        root = NULL;
        Count = 0;
        return target;
    }

    ListNode* par = findparent(Count, root);
    ListNode* last_node;

    if (Count % 2 == 0) {
        last_node = par->left;
        root->value = last_node->value;

        par->left = NULL;
        delete last_node;
    }
    else {
        last_node = par->right;
        root->value = last_node->value;

        par->right = NULL;
        delete last_node;
    }
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