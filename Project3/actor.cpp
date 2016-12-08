#include "Actor.h"
#include "StudentWorld.h"
#include "GameController.h"

// Students:  Add code to this file (if you wish), Actor.h, StudentWorld.h, and StudentWorld.cpp


////////////////////////////////
//CLASS Actor
////////////////////////////////


//TODO: make constructor parameters better. Dir, size, depth can probably be in body
Actor::Actor(StudentWorld *actor, int s_x, int s_y, int ID,  Direction dir, double size, unsigned int depth) : GraphObject(ID, s_x, s_y, dir, size, depth), getworld(actor)

{
    //TODO: SPECS SAY SETVISIBLE(TRUE)
    
    setVisible(true);
    
    
}

StudentWorld* Actor::getWorld() const
{
    return getworld;
}





void Actor::setAttackPoints(int attack)
{
    m_attackPoints = attack;
}


int Actor::getAttackPoints()
{
    return m_attackPoints;
}

bool Actor::overLapping(int x, int y, int ID)
{
    if(ID == IID_BOULDER)
    {
        for(int x_c = 0; x_c<4; x_c++)
            for(int y_c = 0; y_c<4; y_c++)
                if(getWorld()->objectExists(x+x_c, y+y_c, this,IID_BOULDER))
                    return true;
    }
    
    if(ID==10)
    {
        //TODO: MAYBE USE ALL HERE?
    }
    
    return false;
}



//virtual destructor
Actor::~Actor()
{}

//////////////////////////
///////////
////////////////////////////
//////////////////////////////





Characters::Characters(StudentWorld *actor, int x, int y, int ID, Direction dir, double size, unsigned int depth) : Actor(actor, x, y, ID, dir, size, depth), m_alive(true)
{
    
    
}






void Characters::setHitPoints(int hitpoints)
{
    m_hitpoints = hitpoints;
}

int Characters::getHitPoints()
{
    return m_hitpoints;
}


void Characters::decreaseHitPoints(int amount)
{
    m_hitpoints-=amount;
    
    if(m_hitpoints<=0)
        m_alive = false;
}


bool Characters::isAlive()
{
    if(!m_alive)
        return false;
    return true;
}



////////////////////////////////
//CLASS Frackman
////////////////////////////////



Frackman::Frackman(StudentWorld *actor) : Characters(actor, 30, 60, IID_PLAYER, right, 1.0, 0.0)
{
    setVisible(true);
    
    
    setHitPoints(10);
    m_waterUnits = 5;
    m_goldNugs = 1; //TODO: CHANGE THIS TO 0
    m_sonar = 1;
}


Frackman::~Frackman()
{}



void Frackman::doSomething()
{
    
    //first check to see if alive
    if(!isAlive())
        return;
    
    /*
     TODO: If the FrackMan’s 4x4 image currently overlaps any Dirt objects within the oil field, then it will call upon the StudentWorld object to:
     a. Remove/destroy the Dirt objects from the 4x4 area occupied by the FrackMan (from x, y to x+3,y+3 inclusive)
     b. Make a digging sound by playing the SOUND_DIG sound (see the StudentWorld section of this document for details on how to play a sound).
     */
    /*
     if escape: abort... set to dead
     if spacebar: squirt, reduce water count by 1
     
     */
    //delete all dirt near frackman
    //TODO: FIX THIS
    
    for(int x=0; x<4; x++)
        for(int y = 0; y<4; y++)
            if(getWorld()->dirtExists(getX()+x,getY()+y))
            {
                getWorld()->deleteDirt(getX()+x,getY()+y);
                getWorld()->playSound(SOUND_DIG);
            }
    
    
    
    int ch = 0;
    //TODO: HAVING A BUNCH OF PROBLEMS WITH THIS, WONT COMPILE
    if(getWorld()->getKey(ch)==true)
    {
        //if the
        if(ch==KEY_PRESS_ESCAPE)
        {
            decreaseHitPoints(1000);
            return;
        }
        else if(ch==KEY_PRESS_SPACE)
        {
            squirt();
        }
        else if(ch==KEY_PRESS_TAB)
        {
            dropGold();
        }
        //decode ch (change direction or move)
        else if(ch=='z' || ch=='Z')
        {
            sonar();
        }
        else move(ch);//TODO:: IMPLEMENT THIS
        
        
    }
    
    
}


void Frackman::sonar()
{
    if(m_sonar==0)
        return;
    
    getWorld()->useSonar();
    getWorld()->playSound(SOUND_SONAR);
    
    m_sonar--;
}




//TODO: MAKE SURE THIS WORKS
void Frackman::squirt()
{
    if(m_waterUnits==0)
        return;
    
    
    
    
    getWorld()->addSquirt();
    
    //TODO: SOUND NOT WORKING
    
    getWorld()->playSound(SOUND_PLAYER_SQUIRT);
    
    m_waterUnits--;
    
}

/*
 @param ch: inputted keystroke by user
 function checks if a player changes direction, if he did, then only change direction, do not move
 If player is already moving in one direction, and wants to keeps moving in that direction,
 and no rock is in the way, keep moving in that direction
 @return void
 */
void Frackman::move(int ch)
{
    
    
    //if user presses 'left'
    if(ch==KEY_PRESS_LEFT && getDirection()==left && getX()!=0 && !overLapping(getX()-1,getY(),IID_BOULDER))
        moveTo(getX()-1,getY());
    else if(ch==KEY_PRESS_LEFT && (getDirection()==right || getDirection() == up || getDirection()==down))
        setDirection(left);
    
    //if user presses 'right'
    if(ch==KEY_PRESS_RIGHT && getDirection()==right && getX()!=60 && !overLapping(getX()+1,getY(),IID_BOULDER))
        moveTo(getX()+1,getY());
    else if(ch==KEY_PRESS_RIGHT && (getDirection()==left || getDirection() == up || getDirection()==down))
        setDirection(right);
    
    
    //if user presses down
    if(ch==KEY_PRESS_DOWN && getDirection()==down && getY()!=0 && !overLapping(getX(),getY()-1,IID_BOULDER))
        moveTo(getX(),getY()-1);
    else if(ch==KEY_PRESS_DOWN && (getDirection()==left || getDirection() == up || getDirection()==right))
        setDirection(down);
    
    //user presses up
    if(ch==KEY_PRESS_UP && getDirection()==up && getY()!=60 && !overLapping(getX(),getY()+1,IID_BOULDER))
        moveTo(getX(),getY()+1);
    else if(ch==KEY_PRESS_UP && (getDirection()==left || getDirection() == right || getDirection()==down))
        setDirection(up);
    
    
    
    
}




void Frackman::pickUpObjects(int ID)
{
    if(ID==IID_GOLD)
    {
        m_goldNugs++;
    }
    if(ID==IID_SONAR)
    {
        m_sonar+=2; //TODO: Make sure this is correct
    }
    if(ID==IID_WATER_POOL)
    {
        m_waterUnits+=5;
    }
    
}


void Frackman::dropGold()
{
    
    if(m_goldNugs==0)
        return;
    
    getWorld()->addGold(getX(),getY(),true);
    m_goldNugs--;
    
}





////////////////////////////////
//CLASS PROTESTORS
////////////////////////////////

//TODO: DETERMINE WHERE PROTESTORS START... IM PUTTING THEM AT (60,60)

Protestor::Protestor(StudentWorld *actor, int ID)
:Characters(actor, 60, 60, ID, left, 1.0, 0), m_atExit(false)
{
    
    m_restState = 0;
    
    
    
    setHitPoints(5);
    setAttackPoints(2);
    
    setLeaveField(false);
    setTicksToWait(false);
    
    
    
}



void Protestor::doSomething()
{
    if(!isAlive())
        return;
    if(getHitPoints()<=0 && !leavingField())
    {
        setLeaveField(true);
        setTicksToWait(false);
    }
    
    
    //if not alive, return immediately, (i dont think this will ever get called...)
    
    //que based algorithm to find exit, stop when at end
    if(leavingField())
    {
        if(getX()==60 && getY()==60)
        {
            setAtExit(true);
            return;
        }
        
        
        moveTo(getX()+1, getY());
        //if the protestor is at the end (60,60), set atExit to true
        if(this->getX()==60 && this->getY()==60)
        {
            //this essentially sets it to dead
            
            return;
        }
        
        
        //TODO: IMPLEMENT QUE BASED EXIT
        
        return;
    }
    
    
    
    
    //if the protestor cannot move this turn(ticks to wait), decrement ticks to wait, move on
    if(getTicksToWait()!=0)
    {
        m_ticksToWait--;
        return;
    }
    
    
    
    
    
    if(getTicksToWait()==0 && isAlive())
    {
        moveTo(getX()-1,getY()); //TODO: CHANGE THIS BACK
        if(getWorld()->withinRadiusOfFrackman(this, 4) && getWorld()->protestorFacingFrackman(this))
        {
            //TODO: this should be either > or >=
            if(getLastShout()>15)
            {
                shout();
            }
            
            setLastShout();
            return;
        }
        
        
        
        
        
        //TODO: implement movement
    }
    
    //if the protestor is in a rest state, skip this tur
    //TODOO: FIX THIS with bool resting()
    if(m_restState)
        return;
    
    
    
    //at the end, set the number of ticks to wait, only if the protestor is not leaving oil field
    if(!leavingField())
        setTicksToWait(false);
    
}

//if leaving field, no need to wait
//if stunned, set new ticks to wait
//if not stunned, but ticks are up and protestor made a move, reset ticks
//else decrement ticks to wait
void Protestor::setTicksToWait(bool stun)
{
    if(leavingField())
        m_ticksToWait=0;
    
    if(stun)
    {
        int m_ticksToWait = 100-getWorld()->getLevel()*10;
        if(50<m_ticksToWait)
            m_ticksToWait = 50;
        return;
        
    }
    
    //this is a safeguard against starting position
    if(getTicksToWait()!=0 && getX()!=60 && getY()!=60)
    {
        m_ticksToWait--;
        return;
    }
    
    //max(0,3-level/4)
    if(!stun)
    {
        m_ticksToWait = 0;
        if((0<3-getWorld()->getLevel()/4))
            m_ticksToWait = 3-getWorld()->getLevel()/4;
        
        
        return;
    }
    
    
}

int Protestor::getTicksToWait()
{
    return m_ticksToWait;
}

void Protestor::shout()
{
    getWorld()->playSound(SOUND_PROTESTER_YELL);
    getWorld()->attackFrackman(getAttackPoints());
}

void Protestor::setLastShout()
{
    m_lastShout=0;
}

int Protestor::getLastShout()
{
    return m_lastShout;
}


//TODO: not sure if this works
void Protestor::decreaseHitPoints(int amount)
{
    //protestors cannot be annoyed if leaving field
    if(leavingField())
        return;
    else
    {
        Characters::decreaseHitPoints(amount);
        if(getHitPoints()>0)
        {
            getWorld()->playSound(SOUND_PROTESTER_ANNOYED);
            setTicksToWait(true);
        }
        else
        {
            setLeaveField(true);
            getWorld()->playSound(SOUND_PROTESTER_GIVE_UP);
            setTicksToWait(false);
        }
        
    }
    
}

void Protestor::foundGold(bool youStillGotsNoMoneyTho)
{
    if(youStillGotsNoMoneyTho)//get outta here
        setLeaveField(true);
    
}


bool Protestor::isAlive()
{
    if(atExitOfField())
        return false;
    
    return true;
}



/* Each Regular Protester must decide how many squares,
 numSquaresToMoveInCurrentDirection, it will decide to move left before possibly switching its direction. This value must be: 8 <= numSquaresToMoveInCurrentDirection <= 60*/




void Protestor::setLeaveField(bool leave)
{
    leaveOilField = leave;
}
bool Protestor::leavingField()
{
    return leaveOilField;
}

bool Protestor::atExitOfField()
{
    return m_atExit;
}

void Protestor::setAtExit(bool yayOrNay)
{
    m_atExit=yayOrNay;
}





////////////////////////////////
//CLASS HARDCORE PROTESTORS
////////////////////////////////

HardCoreProtestor::HardCoreProtestor(StudentWorld *actor):Protestor(actor, IID_HARD_CORE_PROTESTER)
{
    setLeaveField(false);
    
    ///TODO: PROLLY NEED TO FIX THIS
    setTicksToWait(false);
    
    
}

















////////////////////////////////
//CLASS Objects
////////////////////////////////






//going to say all objects have m_hitpoints = 1, then when m_alive is false, m_hitpoints will be set to -1
Objects::Objects(StudentWorld* Object, int s_x, int s_y, int ID, Direction dir, double size, unsigned int depth)
:Actor(Object, s_x, s_y, ID, dir, size, depth)
{
    m_alive = true;
    setVisible(true);
}




void Objects::setTicksLeft(int numTicks)
{
    m_ticksLeft = numTicks;
}

int Objects::getTicksLeft()
{
    return m_ticksLeft;
}




void Objects::setRadius(double radius)
{
    radiusOfAction = radius;
}

double Objects::getRadiusOfAct()
{
    return radiusOfAction;
}


//THIS IS GOING TO BE USED BY ALL ACTORS
bool Objects::isAlive()
{
    if(!m_alive)
        return false;
    
    return true;
}

void Objects::isNowDead()
{
    m_alive = false;
}



///PICKUPABLE AND WHATNOT

void Objects::setPickupableByFrack(bool tOrF)
{
    m_pickupableByFrack = tOrF;
}

bool Objects::isPickupableByFrack()
{
    return m_pickupableByFrack;
}


////THESE ARE FOR SCORE INCREASING WITH GOODIES OR OBJECTS

void Objects::setScorePoints(int amount)
{
    scoreIncrease = amount;
}

int Objects::getScoreIncrease()
{
    return scoreIncrease;
}




////////////////////////////////
//CLASS Boulder
////////////////////////////////

//Constructor
Boulder::Boulder(StudentWorld* Object, int x, int y)
:Objects(Object, x, y, IID_BOULDER, down, 1.0, 1)
{
    is_stable = true;
    m_falling = false;
    
    setTicksLeft(31); //using this as the waiting period
    setRadius(3);
    setAttackPoints(100);
    
}


/*
 return true if dirt is under boulder
 return false if no dirt under boulder
 */
bool Boulder::dirtBeneath()
{
    
    int xCoord = this->getX();
    int yCoord = this->getY();
    
    
    for(int i = 0; i<4; i++)
    {
        if(getWorld()->dirtExists(xCoord+i, yCoord-1))
        {
            return true;
        }
    }
    
    return false;
    
}

/*
 return true if a boulder is under boulder
 return false if no boulder is under boulder
 */
bool Boulder::boulderBeneath()
{
    int xCoord = this->getX();
    int yCoord = this->getY();
    
    for(int i = 0; i<4; i++)
    {
        if(getWorld()->objectExists(xCoord+i, yCoord-1, this, IID_BOULDER))
        {
            return true;
        }
    }
    
    return false;
}




/*
 If the boulder is not alive, return
 
 if the boulder is falling,
 fall more
 */
//      finish falling state
//TODO: MAKE BOULDER CAUSE ANNOYANCE IF IT COMES NEAR FRACKMAN OR PROTESTORS
void Boulder::doSomething()
{
    
    
    if(!isAlive())
        return;
    
    /*If the object is in the falling state
     if  there is nothing beneath it,
     move down 1 spot
     if there is a boulder or dirt beneath it, set to dead
     */
    if(m_falling)
    {
        
        //if there is a boulder or dirt beneath, or tries to move out of bounds
        //set m_alive to false, and return
        if(dirtBeneath() || boulderBeneath() || getY()==0)
        {
            isNowDead();
            return;
        }
        if(getWorld()->withinRadiusOfFrackman(this, getRadiusOfAct()))
        {
            getWorld()->attackFrackman(getAttackPoints());
        }
        
        //If rock is not dead, play sound and fall each tick, then return
        getWorld()->playSound(SOUND_FALLING_ROCK);
        moveTo(getX(),getY()-1);
        return;
        
        
        
    }
    
    //TODO: Do i need to play a sound when m_waiting turns to zero?
    if(getTicksLeft()!=31)
    {
        if(getTicksLeft() == 0)
        {
            m_falling = true;
            return;
        }
        
        setTicksLeft(getTicksLeft()-1);
        return;
    }
    //only check for dirt beneath, if no dirt, set the ticks left process in motion
    //after 30 ticks the boulder will fall
    if(!dirtBeneath())
    {
        setTicksLeft(30);
        return;
        
    }
    
    
    
}
/*
 if falling==true
 decrement position by one, return
 if the boulder is in m_waiting=true
 m_waiting--
 if (m_waiting ==0)
 fall, play sound
 else (check if stable)
 if not stable --> set to m_waiting
 if stable --> do nothing
 */






////////////////////////////////
//CLASS Squirt
////////////////////////////////
//the one at the end is hitpoints, it will turn to -1 when the object dies

Squirt::Squirt(StudentWorld* object, int x, int y, Direction dir) :Objects(object, x, y, IID_WATER_SPURT, dir, 1.0, 1)
{
    setVisible(true);
    
    setTicksLeft(4);
    setAttackPoints(2);
    setScorePoints(10);
    
}

//squirt attack
//TODO: FINISH IMPLEMENTING
void Squirt::doSomething()
{
    if(getTicksLeft()==0)
        isNowDead();
    
    /* TODO: FINISH ATTACKING PROTESTOR
     for(int i = 0; i<getWorld()-; i++)
     {
     if(getWorld()->retrieveObject(i)->getID()==IID_PROTESTER)
     {
     if(getWorld()->withinRadiusPlacingObjects(getWorld()->retrieveObject(i)->getX(),getWorld()->retrieveObject(i)->getY(), 3.0))
     {
     
     }
     
     
     }
     }*/
    //10 is to check for all protestors
    
    
    if(getWorld()->attackProtestor(IID_WATER_SPURT, getAttackPoints(), 3.0, this))
    {
        isNowDead();
        return;
    }
    
    /*
     if(getWorld()->withinRadiusOfObjects(getX(),getY(), 3.0, 10))
     {
     isNowDead();
     
     attackProtestor(IID_
     
     
     
     //TODO: NO IDEA WHY OFF BY ONE ERROR HERE
     //getWorld()->protestorFoundGold(protXCoord, protYCoord, 25);
     
     
     
     }
     */
    
    
    //decide which way to move the squirt
    int x = 0, y = 0;
    if(this->getDirection()==up)
    {
        y=1;
    }
    else if(this->getDirection()==right)
    {
        x=1;
    }
    else if(this->getDirection()==left)
    {
        x=-1;
    }
    else if(this->getDirection()==down)
    {
        y=-1;
    }
    
    int newX = getX()+x;
    int newY = getY()+y;
    
    //check if there is any dirt there
    for(int x = 0; x<4;x++)
        for(int y = 0; y<4;y++)
            if(getWorld()->dirtExists(newX+x, newY+y))
            {
                isNowDead();
                return;
            }
    
    //check if there are any boulders
    if(overLapping(newX,newY,IID_BOULDER))
    {
        isNowDead();
        return;
    }
    
    moveTo(newX,newY);
    
    setTicksLeft(getTicksLeft()-1);
    
    return;
}



////////////////////////////////
//CLASS GOLD
////////////////////////////////
//the one at the end is hitpoints, it will turn to -1 when the object dies

Gold::Gold(StudentWorld* object, int x, int y, bool placedByFrack) : Objects(object, x, y, IID_GOLD, right, 1.0, 2)
{
    
    
    
    //this will occur when objects are placed at start of game
    //will be set to visible when frackman drops gold
    if(!placedByFrack)
    {
        
        setVisible(false);
        setPickupableByFrack(true);
        m_permanent = true;
        //this could be anything >0, just cannot be <=0.
        setTicksLeft(100000);
        
    }
    
    //if the frackman places the gold, he cannot pick it up.. else he can pick it p
    if(placedByFrack)
    {
        setVisible(true);
        setPickupableByFrack(false);
        m_permanent = false;
        setTicksLeft(100);
    }
    
    
    //NOT SURE HOW TO USE THIS YET
    //setRadius(4);
    
}


void Gold::doSomething()
{
    if(!isAlive())
    {
        return;
    }
    //TODO: FIX THE VISIBILITY ISSUE
    if(!isVisible() && getWorld()->withinRadiusOfFrackman(this,4))
    {
        setVisible(true);
        return;
    }
    
    if(isPickupableByFrack() && getWorld()->withinRadiusOfFrackman(this, 3.0))
    {
        isNowDead();
        getWorld()->gotGoodie(IID_GOLD,getScoreIncrease());
        return;
    }
    
    
    //TODO: FINISH IMPLEMENTING THIS,, (PROTESTORS AND WHATNOT)
    if(!isPickupableByFrack())
    {
        int x_coord = getX();
        int y_coord = getY();
        
        //10 to check for all protestors,
        if(getWorld()->withinRadiusOfObjects(x_coord,y_coord, 3, 10))
        {
            isNowDead();
            getWorld()->playSound(SOUND_PROTESTER_FOUND_GOLD);
            
            //TODO: NO IDEA WHY OFF BY ONE ERROR HERE
            getWorld()->protestorFoundGold(x_coord, y_coord, 25);
            
            
            
        }
        
        
        
        
        setTicksLeft(getTicksLeft()-1);
    }
    
    if(getTicksLeft()==0)
        isNowDead();
    
    
}




////////////////////////////////
//CLASS SONAR
////////////////////////////////

SonarKit::SonarKit(StudentWorld *actor) : Objects(actor, 0, 60, IID_SONAR, right, 1.0, 2)
{
    
    //setVisible(true);
    
    setPickupableByFrack(true);
    
    int ticksLeft = 100;
    if(300-10*getWorld()->getLevel()<100)
    {
        ticksLeft = 300 - 10*getWorld()->getLevel();
    }
    
    setTicksLeft(ticksLeft);
    setScorePoints(75);
    
}


void SonarKit::doSomething()
{
    if(!isAlive())
    {
        return;
    }
    
    
    if(getWorld()->withinRadiusOfFrackman(this, 3))
    {
        isNowDead();
        getWorld()->gotGoodie(IID_SONAR, getScoreIncrease());
        
    }
    
    
    //decrement ticks
    setTicksLeft(getTicksLeft()-1);
    
    //if no more ticks, dead
    if(getTicksLeft()==0)
        isNowDead();
}





////////////////////////////////
//CLASS WATERPOOL
////////////////////////////////


WaterPool::WaterPool(StudentWorld *actor, int x, int y) : Objects(actor, x, y, IID_WATER_POOL, right, 1.0, 2)
{
    setPickupableByFrack(true);
    
    //TODO: THIS COULD PROBABLY BE NICER
    int ticksLeft = 100;
    if(300-10*getWorld()->getLevel()<100)
    {
        ticksLeft = 300 - 10*getWorld()->getLevel();
    }
    
    setTicksLeft(ticksLeft);
    setScorePoints(100);
    
}

void WaterPool::doSomething()
{
    if(!isAlive())
        return;
    
    
    //TODO:: FIX ALL THIS RADIUS SHIT
    if(getWorld()->withinRadiusOfFrackman(this, 3))
    {
        isNowDead();
        getWorld()->gotGoodie(IID_WATER_POOL, getScoreIncrease());
        
    }
    
    
    
    
    //decrement ticks, check if aliv
    setTicksLeft(getTicksLeft()-1);
    if(getTicksLeft()==0)
        isNowDead();
}


////////////////////////////////
//CLASS Oil Barrel
////////////////////////////////

OilBarrel::OilBarrel(StudentWorld *actor, int x, int y) : Objects(actor, x, y, IID_BARREL, right, 1.0, 2)
{
    setVisible(false);
    setScorePoints(1000);
}

void OilBarrel::doSomething()
{
    if(!isAlive())
        return;
    
    //TODO: FIX RADIUS SHIT
    if(!isVisible() && getWorld()->withinRadiusOfFrackman(this, 4.0))
    {
        setVisible(true);
        return;
    }
    if(getWorld()->withinRadiusOfFrackman(this, 3.0))
    {
        isNowDead();
        
        getWorld()->gotGoodie(IID_BARREL, getScoreIncrease());
        getWorld()->playSound(SOUND_FOUND_OIL);
        
    }
    
    
    
    
}




////////////////////////////////
//CLASS Dirt
////////////////////////////////
//TODO: SHOULD THIS BE DERIVED FROM ACTOR OR GRAPHOBJECT????
Dirt::Dirt(StudentWorld* actor, int x, int y)
:GraphObject(IID_DIRT, x, y, right, .25, 3.0)
{
    
    setVisible(true);
}



















