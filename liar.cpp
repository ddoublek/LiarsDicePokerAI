//Liars Dice AI

/*
	Basic Rules:
	Every turn, the player must guess how much of a single dice there must be
	The next person must choose to either 
	1. make a higher bet with either a higher number or a higher amount of dice(in this case the number can decrease)
	2. Doubt the person right before them
	2-2. When the doubt is done all player reveal their dices
	2-3a. If there is at least as much dice of the number the previous person called, the person who called doubt loses a dice
	2-3b. If not enough, then the person doubted loses a dice
	3. This continuous until there is only 1 person with dice left

	Extra:
	1) 1 could possible be counted as any number wanted
	1-b) 1 could have special treatment in when being counted (ex) 1 only need half to call)


	AI:
	Basic is will believe in probability for odds of each dice and will make fair move
	Most likely will make it so start isn't too small -- make AI more aggressive winner
	Will keep track of what opponent(s) say and see guess range/bluff range
		-Also helps AI figure out what is a good guess for them too
		-Might differentiate depending on how many people are playing -- changes how people guess
	AI will always say at least 1 sentence
	AI will keep track of how opponent plays after hearing these sentences
	(Possible) also have players say a line and store their reactions

	Extra Psychology:
	At beginning might be able to choose what award shape is like (ex - stars, food etc) -- find out about personality
	Depending on actions try to group person into "personality type" and help infer how they play

	Liar liar pants on fire -- try to change direction of 'liar' by saying someone else is liar

	Simple Tendancy predictor
	Splits people in basic groups Aggr / Pass / Logic / Unknown
	Also will save data from players past decisions

	Tendency might weaken as dice amount decreases? -- less amount of choices
	How to read in data player did?

	1 3 3 4 6

	Player guess 2 of 6
	--Read as range + 1, Top Number, Not top amount
	--If bet is over 3 of expected have possible "aggressive" tag or actually have around amount

	AI 2 4 5 6 6
	--Basic assumption player will have 1 or 2 of a number
	--See basic probability of winning if raise call or doubt


	Weight of decision
	--Statistics, Overall Data, Past Games from Player 

	At most 9 rounds -- possibly 18 digits or split up

	AI will base on logical plays but as more data comes, will put more tendency on player style / overall players?
	--Will play the way that produces more wins

*/

//For run script -- Javascript program -display image in popup window
//Close window -- end program
//fork() -- take process split to 2 parallel (way of multithreading), exec() -- file pathway, take current proces , executes that instead
//fork when ready to display -- return 2 numbers if parent if child, if parent -- wait for child to finish, if child call exec
//call exec -- popup image --> end process --> back to 1 process

//#include <iostream> -- in dice.h
//#include "dice.h" -- Included in Player & AI files
//#include "player.h" -- Included in Game file
#include "game.h"
#include "ai.h"
#include <string>
#include <algorithm>

int main(){

	//For complete randomness    
  	std::srand(time(NULL)); 
  	

	//Later on will add this feature though beginning will just be 2 people
	//numPlayers should equal sum of all players
	//Default is 1 real player and 1 Smart AI
	unsigned int numPlayers = 2;
	unsigned int numRealPlayers = 1;
	unsigned int numSmartAI = 1;
	// unsigned int numNormalAI;

	// while(true){
	// 	std::cout << "Type in how many players you want: "<< std::endl;
	// }

	if(numPlayers != numRealPlayers + numSmartAI){
		std::cerr << "ERROR:TOTAL PLAYER NUMBERS DOES NOT MATCH" << std::endl;	
	} 
	
	std::pair<Player, AI> allPlay;
	std::string theirName;
	Player tempPlayer;
	AI tempAI;

	tempPlayer.setName("Player_" + std::to_string(1));
	tempAI.setName("AI_" + std::to_string(1));

	allPlay.first = tempPlayer;
	allPlay.second = tempAI;



	Game currGame(allPlay);

	// currGame.playerStats();

	std::string actionString; //, betString
	int actionTaken, numDie, dieNum, moveResult, aiMoveResult; //betAmount, betMin, betMax;
	// numDie = dieNum = -1;
	// betMin = 1;
	// betMax = MAX_BET;

	assert(MAX_BET == 10); //For now, this shouldn't stay like this forver


	//Initial Rolling (Edge case) --Might have to make Game Private?
	currGame.playerRolls();
	currGame.ai1.firstTime();
	currGame.ai1.gameStart(currGame.isPlayerTurn(), currGame.player1.diceLeft());

	//Actual playing of game. For now AI will be handled by player
	while(true){
		std::cout << std::endl; //Make things look more pretty :D
		currGame.playerStats();
		std::cout << "Current call is " << currGame.getHighest().first << " dice of " << currGame.getHighest().second << " by ";
		if(currGame.playerHighGuess()) std::cout << tempPlayer.getName();
		else std::cout << tempAI.getName();
		std::cout<< std::endl;
		
		std::cout << "CURRENT MOVE IS: ";

		if(currGame.isPlayerTurn()) {
			std::cout << tempPlayer.getName() << std::endl;
		}
		else{
			std::cout << tempAI.getName() << std::endl;	
		} 

		//This will only be used for debugging
		//currGame.cheatShow();
		currGame.playerShow();
		//Not going to be adding betting for this simple version

		// std::cout << "Please make betting action, either call/check(\"call\"), raise(\"raise\"), or fold(\"fold\"): ";
		// std::cin >> betString;

		// std::transform(betString.begin(), betString.end(), betString.begin(), ::tolower);

		// //NEED TO MAKE SURE THIS WORKS FOR BOTH AI AND PLAYER
		// if(betString == "call") currGame.setBet(betMin);
		// else if(betString == "raise"){
		// 	//betMin = currGame.betLimit();
		// 	while(true){
				
		// 		std::cout << "What is your bet (MAX:" << currGame.betLimit() << "): ";
		// 		std::cin >> betAmount;
		// 		if(betAmount <= betMin){
		// 			std::cout << "BET IS LOWER THAN CURRENT BET " << betMin << " TRY AGAIN" << std::endl;
		// 			continue;
		// 		}
		// 		if(betAmount > betMin){
		// 			std::cout << "BET IS HIGHER THAN BET LIMIT " << currGame.betLimit() << " TRY AGAIN" << std::endl;
		// 			continue;
		// 		}
		// 		currGame.setBet(betAmount);
		// 		break; //Hopefully this is a 1 loop break
		// 	}
		// 	//Change turn for alternating betting turns


		// 	continue;	
		// }
		// else if(betString == "fold") {
		// 	currGame.chipInteract(0, betMin); //Currently only Player, will obviously have to change this
		// 	currGame.playerRolls(); //Again, kind of scary to give this power to main
		// }
		// else{
		// 	std::cout << "INVALID BET RESPONSE TRY AGAIN" << std::endl;
		// 	continue; //This might cause a bit problem when the second continue fails and has to do from the start
		// }

		if(!currGame.isPlayerTurn()){

			aiMoveResult = currGame.ai1.aiMove(currGame.getHighest().first, currGame.getHighest().second);
			if(aiMoveResult == 1) currGame.ai1.takeData(currGame.player1.getValues());
			actionTaken = aiMoveResult +1;
			moveResult = currGame.makeMove(actionTaken, currGame.ai1.getCurrGuess().first, currGame.ai1.getCurrGuess().second); 
		} else{
			std::cout << "Please make a move, either MAKE A HIGHER GUESS(\"guess\"), DOUBT CURRENT GUESS(\"doubt\") or QUIT(\"quit\"): ";
			std::cin >> actionString;

			std::transform(actionString.begin(), actionString.end(), actionString.begin(), ::tolower);

			if(actionString == "guess") actionTaken = 1;
			else if(actionString == "doubt") actionTaken = 2;
			else if(actionString == "quit") break;
			else{
				//std::cout << actionString << std::endl;
				std::cout << "INVALID ACTION TRY AGAIN" << std::endl;
				continue;
			}

			if(actionTaken == -1) break; //For now this will be the not want to play

			if(actionTaken == 1){
				std::cout << "Please type in your guess in NUM_DICE(1-" << currGame.diceInPlay() << ") of DIE_NUMBER(1-6): ";
				std::cin >> numDie >> dieNum;
			} else if(actionTaken == 2){ //In this case Doubting so no guess needed
				numDie = dieNum = -1;
			} else{
				std::cout << "INVALID MOVE TRY AGAIN" << std::endl;
				continue; //If made invalid	
			} 
			std::cout << std::endl;
			moveResult = currGame.makeMove(actionTaken, numDie, dieNum);
		}


		

		
		//I think I should change this to switch case?
		if(moveResult == -1){
			currGame.ai1.gameStart(currGame.isPlayerTurn(), currGame.player1.diceLeft());
			continue;
		}
		if(moveResult == -2){
			std::cout << "NEED TO CHECK CODE PROBLEM" <<std::endl;
			exit(0);
		}
		if(moveResult == 2){
			std::cout << tempPlayer.getName() << " IS OUT OF DICE AND HAS LOST" << std::endl;
			std::cout << std::endl;
			std::cout << tempAI.getName() << " HAS WON" << std::endl;
		
			
		}
		if(moveResult == 3){
			std::cout << tempAI.getName() << " IS OUT OF DICE AND HAS LOST" << std::endl;
			std::cout << std::endl;
			std::cout << tempPlayer.getName() << " HAS WON" << std::endl;
		}

		if(moveResult == 2 || moveResult == 3){
			bool playAgain;

			while(true){
				std::cout << "Play Again(yes/no)?";
				std::cin >> actionString;

				std::transform(actionString.begin(), actionString.end(), actionString.begin(), ::tolower);
				if(actionString == "yes"){
					playAgain = true;
					break;	
				} 
				if(actionString == "false"){
					playAgain = false;
					break;	
				} 
				else continue;
			}
			if(playAgain == false) break;
			else{
				currGame.restartGame();
				continue;
			}
		}
	}



	return 1;
}