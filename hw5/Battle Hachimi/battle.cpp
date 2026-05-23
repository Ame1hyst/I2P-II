#include "14948.h"
#include <algorithm>
#include <iostream>
using namespace std;

void Item::damage(int &hp, int &defense)
{
    if (m_weight > defense)
        hp = max(0, hp - (m_weight - defense));
}

Laptop::Laptop(int weight, int power)
    : Item("Laptop", weight), m_power(power) {}

void Laptop::use(int &hp, int &defense)
{
    int before = hp;
    damage(hp, defense);
    cout << "Laptop" << (hp < before ? " hit" : " blocked") << "\n";
    hp = max(0, hp - m_power);
}

Notebook::Notebook(int weight, int notes)
    : Item("Notebook", weight), m_notes(notes) {}

void Notebook::use(int &hp, int &defense)
{
    int before = hp;
    damage(hp, defense);
    cout << "Notebook" << (hp < before ? " hit" : " blocked") << "\n";
    defense = max(0, defense - m_notes);
}

GPU::GPU(int weight)
    : Item("GPU", weight) {}

void GPU::use(int &hp, int &defense)
{
    int before = hp;
    damage(hp, defense);
    cout << "GPU" << (hp < before ? " hit" : " blocked") << "\n";
    hp = max(0, hp - defense);
}

Shoe::Shoe(int weight, int smell)
    : Item("Shoe", weight), m_smell(smell) {}

void Shoe::use(int &hp, int &defense)
{
    int before = hp;
    damage(hp, defense);
    cout << "Shoe" << (hp < before ? " hit" : " blocked") << "\n";
    hp      = max(0, hp      - m_smell);
    defense = max(0, defense - m_smell);
}