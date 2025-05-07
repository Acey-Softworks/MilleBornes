#ifndef CARD_HPP
#define CARD_HPP

#include <string>
using namespace std;

class Card {
    public:
        string name;    // name of card
        int val;        // value of card

        Card() {} // default
        Card(string n, int v) {
            name = n;
            val = v;
        }
};

#endif // CARD_HPP