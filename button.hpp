//Copyright 2022, Supercontinent Software Ltd.
//
//	button.hpp
//
//Class for button interface. Handles printing and taking input.
//It knows its size and location for taking mouse clicks.
//It contains its own sprite for easy printing.

#ifndef BUTTON_HPP__
#define BUTTON_HPP__

//Button.cpp and Button.hpp
#include <string.h>
#include <SFML/Graphics.hpp>



//These aren't very useful for now, since we refer to the buttons within inputLayer and mainMenu by simple number for array access.
enum buttonType {
	newGame, loadGame, toggleEditor, newCampaign, newSkirmish, loadCampaign, loadSkirmish,
	saveGame, exitToMainMenu, endTurn, toggleSound, restart, loadGameFromGame, resume_Play, toggle_Speed
};




class Button
{
	//Shouldn't be needed as public, only referenced in checking if mouse is within button.
	int xCoord = 0;
	int yCoord = 0;
	int width = 0;
	int height = 0;


public:
	std::string label = "---";
	bool visible = false;
	buttonType myType;

	//GRAPHICS ///////////////////////////
	//Each button has its own individual texture loaded separately from the texture array
	sf::Sprite mySprite;
	sf::Texture* myTexture;
	//GRAPHICS ///////////////////////////

	Button(int inputXCoord, int inputYCoord, int inputButtonType, sf::Texture* inputTexture)
	{
		//Assign values to members
		int xCoord = inputXCoord;
		int yCoord = inputYCoord;

		myType = buttonType (inputButtonType );
		myTexture = inputTexture;
		mySprite.setTexture(*myTexture);
		mySprite.setPosition(xCoord, yCoord);

		sf::Vector2u dimensions = myTexture->getSize();
		int width = dimensions.x;
		int height = dimensions.y;
		bool visible = false;
	}

	bool checkWithinButton(int inputXCoord, int inputYCoord)
	{
		if (inputXCoord < xCoord + width && inputXCoord > xCoord)
			if (inputYCoord < yCoord + height && inputYCoord > yCoord)
				return true;

		return false;
	}

};



#endif /* BUTTON_HPP__ */