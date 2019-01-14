#ifndef STUDENTWORLD_H_
#define STUDENTWORLD_H_

#include "GameWorld.h"
#include <string>
#include <list>

// Students:  Add code to this file, StudentWorld.cpp, Actor.h, and Actor.cpp

//responsible for keeping track
//of game world and all of the actors / objects
//responsible for initializing the game world at the start of the game
//asking all the actors to
//do something during each tick of the game, destroying an actor when it disappears(e.g.,
//an alien dies, an alien, projectile or star flies off the screen, an explosion finishes
//exploding, a projectile impacts a ship, etc.), and destroying *all* of the actors in the game
//world when the user loses a life.

//must be derived from our GameWorld class 

//must implement at least these three methods (which are defined as
//pure virtual in our GameWorld class):
//virtual int init() = 0;   called at beginning of each level. make level and populate with stars
//After the init() method finishes initializing your data structures / objects for the current
//level, it must return GWSTATUS_CONTINUE_GAME.
//virtual int move() = 0;   run a single tick: ask all to do sth; dispose dead stuff
//(by deleting its object and removing any reference to the object from the StudentWorlds data structures).
//virtual void cleanUp() = 0;   called when NachenBlaster completes the current level or loses a life
//responsible for freeing all actors that are currently in the game

//The code that you write must never call any of these three functions
class Actor;
class NachenBlaster;
class StudentWorld : public GameWorld
{
public:
	StudentWorld(std::string assetDir);
	//***********************Implement a constructor that initializes data members
	virtual int init();
	virtual int move();
	virtual void cleanUp();
	void addActor(Actor* actor);    //to add new actors such as newly shot cabbages
	void removeDeadObjects();       //to dispose of dead Actors
	bool isCurrentLevelCompleted() const;       //returns 1 if current level is completed by the NachenBlaster
	bool shouldWeAddNewAlien() const;       //returns 1 if a new alien ship should be added
	int whatAlien() const;      //decides what type of alien ship to add
	void incKilledAliens()      //increments the number of killed aliens in current level by 1
	{
		numberOfKilledAliensInCurrentLevel++;
	}
	bool isCollided(Actor* actor1, Actor* actor2) const;    //returns true of actors 1 and 2 have collided
	Actor* collidedWith(Actor* actor1);     //returns a pointer to the actor that actor 1 has collided with
											//if actor1 hasn't collided with anyone, returns nullptr
	NachenBlaster* getCollidingPlayer(Actor* actor) const;  //return pointer to NachenBlaster if collided
															//if hasn't collided returns nullptr
	NachenBlaster* getPlayer() const;       //returns pointer to NachenBlaster
	~StudentWorld();

private:
	std::list <Actor*> actorList;
	NachenBlaster* nachen;
	int alienShipsOnScreen() const;     //returns current number of alien ships on the screen
	int aliensCreatedInCurrentLevel;
	int aliensToKillInCurrentLevel;     //T
	int numberOfKilledAliensInCurrentLevel;     //D
	int aliensDied;     //number of aliens that got removed from actorList
};



#endif // STUDENTWORLD_H_