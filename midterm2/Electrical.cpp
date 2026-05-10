#include <iostream>
#include <queue>
using namespace std;

long long heap[10005];
int sz = 0;

void push(long long val) {
    heap[++sz] = val;
    int i = sz;
    while (i > 1 && heap[i] < heap[i/2]) {
        swap(heap[i], heap[i/2]);
        i /= 2;
    }
}

long long pop() {
    long long ret = heap[1];
    heap[1] = heap[sz--];
    int i = 1;
    while (true) {
        int smallest = i;
        if (2*i <= sz && heap[2*i] < heap[smallest]) smallest = 2*i;
        if (2*i+1 <= sz && heap[2*i+1] < heap[smallest]) smallest = 2*i+1;
        if (smallest == i) break;
        swap(heap[i], heap[smallest]);
        i = smallest;
    }
    return ret;
}

int main() {
    int n;
    cin >> n;
    
    for (int i = 0; i < n; i++) {
        long long x;
        cin >> x;
        push(x);
    }
    
    if (n == 1) {
        cout << heap[1];
        return 0;
    }
    
    long long total = 0;
    while (sz > 1) {
        long long a = pop();
        long long b = pop();
        long long merged = a + b;
        total += merged;
        push(merged);
    }
    
    cout << total;
    return 0;
}