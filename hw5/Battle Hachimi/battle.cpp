#include <iostream>
#include <algorithm>
#include <string>
using namespace std;

class Item
{
public:
    Item(const std::string &name, int weight)
        : m_name(name), m_weight(weight)
    {
    }
    ~Item() = default;

    void damage(int &hp, int &defense);

private:
    const std::string m_name;
    const int m_weight;
};

class Laptop : public Item
{
public:
    Laptop(int weight, int power);
    ~Laptop() = default;
    void use(int &hp, int &defense); // first damage, then decrease hp by [power] regardless of defense
private:
    int m_power;
};

class Notebook : public Item
{
public:
    Notebook(int weight, int notes);
    ~Notebook() = default;
    void use(int &hp, int &defense); // first damage, then lower defense by [notes]
private:
    int m_notes;
};

class GPU : public Item
{
public:
    GPU(int weight);
    ~GPU() = default;
    void use(int &hp, int &defense); // first damage, then decrease hp by [defense] regardless of defense
};

class Shoe : public Item
{
public:
    Shoe(int weight, int smell);
    ~Shoe() = default;
    void use(int &hp, int &defense); // first damage, then hp and defense both reduced by [smell]
private:
    int m_smell;
};

void Item::damage(int &hp, int &defense){
    if(m_weight > defense){
        hp -= m_weight - defense;
        if(hp <= 0) hp = 0;
    }
}

Laptop::Laptop(int weight, int power) : Item("Laptop", weight), m_power(power) {}

void Laptop::use(int &hp, int &defense){
    int before = hp;
    damage(hp, defense);
    cout << "Laptop" << (hp < before ? " hit" : " blocked") << "\n";
    hp -= m_power;
    if(hp <= 0) hp = 0;
}

Notebook::Notebook(int weight, int notes) : Item("Notebook", weight), m_notes(notes) {}
void Notebook::use(int &hp, int &defense){
    int before = hp;
    damage(hp, defense);
    cout << "Notebook" << (hp < before ? " hit" : " blocked") << "\n";
    defense -= m_notes;
    if(defense <= 0) defense = 0;
}

GPU::GPU(int weight) : Item("GPU", weight) {}
void GPU::use(int &hp, int &defense){
    int before = hp;
    damage(hp, defense);
    cout << "GPU" << (hp < before ? " hit" : " blocked") << "\n";
    hp -= defense;
    if(hp <= 0) hp = 0;   
}

Shoe::Shoe(int weight, int smell) : Item("Shoe", weight), m_smell(smell) {}
void Shoe::use(int &hp, int &defense){
    int before = hp;
    damage(hp, defense);
    cout << "Shoe" << (hp < before ? " hit" : " blocked") << "\n";
    hp -= m_smell;
    if(hp <= 0) hp = 0;
    defense -= m_smell;
    if(defense <= 0) defense = 0;   
}