#include "function.h"
#include <iostream>
using std::cin;
using std::cout;

// Dereference the iterator
template <typename T>
T &linked_list<T>::iterator::operator*()
{
    return n->val;
}

// Forward the iterator by pre-increment
template <typename T>
typename linked_list<T>::iterator linked_list<T>::iterator::operator++()
{
    n = n->next;
    return *this;
}

// Merge two sorted linked lists internally
template <typename T>
typename linked_list<T>::node *linked_list<T>::merge(node *lhs, node *rhs)
{
    if (!lhs) return rhs;
    if (!rhs) return lhs;
    if (lhs->val <= rhs->val) {
        lhs->next = merge(lhs->next, rhs);
        return lhs;
    } else {
        rhs->next = merge(lhs, rhs->next);
        return rhs;
    }
}

template class linked_list<int>;
template class linked_list<string>;