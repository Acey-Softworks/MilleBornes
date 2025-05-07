#include "Player.hpp"

bool Player::validatePlay(int pChoice, queue<Player*>& playerQueue) {
    auto it = pHand.begin();
    advance(it, pChoice - 1); // move iterator to selected card

    if (it->val <= -1) { // HAZARD CARD
        Player *target = playerQueue.front();

        // SPEED LIMIT HAZARD
        if ((it->name == "HAZARD: SPEED LIMIT 50mph" || it->name == "HAZARD: SPEED LIMIT 25mph") && !checkSafety(target, it->name)) {
            int limit = (it->name == "HAZARD: SPEED LIMIT 50mph") ? 50 : 25; // get the speed limit from the hazard card (50 or 25)
            target->pSpeedPile.push(*it); // add to speed pile
            target->setSpeedLimit(limit); // set the speed limit
            pHand.erase(it); // remove card from hand
            cout << "Card played." << endl << endl;
            return true;
        }
        else if (!target->pBattlePile.empty() && target->pBattlePile.top().val == 1 && !checkSafety(target, it->name)) {
            target->pBattlePile.push(*it); // add to battle pile
            pHand.erase(it); // remove card from hand
            cout << "Card played." << endl << endl;
            return true;
        }
        else {
            cout << "Invalid move." << endl << endl;
            return false;
        }
    }
    else if (it->val == 0) { // SAFETY CARD
        pSafetyArea.push_back(*it); // add to safety area
        pHand.erase(it); // remove card from hand
        cout << "Card played." << endl << endl;
        return true;
    }
    else if (it->val == 1) { // REMEDY CARD
        if (!pSpeedPile.empty() && it->name == "REMEDY: END OF LIMIT" && (pSpeedPile.top().name == "HAZARD: SPEED LIMIT 50mph" || pSpeedPile.top().name == "HAZARD: SPEED LIMIT 25mph")) {
            pSpeedPile.push(*it); // add to speed pile
            pHand.erase(it); // remove card from hand
            setSpeedLimit(200); // set speed limit to default
            cout << "Card played." << endl << endl;
            return true;
        }
        else if ((pBattlePile.empty() && it->name == "REMEDY: ROLL!") || (it->name == "REMEDY: ROLL!" && pBattlePile.top().val == 1)) {
            pBattlePile.push(*it); // add to battle pile
            pHand.erase(it); // remove card from hand
            cout << "Card played." << endl << endl;
            return true;
        }
        else if (!pBattlePile.empty() && pBattlePile.top().val == -1 && validateHazardRemedyRelationship(pChoice)) {
            pBattlePile.push(*it); // replace the hazard card from the battle pile
            pHand.erase(it); // remove card from hand
            cout << "Card played." << endl << endl;
            return true;
        }
        else {
            cout << "Invalid move." << endl << endl;
            return false;
        }
    }
    else { // DISTANCE CARD
        if (!pSafetyArea.empty() && getSafetyCard("SAFETY: RIGHT OF WAY") && !checkSafety(this, it->name) && (countMiles() + it->val) <= 700) {
            cout << "You have the RIGHT OF WAY safety card. You can play any distance card without playing a ROLL! card" << endl;
            pDistancePile.push(*it); // add to distance pile
            pHand.erase(it); // remove card from hand
            cout << "Card played." << endl << endl;
            return true;
        }
        else if (getBattleCard("REMEDY: ROLL!") && it->val <= getSpeedLimit() && (countMiles() + it->val) <= 700) {
            pDistancePile.push(*it); // add to distance pile
            pHand.erase(it); // remove card from hand
            cout << "Card played." << endl << endl;
            return true;
        }
        else {
            cout << "Invalid move." << endl << endl;
            return false;
        }
    }

    cout << "Invalid move." << endl << endl;
    return false;
}

void Player::initializePlayerHand(list<Card>& deck) {
    //cout << "Initializing " << name << "'s hand..." << endl << endl;
    
    for (int i = 0; i < 6; i++) {
        pHand.push_back(deck.front());
        deck.pop_front();
    }
}

void Player::printHand() {
    int deckCount = 1;
    //cout << name << "'s Hand:" << endl;
    for (auto i = pHand.begin(); i != pHand.end(); i++) {
        cout << deckCount << ". " << i->name << endl;
        deckCount++;
    }
}

void Player::printPlayerArea() {
    cout<< name << "'s Area:" << endl;
    
    if (!pSafetyArea.empty()) {
        cout << "Safety Area: " << endl;
        for (auto i = pSafetyArea.begin(); i != pSafetyArea.end(); i++) {
            cout << i->name << endl;
        }
    }
    else {
        cout << "Safety Area: Empty" << endl;
    }
    cout << endl;
    
    if (!pSpeedPile.empty()) {
        cout << "Speed Pile: " << endl;
        cout << pSpeedPile.top().name << endl;
    }
    else {
        cout << "Speed Pile: Empty" << endl;
    }
    cout << endl;
    
    if (!pBattlePile.empty()) {
        cout << "Battle Pile: " << endl;
        cout << pBattlePile.top().name << endl;
    }
    else {
        cout << "Battle Pile: Empty" << endl;
    }
    cout << endl;
    
    queue<Card> temp = pDistancePile; // copy distance pile to temp stack
    if (!temp.empty()) {
        cout << "Distance Pile: " << endl;
        while (!temp.empty()) {
            cout << temp.front().val << "mi, ";
            temp.pop();
        }
        cout << countMiles() << " MILES TOTAL." << endl;
    }
    else {
        cout << "Distance Pile: Empty" << endl;
    }
    cout << endl;

    cout << endl << "Hand: " << endl;
    printHand();
}

bool Player::validateHazardRemedyRelationship(int pChoice) {
    auto it = pHand.begin();
    advance(it, pChoice - 1); // move iterator to the selected card

    if (it->name == "REMEDY: GAS" && pBattlePile.top().name == "HAZARD: OUT OF GAS") {
        return true;
    }
    else if (it->name == "REMEDY: SPARE TIRE" && pBattlePile.top().name == "HAZARD: FLAT TIRE") {
        return true;
    }
    else if (it->name == "REMEDY: REPAIRS" && pBattlePile.top().name == "HAZARD: ACCIDENT") {
        return true;
    }
    else if (it->name == "REMEDY: END OF LIMIT" && (pSpeedPile.top().name == "HAZARD: SPEED LIMIT 50mph" || pSpeedPile.top().name == "HAZARD: SPEED LIMIT 25mph")) {
        return true;
    }
    else if ((it->name == "REMEDY: ROLL!" && pBattlePile.top().name == "HAZARD: STOP!") || (it->name == "REMEDY: ROLL!" && pBattlePile.top().val == 1)) {
        return true;
    }
    else {
        return false;

    }
}

bool Player::getSafetyCard(string cardName) {
    if (!pSafetyArea.empty()) {
        for (Card card : pSafetyArea) {
            if (card.name == cardName) {
                return true;
            }
        }
    }

    return false;
}

bool Player::checkSafety(Player* targetPlayer, string hazardCardName) {
    if (!pSafetyArea.empty()) {
        for (Card card : targetPlayer->pSafetyArea) {
            if (card.name == "SAFETY: EXTRA TANK" && hazardCardName == "HAZARD: OUT OF GAS") {
                return true;
            }
            else if (card.name == "SAFETY: PUNCTURE PROOF" && hazardCardName == "HAZARD: FLAT TIRE") {
                return true;
            }
            else if (card.name == "SAFETY: DRIVING ACE" && hazardCardName == "HAZARD: ACCIDENT") {
                return true;
            }
            else if (card.name == "SAFETY: RIGHT OF WAY" && (hazardCardName == "HAZARD: SPEED LIMIT 50mph" || hazardCardName == "HAZARD: SPEED LIMIT 25mph" || hazardCardName == "HAZARD: STOP!")) {
                return true;
            }
        }
    }

    return false;
    
}

bool Player::getBattleCard(string cardName) {
    if (!pBattlePile.empty()) {
        if (pBattlePile.top().name == cardName) {
            return true;
        }
    }

    return false;
}

int Player::getSpeedLimit() {
    if (pSpeedPile.empty()) {
        return 200;
    }
    else {
        return speedLimit;
    }
}

void Player::setSpeedLimit(int l) {
    speedLimit = l; // set the speed limit

    if (l == 200) {
        cout << "Speed limit reset to default." << endl;
    }
    else {
        cout << "Opponents speed limit set to: " << speedLimit << "mph" << endl;
    }
}

int Player::countMiles() {
    int playerDistanceTotal = 0;

    // Calculate the total distance from the player's distance pile
    queue<Card> temp = pDistancePile;
    while (!temp.empty()) {
        playerDistanceTotal += temp.front().val; // add the value of the card to the total
        temp.pop();
    }

    return playerDistanceTotal;
}