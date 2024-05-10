//
// Created by Victor Navarro on 15/02/24.
//

#include "Player.h"
#include <iostream>
#include <fstream>

using namespace std;

Player::Player(char* _name, int _health, int _attack, int _defense, int _speed) : Character(_name, _health, _attack, _defense, _speed, true) {
    level = 1;
    experience = 0;
}

Player::Player(char* _name, int _health, int _attack, int _defense, int _speed, bool _isPlayer, int _level, int _experience) : Character(_name, _health, _attack, _defense, _speed, _isPlayer) {
    level = _level;
    experience = _experience;
}

void Player::doAttack(Character *target) {
    target->takeDamage(attack);

    if (target->getHealth() <= 0)
    {
        auto enemyTarget = dynamic_cast<Enemy*>(target);

        if (enemyTarget != nullptr) {
            gainExperience(enemyTarget->getExperience());
        }
    }
}

void Player::takeDamage(int damage) {
    int trueDamage = damage - defense;
    if (trueDamage < 0) {
        trueDamage = 0;
    }

    health-= trueDamage;
    if(health <= 0) {
        health = 0;
    }

    cout << name << " took " << trueDamage << " damage!" << endl;
    cout << name << " has " << health << " health left!" << endl;

    if(health <= 0) {
        cout << name << " has been defeated!" << endl;
    }

}

void Player::levelUp() {
    level++;

    cout << getName() << " is in a new level (" << level << ")" << endl;
    int oldSpeed = getSpeed(), oldHealth = getOriginalHealth(), oldDefense = getDefense(), oldAttack = getAttack();
    moreStats();
    cout << getName() << " new stats: " << endl
    << "Speed: " << oldSpeed << " ---> " << getSpeed() << endl
    << "Health: " << oldHealth << " ---> " << getOriginalHealth() << endl
    << "Defense: " << oldDefense << " ---> " << getDefense() << endl
    << "Speed: " << oldAttack << " ---> " << getAttack() << endl;
}

void Player::gainExperience(int exp) {
    experience += exp;
    if (experience >= 100) {
        levelUp();
        experience = 100-experience;
    }
}

Character* Player::selectTarget(vector<Enemy*> possibleTargets) {
    int selectedTarget = 0;
    cout << "Select a target: " << endl;
    for (int i = 0; i < possibleTargets.size(); i++) {
        cout << i << ". " << possibleTargets[i]->getName() << endl;
    }

    //TODO: Add input validation
    cin >> selectedTarget;
    return possibleTargets[selectedTarget];
}

Action Player::takeAction(vector<Enemy*> enemies) {
    int action = 0;
    while (true) {
        cout << getName() << " turn!" << endl
             << "Select an action: " << endl
             << "1. Attack" << endl
             << "2. Defend" << endl;
        cin >> action;
        if (action == 1 || action == 2) {
            break;
        } else {
            cout << "Invalid action" << endl;
        }
    }
    Action currentAction;
    Character* target = nullptr;

    if (getIsDefending()) {
        resetDefense();
    }

    switch(action) {
        case 1:
            target = selectTarget(enemies);
            currentAction.target = target;
            currentAction.action = [this, target](){
                doAttack(target);
            };
            currentAction.speed = getSpeed();
            break;
        case 2:
            currentAction.target = this;
            currentAction.action = [this](){
                defend();
            };
            currentAction.speed = DEFENSE_SPEED_PRIORITY;
            break;
        default:
            cout << "Invalid action" << endl;
            break;
    }

    return currentAction;
}

void Player::serialize(const char* filename) {
    std::ofstream file(filename); {
        if (file.is_open()) {
            file << name << std::endl;
            file << originalHealth << std::endl;
            file << attack << std::endl;
            file << defense << std::endl;
            file << speed << std::endl;
            file << isPlayer << std::endl;
            file << level << std::endl;
            file << experience << std::endl;
        } else {
            std::cout << "Error opening file" << std::endl;
        }
    }
}
void Player::deserialize(const char *filename) {
    std::ifstream file(filename);
    if (file.is_open()) {
        file >> name;
        file >> originalHealth;
        health = originalHealth;
        file >> attack;
        file >> defense;
        file >> speed;
        file >> isPlayer;
        file >> level;
        file >> experience;
        file.close();

        cout << name << " has been loaded!" << endl;
    } else {
        std::cerr << "New game. Making a new player" << std::endl;
        while (true) {
            cout << "Enter your player's name: ";
            cin >> name;
            cout << "Are you sure you want to create a new player with the name " << name << "? (y/n): ";
            char confirm;
            cin >> confirm;
            if (confirm == 'y') {
                break;
            }
        }
        originalHealth = 50;
        health = originalHealth;
        attack = 20;
        defense = 10;
        speed = 10;
        isPlayer = true;
        level = 1;
        experience = 0;
    }
}