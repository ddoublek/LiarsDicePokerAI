//dice.h
//If wnat this class to be used in more than one project might want to make everything templated
#ifndef _DICE_H_
#define _DICE_H_

#include <stdlib.h> //Used for srand and rand
#include <time.h> // Used for the seeding of the randomization
#include <iostream>

class Dice {
public:
	Dice();
	unsigned int showNumber() const {return side;}
	void rollDice();
	void fixedRoll(unsigned int i) {side = i;}
private:
	unsigned int side;
};

Dice::Dice(){
	side = 0;
}

void Dice::rollDice(){
	side = rand() % 6 + 1;
	if(side < 1 || side > 6){
		std::cerr << "ERROR NUMBER IS" << side << std::endl;	
	}
}

#endif