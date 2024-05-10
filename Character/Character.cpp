//
// Created by Victor Navarro on 15/02/24.
//

#include "Character.h"

// Character class constructor
Character::Character(char* _name, int _health, int _attack, int _defense, int _speed, bool _isPlayer) {
    strncpy(name, _name, 25);
    health = _health;
    attack = _attack;
    defense = _defense;
    speed = _speed;
    isPlayer = _isPlayer;
    originalHealth = _health;
}

const char* Character::getName() {
    return name;
}
int Character::getHealth() {
    return health;
}
int Character::getOriginalHealth() {
    return originalHealth;
}
int Character::getAttack() {
    return attack;
}
int Character::getDefense() {
    return defense;
}
int Character::getSpeed() {
    return speed;
}
bool Character::getIsPlayer() {
    return isPlayer;
}
bool Character::getIsDefending() {
    return isDefending;
}
string Character::toString() {
    return "Name: " + string(name) + "\nHealth: " + to_string(health) + "\nAttack: " + to_string(attack) + "\nDefense: " + to_string(defense) + "\nSpeed: " + to_string(speed);
}
bool Character::flee(Character*target) {
    if(this->speed > target->speed)
        return true;

    int chance = rand() % 100;
    return chance > 30;
}
void Character::defend() {
    defense = defense * 3;
    isDefending = true;
}
void Character::resetDefense() {
    defense = defense / 3;
    isDefending = false;
}

void Character::heal() {
    health = originalHealth;
}

void Character::moreStats() {
    speed = speed + 3;
    originalHealth = originalHealth + 10;
    defense = defense + 5;
    attack = attack + 2;
}