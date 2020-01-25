//game.h

#ifndef _GAME_H_
#define _GAME_H_

#include <vector>
#include <utility>
#include <cassert>
#include "player.h"
#include "ai.h"

class Game{
public:
	Game();
	Game(std::pair <Player, AI> starters);

	//Accessors
	void playerStats();
	std::pair < int, int > const getHighest(){return currHighest;}
	bool isPlayerTurn() {return playerTurn;}
	bool playerHighGuess(){return highGuess;}
	int diceInPlay(){return player1.diceLeft() + ai1.diceLeft();}
	int betLimit(){return std::min(MAX_BET, std::min(player1.chipsLeft(), ai1.chipsLeft()));}

	//Modifier
	void setBet(int givenBet){currBet = givenBet;}
	//Questionable whether this should actually be public
	bool chipInteract(int action, int finBet);

	//void addPlayer(Player newPlayer); -- Current version will only take 2 players
	
	//Should this be public or private...
	//Or rather should this even be a function?
	//Possibly will use if player leaves game?
	//void removePlayer(Player gonePlayer); 

	//Ignoring bad coding practice for now making these public
	//std::pair <Player, AI> contestants;
	Player player1;
	AI ai1;

	//MISC Functions

	int makeMove(int move, int numDice, int dieNum);
	void playerRolls();
	//Might have 3 versions, cheat, Player 1, Player2 / AI
	void cheatShow();
	void playerShow();
	void restartGame();

private:
	
	
	//First = Number of Dice  Second = Number ON Die (1~6)
	std::pair < int, int > currHighest;
	std::vector<int> totalAmounts;
	int currBet; //Making this a member variable makes things simpler?

	bool playerTurn;
	bool highGuess; // In the POV of player
	bool betTurn; //Betting should not change the start order -- that's part of the psychology!

	int removeEliminated();
	int checkDoubt();
	bool takeGuess(std::pair < int, int > givenGuess);
	
};

Game::Game(std::pair <Player, AI> starters){
	player1 = starters.first;
	ai1 = starters.second;
	currHighest.first = 0;
	currHighest.second = 0;
	playerTurn = true;
	highGuess = false;
}

void Game::playerStats(){

	std::cout << player1.getName() << " has " << player1.diceLeft() << " dice (HUMAN)" << std::endl;
	if(player1.isAI()) std::cerr << "PLAYER SHOULD NOT BE AI" << std::endl;
	std::cout << ai1.getName() << " has " << ai1.diceLeft() << " dice (AI)" << std::endl;
	if(!ai1.isAI()) std::cerr << "AI SHOULD NOT BE PLAYER" << std::endl;


}

// void Game::addPlayer(Player newPlayer){
// 	contestants.push_back(newPlayer);
// }

// void Game::removePlayer(Player gonePlayer){

// }

int Game::removeEliminated(){

	if(!player1.hasDice()){// || player1.chipsLeft() < 0
		std::cout << player1.getName() << " is eliminated!" << std::endl;
		return 0;
	}	
	if(!ai1.hasDice()){// || ai1.chipsLeft() < 0
		std::cout << ai1.getName() << " is eliminated!" << std::endl;
		return 1;
	}
	return -1;

}

bool Game::takeGuess(std::pair < int, int > givenGuess){
	if(givenGuess.first < currHighest.first) return false;
	if(givenGuess.first > currHighest.first || givenGuess.second > currHighest.second){
		
		if(playerTurn) std::cout << player1.getName();
		else std::cout << ai1.getName();

		std::cout << " has made the guess " << givenGuess.first <<  " amount(s) of " << givenGuess.second << std::endl; 

		currHighest = givenGuess;
		playerTurn = !playerTurn;
		highGuess = !highGuess;

		return true;	
	} 
	return false;
}

bool Game::chipInteract(int action, int finBet){
	if(finBet < 1 || finBet > MAX_BET) std::cout << "ERROR IN CURRENT BET " << finBet << std::endl;
	assert(-finBet < 0); //This is an important part of the program
	if(action == 0){
		player1.chipChange(-finBet);
		ai1.chipChange(finBet);
	}
	if(action == 1){
		player1.chipChange(finBet);
		ai1.chipChange(-finBet);
	}
	return true; //Might change meaning later on but elimination will be detected by diff function
}

int Game::checkDoubt(){
	cheatShow();
	std::cout << std::endl;
	int actualNumAmount = 0;
	std::cout << player1.getName() << " HAS " << player1.getSpecificNumber(currHighest.second) << " AMOUNT OF " << currHighest.second << std::endl;
	actualNumAmount += player1.getSpecificNumber(currHighest.second);
	std::cout << ai1.getName() << " HAS " << ai1.getSpecificNumber(currHighest.second) << " AMOUNT OF " << currHighest.second << std::endl;
	actualNumAmount += ai1.getSpecificNumber(currHighest.second);

	// std::cout << std::endl;

	std::cout << "CURRENT GUESS IS " << currHighest.first << " AMOUNTS OF " << currHighest.second << std::endl;
	std::cout << "ACTUAL AMOUNT IS " << actualNumAmount << " AMOUNTS OF " << currHighest.second << std::endl;

	std::cout << std::endl;

	bool isFake;

	if(actualNumAmount < currHighest.first){
		std::cout << "DOUBT HAS SUCCEEDED" << std::endl;
		isFake = true;
	} else{
		std::cout << "DOUBT HAS FAILED" << std::endl;
		isFake = false;
	}

	//If Player is caught or player fails to catch
	//Whoever loses dice will always make first move -- unless I want to change the rules ;D
	if((highGuess && isFake) || (!highGuess && !isFake)){
		std::cout << player1.getName() << " LOSES 1 DICE" <<std::endl;
		player1.loseDice();
		playerTurn = true;
		highGuess = false;
		currHighest.first = 0;
		currHighest.second = 0;
		//chipInteract(0, currBet);
	} else{
		std::cout << ai1.getName() << " LOSES 1 DICE" <<std::endl;
		ai1.loseDice();
		playerTurn = false;
		highGuess = true;
		currHighest.first = 0;
		currHighest.second = 0;
		//chipInteract(1, currBet);
	}

	std::cout << std::endl;

	//playerStats();
	int returnState = removeEliminated();
	//std::cout << "---ReturnSate is " << returnState << std::endl;
	return returnState;

}

//0 if highest Guess will change
//1 if Doubting player
//-1 if Invalid move
int Game::makeMove(int move, int numDice, int dieNum){
	int firstOld, secondOld;
	if(move > 2){ //Hopefully this shouldn't happen
		std::cout << "INVALID MOVE" << std::endl;
		return -1; 	
	} 

	if(move == 1){ //This is case where the player makes a higher guess
		if(numDice < 1 || numDice > diceInPlay() || dieNum <1 || dieNum >6){
			std::cout << "INVALID GUESS" << std::endl;
			return -1;
		}
		if(numDice < currHighest.first || (numDice == currHighest.first && dieNum <= currHighest.second)){
			std::cout << "NOT HIGHER GUESS" <<std::endl;
			return -1;
		}

		firstOld = currHighest.first;
		secondOld = currHighest.second;

		takeGuess(std::make_pair(numDice, dieNum));

		if(firstOld == currHighest.first && secondOld == currHighest.second){
			std::cout << "GUESS MAKING UPDATE PROBLEM" <<std::endl;
			return -2;
		}
		return 0;
	} 

	else if(move == 2){ //DOUBT!
		if(currHighest.first == 0){
			std::cout << "CANNOT DOUBT YET" <<std::endl;
			return -1;
		}

		firstOld = diceInPlay();

		int eliminate = checkDoubt();
		//std::cout << "---- Elimiate is currently " << eliminate << std::endl; 

		if(firstOld == diceInPlay()){
			std::cout << "DOUBT DICE REDUCE PROBLEM" <<std::endl;
			return -2;
		}

		if(eliminate == -1){
			playerRolls();
			return 1;	
		} 
		else if(eliminate == 0) return 2;
		else if(eliminate == 1) return 3; 
		else{
			std::cout << "ELIMINATION STEP PROBLEM" <<std::endl;
			return -2;
		}
	}

	else{ //Hopefully this shouldn't happen
		std::cout << "INVALID MOVE" << std::endl;
		return -1; 
	}
}

void Game::playerRolls(){
	player1.rollAll();
	ai1.rollAll();
	// player1.rigRoll(1, 4, 1, 2, 1);
	// ai1.rigRoll(6, 1, 2, 1, 1);
}

void Game::cheatShow(){
	player1.seeHand();
	ai1.seeHand();
}

void Game::playerShow(){
	player1.seeHand();
	ai1.hideHand();
}

void Game::restartGame(){
	player1.resetDice();
	ai1.resetDice();
}

#endif