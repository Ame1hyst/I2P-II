#include <iostream>
using namespace std;

class ListNode
{
    friend class List_stack; //make List_stack a friend
    public:
        ListNode( const int &info ) //constructor
        : data( info ), nextPtr( NULL ), prevPtr( NULL )
        {
        } //end ListNode constructor

    private:
        int data; //data
        ListNode *nextPtr; // next node in list
        ListNode *prevPtr;
}; //end class ListNode

class List_stack {
    public:
        List_stack();
        ~List_stack();
        void push(const int &);
        void pop();
        void print();
    private:
        ListNode *head;
        ListNode *tail;
};

List_stack::List_stack(){
    head = NULL;
    tail = NULL;
}

List_stack::~List_stack(){
    while (head != NULL)
    {
        ListNode *temp = head->nextPtr;
        delete head;
        head = temp;
    }
    
}

void List_stack::push(const int &n){
    ListNode *node = new ListNode(n);
    if(head == NULL){
        head = node;
        tail = node;
    }
    else{
        head->prevPtr = node;
        node->nextPtr = head;
        head = node;
    }
}

void List_stack::pop(){
    if(head == NULL) return;
    ListNode *target = head;
    head = head->nextPtr;
    if(head == NULL) tail = NULL;
    else head->prevPtr = NULL;

    delete target;

}

void List_stack::print(){
    for(ListNode *cur = head; cur != NULL; cur = cur->nextPtr){
        cout << cur->data; 
        if(cur->nextPtr) cout << " ";
    }
}

