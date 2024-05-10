//
// Created by Victor Navarro on 19/02/24.
//

#include "Combat.h"
#include <string>
#include <iostream>
#include <utility>
#include <algorithm>

using namespace std;

bool compareSpeed(Character *a, Character *b) {
    return a->getSpeed() > b->getSpeed();
}

Combat::Combat(vector<Character *> _participants) {
    participants = std::move(_participants);
    newParticipants = participants;
    for(auto participant : participants) {
        if (participant->getIsPlayer()) {
            partyMembers.push_back((Player *) participant);
        } else {
            enemies.push_back((Enemy *) participant);
        }
    }
}

Combat::Combat(vector<Player*> _partyMembers, vector<Enemy*> _enemies) {
    partyMembers = std::move(_partyMembers);
    enemies = std::move(_enemies);
    participants = vector<Character*>();
    participants.insert(participants.end(), partyMembers.begin(), partyMembers.end());
    participants.insert(participants.end(), enemies.begin(), enemies.end());
}

Combat::Combat() {
    participants = vector<Character*>();
}

void Combat::addParticipant(Character *participant) {
    participants.push_back(participant);
    if(participant->getIsPlayer()){
        partyMembers.push_back((Player*) participant);
    } else {
        enemies.push_back((Enemy*) participant);
    }
}

void Combat::combatPrep() {
    // Sort participants by speed
    sort(participants.begin(), participants.end(), compareSpeed);
}

string Combat::toString() {
    string result = "";
    vector<Character*>::iterator it;
    for(it = participants.begin(); it != participants.end(); it++){
        result += (*it)->toString() + "\n";
    }
    cout<<"===================="<<endl;
    return result;
}

Character* Combat::getTarget(Character* attacker) {
    vector<Character*>::iterator it;
    for(it = participants.begin(); it != participants.end(); it++){
        if((*it)->getIsPlayer() != attacker->getIsPlayer()){
            return *it;
        }
    }
    //TODO: Handle this exception
    return nullptr;
}

void Combat::doCombat() {
    while (true) {
        cout << "The Combat begins!" << endl;
        combatPrep();
        int round = 1;
        // This while represents the combat rounds
        while (enemies.size() > 0 && partyMembers.size() > 0) {
            cout << "\nRound " << round << endl;
            vector<Character *>::iterator it = participants.begin();
            registerActions(it);
            executeActions(it);

            round++;
        }

        if (enemies.empty()) {
            cout << "You win!" << endl;
        } else {
            cout << "You lose!" << endl;

        }

        cout << "Do you want to battle another time? (y/n)" << endl;
        char playAgain;
        cin >> playAgain;

        if (playAgain == 'n') {
            for (auto player: partyMembers) {
                player->serialize("StatsPlayer.txt");
            }
            break;
        } else {
            participants.clear();
            partyMembers.clear();
            enemies.clear();

            // Reset participants
            vector<Character *> newParticipants = getNewParticipants();
            for (auto participant: newParticipants) {
                addParticipant(participant);
            }

            // Reset health of the participants
            HealParticipants();

            cout << "Do you think that you can be better? (y/n)" << endl;
            char increaseDifficulty;
            cin >> increaseDifficulty;

            // Increase difficulty of the enemies
            if (increaseDifficulty == 'y') {
                for (auto enemy: enemies) {
                    enemy->moreDificcult();
                }
            }
        }
    }
}

void Combat::executeActions(vector<Character*>::iterator participant) {
    while(!actionQueue.empty()) {
        Action currentAction = actionQueue.top();
        currentAction.action();
        actionQueue.pop();

        //Check if there are any dead characters
        checkParticipantStatus(*participant);
        checkParticipantStatus(currentAction.target);
    }
}

void Combat::checkParticipantStatus(Character *participant) {
    if(participant->getHealth() <= 0) {
        if(participant->getIsPlayer()) {
            partyMembers.erase(remove(partyMembers.begin(), partyMembers.end(), participant), partyMembers.end());
        } else {
            enemies.erase(remove(enemies.begin(), enemies.end(), participant), enemies.end());
        }
        participants.erase(remove(participants.begin(), participants.end(), participant), participants.end());
    }
}

void Combat::registerActions(vector<Character*>::iterator participantIterator) {
    // Represents the turn of each participant
    // The action that each participant will take is stored in a priority queue
    while(participantIterator != participants.end()) {
        if((*participantIterator)->getIsPlayer()) {
            Action playerAction = ((Player*) *participantIterator)->takeAction(enemies);
            actionQueue.push(playerAction);
        } else {
            Action enemyAction = ((Enemy*) *participantIterator)->takeAction(partyMembers);
            actionQueue.push(enemyAction);
        }

        participantIterator++;
    }
}

vector<Character*> Combat::getNewParticipants() {
    return newParticipants;
}

void Combat::HealParticipants() {
    vector<Character*>::iterator it;
    for(it = participants.begin(); it != participants.end(); it++){
        (*it)->heal();
    }
}


