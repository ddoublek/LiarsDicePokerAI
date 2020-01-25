//player.h
//Possibly make AI class be inhereted from class - might choose later on for now go simple

#ifndef _PLAYER_H_
#define _PLAYER_H_

#include "dice.h"
#include <vector>
#include <string>
#include <utility> //For Pairs

int NUM_DICE = 5;
int STARTING_CHIPS = 20; // At most 10 rounds, doesn't force betting
int MAX_BET = 10; //So the game isn't completely about luck

class Player {
public:
	Player();
	virtual ~Player() {}; //Needed for polymorphysm functions

	//Acessor
	void seeHand() const;
	void hideHand() const;
	bool isAI() const {return false;}
	std::string getName(){return name;}
	int diceLeft() {return hand.size();}
	void showCount() const;
	bool hasDice() const {return hand.size() != 0;}
	//std::pair < int, int > getGuess() const {return currGuess;}
	int getSpecificNumber(int numFind){return values[numFind];}
	int chipsLeft() {return chips;}
	std::vector<int> getValues(){return values;}

	//Modifier
	void loseDice(){hand.pop_back();}
	void setName(std::string newName) {name = newName;}
	void rollAll();
	void rigRoll(unsigned int a = 0, unsigned int b = 0, unsigned int c = 0, unsigned int d = 0, unsigned int e = 0);
	//void makeGuess(int numDice, int rollNum) {currGuess.first = numDice, currGuess.second = rollNum;}
	//Also a dangerous function to give to this class
	bool chipChange(int change);
	void resetDice();

protected:
	std::string name; //Later on will need this when more than 1 player
	std::vector<Dice> hand;
	std::vector<int> values;
	

	//Let's Go Crazy With Gambling!
	int chips;
};

Player::Player(){

	name = "DEFAULT_PLAYER";
	values.assign(7, 0);

	for(int i = 0; i < NUM_DICE; i++){
		Dice temp;
		hand.push_back(temp);
	}

	chips = STARTING_CHIPS;
}


void Player::seeHand() const{

	std::cout << name << " hand is " << std::endl;
	for(unsigned int i = 0; i < hand.size(); i++){
		std::cout << "--- ";
	}
	std::cout << std::endl;
	for(unsigned int i = 0; i < hand.size(); i++){
		std::cout << "|" << hand[i].showNumber() << "| ";
	}
	std::cout << std::endl;
	for(unsigned int i = 0; i < hand.size(); i++){
		std::cout << "--- ";
	}
	std::cout << std::endl;
}

void Player::hideHand() const{

	std::cout << name << " hand is " << std::endl;
	for(unsigned int i = 0; i < hand.size(); i++){
		std::cout << "--- ";
	}
	std::cout << std::endl;
	for(unsigned int i = 0; i < hand.size(); i++){
		std::cout << "|?| ";
	}
	std::cout << std::endl;
	for(unsigned int i = 0; i < hand.size(); i++){
		std::cout << "--- ";
	}
	std::cout << std::endl;
}

void Player::showCount() const{

	for(unsigned int i = 1; i <= 6; i++){
		std::cout << values[i] << std::endl;
	}
}

void Player::rollAll(){

	std::fill(values.begin(), values.end(), 0);
	for(unsigned int i = 0; i < hand.size(); i++){
		hand[i].rollDice();
		values[hand[i].showNumber()]++;
	}
}

bool Player::chipChange(int change){
	chips += change;
	if(change <= 0) return false;
	else return true;
}

void Player::rigRoll(unsigned int a, unsigned int b, unsigned int c, unsigned int d, unsigned int e){
	std::fill(values.begin(), values.end(), 0);
	
	if(a != 0){
		hand[0].fixedRoll(a);
		values[a]++;
	}

	if(b != 0){
		hand[1].fixedRoll(b);
		values[b]++;
	}

	if(c != 0){
		hand[2].fixedRoll(c);
		values[c]++;
	}

	if(d != 0){
		hand[3].fixedRoll(d);
		values[d]++;
	}

	if(e != 0){
		hand[4].fixedRoll(e);
		values[e]++;
	}
	
}

void Player::resetDice(){
	while(diceLeft() != NUM_DICE){
		Dice temp;
		hand.push_back(temp);
	}
}

#endif