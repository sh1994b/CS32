#include "Actor.h"
#include "StudentWorld.h"


Actor::Actor(int imageID, double startX, double startY, Direction dir, double size, unsigned int depth, StudentWorld* thisWorld)
	:GraphObject(imageID, startX, startY, dir, size, depth), m_dead(0), sw(thisWorld)
{}


void Actor::die()
{
	m_dead = 1;
}

bool Actor::isDead() const
{
	return m_dead;
}

StudentWorld* Actor::getWorld() const
{
	return sw;
}

bool Actor::doMainActs()
{
	if (isDead())
		return 1;
	if (getX() <= -1 || getX() > VIEW_WIDTH)
		die();
	return 0;
}


void Explosion::doSomething()
{
	setSize(1.5 * getSize());
	if (ticksPassed >= 4)
		die();
	ticksPassed++;
}


//*****************see if you can put some stuff in base class' doSomething()
void Star::doSomething()
{
	if (doMainActs())       //checks if star is dead or flown off screen
		return;
	else
	{
		moveTo(getX() - 1, getY());
	}
}


void Projectile::checkForCollisions(int damage, bool alienOrPlayer)
{
	if (!alienOrPlayer)     //if attacked object is an alien
	{
		Actor* ac = getWorld()->collidedWith(this);
		//if actor has collided with an alien
		if (ac != nullptr)
		{
			getWorld()->playSound(SOUND_BLAST);
			Alien* al = static_cast<Alien*>(ac);  //change actor pointer to be an alien pointer
			al->sufferDamage(damage);     //damage alien ship
			die();
			return;
		}
	}
	else        //if attacked object is the NachenBlaster
	{
		NachenBlaster* nachen = getWorld()->getCollidingPlayer(this);
		if (nachen != nullptr)
		{
			nachen->sufferDamage(damage);
			die();
		}
	}
}


//alienOrPlayer is 0 when attacked object is an alien and 1 when it's the NachenBlaster
//horizontalMovement is the amount that projectile should move considering direction
//rotateOrNot is 0 when Projectile shouldn't rotate and 1 when it should
//playerDamage is how much damage the Projectile should cause
void Projectile::doCommonThings(bool alienOrPlayer, int horizontalMovement, bool rotateOrNot, int playerDamage)
{
	if (doMainActs())       //checks if Projectile is dead or flown off screen
		return;

	checkForCollisions(playerDamage, alienOrPlayer);

	moveTo(getX() + horizontalMovement, getY());

	if (rotateOrNot)
		setDirection(getDirection() + 20);

	checkForCollisions(playerDamage, alienOrPlayer);
}



void Spaceship::sufferDamage(int byHowMuch)
{
	if (m_hitPoints - byHowMuch >= 0)
		m_hitPoints -= byHowMuch;
	else
		m_hitPoints = 0;
}

void Spaceship::incHitPoints(int byHowMuch) //*****************can you combine with sufferDamage?
{
	if (m_hitPoints + byHowMuch <= 50)
		m_hitPoints += byHowMuch;
	else
		m_hitPoints = 50;
}

//direction is either 0 or 180 depending on what ship is firing a projectile
//in case of Player using fire(), cabbageOrTorpedo must be 0 when cabbage is shot
//and 1 if Torpedo is shot
void Spaceship::fire(int soundID, Direction dir, std::string shipType, bool cabbageOrTorpedo)
{
	getWorld()->playSound(soundID);

	if (shipType == "Smallgon" || shipType == "Smoregon")
		projectileFired = new Turnip(getX() - 14, getY(), getWorld());

	else if (shipType == "Snagglegon")
		projectileFired = new AlienTorpedo(getX() - 14, getY(), getWorld());

	else if (shipType == "Player" && cabbageOrTorpedo)
		projectileFired = new PlayerTorpedo(getX() + 12, getY(), getWorld());

	else if (shipType == "Player" && !cabbageOrTorpedo)
		projectileFired = new Cabbage(getX() + 12, getY(), getWorld());

	getWorld()->addActor(projectileFired);
}



void NachenBlaster::doSomething()
{
	if (isDead())
		return;
	if (getHitPoints() <= 0)
		die();

	int ch;
	if (getWorld()->getKey(ch))
	{
		switch (ch)
		{
		case KEY_PRESS_LEFT:
			if (getX() > 0)
				moveTo(getX() - 6, getY());
			break;
		case KEY_PRESS_RIGHT:
			if (getX() < VIEW_WIDTH - 1)
				moveTo(getX() + 6, getY());
			break;
		case KEY_PRESS_UP:
			if (getY() < VIEW_HEIGHT - 1)
				moveTo(getX(), getY() + 6);
			break;
		case KEY_PRESS_DOWN:
			if (getY() > 0)
				moveTo(getX(), getY() - 6);
			break;
		case KEY_PRESS_SPACE:
			if (cabbagePoints >= 5)
			{
				cabbagePoints -= 5;
				fire(SOUND_PLAYER_SHOOT, 0, "Player");
			}
			else
				;
		case KEY_PRESS_TAB:
			if (fTorpedo >= 1)
			{
				fTorpedo--;
				fire(SOUND_TORPEDO, 0, "Player");
			}
			else
				;
		default:
			;


		}
	}
	else
		;
}


int NachenBlaster::getHealth() const
{
	//health is percentage of current hit points (out of total hit points)
	double health = getHitPoints() / 50 * 100;
	return (int)health;
}

int NachenBlaster::getCabbages() const
{
	//"Cabbages" is the percentage of available cabbage supply
	double cab = (double)cabbagePoints / 30 * 100;
	return (int)cab;
}


void NachenBlaster::incCabbagePoints()
{
	cabbagePoints++;
}

void NachenBlaster::incTorpedo(int byHowMuch)
{
	fTorpedo += byHowMuch;
}


void Cabbage::doSomething()
{
	doCommonThings(0, 8, 1, 2);
}

void Turnip::doSomething()
{
	doCommonThings(1, -6, 1, 2);
}

void PlayerTorpedo::doSomething()
{
	doCommonThings(0, 8, 0, 8);
}

void AlienTorpedo::doSomething()
{
	doCommonThings(1, -8, 0, 8);
}


void Alien::checkForCollisions(int damage, bool isSmoregon)
{
	int increaseInScore = 250;
	if (damage == 15)   //was a snagglesgon
		increaseInScore = 1000;

	NachenBlaster* nachen = getWorld()->getCollidingPlayer(this);

	//if alien ship collided with the NachenBlaster itself:
	if (nachen != nullptr)
	{
		nachen->sufferDamage(damage);
		if (nachen->getHitPoints() >= 0)  //if it hit the player ship but didn't kill it
			getWorld()->playSound(SOUND_BLAST);
	}
	//if alien ship died by a projectile fired at it or by colliding with the player
	if (nachen != nullptr || getHitPoints() <= 0)
	{
		die();
		getWorld()->incKilledAliens();       //increase # of killed aliens in current level
		getWorld()->increaseScore(increaseInScore);
		getWorld()->playSound(SOUND_DEATH);
		getWorld()->addActor(new Explosion(getX(), getY(), getWorld()));

		if (damage == 15) //it was a snagglegon
			if (randInt(1, 6) == 1)
				getWorld()->addActor(new ExtraLifeGoodie(getX(), getY(), getWorld()));
		if (isSmoregon)
			if (randInt(1, 3) == 3)
			{
				if (randInt(1, 2) == 1)
					getWorld()->addActor(new RepairGoodie(getX(), getY(), getWorld()));
				else
					getWorld()->addActor(new TorpedoGoodie(getX(), getY(), getWorld()));
			}
	}

}

void Alien::doCommonThings(bool isSnagglegon, bool isSmoregon)
{
	if (doMainActs())       //checks if alien is dead or flown off screen
		return;

	int damage = 5;
	if (isSnagglegon)
		damage = 15;

	checkForCollisions(damage, isSmoregon);

	if (getY() >= VIEW_HEIGHT - 1)
		m_travelDirection = 1;
	else if (getY() <= 0)
		m_travelDirection = 2;
	else if (!isSnagglegon && m_flightPlan == 0)
	{
		m_travelDirection = randInt(0, 2);
		m_flightPlan = randInt(1, 32);
	}

	int a = 20;
	if (isSnagglegon)
		a = 15;
	int b = 5;
	if (isSnagglegon)
		b = 10;
	NachenBlaster* p = getWorld()->getPlayer();
	if (p->getX() < getX() && p->getY() <= getY() + 4 && p->getY() >= getY() - 4)
		if (randInt(1, (a / getWorld()->getLevel()) + b) == 1)
		{
			if (!isSnagglegon)
				fire(SOUND_ALIEN_SHOOT, 180, "Smallgon");
			else
				fire(SOUND_TORPEDO, 180, "Snagglegon");

		}

	if (m_travelDirection == 2)
		moveTo(getX() - m_travelSpeed, getY() + m_travelSpeed);
	if (m_travelDirection == 1)
		moveTo(getX() - m_travelSpeed, getY() - m_travelSpeed);
	if (m_travelDirection == 0)
		moveTo(getX() - m_travelSpeed, getY());

	checkForCollisions(damage, isSmoregon);
}


void Smallgon::doSomething()
{
	doCommonThings(0, 0);
}

void Smoregon::doSomething()
{
	doCommonThings(0, 1);
}

void Snagglegon::doSomething()
{
	doCommonThings(1, 0);
}

bool Goodie::doMainActs()
{
	if (isDead())
		return 1;
	if (getY() <= -1)
		die();
	return 0;
}

bool Goodie::checkForCollisions()
{
	NachenBlaster* nachen = getWorld()->getCollidingPlayer(this);
	if (nachen != nullptr)
	{
		getWorld()->increaseScore(100);
		die();
		getWorld()->playSound(SOUND_GOODIE);
		grantReward(nachen);
		return 1;
	}
	return 0;
}


void Goodie::doCommonThings()
{
	if (doMainActs())       //checks if goodie is dead or flown off screen
		return;
	if (checkForCollisions())
		return;
	moveTo(getX() - .75, getY() - .75);
	checkForCollisions();
}

void ExtraLifeGoodie::grantReward(NachenBlaster* nachen)
{
	getWorld()->incLives();
}

void RepairGoodie::grantReward(NachenBlaster* nachen)
{
	nachen->incHitPoints(10);
}

void TorpedoGoodie::grantReward(NachenBlaster* nachen)
{
	nachen->incTorpedo(5);
}

void Goodie::doSomething()
{
	doCommonThings();
}