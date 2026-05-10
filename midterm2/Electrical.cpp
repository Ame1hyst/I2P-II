#include <iostream>
using namespace std;

long long heap[10005];
int sz = 0;

void push(long long x){
    heap[++sz] = x;
    int i = sz;

    while (i > 1 && heap[i] < heap[i/2])
    {
        swap(heap[i], heap[i/2]);
        i /= 2;
    }    
}

long long pop(){
    long long target = heap[1];
    heap[1] = heap[sz--];
    int i = 1;


    while (true)
    {   
        int smallest = i;
        int left = 2*i;
        int right = 2*i + 1;

        if(left <= sz && heap[smallest] > heap[left]) smallest = left;
        if(right <= sz && heap[smallest] > heap[right]) smallest = right;
        if(smallest == i) break;

        swap(heap[smallest], heap[i]);
        i = smallest;
    }

    return target;
    
}

int main(void){
    int n;
    cin >> n;

    for(int i = 0; i < n; i++){
        long long x;
        cin >> x;
        push(x);
    }

    if(sz == 1) {cout << heap[1]; return 0;};

    long long total = 0;
    while (sz > 1)
    {
        long long a = pop();
        long long b = pop();
        long long sum = a + b;
        total += sum;
        push(sum);
    }
    
    cout << total;

    return 0;
}