#ifndef STUDENTWORLD_H_
#define STUDENTWORLD_H_


#include "GameWorld.h"
#include "GameConstants.h"
#include "Actor.h"
#include <string>
#include <vector>

// Students:  Add code to this file, StudentWorld.cpp, Actor.h, and Actor.cpp
class GraphObject;

const int anyObject = 0;


class StudentWorld : public GameWorld
{
public:
    StudentWorld(std::string assetDir);
    ~StudentWorld();
    
    
    
    //GAMEPLAY
    
    virtual int init();
    virtual int move();
    virtual void cleanUp();
    
    //do Something
    int Ask_All_Actors_doSomething();
    
    
    
    //check if anything is there
    bool dirtExists(const int x, const int y);
    bool objectExists(const int x, const int y, Actor *thisBoulder,int ID);
    /* bool protestorExistsHere(const int x,const int y)
     {
     for(int i = 0; i<all_objects.size(); i+++
     if(all_objects[i]->getID()
     }
     */
    
    //Helper functions to place objects
    void numObjectsPlacedAtBeginning();
    bool placeObject(int ID);
    //void determineRandom(int &x, int &y);
    void addObjectToVector(Actor *object);
    
    //Creating new Objects
    void createDirt();
    void addBoulder(int x, int y);
    void addGold(int x, int y, bool visible);
    void addSquirt();
    void addBarrel(const int x, const int y);
    void addSonar();
    void addProtestor();
    void addHCProtestor();
    void addWaterPool();
    
    void useSonar();
    
    
    //Adding Objects Due to Ticks During GamePlay
    void setNumTicks(const int ID);
    void addProtestorsDuringGamePlay();
    void addGoodiesDuringGamePlay();
    
    
    
    
    
    
    //deletion
    void deleteTheDead();
    void deleteDirt(int x, int y);
    
    
    
    
    //Radius functions TODO: PROBABLY SHOULDNT HAVE THIS MANY
    bool withinRadius( double x1, double y1, double x2, double y2, double radius);
    bool withinRadiusOfObjects(int x, int y, double radAcceptable, int ID);
    bool withinRadiusOfFrackman(GraphObject *object, double radius);
    
    
    
    
    
    //got goodie
    void gotGoodie(int ID, int scoreIncrease);
    
    
    
    //protestors and whatnot
    bool probabilityOfHardCoreProtestor();
    
    //attacking functions MOVE THIS
    void attackFrackman(int attackAmount)
    {
        frackMan->decreaseHitPoints(attackAmount);
    }
    //Todo: so not sure if this works
    //THESE ARE ONLY HERE FOR NOW... MOVE THEM TO .CPP later
    bool protestorFacingFrackman(Actor *protestor)
    {
        if(protestor->getDirection()==Actor::up)
        {
            if(frackMan->getX()>protestor->getY())
                return true;
        }
        else if(protestor->getDirection()==Actor::down)
        {
            if(frackMan->getX()<protestor->getY())
                return true;
        }
        else if(protestor->getDirection()==Actor::left)
        {
            if(frackMan->getX()<protestor->getX())
                return true;
        }
        else if(protestor->getDirection()==Actor::right)
        {
            if(frackMan->getX()>protestor->getX())
                return true;
        }
        return false;
    }
    //bool protestorLineOfSight(Actor *protestor)
    void protestorFoundGold(const int x, const int y, const int amountToRaiseScore)
    {
        
        increaseScore(amountToRaiseScore);
        for(int i = 0; i<all_objects.size(); i++)
        {
            
            if(((all_objects[i]->getID()==IID_PROTESTER || all_objects[i]->getID()==IID_HARD_CORE_PROTESTER) && withinRadius(x, y, all_objects[i]->getX(), all_objects[i]->getY(), 3.0)))
            {
                all_objects[i]->foundGold(true);
                return;
            }
        }
        
    }
    
    bool attackProtestor(int ID, int attackAmount, double radius, Actor* object);
    
    
    
    
    
    //visuals
    //void updateDisplayText();
    
    
    
    
private:
    std::vector<Actor*> all_objects; //this will be all else
    Dirt *dirt[64][60];
    
    Frackman *frackMan; //this one will be for frackman
    int barrelsLeft;
    
    int m_ticksProtestor;
    int numProtestors;
    int probabilityOfGoodie;
    
    
};

#endif // STUDENTWORLD_H_
