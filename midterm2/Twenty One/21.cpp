#include "13182.h"
#include <iostream>
using namespace std;

// Guest
Guest::Guest(){
	name = "";
	money = 0;
	skill = 0;
}

Guest::Guest(string s, int m, int ski){
	name = s;
	money = m;
	skill = ski;	
}

Guest::~Guest(){}

string Guest::get_name(){ return name;}
int Guest::get_money(){ return money;}
int Guest::get_skill(){ return skill;}
void Guest::Win(int m){ money += m;}

// Casino
Casino::Casino(){
	fee = 0;

	guest_num = 0;
	income = 0;
	list_num = 0;
}

Casino::~Casino(){
	for(int i = 0; i < guest_num; i++){
		delete guest[i];
	}
}

void Casino::Enterance(int f){fee = f;}

void Casino::GuestEnter(string s, int m, int ski){
	for(int i = 0; i < list_num; i++){
		if(blacklist[i] == s) return;
	}

	for(int i = 0; i < guest_num; i++){
		if(guest[i]->get_name() == s) return;
	}

	if( m <= fee){
		income += m;
		blacklist[list_num++] = s;
	}
	else{
		m -= fee;
		income += fee;
		guest[guest_num++] = new Guest(s, m, ski);

	}

}
void Casino::Win(string s, int m) {
    int idx = -1;
    for (int i = 0; i < guest_num; i++)
        if (guest[i]->get_name() == s) { idx = i; break; }
    if (idx == -1) return;

    Guest* g = guest[idx];

    // cheater check
    if (m > 2 * g->get_skill()) {
        income -= m;                        
        blacklist[list_num++] = s;

        delete guest[idx];
        for (int i = idx; i < guest_num - 1; i++) guest[i] = guest[i + 1];
        guest_num--;
        return;
    }

    // normal
    if (m >= 0) {
        income -= m;
        g->Win(m);
    } else {
        int lose = -m; // lose money                  
        if (g->get_money() <= lose) {
            // bankrupt
            income += g->get_money();
            blacklist[list_num++] = s;
            delete guest[idx];
            for (int i = idx; i < guest_num - 1; i++) guest[i] = guest[i + 1];
            guest_num--;
        } else {
            income += lose;
            g->Win(m);
        }
    }
}


void Casino::EndDay() {
    for (int i = 0; i < guest_num; i++) delete guest[i];
    guest_num = 0;                          
}

void Casino::Result() {
    cout << income << "\n";
    for (int i = 0; i < list_num; i++)
        cout << blacklist[i] << "\n";
}