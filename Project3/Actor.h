#ifndef ACTOR_H_
#define ACTOR_H_

#include "GraphObject.h"
#include "StudentWorld.h"
#include <stack>
#include <string>
using namespace std;

// contains base, NachenBlaster, and Star class declarations
// as well as constants required by these classes 

class Actor : public GraphObject
{
public:
	Actor(int imageID, double startX, double startY, Direction dir, double size, unsigned int depth, StudentWorld* thisWorld);
	virtual void doSomething() = 0;
	void die();
	bool isDead() const;
	StudentWorld* getWorld() const;
	virtual bool isAlien() const        //returns 1 if Actor is an alien, 0 otherwise
	{
		return 0;
	}
	virtual bool doMainActs();      //if actor is dead, returns 1
									//if actor has flown out of screen, sets its status to dead
private:
	bool m_dead;        //dead/alive status
	StudentWorld* sw;
};


class Explosion : public Actor
{
public:
	Explosion(double x, double y, StudentWorld* thisWorld)
		:Actor(IID_EXPLOSION, x, y, 0, 1, 0, thisWorld), ticksPassed(0)
	{}
	void doSomething();
private:
	int ticksPassed;
};


class Star : public Actor
{
public:
	Star(int width, StudentWorld* thisWorld)
		:Actor(IID_STAR, width, randInt(0, VIEW_HEIGHT - 1), 0, (double)randInt(5, 50) / 100, 3, thisWorld)
	{}
	void doSomething();
};


class Goodie : public Actor
{
public:
	Goodie(int imageID, double startX, double startY, StudentWorld* thisWorld, string goodieType)
		:Actor(imageID, startX, startY, 0, .5, 1, thisWorld), m_goodieType(goodieType)
	{}
	void doSomething();
	bool checkForCollisions();
	void doCommonThings();
	virtual bool doMainActs();
	virtual void grantReward(NachenBlaster* nachen) = 0;
private:
	string m_goodieType;

};


class ExtraLifeGoodie : public Goodie
{
public:
	ExtraLifeGoodie(double startX, double startY, StudentWorld* thisWorld)
		:Goodie(IID_LIFE_GOODIE, startX, startY, thisWorld, "life")
	{}
	void grantReward(NachenBlaster* nachen);
};

class RepairGoodie : public Goodie
{
public:
	RepairGoodie(double startX, double startY, StudentWorld* thisWorld)
		:Goodie(IID_REPAIR_GOODIE, startX, startY, thisWorld, "repair")
	{}
	void grantReward(NachenBlaster* nachen);
};

class TorpedoGoodie : public Goodie
{
public:
	TorpedoGoodie(double startX, double startY, StudentWorld* thisWorld)
		:Goodie(IID_TORPEDO_GOODIE, startX, startY, thisWorld, "torpedo")
	{}
	void grantReward(NachenBlaster* nachen);
};

class Projectile : public Actor
{
public:
	Projectile(int imageID, double startX, double startY, StudentWorld* thisWorld, int damage, Direction dir)
		:Actor(imageID, startX, startY, dir, .5, 1, thisWorld), damagePoints(damage)
	{}
	virtual void doSomething() = 0;
	virtual void doCommonThings(bool alienOrPlayer, int horizontalMovement, bool rotateOrNot, int playerDamage);
	virtual void checkForCollisions(int damage, bool alienOrPlayer);
private:
	int damagePoints;
};



class Spaceship : public Actor
{
public:
	Spaceship(int imageID, double startX, double startY, Direction dir, double size, unsigned int depth, double hitPoints, StudentWorld* thisWorld)
		:Actor(imageID, startX, startY, dir, size, depth, thisWorld), m_hitPoints(hitPoints)
	{}
	virtual void doSomething() = 0;
	void fire(int soundID, Direction dir, std::string shipType, bool cabbageOrTorpedo = 0);
	double getHitPoints() const
	{
		return m_hitPoints;
	}
	void sufferDamage(int byHowMuch);   //decrease ship's hitPoints by specified value
	void incHitPoints(int byHowMuch);
private:
	double m_hitPoints;
	Projectile* projectileFired;
};




class Alien : public Spaceship
{
public:
	Alien(StudentWorld* thisWorld, int imageID, double hitPoints, int flightPlan, double speed, int direction = 0)
		:Spaceship(imageID, VIEW_WIDTH - 1, randInt(0, VIEW_HEIGHT - 1), 0, 1.5, 1, hitPoints, thisWorld), m_flightPlan(flightPlan), m_travelSpeed(speed),
		m_travelDirection(direction), m_goodie(nullptr)
	{}
	virtual void doSomething() = 0;
	virtual void doCommonThings(bool isSnagglegon, bool isSmoregon);
	virtual bool isAlien() const
	{
		return 1;
	}
	void checkForCollisions(int damage, bool isSmoregon);
private:
	int m_flightPlan;
	double m_travelSpeed;
	int m_travelDirection;  //0:left, 1:down and left, 2:up and left
	Goodie* m_goodie;
};


class Cabbage;
class NachenBlaster : public Spaceship
{
public:
	NachenBlaster(StudentWorld* thisWorld)
		:Spaceship(IID_NACHENBLASTER, 0, 128, 0, 1.0, 0, 50, thisWorld), cabbagePoints(30), fTorpedo(0)
	{}
	void doSomething();
	int getHealth() const;
	int getCabbages() const;    //returns the Cabbages percentage
	int getCabbagePoints() const        //returns number of cabbage energy points
	{
		return cabbagePoints;
	}
	void incCabbagePoints();        //increments cabbage points by one
	void incTorpedo(int byHowMuch);
	int getTorpedoes() const
	{
		return fTorpedo;
	}
private:
	int cabbagePoints;
	int fTorpedo;
};


class Cabbage : public Projectile
{
public:
	Cabbage(double startX, double startY, StudentWorld* thisWorld)
		:Projectile(IID_CABBAGE, startX, startY, thisWorld, 2, 0)
	{}
	void doSomething();

};


class Turnip : public Projectile
{
public:
	Turnip(double startX, double startY, StudentWorld* thisWorld)
		:Projectile(IID_TURNIP, startX, startY, thisWorld, 2, 0)
	{}
	void doSomething();
};


class Torpedo : public Projectile
{
public:
	Torpedo(double startX, double startY, StudentWorld* thisWorld, Direction dir)
		:Projectile(IID_TORPEDO, startX, startY, thisWorld, 8, dir)
	{}
	void doSomething() = 0;
};

class PlayerTorpedo : public Torpedo
{
public:
	PlayerTorpedo(double startX, double startY, StudentWorld* thisWorld)
		:Torpedo(startX, startY, thisWorld, 0)
	{}
	void doSomething();
};

class AlienTorpedo : public Torpedo
{
public:
	AlienTorpedo(double startX, double startY, StudentWorld* thisWorld)
		:Torpedo(startX, startY, thisWorld, 180)
	{}
	void doSomething();
};


class Smallgon :public Alien
{
public:
	Smallgon(StudentWorld* thisWorld)
		:Alien(thisWorld, IID_SMALLGON, 5 * (1 + (thisWorld->getLevel() - 1) * .1), 0, 2.0)
	{}
	void doSomething();
};



class Smoregon :public Alien
{
public:
	Smoregon(StudentWorld* thisWorld)
		:Alien(thisWorld, IID_SMOREGON, 5 * (1 + (thisWorld->getLevel() - 1) * .1), 0, 2.0)
	{}
	void doSomething();
};



class Snagglegon :public Alien
{
public:
	Snagglegon(StudentWorld* thisWorld)
		:Alien(thisWorld, IID_SNAGGLEGON, 10 * (1 + (thisWorld->getLevel() - 1) * .1), 1, 1.75, 1)
	{}
	void doSomething();
};




#endif // ACTOR_H_