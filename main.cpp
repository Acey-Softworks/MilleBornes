#include <iostream>
#include <list>
#include <random>
#include <algorithm>
#include <string>
using namespace std;

#include "Player.hpp"
#include "Player.cpp"

/*
Author: Adrian Carballo
Description: The object of this game is to be the first player to accumulate a total
                of exactly 700 miles in each hand played, while avoiding hazards
                from other players.
V2:
- removed the number of the card from the struct
- created a function to initialize the deck of cards
- created a function to add cards to the deck
- created a function prototype to shuffle the deck of cards

V3:
- added a function to shuffle deck of cards

V4:
- redone shuffleDeck function to use shuffle from the STL
- made Card a class instead of a struct
- add player class to hold player info and player decks

V5:
- added gameplay loop
- started validatePlay function to validate the play of a card

V6: 
- fixed gameplay loop problems form previous versions
- finished validatePlay function to validate the play of a card
*/

void initializeDeck(list<Card>&);
void addToDeck(int, string, int, list<Card>&);
void shuffleDeck(list<Card>&);
void printDeck(list<Card>&);
bool validateWin(Player&);

int main() {
    list<Card> deck;

    initializeDeck(deck);

    string playerName;
    queue<Player*> playerQueue;

    cout << "WELCOME TO MILLE BORNES!" << endl;

    Player player1("Player 1"); // create player object
    player1.initializePlayerHand(deck);
    playerQueue.push(&player1); // add player to queue

    Player player2("Player 2"); // create player object
    player2.initializePlayerHand(deck);
    playerQueue.push(&player2); // add player to queue

    bool gameOver = false;
    bool validPlay = false;
    while (true) {
        Player* player = playerQueue.front(); // get the current player
        playerQueue.pop(); // remove the current player from the queue
        playerQueue.push(player); // push current player to back of queue

        cout << "-----------------------------------------------------" << endl << endl;
        cout << player->name << "'s turn!" << endl;
            
        int pChoice = 0;

        cout << player->name << " draws a card from the deck..." << endl << endl;
        player->pHand.push_back(deck.front());
        deck.pop_front(); // remove the card from the deck

        player->printPlayerArea();
        cout << "0. Discard a card" << endl << endl;

        bool validPlay = false;
        while (!validPlay) {
            cout << "Please select a card to play from your hand (0 - 7): ";
            cin >> pChoice;
            cout << endl;

            if (pChoice == 0) { // Discard a card
                cout << "Choose a card to discard: ";
                cin >> pChoice;
                auto it = player->pHand.begin();
                advance(it, pChoice - 1); // move iterator to the selected card
                cout << "Discarding... " << it->name << endl;
                player->pHand.erase(it); // remove the card from the hand

                //cout << "Your area after discarding:" << endl;
                //player->printPlayerArea(); // print the hand after discarding
                //cout << endl;
                validPlay = true;
            }
            else if (pChoice > 0 && pChoice <= 7) { // Play card
                validPlay = player->validatePlay(pChoice, playerQueue); // validate the play
            }
            else { // Invalid card choice (not 0 - 7)
                cout << "Invalid choice. Please try again." << endl;
                validPlay = false;
            }

            if (validateWin(*player)) {
                cout << player->name << " wins!" << endl;
                return 0;
            }
        }
    }

    return 0;
}

void initializeDeck(list<Card> &deck) {
    // HAZARD CARDS
    addToDeck(2, "HAZARD: OUT OF GAS", -1, deck);         // initialize OUT OF GAS cards
    addToDeck(2, "HAZARD: FLAT TIRE", -1, deck);         // initialize FLAT TIRE cards
    addToDeck(2, "HAZARD: ACCIDENT", -1, deck);         // initialize ACCIDENT cards
    addToDeck(2, "HAZARD: SPEED LIMIT 50mph", -1, deck);     // initialize SPEED LIMIT 50 cards
    addToDeck(1, "HAZARD: SPEED LIMIT 25mph", -1, deck);     // initialize SPEED LIMIT 25 cards
    addToDeck(4, "HAZARD: STOP!", -1, deck);             // initialize STOP cards

    // REMEDY CARDS
    addToDeck(6, "REMEDY: GAS", 1, deck);               // initialize GAS cards
    addToDeck(6, "REMEDY: SPARE TIRE", 1, deck);         // initialize SPARE TIRE cards
    addToDeck(6, "REMEDY: REPAIRS", 1, deck);           // initialize REPAIRS cards
    addToDeck(6, "REMEDY: END OF LIMIT", 1, deck);          // initialize END OF LIMIT cards
    addToDeck(14, "REMEDY: ROLL!", 1, deck);             // initialize ROLL! cards

    // SAFETY CARDS
    addToDeck(1, "SAFETY: EXTRA TANK", 0, deck);         // initialize EXTRA TANK card
    addToDeck(1, "SAFETY: PUNCTURE PROOF", 0, deck);     // initialize PUNCTURE PROOF card
    addToDeck(1, "SAFETY: DRIVING ACE", 0, deck);        // initialize DRIVING AVE card
    addToDeck(1, "SAFETY: RIGHT OF WAY", 0, deck);        // initialize RIGHT OF WAY card

    // DISTANCE CARDS
    addToDeck(4, "DISTANCE: 200mi", 200, deck);     // initialize 200mi DISTANCE card
    addToDeck(12, "DISTANCE: 100mi", 100, deck);    // initialize 100mi DISTANCE card
    addToDeck(10, "DISTANCE: 75mi", 75, deck);      // initialize 75mi DISTANCE card
    addToDeck(10, "DISTANCE: 50mi", 50, deck);      // initialize 50mi DISTANCE card
    addToDeck(10, "DISTANCE: 25mi", 25, deck);      // initialize 25mi DISTANCE card

    shuffleDeck(deck);
}

void addToDeck(int n, string name, int val, list<Card> &deck) {
    for (int i = 0; i < n; i++) {
        Card card = Card(name, val);    // create a new card object
        deck.push_back(card);           // add card to deck
    }
}

void shuffleDeck(list<Card>& deck) {
    vector<Card> vDeck(deck.begin(), deck.end()); // copy list to vector

    random_device rd;
    mt19937 mt(rd());

    shuffle(vDeck.begin(), vDeck.end(), mt); // shuffle

    deck.assign(vDeck.begin(), vDeck.end()); // copy vector back to list

    /*
    // print shuffled deck
    cout << "Shuffled Deck: " << endl;
    printDeck(split1);
    cout << endl;
    */
}

void printDeck(list<Card>& deck) {
    int deckCount = 1;
    for (auto i = deck.begin(); i != deck.end(); i++) {
        cout << deckCount << " Card Name: " << i->name << ", Value: " << i->val << endl;
        deckCount++;
    }
}

bool validateWin(Player& player) {
    int playerDistanceTotal = 0;

    // Calculate the total distance from the player's distance pile
    queue<Card> temp = player.pDistancePile;
    while (!temp.empty()) {
        playerDistanceTotal += temp.front().val; // add the value of the card to the total
        temp.pop();
    }

    // Check if the player has reached 1000 miles
    if (playerDistanceTotal >= 700) {
        return true;
    }

    return false;
}