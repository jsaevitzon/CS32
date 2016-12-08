#ifndef ACTOR_H_
#define ACTOR_H_

#include "GraphObject.h"
#include "GameWorld.h"

// Students:  Add code to this file, Actor.cpp, StudentWorld.h, and StudentWorld.cpp


//This file will contain classes for:
//Protestor
//Hardcore Protestor
//Water squirts (that can be shot by the FrackMan)
//Barrels of oil
//Boulders
//gold nuggets
//sonar kits
//water refills
//dirts


//TODO: MAKE SURE THAT THE RETURN TYPES ARE CORRECT


/* May use
 
 GraphObject(int imageID, int startX, int startY, DIRECTION startDirection,float!size= 1.0, unsigned int depth = 0);
 void setVisible(bool shouldIDisplay);
 void getX() const;
 void getY() const;
 void moveTo(int x, int y);
 
 
 DIRECTION getDirection() const; //!Directions:up,down,! eft, right
 void setDirection(DIRECTION d);! //!Directions:!up,!down,!left,!right!
 
 
 */


/*
 things like:
 
 vector<Actor> actors*
 
 
 
 
 */
class StudentWorld;

class Actor : public GraphObject
{
public:
    //every actor exists in student world, has a starting position, an ID, a direction
    Actor(StudentWorld* actor, int s_x, int s_y, int ID, Direction dir, double size, unsigned int depth);
    virtual ~Actor(); //need virtual destructor, may allocate memory dynamically
    
    
    /*
     This function will be used to return a pointer
     to StudentWorld. Member functions of all derived
     classes may call this to access member functions
     in StudenWorld
     */
    StudentWorld* getWorld() const;
    
    
    
    
    /*
     Every derived function will have a doSomething()
     member function. StudentWorld's move() calls each
     Actor's doSomething() method each tick. In charge
     of moving, or any other action taken
     */
    virtual void doSomething() = 0;
    
    //dead or alive
    /*
     Each Actor starts off alive. This function returns
     a boolean value to determine whether the specified
     Actor is alive or dead.
     */
    virtual bool isAlive() = 0;
    
    /*
     @param x, the x coordinate of the object we are passing in
     @param y, the y coordinate of the object we are passing in
     @param ID, the ID of all objects we are checking for
     @return true if given object with (X,Y) coords will be overlapping with any object of type ID
     The function determines, given an Actor's X,Y, coordinates, if
     The actor will be overlapping with any other objects of type ID.
     Only works for objects that are size=1.0
     */
    virtual bool overLapping(int x, int y, int ID);
    
    
    
    
    //Attacks and what not
    /*
     @param attack, the Actor's attack points
     Every object that can cause damage will set its attack points
     Using this function
     */
    void setAttackPoints(int attack);
    /*
     @return int, amount of damag (attack points)the Actor causes
     */
    int getAttackPoints();
    
    
    //THESE COULD GO IN CHARACTER IF I CAN MAKE MULTIPLE VECTORS!!!!!!
    virtual void decreaseHitPoints(int amount){return;} //todo: PROBABLY SHOULD BE BETTER
    virtual void foundGold(bool){return;}
    
    
    
    
    
    
    
    
    //use this to pick things up with frackman... gold with protestors
    //TODO: IMPLEMENT THIS
    //virtual void pickThingsUp(){return;}
    
    
    
    
    
    
private:
    StudentWorld* getworld;
    int m_attackPoints;
    
    
    
};


class Characters : public Actor
{
public:
    Characters(StudentWorld* actor, int x, int y, int ID, Direction dir, double size, unsigned int depth);
    
    
    //TODO: PROBABLY GUNNA NEED TO MOVE THIS TO ACTOR
    //Characters have hitpoints
    void setHitPoints(int hitpoints);
    int getHitPoints();
    void decreaseHitPoints(int amount);
    
    
    
    
    virtual bool isAlive();
    
private:
    
    int m_hitpoints;
    bool m_alive;
    
    
    
    
};


class Frackman : public Characters
{
public:
    //constructor
    Frackman(StudentWorld* actor);
    virtual ~Frackman();
    
    //todo: determine if this needs to be a virtual abstract
    virtual void doSomething();
    
    
    //TODO: IMPLEMENT
    void squirt();
    void sonar();
    
    //TODO: FINISH IMPLEMENTING
    void move(int ch);
    
    //this checks for overlapping with boulders and other stuff
    //bool overLapping(int x, int y);
    
    //picking Up Objects
    void pickUpObjects(int ID);
    
    void dropGold();
    
    
    
private:
    /*
     const int ID = IID_PLAYER;
     const int startX = 30;
     const int startY = 60;
     const Direction startDir = right;
     const double imSize = 1.0;
     const unsigned int depth = 0;
     */
    //is hitpoints health?
    //Direction decodeCH(int ch); TODO: DECIDE IF I NEED THIS
    
    
    
    int m_waterUnits;
    int m_goldNugs;
    int m_sonar;
    
    
    
};


class Protestor : public Characters
{
public:
    Protestor(StudentWorld *actor, int ID);
    
    //protestors wander around aimlessly
    virtual void doSomething();
    //virtual int setRest();
    //virtual void resting();
    virtual void setTicksToWait(bool stun);
    virtual int getTicksToWait();
    
    //shouting
    virtual int getLastShout();
    virtual void setLastShout();
    virtual void shout();
    
    bool facingFrackMan();
    
    
    //moving
    //virtual void setNumSquaresInDirection();
    //virtual int getNumSquaresInDirection();
    
    virtual void decreaseHitPoints(int amount);
    
    
    //protestors also run if they see the Frackman
    //virtual void run();
    
    
    //isAlive() has to check for when the protestor makes it to exit,
    //not dead until health<=0 && atExit
    virtual bool isAlive();
    
    
    
    
    
    //member functions for when protestor is thoroughly annoyed
    //void setLeaveField(bool leave);
    bool leavingField();
    void setLeaveField(bool leave);
    void foundGold(bool areYouEatingTho);
    
    bool atExitOfField();
    void setAtExit(bool);
    
    
private:
    
    int m_restState;
    int m_ticksToWait;
    int m_lastShout;
    
    int numSquaresInDir;
    
    bool leaveOilField;
    bool m_atExit;
    
    
    
    
    
    
};







class HardCoreProtestor : public Protestor
{
public:
    HardCoreProtestor(StudentWorld *actor);
    
    
    /*
     void setLeaveField(bool leave);
     bool getLeaveField();
     
     bool atExitOfField();
     */
    
};











class Objects : public Actor
{
public:
    
    Objects(StudentWorld* actor, int s_x, int s_y, int ID, Direction dir, double size, unsigned int depth);
    
    
    
    //this is just for now, will get back to this
    //TODO: SOMEHOW IMPLEMENT THIS
    virtual bool attackProtestor()
    {return false;}
    
    
    //lifeOrDeath situations
    virtual bool isAlive();
    void isNowDead();
    
    //actions performed after number of ticks
    void setTicksLeft(int numTicks);
    int getTicksLeft();
    
    //objects have a radius that within it some action occurs (boulder does damage, gold turns visible...)
    void setRadius(double radius);
    double getRadiusOfAct();
    
    //Pickupable and whatnot
    void setPickupableByFrack(bool tOrF);
    bool isPickupableByFrack();
    
    //Score that frackman gets when something is achieved
    void setScorePoints(int amount);
    int getScoreIncrease();
    
    
    
private:
    
    double radiusOfAction;
    
    
    int m_ticksLeft;
    int scoreIncrease;
    
    
    bool m_alive;
    bool m_pickupableByFrack;
    
    
    
};




//This may need to be derived from actor
class Boulder : public Objects
{
public:
    Boulder(StudentWorld* actor, int x, int y);
    
    
    
    virtual void doSomething();
    
    
    /*could consolidate these two, but this is cleaner, and functionality is same*/
    //TODO: DECIDE IF THESE SHOULD GO IN A SUPERCLASS
    bool dirtBeneath();
    bool boulderBeneath();
    
    
private:
    bool is_stable;
    bool m_falling;
    
    
    
};



class Squirt : public Objects
{
public:
    Squirt(StudentWorld *actor, int x, int y, Direction dir);
    
    virtual void doSomething();
    
    
private:
    
    
    
};






class Gold : public Objects
{
public:
    Gold(StudentWorld *actor, int x, int y, bool placedByFrack);
    
    virtual void doSomething();
    
private:
    
    bool m_permanent;
    
};


class SonarKit : public Objects
{
public:
    SonarKit(StudentWorld *actor);
    
    virtual void doSomething();
};


class WaterPool : public Objects
{
public:
    
    WaterPool(StudentWorld *actor, int x, int y);
    
    virtual void doSomething();
};


class OilBarrel : public Objects
{
public:
    OilBarrel(StudentWorld *actor, int x, int y);
    
    virtual void doSomething();
    
};



//TODO::: MAYBE MAKE DIRT A GRAPH OBJECT? AND NOT AN OBJECTS
class Dirt : public GraphObject
{
public:
    Dirt(StudentWorld* actor, int x, int y);
    
    
};


#endif // ACTOR_H_
