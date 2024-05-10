//
// Created by Victor Navarro on 29/02/24.
//

#ifndef RPG_ACTION_H
#define RPG_ACTION_H
#include <functional>
#include "../Character/Character.h"

using namespace std;

// Constant that represents the priority of an attack action
const int DEFENSE_SPEED_PRIORITY = 999;

class Action {
public:
    // Speed of who does the action
    int speed = 0;

    // Target of the action
    Character* target = nullptr;

    // Subscriber of the action
    Character* subscriber = nullptr;

    // Pointer to the function that represents the action
    function<void(void)> action = nullptr;
    Action(int _speed, function<void(void)> _action, Character* _subscriber, Character* _target);
    Action();

    bool operator<(const Action& other) const;
};


#endif //RPG_ACTION_H
