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


KuoYangPresent::KuoYangPresent(int k){
    head = NULL;
    mid = NULL;
    tail = NULL;

    sz = 0;
    this->k = k;
    now = 0;

    reverse = false;
}

void KuoYangPresent::Push(int x){
    Node *node = new Node();
    node->val = x;
    node->tag = now;

    if(!reverse){
        node->next = NULL;
        node->prev = tail;
        
        if(tail) tail->next = node;
        else head = node;

        tail = node;
    }
    else{
        node->prev = NULL;
        node->next = head;
         
        if(head) head->prev = node;
        else tail = node;

        head = node;
    }

    sz++;
    if(sz == 1) { mid = node; return;}

    if(!reverse){
        if(sz % 2 == 1) mid = mid->next;
    }
    else{
        if(sz % 2 == 0) mid = mid->prev;
    }


}

void KuoYangPresent::Pop(){
    if(sz == 1){
        delete mid;
        head = tail = mid = NULL;
        sz = 0;
        return;
    }

    Node *target = mid;
    mid = mid->prev;

    if(target->next) target->next->prev = target->prev;
    else tail = target->prev;
    if(target->prev) target->prev->next = target->next;
    else head = target->next;

    sz--;
    delete target;
}

void KuoYangPresent::Reverse(){
    reverse = !reverse;
}

void KuoYangPresent::ProgrammingTanoshi(){
    now++;
}

void KuoYangPresent::KuoYangTeTe(){
    for(Node *cur = head; cur != NULL; cur = cur->next){
        if(now > cur->tag){
            cur->val %= k;
            cur->tag = now;
        }
    }
}

void KuoYangPresent::PrintList(){
    if(!reverse){
        for(Node *cur = head; cur != NULL; cur = cur->next){
            cout << cur->val << " ";
        }
    }
    else{
        for(Node *cur = tail; cur != NULL; cur = cur->prev){
            cout << cur->val << " ";
        }
    }
    cout << "\n";
}

