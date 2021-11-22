#include "Minion.hpp"
#include "MasterBoard.hpp"
#include "inputLayer.hpp"
#include "mainmenu.h"
#include <iostream>
#include <ctype.h>
#include <fstream>
#include <cmath>
#include "compie.hpp"


char playCharInput(sf::RenderWindow* myWindow);

inputLayer::inputLayer(mainMenu* inputMainMenu, sf::RenderWindow* myWindow, sf::Texture* gameTexture, sf::Font* cour)
{
	inputLayerTexture = gameTexture;
	inputLayerFont = cour;
	inputLayerWindow = myWindow;
	MainMenu = inputMainMenu;
}

int inputLayer::printSingleTile(int screenX, int screenY, int actualX, int actualY, MasterBoard* boardToPrint, int playerNumber)
{
	tile* tileToPrint = &boardToPrint->Board[actualX][actualY];

	//First print tile, change sprite depending on if withinVision or not
	tileToPrint->mySprite.setPosition(screenX * 50, screenY * 50);
	tileToPrint->myFogSprite.setPosition(screenX * 50, screenY * 50);
	if (tileToPrint->withinVision[playerNumber] == true)
	{
		inputLayerWindow->draw(tileToPrint->mySprite);
	}
	else
	{
		inputLayerWindow->draw(tileToPrint->myFogSprite);
	}

	//Then print minion if withinVision
	if (tileToPrint->hasMinionOnTop == true && tileToPrint->withinVision[playerNumber] == true)
	{
		tileToPrint->minionOnTop->mySprite.setPosition(screenX * 50, screenY * 50);
		inputLayerWindow->draw(tileToPrint->minionOnTop->mySprite);
	}

	//Initialize effects sprite, even though it may not always be used.
	sf::Sprite effectsSprite;
	effectsSprite.setTexture(*inputLayerTexture);
	effectsSprite.setPosition(screenX * 50, screenY * 50);
	

	if (boardToPrint->cursor.selectMinionFlag == true
		&& boardToPrint->cursor.selectMinionPointer->locationX == actualX
		&& boardToPrint->cursor.selectMinionPointer->locationY == actualY
		&& tileToPrint->withinVision[playerNumber] == true)
	{
		//If minion is selected here.
		effectsSprite.setTextureRect(rectArray[1][2]);
		inputLayerWindow->draw(effectsSprite);
	}
	else
		if (tileToPrint->withinCursorPath == true)
		{
			//If this tile is on cursor path
			effectsSprite.setTextureRect(rectArray[6][2]);
			inputLayerWindow->draw(effectsSprite);
		}
		else
			if (tileToPrint->withinApparentRange == true)
			{
				//If this tile is within apparent range.
				effectsSprite.setTextureRect(rectArray[3][2]);
				inputLayerWindow->draw(effectsSprite);
			}
			else if (tileToPrint->withinRange == true
				&& (boardToPrint->cursor.selectMinionPointer->status == gaveupmovehasntfired
					|| boardToPrint->cursor.selectMinionPointer->status == hasmovedhasntfired)
				&& boardToPrint->cursor.selectMinionPointer->specialtyGroup == transport)
			{
				//If this tile is within range for drop off
				effectsSprite.setTextureRect(rectArray[4][2]);
				inputLayerWindow->draw(effectsSprite);
			}
			else if (tileToPrint->withinRange == true
				&& (boardToPrint->cursor.selectMinionPointer->status == gaveupmovehasntfired
					|| boardToPrint->cursor.selectMinionPointer->status == hasmovedhasntfired))
			{
				//If minion can fire and if this tile is within range.
				effectsSprite.setTextureRect(rectArray[2][2]);
				inputLayerWindow->draw(effectsSprite);
			}



	//Always draw cursor
	if (boardToPrint->cursor.XCoord == actualX && boardToPrint->cursor.YCoord == actualY
		&& (boardToPrint->playerRoster[boardToPrint->playerFlag].playerType != computerPlayer
		|| MainMenu->debugMode == true)	)
	{
		boardToPrint->cursor.mySprite.setPosition(screenX * 50, screenY * 50);
		inputLayerWindow->draw(boardToPrint->cursor.mySprite);

	}

	Minion* minionToPrint = tileToPrint->minionOnTop;
	
	//Must be visible to see effects!
	if (tileToPrint->withinVision[playerNumber] == true)
	{

		//Print if transporting or capturing // Specialty moves
		if (tileToPrint->hasMinionOnTop == true && minionToPrint->isCapturing == true)
		{
			effectsSprite.setTextureRect(rectArray[5][3]);
			inputLayerWindow->draw(effectsSprite);
		}
		else if (tileToPrint->hasMinionOnTop == true && minionToPrint->minionBeingTransported != NULL)
		{
			effectsSprite.setTextureRect(rectArray[5][2]);
			inputLayerWindow->draw(effectsSprite);
		}

		//If minion has done all possible moves, black out triangle at top.
		if (tileToPrint->hasMinionOnTop == true && (minionToPrint->status == hasfired || (minionToPrint->rangeType == rangedFire && minionToPrint->status == hasmovedhasntfired)))
		{
			effectsSprite.setTextureRect(rectArray[4][3]);
			inputLayerWindow->draw(effectsSprite);
		}
	
		if (tileToPrint->hasMinionOnTop == true && minionToPrint != NULL && minionToPrint->health < 95)
		{
			if(minionToPrint->health < 95 && minionToPrint->health >= 85)
				effectsSprite.setTextureRect(rectArray[6][0]);
			if (minionToPrint->health < 85 && minionToPrint->health >= 75)
				effectsSprite.setTextureRect(rectArray[6][1]);
			if (minionToPrint->health < 75 && minionToPrint->health >= 65)
				effectsSprite.setTextureRect(rectArray[7][2]);
			if (minionToPrint->health < 65 && minionToPrint->health >= 55)
				effectsSprite.setTextureRect(rectArray[8][2]);
			if (minionToPrint->health < 55 && minionToPrint->health >= 45)
				effectsSprite.setTextureRect(rectArray[8][3]);
			if (minionToPrint->health < 45 && minionToPrint->health >= 35)
				effectsSprite.setTextureRect(rectArray[7][3]);
			if (minionToPrint->health < 35 && minionToPrint->health >= 25)
				effectsSprite.setTextureRect(rectArray[6][3]);
			if (minionToPrint->health < 25 && minionToPrint->health >= 15)
				effectsSprite.setTextureRect(rectArray[3][3]);
			if (minionToPrint->health < 15 && minionToPrint->health >= 5)
				effectsSprite.setTextureRect(rectArray[2][3]);
	
			inputLayerWindow->draw(effectsSprite);
			
		}
		

	
	}




	/*
	//Potentially add low fuel/ammo if appropriate. If not,
	//Potentially add veterancy if Level 1 - 3:
	bool lowAmmo = false;
	bool lowFuel = false;
	//Minion exists and is below 1/3 fuel
	if (minionToPrint != NULL && (  minionToPrint->currentFuel == 0 || (double (minionToPrint->maxFuel) / double( minionToPrint->currentFuel ) ) >= 3) )
	{
		lowFuel = true;
	}		//Minion exists, has a gun (Max != -1), and it is below 1/3 ammo AND isn't infinite ammo (Max != 0)
	if( (minionToPrint != NULL &&  minionToPrint->maxPriAmmo != 0 && minionToPrint->maxPriAmmo != -1 &&  (minionToPrint->currentPriAmmo == 0 || (double(minionToPrint->maxPriAmmo) / double(minionToPrint->currentPriAmmo)) >= 3)) &&
		(minionToPrint != NULL && minionToPrint->maxSecAmmo != 0 && minionToPrint->maxSecAmmo != -1  && (minionToPrint->currentSecAmmo == 0 || (double(minionToPrint->maxSecAmmo) / double(minionToPrint->currentSecAmmo)) >= 3))   )
	{
		lowAmmo = true;
	}
	*/


	/*	//Below are the add-on sprites for life/ammo/status
	//Either print a,e or @ for combos of fuel/ammo shortage
	if (lowFuel == true && lowAmmo == true)
	{
		waddch(MainMenu->mywindow, '@' + COLOR_PAIR(teamNumber));
	}
	else
		if (lowFuel == true) 
		{
			waddch(MainMenu->mywindow, 'e' + COLOR_PAIR(teamNumber));
		} 
		else
			if (lowAmmo == true)
			{
				waddch(MainMenu->mywindow, 'a' + COLOR_PAIR(teamNumber));
			} 
			else	//Or print veterency status
				if (minionToPrint != NULL && minionToPrint->veterancy > 0)
				{
					if (minionToPrint->veterancy == 3) 
					{
						waddch(MainMenu->mywindow, '+' + COLOR_PAIR(teamNumber));
					}
					else if (minionToPrint->veterancy == 2)
					{
						waddch(MainMenu->mywindow, '=' + COLOR_PAIR(teamNumber));
					} else  waddch(MainMenu->mywindow, '-' + COLOR_PAIR(teamNumber));
				}
				else waddch(MainMenu->mywindow, inputString[6] + COLOR_PAIR(teamNumber));
	
	waddch(MainMenu->mywindow, inputString[7] + COLOR_PAIR(teamNumber));
	*/
	//If minion is damaged indicate the health level on bottom right, otherwise print symbol

	return 1;
}

int inputLayer::printStatus(MasterBoard* boardToPrint, int observerNumber)
{
	tile* currentTile = &boardToPrint->Board[boardToPrint->cursor.getX()][boardToPrint->cursor.getY()];

	//Need string pointer since addstr giving grief about printing strings, and same with snprintf.
	char pointerToPrint[100];
	char* descriptionPointer = &currentTile->description[0];
	char* playerName = &(boardToPrint->playerRoster[currentTile->controller].name[0]);
	sf::String tileDescription; 

	if(boardToPrint->Board[boardToPrint->cursor.XCoord][boardToPrint->cursor.YCoord].withinVision[observerNumber] == true || observerNumber == boardToPrint->playerFlag)
	{
	if (currentTile->controller != 0)
	{
		tileDescription = playerName;
		tileDescription += "'s ";
		tileDescription += descriptionPointer;
	}
	else
	{
		tileDescription = descriptionPointer;
	}

	if (MainMenu->debugMode == true) 
	{
		if (boardToPrint->cursor.selectMinionPointer != NULL) 
		{
			snprintf(pointerToPrint, 100, "\nTrue MP/Apparent/Terrain: %d/%d/%d, \nCursor X/Y: %d/%d\n",
				boardToPrint->cursor.selectMinionPointer->truePathMap[currentTile->locationX][currentTile->locationY].distanceFromMinion,
				boardToPrint->cursor.selectMinionPointer->apparentPathMap[currentTile->locationX][currentTile->locationY].distanceFromMinion,
				boardToPrint->cursor.selectMinionPointer->terrainOnlyPathMap[currentTile->locationX][currentTile->locationY].distanceFromMinion,
				boardToPrint->cursor.XCoord,
				boardToPrint->cursor.YCoord);

		}
		else 
		{
			snprintf(pointerToPrint, 100, "\nCursor X/Y: %d/%d\n",
				boardToPrint->cursor.XCoord,
				boardToPrint->cursor.YCoord);
			
		}
		
		tileDescription += pointerToPrint;
		sf::Text newText(tileDescription, *inputLayerFont, MainMenu->menuTextSize);
		newText.setPosition(WINDOW_WIDTH * 52, (menuLineTracker * MainMenu->menuTextSize) +2 );
		inputLayerWindow->draw(newText);
		if (MainMenu->debugMode == true)
			menuLineTracker += 4;
			else menuLineTracker += 2;

	}
	else 
	{
		sf::Text newText(tileDescription, *inputLayerFont, MainMenu->menuTextSize);
		newText.setPosition(WINDOW_WIDTH * 52, menuLineTracker * MainMenu->menuTextSize + 2);
		inputLayerWindow->draw(newText);
		menuLineTracker += 1;
	}

	


	//If tile is undergoing capture, let us know.
	if (currentTile->capturePoints != 20)
	{
		snprintf(pointerToPrint, 100, " Capture Points Left: %d ", currentTile->capturePoints);

		sf::Text newText(tileDescription, *inputLayerFont, MainMenu->menuTextSize);
		newText.setPosition(WINDOW_WIDTH * 52, menuLineTracker * MainMenu->menuTextSize + 2);
		inputLayerWindow->draw(newText);
		menuLineTracker += 1;
	}

	if (currentTile->hasMinionOnTop == true && currentTile->withinVision[observerNumber])
	{
		Minion* currentMinion = currentTile->minionOnTop;

		//Print out basic minion status.
		sf::String minionDescription = &(boardToPrint->playerRoster[currentMinion->team].name[0]);
		minionDescription += "'s ";
		minionDescription += &currentMinion->description[0];

		sf::Text newText(minionDescription, *inputLayerFont, MainMenu->menuTextSize);
		newText.setPosition(WINDOW_WIDTH * 52, menuLineTracker * MainMenu->menuTextSize + 2);
		inputLayerWindow->draw(newText);
		menuLineTracker += 1;

		if (currentMinion->maxPriAmmo > 0 && currentMinion->maxSecAmmo > 0)		//Both pri/sec have ammo
		{
			snprintf(pointerToPrint, 100, "Health: %d | Fuel: %d \nAmmo(P/S): %d/%d\n", int(currentMinion->health), currentMinion->currentFuel, currentMinion->currentPriAmmo, currentMinion->currentSecAmmo);
		}
		else if (currentMinion->maxPriAmmo > 0 && currentMinion->maxSecAmmo == 0)		//Infinte sec ammo
		{
			snprintf(pointerToPrint, 100, "Health: %d | Fuel: %d \nAmmo(P/S): %d/Infinite\n", int(currentMinion->health), currentMinion->currentFuel, currentMinion->currentPriAmmo);
		} 
		else if (currentMinion->maxSecAmmo == 0 && currentMinion->maxPriAmmo == -1)	//No pri, infinite sec.
		{
			snprintf(pointerToPrint, 100, "Health: %d | Fuel: %d \nAmmo(P/S): None/Infinite\n", int(currentMinion->health), currentMinion->currentFuel);
		}
		else if (currentMinion->maxSecAmmo == -1 && currentMinion->maxPriAmmo > 0)	//Primary with no sec.
		{
			snprintf(pointerToPrint, 100, "Health: %d | Fuel: %d \nAmmo(P/S): %d/None\n", int(currentMinion->health), currentMinion->currentFuel, currentMinion->currentPriAmmo);
		}
		else //if (currentMinion->maxSecAmmo == -1 && currentMinion->maxPriAmmo == -1)		//No weapons. Used as catch all for debug.
		{
			snprintf(pointerToPrint, 100, "Health: %d | Fuel: %d \nNo weapons\n", int(currentMinion->health), currentMinion->currentFuel);
		}
		
		sf::String minionStatus = pointerToPrint;
		sf::Text minionStatusText(minionStatus, *inputLayerFont, MainMenu->menuTextSize);
		minionStatusText.setPosition(WINDOW_WIDTH * 52, menuLineTracker* MainMenu->menuTextSize + 2);
		inputLayerWindow->draw(minionStatusText);
		menuLineTracker += 2;

		if (currentMinion->status == gaveupmovehasntfired)
		{
			minionStatus = "Holding position. Ready to attack.\n";

		}
		else
		if (currentMinion->status == hasmovedhasntfired)
		{
			minionStatus = "Has already moved this turn. ";

			if (currentMinion->rangeType == rangedFire)
			{
				minionStatus += "Cannot attack after moving. \n";

			}
			if (currentMinion->rangeType == directFire)
			{
				minionStatus += "Ready to attack.\n";

			}
		}
		else
		if (currentMinion->status == hasfired)
		{
			minionStatus = "Has already moved this turn. Has attacked this turn. \n";
		}
		else
		if (currentMinion->status == hasntmovedorfired)
		{
			minionStatus = "Ready to move. Ready to attack. \n";
		}

		sf::Text secondMinionStatusText(minionStatus, *inputLayerFont, MainMenu->menuTextSize);
		secondMinionStatusText.setPosition(WINDOW_WIDTH * 52, menuLineTracker* MainMenu->menuTextSize + 2);
		inputLayerWindow->draw(secondMinionStatusText);
		menuLineTracker += 1;
	
	}
	else
	{
	//	waddstr(MainMenu->mywindow, "\n\n");
	}

}
	else
	{
	//	waddstr(MainMenu->mywindow, "\n\n");
	}

	//Print current turn.
	
	sf::String playerStatus = &(boardToPrint->playerRoster[boardToPrint->playerFlag].name[0]);
	snprintf(pointerToPrint, 100, "'s turn. Treasury Total: %d\n", boardToPrint->playerRoster[boardToPrint->playerFlag].treasury);
	playerStatus += pointerToPrint;
	playerStatus += &eventText[0];
	sf::Text playerStatuText(playerStatus, *inputLayerFont, MainMenu->menuTextSize);
	playerStatuText.setPosition(WINDOW_WIDTH * 52, menuLineTracker* MainMenu->menuTextSize + 2);
	inputLayerWindow->draw(playerStatuText);
	menuLineTracker += 2;

	eventText = "";
	
	return 0;
}

int inputLayer::printMinionMenu(MasterBoard* boardToPrint) {

	minionStatus mystatus = boardToPrint->cursor.selectMinionPointer->status;
	
	sf::String boardMessage;

	if (mystatus == hasntmovedorfired)
	{
		boardMessage = "Move cursor(WASD) | Move minion (m)\nDeselect minion(t) | Capture move(c)\n";
	}

	if (mystatus == hasmovedhasntfired || mystatus == gaveupmovehasntfired)
	{
		boardMessage = "Move cursor(WASD) | Attack (r)\nDeselect minion (t) | Capture (c)\n";
	}
	
	if(mystatus == hasfired)
	{
		boardMessage = "No further action possible";
		
	}
	   	
	sf::Text newText(boardMessage, *inputLayerFont, MainMenu->menuTextSize);

	newText.setPosition(WINDOW_WIDTH * 52, menuLineTracker * MainMenu->menuTextSize + 2);

	inputLayerWindow->draw(newText);
	
	menuLineTracker += 2;

	return 0;

}

int inputLayer::printBoardMenu(MasterBoard* boardToPrint) {
		
	sf::String boardMessage = "Move cursor (WASD) | Menu (m)\nSelect minion/property (t)\n";
	
	sf::Text newText(boardMessage, *inputLayerFont, MainMenu->menuTextSize);
	
	newText.setPosition(WINDOW_WIDTH * 52, menuLineTracker*MainMenu->menuTextSize);

	inputLayerWindow->draw(newText);
	
	menuLineTracker += 2;
	
	return 0;
}

int	inputLayer::printPropertyMenu(MasterBoard* boardToPrint) {
	
	int totalLinesPrinted = 0;
	sf::String boardMessage;

	//If this is not the second valid purchase input
	if (requestedMinionToBuy == '\n')
	{
		if (boardToPrint->Board[boardToPrint->cursor.getX()][boardToPrint->cursor.getY()].symbol == 'h')
		{
			boardMessage = "Input Minion to Buy(i,s,a,r,c,R,T,A,P):\nDeselect Property (p)\n";
		}
		if (boardToPrint->Board[boardToPrint->cursor.getX()][boardToPrint->cursor.getY()].symbol == 'A')
		{
			boardMessage = "Input Minion to Buy(v,h,f,b):\nDeselect Property (p)\n";
		}
		if (boardToPrint->Board[boardToPrint->cursor.getX()][boardToPrint->cursor.getY()].symbol == 'P')
		{
			boardMessage = "Input Minion to Buy(B,G,C,V,U,L):\nDeselect Property (p)\n";
		}
		totalLinesPrinted = 2;
	}
	else if (requestedMinionToBuy != '\n')
	{
		if (requestedMinionToBuy == '!')
		{
			boardMessage = "Can't afford, try another symbol.\nInput Minion to Buy | Deselect Property (p)\n";
		}
		else if (requestedMinionToBuy == '?')
		{
			boardMessage = "Invalid input. Try another symbol.\nInput Minion to Buy | Deselect Property (p)\n";
			
		}
		else
		{
			char toOutput[100];
			snprintf(toOutput, 100, "Requested unit costs: %d. \n", requestedUnitPrice);
			boardMessage = toOutput;
			boardMessage += "Confirm (z) | Cancel (p)\n";
			
		}
		totalLinesPrinted = 2;
	}

	
	sf::Text newText(boardMessage, *inputLayerFont, MainMenu->menuTextSize);
	newText.setPosition(WINDOW_WIDTH * 52, menuLineTracker * MainMenu->menuTextSize);
	inputLayerWindow->draw(newText);

	menuLineTracker += totalLinesPrinted;
	
	return 0;

}

int inputLayer::printMenu() {

	sf::String boardMessage = "Save game (s) | Go to main menu (x)\nLoad save game (L) | End turn (e)\nRestart map (r) | Exit menu (m)";

	sf::Text newText(boardMessage, *inputLayerFont, MainMenu->menuTextSize);

	newText.setPosition(WINDOW_WIDTH * 52, menuLineTracker * MainMenu->menuTextSize);

	inputLayerWindow->draw(newText);

	menuLineTracker += 3;

	return 0;

}

int inputLayer::printLowerScreen(MasterBoard* boardToPrint, int observerNumber) {

	if (status == gameBoard)
	{
		printBoardMenu( boardToPrint);
	}
	else

		if (status == menu)
		{
			printMenu();
		}
		else

			if (status == minionAction)
			{
				printMinionMenu(boardToPrint);
			}
			else

				if (status == propertyAction)
				{
					printPropertyMenu(boardToPrint);
				}
				else 
					if (status == insertMinion)
					{
					printInsertMinion();
					}
					else
						if (status == insertTile)
						{
							printInsertTile();
						}

	printStatus(boardToPrint, observerNumber);

	return 0;

}

int inputLayer::printInsertMinion() 
{

	sf::String boardMessage = "Insert a minion by typing its symbol\nExit insert minion menu(x) \n";

	sf::Text newText(boardMessage, *inputLayerFont, MainMenu->menuTextSize);

	newText.setPosition(WINDOW_WIDTH * 52, menuLineTracker * MainMenu->menuTextSize);

	inputLayerWindow->draw(newText);

	menuLineTracker += 2;

	return 0;

}

int inputLayer::printInsertTile()
{
	sf::String boardMessage = "Insert a tile by typing its symbol\nExit insert tile menu(x) \n";

	sf::Text newText(boardMessage, *inputLayerFont, MainMenu->menuTextSize);

	newText.setPosition(WINDOW_WIDTH * 52, menuLineTracker * MainMenu->menuTextSize);

	inputLayerWindow->draw(newText);

	menuLineTracker += 2;

	return 0;

}

int inputLayer::printMissionBriefing(MasterBoard* boardToInput)
{
	inputLayerWindow->clear();
	sf::String boardMessage;

	if (boardToInput->missionFlag == true) 
	{
		boardMessage = boardToInput->campaignName;
		boardMessage += ": ";
	}

	boardMessage += boardToInput->scenarioOrMissionName;
	boardMessage += "\n\n";
	
	boardMessage += MainMenu->missionBriefing;
	boardMessage += "\n\nPress any key to continue.";
	
	sf::Text newText(boardMessage, *inputLayerFont, MainMenu->menuTextSize);


	inputLayerWindow->draw(newText);
	inputLayerWindow->display();

	sf::Event event;
	inputLayerWindow->pollEvent(event);

	//Wait for one input.
	playCharInput(inputLayerWindow);

	return 0;

}

int inputLayer::printUpperScreen(MasterBoard* boardToPrint, int observerNumber) {
	//windowLocation is a single scalar representing x and y.
	//We do some basic math to break it into the two values for the function.
	//Need to convert windowLocation into a better two part variable.
	int windowY = boardToPrint->windowLocationY;
	int windowX = boardToPrint->windowLocationX;
	
	//Go through the whole "board", staying within the bounds of window's x and y coordinates.
	for (int i = windowY; i < (windowY + WINDOW_HEIGHT); i++)
	{
		for (int j = windowX; j < (windowX + WINDOW_WIDTH); j++)
		{
			printSingleTile((j - windowX),  (i - windowY), j , i , boardToPrint, observerNumber);
		}
		
	}

	return 0;
}

int inputLayer::printWaitingScreen(MasterBoard* boardToPrint) 
{
	inputLayerWindow->clear();
	char buffer[100];
	sf::String announceString = boardToPrint->playerRoster[boardToPrint->playerFlag].name;
	announceString += "'s turn. Press any key to begin.  \n";
	sf::Text newText(announceString, *inputLayerFont, 20);
	inputLayerWindow->draw(newText);
	inputLayerWindow->display();

	return 0;

}

int inputLayer::printScreen(MasterBoard* boardToPrint, int observerNumber)
{
	if (status != waitingForNextLocalPlayer) 
	{
	inputLayerWindow->clear();
	printUpperScreen(boardToPrint, observerNumber);
	printLowerScreen(boardToPrint, observerNumber);
	inputLayerWindow->display();
	
	}
	else printWaitingScreen(boardToPrint);
	
	//Reset line tracker after each print.
	menuLineTracker = 0;
	
	return 0;
}

int inputLayer::waitingScreenInput(MasterBoard* boardToInput) 
{
	//Only lasts one input.
	status = gameBoard;

	return 0;
}

int inputLayer::insertMinionInput(char* Input, MasterBoard* boardToInput)
{
	Cursor * myCursor = & boardToInput->cursor;
	tile* myTile = & boardToInput->Board[myCursor->XCoord][myCursor->YCoord];


	//Return to gameBoard if player presses 'x'.
	if (*Input == 'x' )
	{
		status = gameBoard;
		return 1;
	}

	//Prevent minion insertion on top of another, and prevent insertion somewhere that minion couldn't actually move.
	if (myTile->hasMinionOnTop == true 	|| myTile->consultMovementChart(*Input, myTile->symbol) == 99)
		return 1;
												
	requestedUnitPrice = boardToInput->consultMinionCostChart(*Input, '~');

	//If it is real minion, then price > 0
	if (requestedUnitPrice > 0)
	{
		boardToInput->createMinion(*Input, myCursor->getX(), myCursor->getY(), boardToInput->playerFlag, 100, 0, 0, 0, -1, -1, -1);
		status = gameBoard;
		return 0;
	}

	return 1;

}

//If there is a minion underneath, destroy it regardless of any status.
//Activate with debug mode and "z" input.
int inputLayer::deleteMinionInput( MasterBoard* boardToInput)
{
	Cursor* myCursor = &boardToInput->cursor;
	tile* myTile = &boardToInput->Board[myCursor->XCoord][myCursor->YCoord];


	
	if (myTile->hasMinionOnTop != true)
	{
		status = gameBoard;
		return 1;
	}
	else 
	{
		boardToInput->destroyMinion(myTile->minionOnTop, false,this, true);
		status = gameBoard;
		return 0;
	}		
		
	

}

int inputLayer::insertTileInput(char* Input, MasterBoard* boardToInput) 
{
	Cursor* myCursor = &boardToInput->cursor;
	tile* myTile = &boardToInput->Board[myCursor->XCoord][myCursor->YCoord];
	
	//Return to gameBoard if player presses 'q'.
	if (*Input == 'q')
	{
		status = gameBoard;
		return 1;
	}

	//If input tile symbol is invalid, return 1.
	if (myTile->consultMovementChart('i', *Input) == -1)
		return 1;

	//Prevent minion insertion on top of another, and prevent insertion somewhere that minion couldn't actually move.
	if (myTile->hasMinionOnTop == true && myTile->consultMovementChart(myTile->minionOnTop->type ,  *Input ) == 99)
		return 1;

	//If it is real tile, change the underlying tile.
	//May break things....
	myTile->capturePoints = 20;
	myTile->symbol = *Input;
	
	if (myTile->checkForProperty(myTile->symbol) == true)
	{
		myTile->controller = boardToInput->playerFlag;
	}
	else 
	{
		myTile->controller = 0;
	}

	myTile->setCharacterstics(inputLayerTexture, boardToInput);
	   	 
	status = gameBoard;
	return 0;

}

int inputLayer::gameBoardInput(char* Input, MasterBoard* boardToInput)
{
	if (*Input == 'a' || *Input == 'd' || *Input == 's' || *Input == 'w')
	{
		boardToInput->cursor.move(Input);
	}

	if (*Input == 'x' && MainMenu->debugMode == true)
	{
		status = insertMinion;
	}	

	if (*Input == 'z' && MainMenu->debugMode == true)
	{
		deleteMinionInput(boardToInput);
	}
	
	if (*Input == 'q' && MainMenu->debugMode == true)
	{
		status = insertTile;
	}

	//Need char for shift
	if (*Input == '0')
	{
		if (minionVisibleStatus == hideMinions)
		{
			minionVisibleStatus = showMinions;
		}
		else if (minionVisibleStatus == showMinions)
		{
			minionVisibleStatus = hideMinions;
		}
	}

	//Select minion or property.
	if (*Input == 't')
	{
		//If minion is not selected, select it.Must be successful to set flag.
		if (boardToInput->cursor.selectMinionFlag == false //This is probably not needed since it's always true/false in conj. with inputLayer.
			&& boardToInput->Board[boardToInput->cursor.getX()][boardToInput->cursor.getY()].hasMinionOnTop == true)
		{
			if (boardToInput->selectMinion(boardToInput->cursor.getX(), boardToInput->cursor.getY()) == 0)
			{
				status = minionAction;
			}
		}	//Else if empty property, select it. No minion on top, right team, must be factory to select.
		else
			if ((boardToInput->Board[boardToInput->cursor.getX()][boardToInput->cursor.getY()].symbol == 'h' ||
				boardToInput->Board[boardToInput->cursor.getX()][boardToInput->cursor.getY()].symbol == 'A' ||
				boardToInput->Board[boardToInput->cursor.getX()][boardToInput->cursor.getY()].symbol == 'P')
				&& boardToInput->Board[boardToInput->cursor.getX()][boardToInput->cursor.getY()].controller == boardToInput->playerFlag
				&& boardToInput->Board[boardToInput->cursor.getX()][boardToInput->cursor.getY()].hasMinionOnTop == false)
			{
				status = propertyAction;
			}

	}

	if (*Input == 'm')
	{
		status = menu;
	}

	return 0;
}

int inputLayer::minionInput(char* Input, MasterBoard* boardToInput) {

	//This tracks who may lose after an action. Only one player can lose per action, so only need one number.
	int playerPotentiallyDefeated = 0;

	if (*Input == 'a' || *Input == 'd' || *Input == 's' || *Input == 'w')
	{
		boardToInput->cursor.move(Input);
	}
	
	//Need char for shift
	if (*Input == '0')
	{
		if (minionVisibleStatus == hideMinions)
		{
			minionVisibleStatus = showMinions;
		}
		else if (minionVisibleStatus == showMinions)
		{
			minionVisibleStatus = hideMinions;
		}
	}

	//Deselect
	if (*Input == 't')
	{
		if (boardToInput->cursor.selectMinionFlag == true)
		{
			boardToInput->deselectMinion();
			status = gameBoard;
		}
	}

	//Move minion command
	//If minion selected and hasn't moved or fired, attempt to move.
	//The moveMinion function will check if we are on top of ourselves or another minion.
	if (*Input == 'm' && boardToInput->cursor.selectMinionFlag == true
		&& boardToInput->cursor.selectMinionPointer->status == hasntmovedorfired)
	{
		if (boardToInput->Board[boardToInput->cursor.getX()][boardToInput->cursor.getY()].hasMinionOnTop
		&& boardToInput->Board[boardToInput->cursor.getX()][boardToInput->cursor.getY()].minionOnTop->minionBeingTransported == NULL	
		&&	boardToInput->pickUpMinion(boardToInput->cursor.getX(), boardToInput->cursor.getY()) == 0)
		{
			//Change status if successful pickup occurred
			status = gameBoard;
		}
		else
		if (boardToInput->moveMinion(boardToInput->cursor.getX(), boardToInput->cursor.getY()) == 0)
		{	
			//Change status appropriately for successful movement.
			status = gameBoard;
		}
	}

	//'i' is supply
	//Must have minion selected.
	//Must be APC, hasn't taken second action, cursor is on minion, and regardless of transport status.
	if (*Input == 'i' && boardToInput->cursor.selectMinionFlag == true
		&& boardToInput->cursor.selectMinionPointer->type == 'P'
		&& (boardToInput->cursor.selectMinionPointer->status == hasmovedhasntfired || boardToInput->cursor.selectMinionPointer->status == gaveupmovehasntfired)
		&& boardToInput->cursor.getX() == boardToInput->cursor.selectMinionPointer->locationX
		&& boardToInput->cursor.getY() == boardToInput->cursor.selectMinionPointer->locationY)
	{
		//May not be successful, so not necessarily return 0
		if (boardToInput->individualResupply(boardToInput->cursor.selectMinionPointer, false) == 0)
			status = gameBoard;
	}

	//O is drop
	//Must have minion selected.
	//Must be transport, hasn't taken second action, has a minion to drop, and tile within range, and not blocked by another minion.
	//Also must not be impassable.
	if (*Input == 'o' && boardToInput->cursor.selectMinionFlag == true
		&& boardToInput->cursor.selectMinionPointer->specialtyGroup == transport
		&&	( (boardToInput->cursor.selectMinionPointer->status == hasmovedhasntfired || 
		 boardToInput->cursor.selectMinionPointer->status == gaveupmovehasntfired)
		&& boardToInput->cursor.selectMinionPointer->minionBeingTransported != NULL)
		&& boardToInput->Board[boardToInput->cursor.getX()][boardToInput->cursor.getY()].hasMinionOnTop == false
		&& boardToInput->Board[boardToInput->cursor.getX()][boardToInput->cursor.getY()].withinRange == true
		&& boardToInput->Board[boardToInput->cursor.getX()][boardToInput->cursor.getY()].consultMovementChart(boardToInput->cursor.selectMinionPointer->minionBeingTransported->type, boardToInput->Board[boardToInput->cursor.getX()][boardToInput->cursor.getY()].symbol) != 99)
	{
		if(	boardToInput->dropOffMinion() == 0)
			status = gameBoard;
	}

	bool lastMinionDestroyed = false;
	//Attack command. Pre-reqs: must be in range, must be enemy team and not yours. Must also not be transport type.
	if (*Input == 'r' && boardToInput->cursor.selectMinionFlag == true && boardToInput->cursor.selectMinionPointer->type != transport)
		if (boardToInput->Board[boardToInput->cursor.getX()][boardToInput->cursor.getY()].hasMinionOnTop == true)
			if ((boardToInput->cursor.getX() != boardToInput->cursor.selectMinionPointer->locationX) || (boardToInput->cursor.getY() != boardToInput->cursor.selectMinionPointer->locationY))//Can attack if minion is selected
				if (boardToInput->Board[boardToInput->cursor.getX()][boardToInput->cursor.getY()].minionOnTop->team != boardToInput->cursor.selectMinionPointer->team)//And it's enemy team's.
					if (boardToInput->Board[boardToInput->cursor.getX()][boardToInput->cursor.getY()].withinRange == true)	//In range
					{
						//This is the actual attack portion. Return of 0 indicates successful attack.
						//Note minion's owner so if they lose we know who lost.
						playerPotentiallyDefeated = boardToInput->Board[boardToInput->cursor.getX()][boardToInput->cursor.getY()].minionOnTop->team;
						bool attackSuccess = boardToInput->attackMinion(boardToInput->cursor.getX(), boardToInput->cursor.getY(), this);
						if (attackSuccess == 0)
						{
							status = gameBoard;
						}
					}

	//Press 'c' to capture property minion is currently on.
	//First, minion must be available.
	//Also, must be infantry type.

	if (*Input == 'c' && boardToInput->cursor.selectMinionFlag == true )
		if ((boardToInput->cursor.selectMinionPointer->status == hasmovedhasntfired
		|| boardToInput->cursor.selectMinionPointer->status == gaveupmovehasntfired)
		&& boardToInput->cursor.selectMinionPointer->specialtyGroup == infantry)
	{
		tile* tileToCheck = &boardToInput->Board[boardToInput->cursor.selectMinionPointer->locationX][boardToInput->cursor.selectMinionPointer->locationY];
		playerPotentiallyDefeated = tileToCheck->controller;
		//Must be property and must not be the current player's property (Could be neutral).
		if (tileToCheck->checkForProperty(tileToCheck->symbol) && tileToCheck->controller != boardToInput->playerFlag)
		{
			
			eventText = boardToInput->captureProperty(tileToCheck, boardToInput->cursor.selectMinionPointer, this);
			boardToInput->deselectMinion();
			status = gameBoard;
		}
	}
	

	return 0;
}

int inputLayer::printPlayerDefeat(int playerDefeated, MasterBoard* boardToPrint)
{

	inputLayerWindow->clear();
	sf::String topMenuJoinString = "A player defeated. Press any key to continue.  \n";
	sf::Text newText(topMenuJoinString, *inputLayerFont, MainMenu->menuTextSize);
	MainMenu->mywindow->draw(newText);
	inputLayerWindow->display();

	sf::Event event;
	inputLayerWindow->pollEvent(event);

	//Wait for one input.
	playCharInput(inputLayerWindow);
	
	
	return 0;
}

int inputLayer::printPlayerVictory(int playerVictorious, MasterBoard* boardToPrint) 
{
	inputLayerWindow->clear();
	sf::String topMenuJoinString = " Was victorious! Press any key to return to main menu.  \n";
	sf::Text newText(topMenuJoinString, *inputLayerFont, MainMenu->menuTextSize);
	MainMenu->mywindow->draw(newText);
	inputLayerWindow->display();

	sf::Event event;
	inputLayerWindow->pollEvent(event);

	//Wait for one input.
	playCharInput(inputLayerWindow);

	return 0;
}


int inputLayer::menuInput(char* Input, MasterBoard* boardToInput) {

	//This is a working key.
	if (*Input == 'g' && MainMenu->debugMode == true)
	{
		MainMenu->scrambleMap(boardToInput, this);	//This needs to be cleaned up to deal with minions.
	}

	//Another working key for compie
	//Currently (I think) breaks the game.
	if (*Input == 'c' && MainMenu->debugMode == true)
	{
		MainMenu->computerPlayerRoster[boardToInput->playerFlag].takeMyTurn(boardToInput);
	}

	//Need char for shift
	if (*Input == '0')
	{
		if (minionVisibleStatus == hideMinions)
		{
			minionVisibleStatus = showMinions;
		}
		else if (minionVisibleStatus == showMinions)
		{
			minionVisibleStatus = hideMinions;
		}
	}


	//Ends the turn and passes it to the next player.
	//Autosave every turn.
	if (*Input == 'e')
	{
		if (boardToInput->cursor.selectMinionFlag == true)
			boardToInput->deselectMinion();
		int incrementGameTurn = boardToInput->endTurn(this);
		//If we advanced a gameTurn, mainMenu will keep track of it.
		MainMenu->gameTurn += incrementGameTurn;
		//Have to always keep an autosave!
		MainMenu->gameSave(".\\savegames\\Auto_save.txt", boardToInput);
		
		//If multiplayer, push to remote server and queue "waiting"
		//mainmenu's playGame will keep the player waiting.
		if (MainMenu->gameType == remote)
		{
			MainMenu->skipOneInput = true;
			MainMenu->multiplayerPushSaveGame(boardToInput);
			MainMenu->menuStatus = waitingForRemotePlayer;
		}

	}
	
	//Restart current mission/scenario
	if (*Input == 'r') 
	{
		restartGame(boardToInput);
	}
		
	if (*Input == 'l')
	{	
		//Load the actual save game
		std::ifstream loadGameSave;
		bool loadsuccessful = false;

		//Prompt user and load scenario
		int lineOffset = 1;
		while (loadsuccessful == false)
		{
			inputLayerWindow->clear();
			sf::String loadPrompt = "Choose which save game to load (Case sensitive): \n";
			sf::String loadGameName = MainMenu->playerInputString(inputLayerWindow, inputLayerFont, loadPrompt, lineOffset);
			sf::Event event;

			std::string stdloadGameName = loadGameName;
			loadGameSave.open(".\\savegames\\" + stdloadGameName + "_save.txt");
			if (loadGameSave.is_open())
			{
				inputLayerWindow->clear();
				sf::String successful = "Successfully loaded! Press any key to continue.\n";
				sf::Text newText(successful, *inputLayerFont, MainMenu->menuTextSize);
				inputLayerWindow->draw(newText);
				inputLayerWindow->display();

				loadsuccessful = true;
			}
			else
			{
				inputLayerWindow->clear();
				sf::String loadPrompt = "Could not load scenario.\nPlease check that it exists and the right spelling was used.\n";
				lineOffset = 2;

			}
		}

		//Actually load scenario. Initialize board, etc.
		MainMenu->gameLoad(boardToInput, this, &loadGameSave);
		//Flush event queue to clear out "Enter" and other rifraf
		sf::Event throwAwayEvent;
		while (inputLayerWindow->pollEvent(throwAwayEvent));
		//Give player a chance to click.
		playCharInput(inputLayerWindow);

		status = gameBoard;
		
	}

	//Prompt user and save game.
	if (*Input == 's')
	{
		int lineOffset = 1;
		inputLayerWindow->clear();
		sf::String savePrompt = "Choose a name to save your game.\n";
		sf::String saveGameName = MainMenu->playerInputString(inputLayerWindow, inputLayerFont, savePrompt, lineOffset);

		std::string stdSaveGameName = ".\\savegames\\";
		stdSaveGameName += saveGameName;
		stdSaveGameName += "_save.txt";

		MainMenu->gameSave(stdSaveGameName, boardToInput);
		
		inputLayerWindow->clear();

		sf::String successSave = "Game saved. Press any key to continue.\n";
		sf::Text newText(successSave, *inputLayerFont, MainMenu->menuTextSize);
		inputLayerWindow->draw(newText);
		inputLayerWindow->display();

		//Flush event queue to clear out "Enter" and other rifraf
		sf::Event throwAwayEvent;
		while (inputLayerWindow->pollEvent(throwAwayEvent));

		playCharInput(inputLayerWindow);
		
		status = gameBoard;
	}

	//Exit menu
	if (*Input == 'm')
	{
		status = gameBoard;
	}

	//Exit to main menu
	if (*Input == 'x')
	{
		MainMenu->gameSave(".\\savegames\\Auto_save.txt", boardToInput);
		exitToMainMenu(boardToInput);
	}

	return 0;
}

//Because this is only ordered by player, we don't have to worry about cleaning up compie behavior afterwards. So we can just re-load and go back to gameboard.
int inputLayer::restartGame(MasterBoard* boardToInput)
{

	//Load the actual save game
	std::ifstream loadGame;
	bool loadsuccessful = false;

	//Prompt user and load scenario
	while (loadsuccessful == false)
	{
	
		std::string gameToLoad = boardToInput->scenarioOrMissionName;
		
		if (boardToInput->missionFlag == true) 
		{

			loadGame.open(".\\campaigns\\" + boardToInput->campaignName +"\\" + gameToLoad + ".txt");
		}
		else 
		{
			loadGame.open(".\\scenarios\\" + gameToLoad + ".txt");
		}

		if (loadGame.is_open())
		{
			std::cout << "Scenario/mission successfully loaded!\n";
			loadsuccessful = true;
		}
		else
		{
			std::cout << "Could not load scenario or mission. Please check that it exists and the right spelling was used.\n";

		}

	}
	//Actually load scenario. Initialize board, etc.
	MainMenu->gameLoad(boardToInput, this, &loadGame);

	//We don't always print mission briefing but we do here
	printMissionBriefing(boardToInput );

	status = gameBoard;
	return 0;

}

int inputLayer::NextMission(MasterBoard* boardToInput) 
{
	//Load the actual save game
	std::ifstream loadGame;
	bool loadsuccessful = false;

	//Prompt user and load scenario
	while (loadsuccessful == false)
	{

		std::string gameToLoad = MainMenu->nextMissionName;

		loadGame.open(".\\campaigns\\" + boardToInput->campaignName + "\\" + gameToLoad + ".txt");


		if (loadGame.is_open())
		{
			std::cout << "Scenario/mission successfully loaded!\n";
			loadsuccessful = true;
		}
		else
		{
			std::cout << "Could not load scenario or mission. Please check that it exists and the right spelling was used.\n";

		}

	}
	std::string playerName = boardToInput->playerRoster[1].name;
	//Actually load scenario. Initialize board, etc.
	MainMenu->gameLoad(boardToInput, this, &loadGame);
	boardToInput->playerRoster[1].name = playerName;
	//NEED to transfer player information -  just the name for now.

	//We don't always print mission briefing but we do here
	printMissionBriefing(boardToInput );



	status = gameBoard;
	
	MainMenu->playGame(boardToInput, this);

	return 0;

}

int inputLayer::propertyMenuInput(char* Input, MasterBoard* boardToInput) {

	//Need char for shift
 	if (*Input == '0')
	{
		if (minionVisibleStatus == hideMinions)
		{
			minionVisibleStatus = showMinions;
		}
		else if (minionVisibleStatus == showMinions)
		{
			minionVisibleStatus = hideMinions;
		}
		return 0;
	}

	int treasury = boardToInput->playerRoster[boardToInput->playerFlag].treasury;

	//If this is NOT the second valid purchase input
	//IE we have not yet gotten any valid input for propertyLayer.
	//Thus one of the three states (prestate, too expensive, not a real unit).
	if (requestedMinionToBuy == '\n' || requestedMinionToBuy == '!' || requestedMinionToBuy == '?')
	{
		if (*Input == 'p')
		{
			//Deselect
			status = gameBoard;
			requestedMinionToBuy = '\n';
			return 0;
		}
		//Consult cost table:
		requestedUnitPrice = boardToInput->consultMinionCostChart(*Input, boardToInput->Board[boardToInput->cursor.getX()][boardToInput->cursor.getY()].symbol);

		//If it is a real unit we are trying to purchase
		//Aka unitPrica is not -1 aka non-error
		if (requestedUnitPrice > 0)
		{
			requestedMinionToBuy = *Input;

			//If we can't afford, use special ! character
			if (requestedUnitPrice > treasury)
			{
				//Indicates you can't afford
				requestedMinionToBuy = '!';
			}
		}
		else
		{
			requestedMinionToBuy = '?';
		}

	}

	//This is the second valid input.
	else
	{
		if (*Input == 'p')
		{
			//Cancel purchase
			requestedMinionToBuy = '\n';
		}
		if (*Input == 'z')
		{
			//Confirm purchase
			boardToInput->attemptPurchaseMinion(requestedMinionToBuy, boardToInput->cursor.getX(), boardToInput->cursor.getY(), boardToInput->playerFlag);
			status = gameBoard;
			requestedMinionToBuy = '\n';
			requestedUnitPrice = -1;
		}
	}

	return 0;
}


int inputLayer::exitToMainMenu(MasterBoard* boardToInput)
{

	//Clear up resources we used to avoid memory leak.
	boardToInput->clearBoard(this);

	MainMenu->menuStatus = topmenu;
	MainMenu->skipOneInput = true;
	
	return 0; 
}
