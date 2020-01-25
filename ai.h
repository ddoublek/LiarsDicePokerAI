//ai.h

/*
	0 : Round Did not Happen
	1 : Obvious mistake -- doubt even though had sufficient amount
	2 : 
	3 :
	4 : Chose Lower 1 or 2 Numbers
	5 :
	6 : Followed Probability Amount
	7 : 
	8 : 
	9 : Called +2 Over own dices

*/



#ifndef _AI_H_
#define _AI_H_

#include "dice.h"
#include <vector>
#include <cmath>
#include <algorithm>

class AI : public Player {
public:
	AI() : Player() {name = "DEFAULT_AI", noData = false, playerState = -1;}
	bool isAI() const {return true;}

	//Making this an int for debugging sakes
	int aiMove(int numDie, int dieNum);
	void readHistory();
	void gameStart(bool playerStart, int playerDice);
	void takeData(std::vector<int> playerDice);
	void firstTime();
	bool expectLimitPass();
	std::pair<int ,int> getCurrGuess(){return currGuess;}

private:
	double oddCalculation(int oppDice, int playerDice);
	double bluffCalculation();
	double bluffEquity();
	double moveEquity();
	double playerTendency();
	//most likely won't be using it yet.
	double worldTendency(); 
	bool sufficientData();

	std::vector<std::vector< std::vector< int > > > moveHistory;
	std::vector<std::vector< std::vector< double > > > winRates;
	//std::vector<int> values; -- For reference
	std::vector<double> expectValues;
	std::vector<double> personalBias; //When AI is making guess
	//std::vector<bool> limits;
	//std::
	int oppMove; //This helps track the difference in behavior
	bool playerFirst;
	int playerState;
	int laterState; //From 3 to less dice usually will only have 1 of the number
	int winState; //Player POV
	int loseState;
	bool noData;
	int gameState;
	int opponDice;
	bool firstCall;
	bool strongHand;
	int situation;

	std::pair < int, int > currGuess; 
	std::vector<std::pair<int,int> > allGuesses;
	std::vector<std::pair<int,int> > playerGuesses;
};

void AI::takeData(std::vector<int> playerDice){
	int amountFirst = playerGuesses[0].first;
	if(amountFirst > playerDice[playerGuesses[0].second] + 1) moveHistory[0][situation-1][0]++;
	if(amountFirst == playerDice[playerGuesses[0].second] + 1) moveHistory[0][situation-1][1]++;
	if(amountFirst < playerDice[playerGuesses[0].second] + 1) moveHistory[0][situation-1][2]++;

	if(playerGuesses.size() == 1) moveHistory[1][situation-1][0]++;
	if(playerGuesses.size() > 3) moveHistory[1][situation-1][2]++;
	else moveHistory[1][situation][1]++;
}	

void AI::firstTime(){
	moveHistory = std::vector<std::vector<std::vector<int>>>(4, std::vector<std::vector<int>>(4, std::vector<int>(4,0)));
	winRates = std::vector<std::vector<std::vector<double>>>(4, std::vector<std::vector<double>>(4, std::vector<double>(4,0)));
	gameState = laterState = winState = loseState = -1;
	playerState = 0;
	noData = true;
}

void AI::gameStart(bool playerStart, int playerDice){
	strongHand = false;
	allGuesses.clear();
	expectValues.assign(7, (double)playerDice/6);
	personalBias.assign(7, 0);
	//expectValues.assign(7, false);
	playerFirst = playerStart;
	currGuess.first = currGuess.second = 0;
	opponDice = playerDice;
	firstCall = true;
}

void AI::readHistory(){
	for(unsigned int i = 0; i < moveHistory.size(); i++){
		for(unsigned int j = 0; j < moveHistory[i].size(); j++){
			for(unsigned int k = 0; k < moveHistory[i][j].size(); k++){
				std::cout << moveHistory[i][j][k] << " ";	
			}
		std::cout<<std::endl;
		}
	}
	std::cout<< std::endl;
}


bool AI::sufficientData(){
	int allSum = 0;
	for(int i = 1; i <=6; i++){
		allSum += (int)expectValues[i];
	}
	if(allSum >= 4) return true;
	return false;
}

int AI::aiMove(int numDie, int dieNum){

	

	

	//int situation;

	//Situational Analysis
	if(opponDice >= diceLeft() + 2){
		situation = 1;
	} else if(diceLeft() >= opponDice + 2){
		situation = 2;
	} else if(opponDice <= 3){
		situation = 3;
	} else situation = 4;

	int biasChange;
	int preference;
	int amountPreference = -1;

	if(!noData){
		
		for(int i = 0; i <3; i++){
			if(moveHistory[0][situation - 1][i] > amountPreference){
				amountPreference = moveHistory[0][situation - 1][i];
				preference = i;
			}
		}
		if(amountPreference == 0);
		else{
			if(preference == 0) biasChange = -2;
			if(preference == 1) biasChange = -1;
			if(preference == 2) biasChange = 0;
		}
	} else noData = false; //Easier to do it this way

	int biasChoose = rand() % (amountPreference + 2);




	int biasModifier;
	if(biasChoose < 2){
		//Bias Modifier Determination
		if(situation == 1) biasModifier = winState;
		else if(situation == 2) biasModifier = loseState;
		else if(situation == 3) biasModifier = laterState;
		else if(situation == 4) biasModifier = playerState;
		else std::cout << "ERROR" << std::endl;
	} else{
		biasModifier = biasChange;
	}
	


	int tempAdd = numDie + biasModifier - personalBias[dieNum];
	//std::cout << tempAdd << " " << dieNum << " "  << personalBias[dieNum] << std::endl;

	// for(int i = (int)allGuesses.size() -1; i >=0; i--){
	// 	if(allGuesses[i].second == dieNum){
	// 		tempAdd -= allGuesses[i].first;
	// 		break;
	// 	}
	// }
	if(dieNum != 0 && numDie != 0)allGuesses.push_back(std::make_pair(numDie, dieNum));
	if(dieNum != 0 && numDie != 0)playerGuesses.push_back(std::make_pair(numDie, dieNum));

	// tempAdd
	if(tempAdd <= 0) tempAdd = 0;

	if(expectValues[dieNum] < 1) expectValues[dieNum] = 0; //Take out decimal Place
	expectValues[dieNum] = std::max(expectValues[dieNum], (double)tempAdd);

	for(int i = 1; i <= 6; i++){
		std::cout << "i = " << i << ": " << expectValues[i] << " ";
	}
	std::cout<<std::endl;

	if(expectLimitPass()) {
		//std::cout << "LIMIT PASS DOUBT" << std::endl;
		return 1; //Player must have been lying at once point	
	}
	//Probability wise these don't happen
	if((opponDice == 5 && expectValues[dieNum] > 3) 
		|| ((opponDice == 4 || opponDice == 3) && expectValues[dieNum] > 2) 
		|| (opponDice == 2 && expectValues[dieNum] == 2)){
		//std::cout << "PROBABILITY PASS DOUBT" << std::endl;
		return 1; //Probability wise won't have 4 of same number	
	} 
	if(expectValues[dieNum] + values[dieNum] < numDie  && (! (situation ==3 && dieNum < 5))){// &&  ((values[dieNum] == 0 && biasModifier != 0))
		//std::cout << "MAX DICE OVERFLOW DOUBT" << std::endl;
		if(firstCall && numDie <= 2){
			//Nothing :D
		}
		else return 1; //Even if add computer amount probably won't have that much dice	
	} 

	int allSum = 0;
	for(int i = 1; i <=6; i++){
		allSum += (int)expectValues[i];
	}
	allSum = opponDice - allSum;
	if(allSum < 0){
		allSum = 0;
		std::cout << "THIS SHOULDN'T HAPPEN NORMALLY" << std::endl;
	}
	for(int i = 1; i <=6; i++){
		if(expectValues[i] < 1){
			expectValues[i] = (double) allSum / opponDice;
		}
	}

	int bluffNum, bluffValue;
	bluffValue = -999;
	int matchAmount;
	int expectedAmount, tempVal, maxVal, currVal;
	double expectedValue;
	expectedAmount = numDie;
	expectedValue = -999; //To make sure doesn't conflict
	int maxDiceNumber = -1;
	int maxNumberDice = -1;

	for(int i = 1; i <= 6; i++){
		if(dieNum + i > 6){
			tempVal = dieNum + i - 6;
			matchAmount = numDie +1;	
		} 
		else{
			tempVal = dieNum + i;
			matchAmount = numDie;	
		} 

		if(matchAmount == numDie && bluffValue < (expectValues[tempVal] + values[tempVal]) - numDie){
			bluffValue = (double)expectValues[tempVal] + (double)values[tempVal] - (double)numDie;
			bluffNum = tempVal;
		}

		if(expectedValue != std::max(expectedValue, ((double)expectValues[tempVal] + values[tempVal]) - numDie)){
			expectedValue = (double)expectValues[tempVal] + (double)values[tempVal] - (double)numDie;
			maxDiceNumber = tempVal;
			maxNumberDice = matchAmount;
			if(values[tempVal] >= 3) strongHand = true;
		}
	}

	int side = rand() % (allGuesses.size() + 4);
	

	if(strongHand) side = side*2;


	if(expectedValue <= -1.5){
		//std::cout << "UNLIKELY CONTINUE DOUBT" << std::endl;
		return 1; // Might change but usually these cases are unreasonable and only chance is to Doubt	
	} 
	else{
		// if(expectedValue > 1){
		// 	currGuess.first = maxNumberDice + 1;
		// } else
		if(maxNumberDice == 0) maxNumberDice = 1;
		if(side < 3){
			if(numDie == 0) currGuess.first = 1;
			else currGuess.first = numDie;
			currGuess.second = bluffNum;
		} else{

			currGuess.first = maxNumberDice;
			if(numDie == 0) currGuess.first += 1; //For stronger guessing
			currGuess.second = maxDiceNumber;

		}

		
		allGuesses.push_back(currGuess);
		int biasAdd = currGuess.first - values[currGuess.second];
		//if(biasAdd <= 0) biasAdd = 0;
		personalBias[currGuess.second] += biasAdd;
		//std::cout << currGuess.first << " " << currGuess.second << " " <<  personalBias[currGuess.second] << std::endl;
		return 0;
	}

	if(maxDiceNumber == -1){
		std::cout << "MAX DICE NUMBER PROBLEM" << std::endl;
		exit(1);	
	} 


	return 0;
}

bool AI::expectLimitPass(){
	int allSum = 0;
	for(int i = 1; i <=6; i++){
		allSum += (int)expectValues[i];
	}
	if(allSum > 5) return true;
	return false;
}


#endif