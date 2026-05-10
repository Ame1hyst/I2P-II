#include <iostream>
using namespace std;

struct Node {
    Node *prev;
    Node *next;
    int val;
    int tag;
};

class KuoYangPresent {
public:
    KuoYangPresent(int k);
    void Push(int x);
    void Pop();
    void Reverse();
    void ProgrammingTanoshi();
    void KuoYangTeTe();
    void PrintList();

private:
    Node *head, *mid, *tail;
    int sz, k;
    int now;
    bool reverse;
};


KuoYangPresent::KuoYangPresent(int k) {
    this->k = k;
    head = tail = mid = NULL;
    sz = 0;
    now = 0;
    reverse = false;
}

void KuoYangPresent::Push(int x) {
    Node *node = new Node();
    node->val = x;
    node->tag = now;   // remember when this node was inserted

    if (!reverse) {
        node->prev = tail; 
        node->next = NULL;
        
        if (tail) tail->next = node; 
        else head = node;
        
        tail = node;
    } else {
        node->next = head; 
        node->prev = NULL;
        
        if (head) head->prev = node; 
        else tail = node;
        
        head = node;
    }
    sz++;

    if (sz == 1) { 
        mid = head; 
        return; 
    }
    if (!reverse) {
        // pushed to physical tail: move mid right when sz turns odd
        if (sz % 2 == 1) mid = mid->next;
    } 
    else {
        // pushed to physical head: move mid left when sz turns even
        if (sz % 2 == 0) mid = mid->prev;
    }
}

void KuoYangPresent::Pop() {
    // For odd sz, physical mid == logical median regardless of reverse flag
    Node* to_del = mid;
    mid = mid->prev;          // new mid: physical left of old mid

    if (to_del->prev) to_del->prev->next = to_del->next;
    else              head = to_del->next;
    if (to_del->next) to_del->next->prev = to_del->prev;
    else              tail = to_del->prev;

    delete to_del;
    sz--;
    // sz is now even; invariant: pright = pleft + 1 ✓
}

void KuoYangPresent::Reverse() {
    reverse = !reverse;
}

void KuoYangPresent::ProgrammingTanoshi() {
    now++;   // lazy: nodes inserted before this call have tag < now
}

void KuoYangPresent::KuoYangTeTe() {
    // Flush lazy mod tags before printing
    for (Node* cur = head; cur; cur = cur->next) {
        if (now > cur->tag) {
            cur->val %= k;
            cur->tag = now;
        }
    }
}

void KuoYangPresent::PrintList() {
    if (!reverse) {
        for (Node* cur = head; cur; cur = cur->next)
            cout << cur->val << " ";
    } else {
        for (Node* cur = tail; cur; cur = cur->prev)
            cout << cur->val << " ";
    }
    cout << "\n";
}