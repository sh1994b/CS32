#ifndef STUDENTWORLD_H_
#define STUDENTWORLD_H_

#include "GameWorld.h"
#include <string>
#include <list>

class Actor;
class NachenBlaster;
class StudentWorld : public GameWorld
{
public:
	StudentWorld(std::string assetDir);
	virtual int init();
	virtual int move();
	virtual void cleanUp();
	void addActor(Actor* actor);    //to add new actors such as newly shot cabbages
	void removeDeadObjects();       //to dispose of dead Actors
	bool isCurrentLevelCompleted() const;       //returns 1 if current level is completed by the NachenBlaster
	bool shouldWeAddNewAlien() const;		//returns 1 if a new alien ship should be added
	int whatAlien() const;		//decides what type of alien ship to add
	void incKilledAliens()		//increments the number of killed aliens in current level by 1
	{
		numberOfKilledAliensInCurrentLevel++;
	}
	bool isCollided(Actor* actor1, Actor* actor2) const;	//returns true of actors 1 and 2 have collided
	Actor* collidedWith(Actor* actor1);		//returns a pointer to the actor that actor 1 has collided with
													//if actor1 hasn't collided with anyone, returns nullptr
	NachenBlaster* getCollidingPlayer(Actor* actor) const;	//return pointer to NachenBlaster if collided
																	//if hasn't collided returns nullptr
	NachenBlaster* getPlayer() const;		//returns pointer to NachenBlaster
		~StudentWorld();

private:
	std::list <Actor*> actorList;
	NachenBlaster* nachen;
	int alienShipsOnScreen() const;		//returns current number of alien ships on the screen
	int aliensCreatedInCurrentLevel;
	int aliensToKillInCurrentLevel;		//T
	int numberOfKilledAliensInCurrentLevel;		//D
	int aliensDied;		//number of aliens that got removed from actorList
};



#endif // STUDENTWORLD_H_