#include "StudentWorld.h"
#include "GameConstants.h"
#include "Actor.h"
#include <string>
#include <sstream>
#include <iomanip>
#include <cmath>
#include <iostream>   //***************erase it
using namespace std;

GameWorld* createStudentWorld(string assetDir)
{
	return new StudentWorld(assetDir);
}


StudentWorld::StudentWorld(string assetDir)
	: GameWorld(assetDir), nachen(nullptr), numberOfKilledAliensInCurrentLevel(0), aliensCreatedInCurrentLevel(0), aliensDied(0)
{
	aliensToKillInCurrentLevel = 6 + 4 * getLevel();
}


int StudentWorld::init()
{
	numberOfKilledAliensInCurrentLevel = 0;
	aliensCreatedInCurrentLevel = 0;
	aliensDied = 0;
	nachen = new NachenBlaster(this);
	for (int i = 0; i < 30; i++)
		actorList.push_back(new Star(randInt(0, VIEW_WIDTH - 1), this));      //populate with 30 dynamically allocated stars
	return GWSTATUS_CONTINUE_GAME;
}

int StudentWorld::move()
{
	if (randInt(1, 15) == 1)        //add stars with a 1/15 chance
		actorList.push_back(new Star(VIEW_WIDTH, this));

	//check if a new alien must be added
	//if yes, decide which type to add then add it
	if (shouldWeAddNewAlien())
	{
		aliensCreatedInCurrentLevel++;
		int a = whatAlien();
		if (a == 1)
			actorList.push_back(new Smallgon(this));
		if (a == 2)
			actorList.push_back(new Smoregon(this));
		if (a == 3)
			actorList.push_back(new Snagglegon(this));
	}


	for (list <Actor*>::iterator p = actorList.begin(); p != actorList.end(); )
	{
		if (!(*p)->isDead())
		{
			(*p)->doSomething();             //ask each Actor to do something
			nachen->doSomething();            //ask NachenBlaster to do something

			if (nachen->isDead())
			{
				decLives();
				return GWSTATUS_PLAYER_DIED;        //************************do I need to deal with if he has any lives left?
			}
			if (isCurrentLevelCompleted())
			{
				//********************increase score accordingly
				return GWSTATUS_FINISHED_LEVEL;
			}
			p++;
		}
		/*      else        //if Actor is dead          //**********************probably need to remove this
		{
		if (*p != nullptr)
		{
		delete *p;
		*p = nullptr;
		p = actorList.erase(p);  //erase dead Actor from the list
		}
		}
		*/
	}

	if (nachen->getCabbagePoints() < 30)
		nachen->incCabbagePoints();

	removeDeadObjects();        //remove newly dead actors

	ostringstream oss;
	oss << "Lives: " << getLives() << "  Health: "
		<< nachen->getHealth() << "%" << "  Score: "
		<< getScore() << "  Level: " << getLevel()
		<< "  Cabbages: " << nachen->getCabbages()
		<< "%" << "  Torpedoes: " << nachen->getTorpedoes();
	string s = oss.str();
	setGameStatText(s); //update status bar

	return GWSTATUS_CONTINUE_GAME;
}

void StudentWorld::cleanUp()
{
	for (list <Actor*>::iterator p = actorList.begin(); p != actorList.end();)
	{
		if (*p != nullptr)
		{
			delete *p;
			*p = nullptr;
			p = actorList.erase(p);  //erase dead Actor from the list     
		}
		else
			p++;
	}

	if (nachen != nullptr)
	{
		delete nachen;
		nachen = nullptr;
	}
	numberOfKilledAliensInCurrentLevel = 0;
}


void StudentWorld::addActor(Actor* actor)
{
	actorList.push_back(actor);
}


void StudentWorld::removeDeadObjects()
{
	for (list <Actor*>::iterator p = actorList.begin(); p != actorList.end();)
	{
		if ((*p)->isDead())
		{
			if ((*p)->isAlien())
				aliensDied++;
			//dispose of dead stuff
			delete *p;
			list <Actor*>::iterator q = actorList.erase(p);  //erase dead Actor from the list     
			p = q;      //have p point to the next item
		}
		else
			p++;
	}
}


bool StudentWorld::isCurrentLevelCompleted() const      //***********************test after making ships and all
{
	if (numberOfKilledAliensInCurrentLevel >= aliensToKillInCurrentLevel)       //if NachenBlaster has killed enough alien ships
		return true;
	else
		return false;
}


int StudentWorld::alienShipsOnScreen() const
{

	return aliensCreatedInCurrentLevel - aliensDied;
}


bool StudentWorld::shouldWeAddNewAlien() const
{
	int R = aliensToKillInCurrentLevel - numberOfKilledAliensInCurrentLevel;
	double M = 4 + (.5 * getLevel());

	if (alienShipsOnScreen() < R && alienShipsOnScreen() < M)
		return true;

	return false;
}



int StudentWorld::whatAlien() const
{
	int S1 = 60;
	int S2 = 20 + getLevel() * 5;
	int S3 = 5 + getLevel() * 10;
	int S = S1 + S2 + S3;
	int random = randInt(1, S);

	if (random <= S1)
		return 1;       //add Smallgon with S1/S chance
	else if (random > S1 && random <= S1 + S2)
		return 2;       //add Smoregon with S2/S chance
	else
		return 3;       //add Snagglegon with S3/S2 chance
}



bool StudentWorld::isCollided(Actor* actor1, Actor* actor2) const
{
	double ed;  //euclidean distance
	ed = sqrt(pow(actor1->getX() - actor2->getX(), 2) + pow(actor1->getY() - actor2->getY(), 2));
	if (ed < .75 * (actor1->getRadius() + actor2->getRadius()))
		return true;
	return false;
}


Actor* StudentWorld::collidedWith(Actor* actor1)
{
	for (std::list <Actor*>::iterator p = actorList.begin(); p != actorList.end(); p++)
	{
		//if object is not an Alien, do nothing
		if (!(*p)->isAlien())
			;
		//else check if the euclidean distance is small enough
		//if it is, then return a pointer to the Alien that actor1 collided with
		else if (isCollided(actor1, *p))
			return *p;
	}
	//we went thru loop and no alien has collided with the Projectile actor1
	return nullptr;
}


NachenBlaster* StudentWorld::getCollidingPlayer(Actor* actor) const
{
	if (isCollided(nachen, actor))
		return nachen;
	return nullptr;
}

NachenBlaster* StudentWorld::getPlayer() const
{
	return nachen;
}

StudentWorld::~StudentWorld()
{
	cleanUp();
}



//**************whenever an alien is killed by torpedo or cabbage, call incKilledAliens()