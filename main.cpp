//Project X
//Copyright 2021, Park Family Software Laboratory (ParkLab)


#include <iostream>
#include <ctype.h>
#include <fstream>

#ifdef _WIN32
#include <conio.h>
#include <windows.h>
#elif defined(__unix__)
#include <iostream>
#include <stdio.h>
#define SetConsoleTextAttribute(X,ATTR)
#define GetStdHandle(x)
#define STD_OUTPUT_HANDLE
#define _getch() getc(stdin)
#else
#error "Unknown build target!"
#endif


#include "MasterBoard.hpp"
#include "Tile.hpp"
#include "Minion.hpp"

//Global variables need to be moved.

	
std::string eventText			= "";

int finishLoadingBoard(MasterBoard * LoadBoard) 
{
	
	
	for (int x = 0; x < BOARD_WIDTH; x++)
	{
		for (int y = 0; y < BOARD_HEIGHT; y++)
		{
			switch (LoadBoard->Board[x][y].symbol)
			{
			case('.'):
			{
				LoadBoard->Board[x][y].description = "Clear terrain.";
				LoadBoard->Board[x][y].defenseFactor = 1.1;
				break;
			}
			case('n'):
			{
				LoadBoard->Board[x][y].description = "Housing.";
				LoadBoard->Board[x][y].defenseFactor = 1.3;
				break;
			}
			case('h'):
			{
				LoadBoard->Board[x][y].description = "Factory.";
				LoadBoard->Board[x][y].defenseFactor = 1.3;
				break;
			}			
			case('H'):
			{
				LoadBoard->Board[x][y].description = "Headquarters.";
				LoadBoard->Board[x][y].defenseFactor = 1.4;
				break;
			}
			case('='):
			{
				LoadBoard->Board[x][y].description = "Road.";
				LoadBoard->Board[x][y].defenseFactor = 1.0;
				break;
			}
			case('^'):
			{
				LoadBoard->Board[x][y].description = "Hill.";
				LoadBoard->Board[x][y].defenseFactor = 1.1;
				break;
			}
			case('+'):
			{
				LoadBoard->Board[x][y].description = "Forest.";
				LoadBoard->Board[x][y].defenseFactor = 1.2;
				break;
			}
			}
			//Other terrain types go here

		}
	}
	return 0;
}

int printStatus(MasterBoard* boardToPrint) 
{
	tile currentTile = boardToPrint->Board[boardToPrint->cursor.getX()][boardToPrint->cursor.getY()];

	std::cout << currentTile.description << std::endl;

	if(currentTile.hasMinionOnTop == true)
	{
		Minion* currentMinion = currentTile.minionOnTop;
		std::cout << "Player " << currentMinion->team
			<< "'s " << currentMinion->description
			<< ": " << int(currentMinion->health) <<
			" Health Left." << std::endl;
	
		if (currentMinion->status == gaveupmovehasntfired) 
		{
			std::cout << "Holding position." << std::endl;
			std::cout << "Ready to attack." << std::endl;
		}

		if (currentMinion->status == hasmovedhasntfired)
		{
			std::cout << "Has already moved this turn." << std::endl;
			if (currentMinion->rangeType == rangedFire) 
			{
				std::cout<<"Cannot attack after moving." << std::endl;
			}
			if (currentMinion->rangeType == directFire) 
			{ 
				std::cout << "Ready to attack." << std::endl; 
			}
		}
		if (currentMinion->status == hasfired)
		{
			std::cout << "Has already moved this turn." << std::endl;
			std::cout << "Has attacked this turn." << std::endl;
		}
		if (currentMinion->status == hasntmovedorfired)
		{
			std::cout << "Ready to move." << std::endl;
			std::cout << "Ready to attack." << std::endl;
		}
	}
	else
	{ 
		std::cout << std::endl << std::endl << std::endl; 
	}
	
	//Print current turn.
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_GREEN);
	std::cout << "Player " << boardToPrint->playerFlag << "'s Turn." << std::endl;

	//Debug purposes- print cursor location coordinates and window coordinates.
	std::cout << eventText << "Cursor Location: " << boardToPrint->cursor.getX() << boardToPrint->cursor.getY() << std::endl;
	std::cout << boardToPrint->windowLocation << std::endl;
	eventText = "";

	return 0;
}

int printScreen(MasterBoard * boardToPrint) 
{
	//These are just iterators.
	int i, j;
	

	//windowLocation is a single scalar representing x and y.
	//We do some basic math to break it into the two values for the function.
	//Need to convert windowLocation into a better two part variable.
	int windowY = boardToPrint->windowLocation / BOARD_WIDTH;
	int windowX = boardToPrint->windowLocation % BOARD_WIDTH;

	//Go through the whole "board", staying within the bounds of window's x and y coordinates.
	for (i = windowY; i < (windowY + WINDOW_HEIGHT); i++)
	{
		for (j = windowX; j < (windowX + WINDOW_WIDTH); j++)
		{
			//Print whomever has priority. cursor first, then unit, then terrain.

			//If there is a cursor there, it takes priority for printing.
			if ( i == boardToPrint->cursor.getY() && j == boardToPrint->cursor.getX() ) // * BOARD_WIDTH + j == boardToPrint->cursor.Location)										
			{	
				SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_GREEN);					
				std::cout << '*';
			}	
			else
				//Is there a minion there?
				if (boardToPrint->Board[j][i].hasMinionOnTop == true)						
			{	
				//Determine team and then set the color.
				switch (boardToPrint->Board[j][i].minionOnTop->team)
				{
				case(0):
					SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_GREEN);
					break;
				case(1):
					SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_RED);
					break;
				case(2):
					SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_BLUE);
					break;
				}
				//Print out the minion.
				std::cout << boardToPrint->Board[j][i].minionOnTop->type;
			}
			else if (boardToPrint->Board[j][i].withinRange == true)
			{	
				//If no minion, see if it's "in range" is set
				SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_GREEN);					
				//And if so print the symbol for "in range" which is ':'
				std::cout << ':';																				
			}
			else 
			{
				//Otherwise put out the terrain for that square.
				SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_GREEN);					
				std::cout << boardToPrint->Board[j][i].symbol;
			}
		}
		std::cout << std::endl;
	}

	printStatus(boardToPrint);

	//Buffer the screen to clear the old map window.
	for (i = 0; i < 10; i++)		
		std::cout << std::endl;

	return 0;
}

int scenarioSave(std::string saveGameName, MasterBoard* boardToPrint)
{
	saveGameName += ".txt";
	std::ofstream saveGame(saveGameName);

	//First save the map size:
	saveGame << BOARD_WIDTH << "_" << BOARD_HEIGHT << std::endl;

	//Then save player data:
	saveGame << boardToPrint->playerFlag << std::endl;

	//Terrain save:
	//Iterate through board and save the exact symbol. This should be easy.
	for (int y = 0; y < BOARD_HEIGHT; y++)
	{
		for (int x = 0; x < BOARD_WIDTH; x++)
		{
			saveGame << boardToPrint->Board[x][y].symbol;
		}
		saveGame << std::endl;
	}

	//Note the number of minions:
	saveGame << boardToPrint->totalNumberOfMinions << std::endl;

	//Go through entire minionRoster and save each value associated with each minion, one line per minion.
	//Need to ensure correctness.
	//FIX FIX FIX - need to not just check for initial NULL, there may be minions beyond that first NULL if a guy was killed and not replaced on
	//The array.
	for (int i = 0; i < GLOBALSUPPLYCAP; i++)
	{
		if (boardToPrint->minionRoster[i] != NULL)
		{
			saveGame << boardToPrint->minionRoster[i]->type << " "
				<< boardToPrint->minionRoster[i]->locationX << " "
				<< boardToPrint->minionRoster[i]->locationY << " "
				<< boardToPrint->minionRoster[i]->team << " "
				<< boardToPrint->minionRoster[i]->seniority << " "
				<< int(boardToPrint->minionRoster[i]->rangeType) << " "
				<< int(boardToPrint->minionRoster[i]->status) << " "
				<< int(boardToPrint->minionRoster[i]->health) << std::endl;
		}
	}
	saveGame.close();
	return 1;
}

//Needs serious refactoring. Chains of if statements that can probably be handled differently, plus multiple references to variables.
//Handle with a single local variable doing one reference.
int userInput(char * Input, MasterBoard * boardToInput)		
{
		*Input = _getch();
	
		if (*Input == 'a' || *Input == 'd' || *Input == 's' || *Input == 'w')
		{
			boardToInput->cursor.move(Input);
		}

	
	//Select minion command
		if (*Input == 't')
		{
			if (boardToInput->cursor.selectMinionFlag == false 
				&& boardToInput->Board[boardToInput->cursor.getX()][boardToInput->cursor.getY()].hasMinionOnTop == true)
			{
				boardToInput->selectMinion(boardToInput->cursor.getX(), boardToInput->cursor.getY());
			}
			else
				if (boardToInput->cursor.selectMinionFlag == true)
				{
					boardToInput->deselectMinion();
				}
		}

	//Move minion command
	//If not on top, then move the unit.
	if (*Input == 'm' && boardToInput->cursor.selectMinionFlag == true 
		&& boardToInput->Board[boardToInput->cursor.getX()][boardToInput->cursor.getY()].hasMinionOnTop == false
		&& boardToInput->cursor.selectMinionPointer->status == hasntmovedorfired)
	{
		if (boardToInput->moveMinion(boardToInput->cursor.getX(), boardToInput->cursor.getY()) == 0)
		{	//Change status appropriately for successful movement.
			boardToInput->cursor.selectMinionPointer->status = hasmovedhasntfired;
			boardToInput->deselectMinion();
		}
	}

	//If already on top, just "move" by not moving. This allows the user to fire without actually changing position.
	if(*Input == 'm' && boardToInput->cursor.selectMinionFlag == true 
		&& boardToInput->cursor.selectMinionPointer->locationX == boardToInput->cursor.getX() 
		&& boardToInput->cursor.selectMinionPointer->locationY == boardToInput->cursor.getY()
		&& boardToInput->cursor.selectMinionPointer->status == hasntmovedorfired)
	{
		boardToInput->cursor.selectMinionPointer->status = gaveupmovehasntfired;
		boardToInput->deselectMinion();
	}

	//Attack command. Pre-reqs: must be in range, must be enemy team and not yours.
	if (*Input == 'r' && boardToInput->cursor.selectMinionFlag == true)
		if(boardToInput->Board[boardToInput->cursor.getX()][boardToInput->cursor.getY()].hasMinionOnTop == true)
			if((boardToInput->cursor.getX() != boardToInput->cursor.selectMinionPointer->locationX) || (boardToInput->cursor.getY() != boardToInput->cursor.selectMinionPointer->locationY))//Can attack if minion is selected
				if(boardToInput->Board[boardToInput->cursor.getX()][boardToInput->cursor.getY()].minionOnTop->team != boardToInput->cursor.selectMinionPointer->team)//And it's enemy team's.
					if(boardToInput->Board[boardToInput->cursor.getX()][boardToInput->cursor.getY()].withinRange == true)	//In range
	{	
		//This is the actual attack portion. Return of 0 indicates successful attack.
		bool attackSuccess = boardToInput->attackMinion(boardToInput->cursor.getX(), boardToInput->cursor.getY());
		if (attackSuccess == 0)
		{
			boardToInput->deselectMinion();
		}
	}

	//Ends the turn and passes it to the next player.
	if (*Input == 'p')									
	{
		if (boardToInput->cursor.selectMinionFlag == true)
			boardToInput->deselectMinion();
		boardToInput->endTurn();
	}

	if (*Input == '1')					
	{
		//Prompt user and save scenario.
		std::string saveName = "";
		std::cout << "Choose where to save your game:" << std::endl;
		std::cin >> saveName;
		scenarioSave(saveName, boardToInput);
	}

	boardToInput->checkWindow();

	return 0;
}

//Load saved game and initialize the board with its contents.
int scenarioLoad(MasterBoard* boardToPrint) {
	
	std::ifstream saveGame;
	std::string line;
	int sg_board_width = 0;
	int sg_board_height = 0; 
	char garbage;
	std::string scenarioToLoad = "";
	std::string saveName = "";
	bool loadsuccessful = false;
	

	//Prompt user and load scenario
	while (loadsuccessful == false)
	{
		std::cout << "Choose which scenario to load (Case sensitive): " << std::endl;
		scenarioToLoad = "";

		std::cin >> scenarioToLoad;

		saveGame.open(scenarioToLoad + ".txt");
		if (saveGame.is_open())
		{
			std::cout << "Successfully loaded!" << std::endl;
			loadsuccessful = true;
		}
		else
		{
			std::cout << "Could not load scenario. Please check that it exists and the right spelling was used." << std::endl;

		}


	}

	//First load the map size:
	saveGame >> sg_board_width;
	saveGame >>  garbage;
	saveGame >> sg_board_height;

	//Then load player data:
	saveGame >> boardToPrint->playerFlag;


		for (int y = 0; y < BOARD_HEIGHT; y++)
		{
			
			for (int x = 0; x < BOARD_WIDTH; x++)
			{
				saveGame >> boardToPrint->Board[x][y].symbol;
			}
		}
		finishLoadingBoard(boardToPrint);

	//Then load minion data:
	int numberOfMinions;
	saveGame >> numberOfMinions;
	int health, locationX, locationY, team, seniority, rangeType, status;
	char type;
	for (int y = 0; y < numberOfMinions; y++)
	{
		saveGame >> type 
			>> locationX 
			>> locationY 
			>> team 
			>> seniority 
			>> rangeType 
			>> status 
			>> health;
			boardToPrint->createMinion(type, locationX, locationY, team, health, status);
	}
	

saveGame.close();
return 1;
}

int main()
{
	MasterBoard GameBoard;

	scenarioLoad(&GameBoard);

	printScreen(&GameBoard);
	char Input = ' ';
	while (true)		//Run as long as the user wants. Infinite for loop.
	{
		userInput(&Input, &GameBoard);
		printScreen(&GameBoard);
	}

}


