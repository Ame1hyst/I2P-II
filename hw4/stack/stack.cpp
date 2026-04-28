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
    ListNode *cur = head;
        while (cur!=NULL)
        {
            ListNode *next = cur->nextPtr;
            delete cur;
            cur = next;
        }
}



void List_stack::push(const int &n){
    ListNode *node = new ListNode(n);
    if(!head){
        head = node;
        tail = node;
    }
    else{
        node->nextPtr = head;
        head->prevPtr = node;
        head = node;
    }
}

void List_stack::pop(){
    if(head == NULL) return; 
    ListNode *temp = head;
    head = head->nextPtr;
    if(head != NULL)
        head->prevPtr = NULL;
    else
        tail = NULL; 

    delete temp;
    

}

void List_stack::print(){
    for(ListNode *cur = head; cur !=NULL; cur = cur->nextPtr){
        cout << cur->data;
        if (cur->nextPtr != NULL)
            cout << " ";
    }
}





