#pragma once
#include <iostream>
using namespace std;
class VendingMachine {
private:
    int* data;      
    int capacity;   
    int size;       // current amount

    long long totalRevenue; 
    int totalSold;          

public: //these functions need to be implemented by you.
    VendingMachine(); 

    ~VendingMachine();

    void store(int price); 

    void sell();

    void printResult();
};

VendingMachine::VendingMachine(){
    capacity = 2000000;
    data = new int[capacity];
    size = 0;

    totalRevenue = 0;
    totalSold = 0;
}

VendingMachine::~VendingMachine(){
    delete[] data;
}

void VendingMachine::store(int price){
    int idx = size;
    data[size++] = price;
    while (idx > 0)
    {
        int parent = (idx-1) / 2;
        if(data[parent] > data[idx]){
            swap(data[parent], data[idx]);
            idx = parent;
        }
        else break;
    }
    
}

void VendingMachine::sell(){
    if (size == 0) return;   
    totalRevenue += data[0];
    totalSold++;


    data[0] = data[--size];
    int idx = 0;
    while (true)
    {
        int left = 2*idx + 1;
        int right = 2*idx + 2;
        int smallest = idx;

        if(left < size && data[left] < data[smallest]) smallest = left;
        if(right < size && data[right] < data[smallest]) smallest = right;

        if(smallest = idx) break;
        swap(data[smallest], data[idx]);
        idx = smallest;
    }
    
}

void VendingMachine::printResult(){
    cout << totalSold << " " << totalRevenue << "\n";
}