#ifndef PLAYER_HPP
#define PLAYER_HPP

#include <iostream>
#include <list>
#include <string>
#include <stack>
#include <queue>
using namespace std;

#include "Card.hpp"

class Player {
    public:
        string name;
        list<Card> pHand;
        stack<Card> pBattlePile;
        stack<Card> pSpeedPile;
        int speedLimit = 200; // default speed limit
        list<Card> pSafetyArea;
        queue<Card> pDistancePile;

        Player(string n) : name(n) {
            cout << "Player created: " << name << endl;
        };
        void initializePlayerHand(list<Card>&); // initializes the player hand with 6 cards from the deck
        void printHand(); // prints player hand
        void printPlayerArea(); // prints player area (safety, speed, battle, distance piles)
        bool validatePlay(int, queue<Player*>&); // validates the play of a card
        bool validateHazardRemedyRelationship(int); // validates the relationship between a hazard and remedy card
        bool getSafetyCard(string); // checks if the player has a specific safety card
        bool checkSafety(Player*, string); // checks if the player has a safety card that can counter a hazard card
        bool getBattleCard(string); // gets the name of the battle card at the top of the battle pile
        int getSpeedLimit(); // gets the speed limit of the player
        void setSpeedLimit(int); // sets the speed limit of the player
        int countMiles(); // counts the total miles in the distance pile
    private:
};

#endif // PLAYER_HPP