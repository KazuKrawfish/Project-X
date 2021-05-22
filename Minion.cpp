#include "Minion.hpp"
#include "MasterBoard.hpp"

//
//Default Constructor. Should not be used.
//
Minion::Minion()
{
	locationX = -1;
	locationY = -1;
	type = ' ';
	team = 0;						//Team 0 is the neutral team.
	status = hasntmovedorfired;
	health = 100;
	
}

//
//Always use this constructor, it gives the new Minion all required properties.
//When updating Minion(), also update ATTACK_VALUES_MATRIX, consultAttackValuesChart, and consultMinionCostChart.
//
Minion::Minion(int inputSeniority, int inputX, int inputY, char inputType, int inputTeam, MasterBoard* Environment, int inputHealth)
{
	seniority = inputSeniority;
	minionEnvironment = Environment;
	locationX = inputX;
	locationY = inputY;
	type = inputType;
	health = inputHealth;
	switch (inputType)
	{
	case('i'):
	{		
		description = "Infantry";
		movementRange = 3;
		attackRange = 1;
		rangeType = directFire;
		break;
	}
	case('a'):
	{
		description = "Armor";
		movementRange = 6;
		attackRange = 1; 
		rangeType = directFire;
		break;
	}
	case('r'):
	{
		description = "Artillery";
		movementRange = 5;
		attackRange = 3;
		rangeType = rangedFire;
		break;
	}
	case('c'):
	{
		description = "Cavalry";
		movementRange = 8;
		attackRange = 1;
		rangeType = directFire;
		break; 
	}
	case('R'): 
	{ 
		description = "Rocket";
		movementRange = 4;
		attackRange = 5;
		rangeType = rangedFire;
		break; 
	}
	}

	team = inputTeam;

	Environment->Board[inputX][inputY].hasMinionOnTop = true;
}

//
//Destructor. Minion has no dynamic variables to clean up so it is empty.
//
Minion::~Minion() {}