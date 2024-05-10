//
// Created by Victor Navarro on 15/02/24.
//

#ifndef RPG_CHARACTER_H
#define RPG_CHARACTER_H
#include <string>
#include <cstring>

using namespace std;

class Character {

protected:
    char name[30];
    int health;
    int attack;
    int defense;
    int speed;
    bool isPlayer;
    int originalHealth;
    bool isDefending = false;

public:
    Character(char* _name, int _health, int _attack, int _defense, int _speed, bool _isPlayer);

    virtual void doAttack(Character *target) = 0;
    virtual void takeDamage(int damage) = 0;

    void defend();
    void resetDefense();
    void moreStats();
    void heal();

    bool flee(Character* target);
    bool getIsPlayer();
    bool getIsDefending();

    const char* getName();
    string toString();

    int getHealth();
    int getOriginalHealth();
    int getAttack();
    int getDefense();
    int getSpeed();
};


#endif //RPG_CHARACTER_H