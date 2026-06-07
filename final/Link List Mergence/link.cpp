#include "13863.h"
#include <iostream>
using namespace std;

template <typename T>
T &linked_list<T>::iterator::operator*()
{
    return n->val;
}

// Forward the itarator by pre-increament
template <typename T>
typename linked_list<T>::iterator linked_list<T>::iterator::operator++()
{
    n = n->next;
    return *this;
}
// Merge two sorted linked list internally
template <typename T>
typename linked_list<T>::node *linked_list<T>::merge(node *lhs, node *rhs)
{
    if(!lhs) return rhs;
    if(!rhs) return lhs;
    if(rhs->val <=  lhs->val){
        rhs->next = merge(rhs->next, lhs);
        return rhs;
    }
    if(lhs->val <= rhs->val){
        lhs->next = merge(lhs->next, rhs);
        return lhs;
    }
}

template class linked_list<int>;
template class linked_list<string>;
